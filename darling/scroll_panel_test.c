// Headless smoke test for ScrollPanel (the frozen scroll panel).
// Proves: bounds with contentInset, HARD clamp, ELASTIC overscroll + gravity
// return, and mid-gesture chaining to the parent.

#include "darling/panel/scroll_panel.h"
#include "darling/panel/panel.h"

#include "nio/mem.h"
#include "raster/raster_graphics.h"
#include "vulkan/vk_device.h"
#include "vulkan/vk_graphics.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

static int failures;

static void check(bool cond, const char *name) {
    if (!cond) { failures++; printf("FAIL %s\n", name); }
    else printf("ok %s\n", name);
}

static Panel *content(float w, float h) {
    Panel *p = Panel_0();
    Panel_setSize(p, w, h);
    return p;
}

static void paintProof(bool vulkan) {
    Device *device = nullptr;
    if (vulkan) {
        check(Device_registerRow(Vulkan_row()), "register-vulkan");
        DeviceDesc desc = { .backend = LANG_BACKEND_VULKAN };
        device = Device_new(&desc);
        check(device && VkGraphics_bind(device), "bind-vulkan");
        if (!device) return;
        check(Graphics_registerRow(VkGraphics_getRow()), "register-vulkan-graphics");
        check(Graphics_setGraphics(LANG_BACKEND_VULKAN), "select-vulkan");
    } else {
        check(Graphics_registerRow(RasterGraphics_getRow()), "register-raster");
        check(Graphics_setGraphics(LANG_BACKEND_RASTER), "select-raster");
    }
    ScrollPanel *outer = ScrollPanel(30.0f, 24.0f);
    ScrollPanel *inner = ScrollPanel(10.0f, 8.0f);
    check(Memory_type(inner) == TYPE_SCROLLPANEL_SINGLETON, "nested-panel-runtime-identity");
    Panel *oc = content(80.0f, 50.0f);
    Panel *ic = content(40.0f, 20.0f);
    Panel *blue = content(20.0f, 15.0f);
    Panel *green = content(4.0f, 4.0f);
    ScrollPanel_setContent(outer, oc);
    ScrollPanel_setContent(inner, ic);
    Panel_addContainer(oc, &(*inner).base);
    Panel_setLocation(&(*inner).base, 12.0f, 6.0f);
    Panel_setLocation(blue, -3.0f, -2.0f);
    Panel_setBackgroundColor(blue, 0x0000FFFFu);
    Panel_addContainer(ic, blue);
    Panel_setLocation(green, 24.0f, 17.0f);
    Panel_setBackgroundColor(green, 0x00FF00FFu);
    Panel_addContainer(oc, green);
    Panel_setBackgroundColor(oc, 0xFF0000FFu);
    ScrollPanel_setBarVisible(outer, true, false);
    ScrollPanel_setBarVisible(outer, false, false);
    ScrollPanel_setBarVisible(inner, true, false);
    ScrollPanel_setBarVisible(inner, false, false);
    ScrollPanel_setOffset(outer, 4.0f, 3.0f);
    ScrollPanel_setOffset(inner, 2.0f, 1.0f);
    for (unsigned scale = 1u; scale <= 2u; scale++) {
    check(Graphics_resize(64u * scale, 48u * scale), "resize-paint-proof");
    check(Graphics_begin(), "begin-paint-proof");
    check(Graphics_clear(0x101014FFu), "clear-paint-proof");
    Rectangle incoming = { 0.0f, 0.0f, 64.0f * scale, 48.0f * scale };
    Rectangle viewport = { 5.0f * scale, 4.0f * scale, 30.0f * scale, 24.0f * scale };
    check(Graphics_clip(&incoming), "incoming-clip");
    check(ScrollPanel_paint(outer, &viewport), "paint-nested-viewports");
    Rectangle restored;
    check(Graphics_getClip(&restored) && Rectangle_equals(&incoming, &restored), "restore-parent-clip");
    check(Graphics_end(), "end-paint-proof");
    uint8_t gpu[128u * 96u * 4u];
    size_t pixelBytes = 64u * 48u * scale * scale * 4u;
    const uint8_t *pixels = nullptr;
    if (vulkan) {
        check(VkGraphics_readback(pixelBytes, gpu), "read-gpu-pixels");
        pixels = gpu;
    } else pixels = Image_pixels(RasterGraphics_getFramebuffer());
    bool exact = pixels != nullptr;
    for (unsigned y = 0; pixels && y < 48u * scale; y++)
        for (unsigned x = 0; x < 64u * scale; x++) {
            unsigned lx = x / scale, ly = y / scale;
            uint32_t expected = 0x101014FFu;
            if (lx >= 5u && lx < 35u && ly >= 4u && ly < 28u) expected = 0xFF0000FFu;
            if (lx >= 13u && lx < 23u && ly >= 7u && ly < 15u) expected = 0x0000FFFFu;
            if (lx >= 25u && lx < 29u && ly >= 18u && ly < 22u) expected = 0x00FF00FFu;
            const uint8_t *px = pixels + (y * 64u * scale + x) * 4u;
            uint32_t actual = ((uint32_t) px[0] << 24) | ((uint32_t) px[1] << 16) | ((uint32_t) px[2] << 8) | px[3];
            if (actual != expected) {
                if (exact) printf("pixel mismatch at %u,%u: %08x expected %08x\n", x, y, actual, expected);
                exact = false;
            }
        }
    check(exact, "all-pixels-match-nested-clips-and-single-offset");
    }
    if (vulkan) { VkGraphics_unbind(); Device_destroy(device); }
    else RasterGraphics_shutdown();
}

int main(int argc, char **argv) {
    // 1. Bounds + isNeeded.
    ScrollPanel *sp = ScrollPanel_2(100.0f, 100.0f);
    check(sp != nullptr, "construct");
    Panel *c = content(100.0f, 300.0f);
    ScrollPanel_setContent(sp, c);
    check(ScrollPanel_isNeeded(sp, true) == true, "v-needed-when-overflow");
    check(ScrollPanel_isNeeded(sp, false) == false, "h-not-needed");

    // 2. HARD: clamps, no coast.
    ScrollPanel_setScrollMode(sp, SCROLL_MODE_HARD);
    ScrollPanel_input(sp, SCROLL_INPUT_BEGAN, 0.0f, 0.0f, 0u);
    ScrollPanel_input(sp, SCROLL_INPUT_CHANGED, 0.0f, 500.0f, 16u);
    check(fabsf(ScrollPanel_getOffsetY(sp) - 200.0f) < 0.01f, "hard-clamps-to-max");
    ScrollPanel_input(sp, SCROLL_INPUT_ENDED, 0.0f, 0.0f, 32u);
    ScrollPanel_tick(sp, 100u);
    check(fabsf(ScrollPanel_getOffsetY(sp) - 200.0f) < 0.01f, "hard-no-coast");

    // 3. ELASTIC: overscroll then gravity return.
    ScrollPanel *el = ScrollPanel_2(100.0f, 100.0f);
    Panel *ec = content(100.0f, 300.0f);
    ScrollPanel_setContent(el, ec);
    ScrollPanel_setScrollMode(el, SCROLL_MODE_ELASTIC);
    ScrollPanel_input(el, SCROLL_INPUT_BEGAN, 0.0f, 0.0f, 0u);
    ScrollPanel_input(el, SCROLL_INPUT_CHANGED, 0.0f, 600.0f, 16u);
    float over = ScrollPanel_getOffsetY(el);
    check(over > 200.0f, "elastic-overscrolls");
    check(ScrollPanel_isOverscrolled(el) == true, "elastic-reports-overscroll");
    ScrollPanel_input(el, SCROLL_INPUT_ENDED, 0.0f, 0.0f, 32u);
    for (uint64_t t = 48u; t <= 2000u; t += 16u) ScrollPanel_tick(el, t);
    check(fabsf(ScrollPanel_getOffsetY(el) - 200.0f) < 0.5f, "elastic-gravity-returns");

    // 4. Chaining: HARD child at its edge hands the remainder to an ELASTIC parent.
    ScrollPanel *parent = ScrollPanel_2(200.0f, 200.0f);
    Panel *pc = content(200.0f, 400.0f);
    ScrollPanel_setContent(parent, pc);
    ScrollPanel_setScrollMode(parent, SCROLL_MODE_ELASTIC);

    ScrollPanel *child = ScrollPanel_2(100.0f, 100.0f);
    Panel *cc = content(100.0f, 300.0f);
    ScrollPanel_setContent(child, cc);
    ScrollPanel_setScrollMode(child, SCROLL_MODE_HARD);
    ScrollPanel_setChain(child, SCROLL_CHAIN_AUTO);
    Panel_addContainer(pc, &(*child).base);
    Panel_setLocation(&(*child).base, 0.0f, 0.0f);

    ScrollPanel_input(child, SCROLL_INPUT_BEGAN, 0.0f, 0.0f, 0u);
    ScrollPanel_input(child, SCROLL_INPUT_CHANGED, 0.0f, 500.0f, 16u);
    check(fabsf(ScrollPanel_getOffsetY(child) - 200.0f) < 0.01f, "chain-child-at-edge");
    check(ScrollPanel_getOffsetY(parent) > 0.0f, "chain-parent-consumes-remainder");

    // 5. CONTAIN traps (no chaining).
    ScrollPanel_setOffset(parent, 0.0f, 0.0f);
    ScrollPanel_setOffset(child, 0.0f, 200.0f);
    ScrollPanel_setChain(child, SCROLL_CHAIN_CONTAIN);
    ScrollPanel_input(child, SCROLL_INPUT_BEGAN, 0.0f, 0.0f, 1000u);
    ScrollPanel_input(child, SCROLL_INPUT_CHANGED, 0.0f, 200.0f, 1016u);
    check(fabsf(ScrollPanel_getOffsetY(parent)) < 0.01f, "contain-traps");

    // 6. Content inset: begin/end widen the range (negative-capable model).
    ScrollPanel_setOffset(child, 0.0f, 0.0f);
    ScrollPanel_setBeginOffset(child, 0.0f, 40.0f);
    ScrollPanel_setOffset(child, 0.0f, -100.0f);
    check(fabsf(ScrollPanel_getOffsetY(child) + 40.0f) < 0.01f, "begin-inset-is-min-offset");

    // 7. Scrollbar thumb drag: the bar computes, the PANEL writes the offset.
    // Regression: syncBars used to overwrite the dragged bar value, freezing
    // the thumb (the missing syncFromBar half of the two-way sync).
    ScrollPanel_setOffset(sp, 0.0f, 0.0f);
    ScrollPanel_setBarVisible(sp, true, true);
    ScrollPanel_setGrabbable(sp, true);
    float trackX = 100.0f - SCROLLPANEL_INSET_DEFAULT - ScrollPanel_getBarThickness(sp);
    float trackY = SCROLLPANEL_INSET_DEFAULT;
    float trackH = 100.0f - 2.0f * SCROLLPANEL_INSET_DEFAULT;
    check(ScrollPanel_barDragBegin(sp, trackX + 1.0f, trackY + 2.0f), "thumb-grab-begins");
    check(ScrollPanel_isBarDragging(sp), "thumb-reports-dragging");
    ScrollPanel_barDragTo(sp, trackX + 1.0f, trackY + trackH);
    check(fabsf(ScrollPanel_getOffsetY(sp) - 200.0f) < 0.01f, "thumb-drag-writes-offset");
    ScrollPanel_barDragEnd(sp);
    check(!ScrollPanel_isBarDragging(sp), "thumb-drag-ends");
    check(ScrollPanel_barDragBegin(sp, trackX + 1.0f, trackY + trackH), "thumb-grab-begins-at-bottom");
    ScrollPanel_barDragTo(sp, trackX + 1.0f, trackY);
    check(fabsf(ScrollPanel_getOffsetY(sp)) < 0.01f, "thumb-drag-back-to-top");
    ScrollPanel_barDragEnd(sp);

    paintProof(argc > 1 && strcmp(argv[1], "--vulkan") == 0);

    ScrollPanel_input(child, SCROLL_INPUT_ENDED, 0.0f, 0.0f, 1032u);
    check(!ScrollPanel_isGestureHeld(parent), "chained-parent-released");
    float prior = ScrollPanel_getOffsetY(child);
    ScrollPanel_setOffset(child, NAN, 20.0f);
    check(ScrollPanel_getOffsetY(child) == prior, "nonfinite-jump-preserves-offset");
    ScrollPanel_setBeginOffset(child, NAN, 80.0f);
    float insetY;
    ScrollPanel_getBeginOffset(child, nullptr, &insetY);
    check(insetY == 40.0f, "nonfinite-inset-preserves-state");
    ScrollPanel_setVerticalMode(child, 123);
    check(ScrollPanel_getVerticalMode(child) == SCROLL_MODE_HARD, "invalid-mode-preserves-state");
    char text[4096];
    bool truncated;
    ScrollPanel_toStringStruct(child, text, sizeof(text), &truncated);
    check(!truncated && strstr(text, "offsetY=") && strstr(text, "chainOwner="), "structure-string-mirrors-state");
    ScrollPanel_toStringStruct(child, text, 4u, &truncated);
    check(truncated, "structure-string-reports-truncation");
    ScrollPanel_free(child);
    check(Panel_getParent(cc) == nullptr && Component_getHeight(&(*cc).component) == 300.0f,
          "free-detaches-and-preserves-borrowed-content");

    if (failures == 0) printf("scroll_panel_test: all green\n");
    return failures == 0 ? 0 : 1;
}
