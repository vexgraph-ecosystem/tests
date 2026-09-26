#include <stdio.h>

#include "annotation/overview.h"
#include "lang/graphics.h"
#include "lang/image.h"
#include "raster/raster_graphics.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ImageFitTest (tests/graphvex/image_fit_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the picture contract headless, in two sections:
 *   1. Image_fitRect math — STRETCH / CONTAIN / COVER / WINDOW geometry, the
 *      two-door window spelling (width vs height), the clamp to the image, the
 *      anchors, and the cold seams.
 *   2. Pixels — a background fill + Graphics_drawImageFit through the software
 *      row, read back: CONTAIN letterboxes, COVER crops full-bleed, and a
 *      WINDOW shows the anchored source region at scale.
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

// A 4x2 test image: left half red, right half blue (opaque RGBA8).
static Image *makeHalfImage(void) {
    uint8_t rgba[4 * 2 * 4];
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 4; x++) {
            uint8_t *p = rgba + ((size_t) y * 4u + (size_t) x) * 4u;
            bool left = x < 2;
            p[0] = left ? 255 : 0;
            p[1] = 0;
            p[2] = left ? 0 : 255;
            p[3] = 255;
        }
    }
    Image *img = Image_2(4, 2);
    if (img == nullptr)
        return nullptr;
    Image_upload(rgba, 4, 2, img);
    return img;
}

static bool rectEq(const Rectangle *r, float x, float y, float w, float h) {
    return (*r).x == x && (*r).y == y && (*r).width == w && (*r).height == h;
}

int main(void) {
    int failures = 0;

    // ---- 1. Fit math (a 4x2 image into a 40x40 widget) ----
    Image *half = makeHalfImage();
    CHECK(half != nullptr);
    Rectangle widget = { 0.0f, 0.0f, 40.0f, 40.0f };
    ImageFit fit;

    // STRETCH: the whole image into the widget, no clip.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_STRETCH, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 40.0f, 40.0f));
    CHECK(fit.needsClip == false);

    // CONTAIN: scale 10 -> 40x20, letterboxed 10 top and bottom.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_CONTAIN, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 10.0f, 40.0f, 20.0f));
    CHECK(fit.needsClip == false);

    // COVER: scale 20 -> 80x40, overflowing 20 each side; clipped to the widget.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_COVER, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, -20.0f, 0.0f, 80.0f, 40.0f));
    CHECK(fit.needsClip == true);
    CHECK(rectEq(&fit.clip, 0.0f, 0.0f, 40.0f, 40.0f));

    // WINDOW, width door: 2 source px across -> scale 20 (40/2), top-left.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 2.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 80.0f, 40.0f));
    CHECK(fit.needsClip == true);

    // WINDOW, top-right anchor: the window's right edge sits on the image's.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_RIGHT, 2.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, -40.0f, 0.0f, 80.0f, 40.0f));

    // WINDOW, height door: 1 source px down -> the same window as 1 px across
    // (the widget is square), scale 40.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 0.0f, 1.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 160.0f, 80.0f));

    // WINDOW unset = widget px (1:1), clamped to the image: the 4x2 image caps
    // the window at its cover width (2), so unset == the 2 px window.
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 0.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 80.0f, 40.0f));

    // WINDOW overshoot clamps to the image (you cannot show missing pixels).
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 9999.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 80.0f, 40.0f));

    // WINDOW on an image larger than the widget: unset is true 1:1 (scale 1).
    Image *big = Image_2(80, 80);
    CHECK(big != nullptr);
    CHECK(Image_fitRect(big, &widget, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 0.0f, 0.0f, &fit));
    CHECK(rectEq(&fit.dst, 0.0f, 0.0f, 80.0f, 80.0f));
    Image_destroy(big);

    // Cold seams (the Cold-Strict, Hot-Minimal Validation Law).
    CHECK(Image_fitRect(nullptr, &widget, IMAGE_FIT_STRETCH, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit) == false);
    CHECK(Image_fitRect(half, nullptr, IMAGE_FIT_STRETCH, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit) == false);
    CHECK(Image_fitRect(half, &widget, IMAGE_FIT_STRETCH, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, nullptr) == false);
    Rectangle zero = { 0.0f, 0.0f, 0.0f, 40.0f };
    CHECK(Image_fitRect(half, &zero, IMAGE_FIT_COVER, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit) == false);
    CHECK(Image_fitRect(half, &widget, (ImageFitMode) 99, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit) == false);

    // ---- 2. Pixels (bg fill + fitted draw through the software row) ----
    CHECK(Graphics_registerRow(RasterGraphics_getRow()));
    CHECK(Graphics_setGraphics(LANG_BACKEND_RASTER));
    CHECK(Graphics_resize(40, 40));
    Image *fb = RasterGraphics_getFramebuffer();
    Rectangle full = { 0.0f, 0.0f, 40.0f, 40.0f };

    // CONTAIN: green background letterboxes the red/blue image (rows 10..29).
    CHECK(Graphics_clear(0x00FF00FFu));
    CHECK(Graphics_drawImageFit(half, &full, IMAGE_FIT_CONTAIN, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit));
    CHECK(px(fb, 20, 2, 1) == 0xFF && px(fb, 20, 2, 0) == 0x00);    // letterbox: bg green
    CHECK(px(fb, 5, 20, 0) == 0xFF && px(fb, 5, 20, 2) == 0x00);    // image left: red
    CHECK(px(fb, 35, 20, 2) == 0xFF && px(fb, 35, 20, 0) == 0x00);  // image right: blue
    CHECK(px(fb, 20, 38, 1) == 0xFF && px(fb, 20, 38, 0) == 0x00);  // letterbox: bg green

    // COVER: full-bleed — no background survives anywhere.
    CHECK(Graphics_clear(0x00FF00FFu));
    CHECK(Graphics_drawImageFit(half, &full, IMAGE_FIT_COVER, IMAGE_ANCHOR_CENTER, 0.0f, 0.0f, &fit));
    CHECK(px(fb, 2, 2, 1) == 0x00);                                  // corner is image, not bg
    CHECK(px(fb, 2, 20, 0) == 0xFF && px(fb, 2, 20, 2) == 0x00);     // left: red
    CHECK(px(fb, 35, 20, 2) == 0xFF && px(fb, 35, 20, 0) == 0x00);   // right: blue

    // WINDOW top-left, 2 source px across: only the red half shows.
    CHECK(Graphics_clear(0x00FF00FFu));
    CHECK(Graphics_drawImageFit(half, &full, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_LEFT, 2.0f, 0.0f, &fit));
    CHECK(px(fb, 5, 20, 0) == 0xFF && px(fb, 5, 20, 2) == 0x00);
    CHECK(px(fb, 35, 20, 0) == 0xFF && px(fb, 35, 20, 2) == 0x00);   // still red: window crops

    // WINDOW top-right: the same 2 px window, anchored to the far edge (blue).
    CHECK(Graphics_clear(0x00FF00FFu));
    CHECK(Graphics_drawImageFit(half, &full, IMAGE_FIT_WINDOW, IMAGE_ANCHOR_TOP_RIGHT, 2.0f, 0.0f, &fit));
    CHECK(px(fb, 5, 20, 2) == 0xFF && px(fb, 5, 20, 0) == 0x00);
    CHECK(px(fb, 35, 20, 2) == 0xFF && px(fb, 35, 20, 0) == 0x00);   // blue: right-anchored

    // Cold seams: an unselected row degrades the fitted draw to false.
    CHECK(Graphics_setGraphics(LANG_BACKEND_NONE) == false);   // raster stays active
    Image_destroy(half);
    RasterGraphics_shutdown();

    if (failures == 0)
        printf("PASS image_fit_test: STRETCH/CONTAIN/COVER/WINDOW math + anchors + clamp + pixels\n");
    else
        fprintf(stderr, "FAIL image_fit_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
