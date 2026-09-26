#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"
#include "lang/component.h"
#include "lang/element_painter.h"
#include "lang/graphics.h"
#include "lang/label.h"
#include "lang/graphics_panel.h"
#include "lang/size.h"
#include "objects/reactive.h"
#include "raster/raster_graphics.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: LabelTest (tests/graphvex/label_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the Label: the [] formatter (bare reactive + [%.2f]), the reactive
 * pull (a reactive write updates the text on the next render, no setText), the
 * SIZE_AUTO measure (font size + padding), the erase-on-plain-setText rule,
 * and the rendered glyph pixels.
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

    // 1. Plain text.
    Label *l = Label_2("hello world", 0xFFFFFFFFu);
    CHECK(Label_isValid(l));
    CHECK(strcmp(Label_getText(l), "hello world") == 0);

    // 2. A bare [] slot consumes a Reactive; a change re-renders (no setText).
    Reactive *gold = Reactive_1(0);
    Label_setText(l, "gold: []", gold);
    CHECK(strcmp(Label_getText(l), "gold: 0") == 0);
    Reactive_set(gold, 42);
    Label_render(l);                                     // the owner pulls the atomic value
    CHECK(strcmp(Label_getText(l), "gold: 42") == 0);   // no setText, just a render
    Reactive_set(gold, 7);
    Label_render(l);
    CHECK(strcmp(Label_getText(l), "gold: 7") == 0);

    // 3. A [%.2f] slot formats a float.
    Label_setText(l, "pi = [%.2f]", 3.14159);
    CHECK(strcmp(Label_getText(l), "pi = 3.14") == 0);

    // 4. Escape: \[ is a literal '['.
    Label_setText(l, "a \\[b] c");
    CHECK(strcmp(Label_getText(l), "a [b] c") == 0);

    // 5. Erase-on-plain-setText: after a slotless setText, the old reactive can
    //    no longer change the text.
    Label_setText(l, "count: []", gold);
    CHECK(strcmp(Label_getText(l), "count: 7") == 0);
    Label_setText(l, "static now");
    CHECK(strcmp(Label_getText(l), "static now") == 0);
    Reactive_set(gold, 99);
    Label_render(l);
    CHECK(strcmp(Label_getText(l), "static now") == 0);   // binding erased

    // 5b. AUTO by default: a label with no setSize measures its text on render.
    Label *auto_ = Label_2("hi", 0xFFFFFFFFu);
    CHECK(Label_isAutoWidth(auto_) == true);            // AUTO out of the box
    CHECK(Label_isAutoHeight(auto_) == true);
    Label_render(auto_);
    GraphicsComponent *agc = Component_graphics(&(*auto_).component, 0);
    CHECK(GraphicsComponent_getWidth(agc) == (float) SIZE_AUTO);   // sentinel kept
    CHECK(GraphicsComponent_getResolvedWidth(agc) == 16.0f);       // 2 chars x 8px
    CHECK(GraphicsComponent_getResolvedHeight(agc) == 8.0f);       // 1 line
    Label_setSize(auto_, 100.0f, 20.0f);                // concrete clears AUTO
    CHECK(Label_isAutoWidth(auto_) == false);
    CHECK(Label_isAutoHeight(auto_) == false);
    CHECK(GraphicsComponent_getResolvedWidth(agc) == 100.0f);
    CHECK(GraphicsComponent_getResolvedHeight(agc) == 20.0f);
    Label_free(auto_);

    // 5c. AUTO equivalence honors explicit newlines: two lines tall, widest line
    //     wide.
    Label *multi = Label_2("ab\\ncdef", 0xFFFFFFFFu);
    Label_render(multi);
    GraphicsComponent *mgc = Component_graphics(&(*multi).component, 0);
    CHECK(GraphicsComponent_getResolvedWidth(mgc) == 32.0f);   // "cdef" = 4 x 8
    CHECK(GraphicsComponent_getResolvedHeight(mgc) == 16.0f);  // 2 lines x 8
    Label_free(multi);

    // 5d. AUTO height with a concrete width wraps: 6 chars at a 24px width
    //     breaks into 2 lines.
    Label *wrap = Label_2("abcdef", 0xFFFFFFFFu);
    Label_setSize(wrap, 24.0f, (float) SIZE_AUTO);
    Label_render(wrap);
    GraphicsComponent *wgc = Component_graphics(&(*wrap).component, 0);
    CHECK(GraphicsComponent_isAutoHeight(wgc) == true);
    CHECK(GraphicsComponent_getResolvedWidth(wgc) == 24.0f);   // declared width
    CHECK(GraphicsComponent_getResolvedHeight(wgc) == 16.0f);  // 2 wrapped lines
    Label_free(wrap);

    // 6. Render: a Label 'A' draws its glyph through the software row.
    CHECK(Graphics_registerRow(RasterGraphics_getRow()));
    CHECK(Graphics_setGraphics(LANG_BACKEND_RASTER));
    CHECK(Graphics_resize(64, 64));
    CHECK(Graphics_clear(0x000000FFu));

    Label *glyph = Label_2("A", 0xFFFFFFFFu);
    GraphicsPanel *root = GraphicsPanel_1(0x000000FFu);
    GraphicsComponent *gg = Component_graphics(&(*glyph).component, 0);
    GraphicsComponent_setSize(gg, 8.0f, 8.0f);
    GraphicsPanel_add(root, &(*glyph).component);
    Component_layout(GraphicsPanel_component(root), 0.0f, 0.0f, 64.0f, 64.0f);
    CHECK(ElementPainter_paint(GraphicsPanel_component(root)));
    Image *fb = RasterGraphics_getFramebuffer();
    // 'A' row 0 = 0x38 (0011 1000): bit 3 is on, bit 0 is off.
    CHECK(px(fb, 3, 0, 0) == 255);   // a lit glyph pixel
    CHECK(px(fb, 0, 0, 0) == 0);     // an unlit pixel (background)

    Label_free(l);        // l was never added to a tree
    GraphicsPanel_free(root);     // frees glyph too (the root owns it)
    Reactive_free(gold);
    RasterGraphics_shutdown();

    if (failures == 0)
        printf("PASS label_test: [] formatter, reactive pull, AUTO default+lines+wrap, erase, glyph pixels\n");
    else
        fprintf(stderr, "FAIL label_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
