#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"
#include "filter/filter.h"
#include "lang/compositor.h"
#include "lang/filter_stack.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: CompositorTest (tests/graphvex/compositor_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the board compositor headless: two board Images over-composite in
 * z-order (scene bottom, content top) into one canvas, and an ordered filter
 * stack folds the canvas into the output.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - fill(img, r, g, b, a) : paint a solid RGBA8 image
 *   - main(void)
 * ============================================================================
 */

extern const FilterRow *Contrast_row(void);

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

static void fill(Image *img, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    uint8_t *px = Image_pixels(img);
    uint32_t w = Image_width(img);
    uint32_t h = Image_height(img);
    for (size_t i = 0; i < (size_t) w * h; i++) {
        px[i * 4 + 0] = r; px[i * 4 + 1] = g; px[i * 4 + 2] = b; px[i * 4 + 3] = a;
    }
}

int main(void) {
    int failures = 0;
    (void) Filter_registerRow(Contrast_row());

    // scene = opaque blue (bottom), content = 50% red (top).
    Image *scene = Image_2(8, 8);
    Image *content = Image_2(8, 8);
    fill(scene, 0, 0, 255, 255);
    fill(content, 255, 0, 0, 128);

    Image *out = Image_2(8, 8);
    Compositor *comp = Compositor_2(8, 8);
    CHECK(Compositor_isValid(comp));
    CHECK(Compositor_width(comp) == 8 && Compositor_height(comp) == 8);

    // 1. Empty compositor -> canvas is cleared (black).
    CHECK(Compositor_composite(comp, nullptr, out));
    const uint8_t *o = Image_pixels(out);
    CHECK(o[0] == 0 && o[2] == 0 && o[3] == 0);

    // 2. Scene + content over-composite: red@0.5 over blue -> ~purple.
    Compositor_setScene(comp, scene);
    Compositor_setContent(comp, content);
    CHECK(Compositor_getScene(comp) == scene);
    CHECK(Compositor_getContent(comp) == content);
    CHECK(Compositor_composite(comp, nullptr, out));
    o = Image_pixels(out);
    CHECK(o[0] > 100 && o[0] < 160);   // red blended in
    CHECK(o[1] == 0);
    CHECK(o[2] > 100 && o[2] < 160);   // blue showing through
    CHECK(o[3] == 255);

    // 3. Order is meaning: content-only (top) is pure red@0.5 over black.
    Image *contentOnly = Image_2(8, 8);
    Compositor *comp2 = Compositor_2(8, 8);
    Compositor_setContent(comp2, content);
    CHECK(Compositor_composite(comp2, nullptr, contentOnly));
    const uint8_t *co = Image_pixels(contentOnly);
    CHECK(co[0] > 100 && co[2] == 0);   // no blue below -> no blue

    // 4. Filter chain folds the canvas: contrast changes the output.
    Filter *contrast = Filter_new(&(FilterDesc){ .kind = FILTER_CONTRAST, .param = {2.0f} });
    FilterStack *stack = FilterStack_1(1);
    CHECK(FilterStack_add(stack, contrast));
    Compositor_setFilters(comp, stack);
    CHECK(Compositor_getFilters(comp) == stack);
    Image *filtered = Image_2(8, 8);
    CHECK(Compositor_composite(comp, nullptr, filtered));
    CHECK(memcmp(Image_pixels(out), Image_pixels(filtered), 8 * 8 * 4) != 0);

    // 5. Cold seams.
    CHECK(Compositor_composite(nullptr, nullptr, out) == false);
    CHECK(Compositor_composite(comp, nullptr, nullptr) == false);
    CHECK(Compositor_getScene(nullptr) == nullptr);
    CHECK(Compositor_width(nullptr) == 0);
    Compositor_destroy(nullptr);

    Image_destroy(scene);
    Image_destroy(content);
    Image_destroy(out);
    Image_destroy(contentOnly);
    Image_destroy(filtered);
    Compositor_destroy(comp);
    Compositor_destroy(comp2);
    FilterStack_destroy(stack);
    Filter_destroy(contrast);

    if (failures == 0)
        printf("PASS compositor_test: collage + z-order + filter chain\n");
    else
        fprintf(stderr, "FAIL compositor_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
