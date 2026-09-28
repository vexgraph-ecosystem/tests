// White-box regression for the local probe painter's ordinary-Panel traversal.
#include "../../main/scroll_scene.c"

#include "lang/image.h"
#include "raster/raster_graphics.h"

#include <assert.h>
#include <stdio.h>

static void expectPixel(int x, int y, uint32_t color) {
    Image *frame = RasterGraphics_getFramebuffer();
    assert(frame);
    uint8_t *pixels = Image_pixels(frame);
    assert(pixels);
    size_t at = ((size_t) y * Image_width(frame) + (size_t) x) * 4u;
    assert(pixels[at] == (uint8_t) (color >> 24));
    assert(pixels[at + 1u] == (uint8_t) (color >> 16));
    assert(pixels[at + 2u] == (uint8_t) (color >> 8));
    assert(pixels[at + 3u] == (uint8_t) color);
}

int main(void) {
    assert(Graphics_registerRow(RasterGraphics_getRow()));
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    assert(Graphics_resize(100u, 100u));
    assert(Graphics_clear(PANEL_COLOR_BLACK));
    Rectangle clip = { 20.0f, 20.0f, 60.0f, 60.0f };
    assert(Graphics_clip(&clip));

    Panel *parent = Panel_0();
    Panel *child = Panel_0();
    assert(parent && child);
    Panel_setLocation(parent, -120.0f, -120.0f);
    Panel_setSize(parent, 10.0f, 10.0f);
    Panel_setLocation(child, 130.0f, 130.0f);
    Panel_setSize(child, 80.0f, 80.0f);
    Panel_setBackgroundColor(child, 0xA1375BFFu);
    Panel_addContainer(parent, child);
    // The child overlaps the clip even though neither rectangle contains a
    // corner of the other and the parent itself is entirely outside it.
    paintNode(parent, 0.0f, 0.0f);
    expectPixel(30, 30, 0xA1375BFFu);
    expectPixel(5, 30, PANEL_COLOR_BLACK);
    assert(Graphics_clip(nullptr));

    // A ScrollPanel *does* introduce an actual viewport clip. A descendant
    // outside that viewport is hidden even when its ordinary parent extends.
    assert(Graphics_clear(PANEL_COLOR_BLACK));
    ScrollPanel *viewport = ScrollPanel_2(40.0f, 40.0f);
    Panel *content = Panel_0();
    Panel_setSize(content, 100.0f, 100.0f);
    ScrollPanel_setContent(viewport, content);
    Panel *outside = Panel_0();
    Panel_setLocation(outside, 50.0f, 10.0f);
    Panel_setSize(outside, 20.0f, 20.0f);
    Panel_setBackgroundColor(outside, 0xA1375BFFu);
    Panel_addContainer(content, outside);
    s_page = viewport;
    paintScrollPanelNode(viewport, 0.0f, 0.0f);
    expectPixel(60, 20, PANEL_COLOR_BLACK);
    s_page = nullptr;

    // A delayed contact-change after fingers-up must not reacquire a new
    // gesture while the spring/momentum handoff is underway.
    ScrollScene_build();
    ScrollScene_paint(100, 100);
    ScrollScene_scrollDirectBegin(10.0f, 10.0f, 0.0f, 30.0f, 100u);
    ScrollScene_scrollDirectEnd(120u);
    assert(ScrollCapture_getState(s_scrollCapture) == SCROLL_CAPTURE_MOMENTUM_GRACE);
    ScrollScene_scrollDirectChange(10.0f, 10.0f, 0.0f, 2.0f, 130u);
    assert(ScrollCapture_getState(s_scrollCapture) == SCROLL_CAPTURE_MOMENTUM_GRACE);
    ScrollScene_free();

    // One layout in points, two native-pixel scales. A scale change keeps the
    // current logical scroll position instead of resizing the visible cards.
    ScrollScene_buildScaled(2.0f, 2.0f);
    Panel *rowBase = s_nestedBase[0];
    assert(rowBase != nullptr);
    assert(Component_getWidth(&(*rowBase).component) == 1200.0f);
    assert(Component_getHeight(&(*rowBase).component) == 1200.0f);
    ScrollScene_setPageOffset(300.0f, 200u);
    ScrollScene_rescale(1.0f, 1.0f, 220u);
    rowBase = s_nestedBase[0];
    assert(Component_getWidth(&(*rowBase).component) == 600.0f);
    float pageX = 0.0f, pageY = 0.0f;
    ScrollPanel_getOffset(s_page, &pageX, &pageY);
    assert(pageY == 150.0f);
    ScrollScene_free();

    RasterGraphics_shutdown();
    puts("PASS scroll_scene_cull_test");
    return 0;
}
