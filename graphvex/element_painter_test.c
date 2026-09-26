#include <stdio.h>

#include "annotation/overview.h"
#include "lang/component.h"
#include "lang/element_painter.h"
#include "lang/graphics.h"
#include "lang/graphics_panel.h"
#include "raster/raster_graphics.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ElementPainterTest (tests/graphvex/element_painter_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the UI paint pass headless: a GraphicsPanel tree (a background panel + a
 * bordered button) placed by the absolute layout, painted through the software
 * row, read back — and an anchored child paints at its tracked position across
 * layouts.
 *
 * STRUCT FIELDS: none — procedural test harness.
 * ============================================================================
 */

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

static int px(Image *img, int x, int y, int c) {
    return Image_pixels(img)[((size_t) y * Image_width(img) + (size_t) x) * 4u + (size_t) c];
}

int main(void) {
    int failures = 0;

    CHECK(Graphics_registerRow(RasterGraphics_getRow()));
    CHECK(Graphics_setGraphics(LANG_BACKEND_RASTER));
    CHECK(Graphics_resize(64, 64));
    Image *fb = RasterGraphics_getFramebuffer();
    CHECK(Graphics_clear(0x000000FFu));

    // Root panel (dark) + a bordered blue button.
    GraphicsPanel *root = GraphicsPanel_1(0x1E1E24FFu);
    GraphicsPanel_setSize(root, 64.0f, 64.0f);

    GraphicsPanel *button = GraphicsPanel_1(0x3366CCFFu);
    GraphicsPanel_setSize(button, 20.0f, 10.0f);
    GraphicsPanel_setLocation(button, 40.0f, 40.0f);
    GraphicsPanel_setBorder(button, 0xFFFFFFFFu, 2.0f);
    CHECK(GraphicsPanel_add(root, GraphicsPanel_component(button)));

    Component_layout(GraphicsPanel_component(root), 0.0f, 0.0f, 64.0f, 64.0f);
    CHECK(ElementPainter_paint(GraphicsPanel_component(root)));

    CHECK(px(fb, 5, 5, 0) == 0x1E && px(fb, 5, 5, 2) == 0x24);      // root fill
    CHECK(px(fb, 45, 45, 0) == 0x33 && px(fb, 45, 45, 2) == 0xCC);  // button fill
    CHECK(px(fb, 41, 41, 0) == 0xFF && px(fb, 41, 41, 1) == 0xFF);  // button border

    // Anchor tracking: bottom-right anchored button re-locates across layouts.
    CHECK(Graphics_clear(0x000000FFu));
    GraphicsPanel_setAnchor(button, GRAPHICS_COMPONENT_ANCHOR_BOTTOM_RIGHT);
    GraphicsPanel_setPivot(button, GRAPHICS_COMPONENT_PIVOT_BOTTOM_RIGHT);
    GraphicsPanel_setOrigin(button, GRAPHICS_COMPONENT_ORIGIN_BOTTOM_RIGHT);
    GraphicsPanel_setLocation(button, 4.0f, 4.0f);
    Component_layout(GraphicsPanel_component(root), 0.0f, 0.0f, 64.0f, 64.0f);
    CHECK(ElementPainter_paint(GraphicsPanel_component(root)));
    // abs = (64-20-4, 64-10-4) = (40, 50) -> interior at (45,55).
    CHECK(px(fb, 45, 55, 0) == 0x33 && px(fb, 45, 55, 2) == 0xCC);

    // Cold seams.
    CHECK(ElementPainter_paint(nullptr) == false);

    GraphicsPanel_free(root);   // frees button too
    RasterGraphics_shutdown();

    if (failures == 0)
        printf("PASS element_painter_test: GraphicsPanel tree place -> paint -> pixels (bg + border + track)\n");
    else
        fprintf(stderr, "FAIL element_painter_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
