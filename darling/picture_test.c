#include <stdio.h>

#include "annotation/overview.h"
#include "darling/picture/picture.h"
#include "lang/graphics.h"
#include "lang/image.h"
#include "raster/raster_graphics.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: PictureTest (tests/darling/picture_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the picture widget headless: a Panel with a background color and a
 * graphvex Image drawn inside it through the R3 fit contract. Paints the panel
 * parts into the software row and reads the pixels back per mode — stretch,
 * contain (letterbox), cover (crop), and the anchored source-pixel window
 * (the fillWidth / fillHeight crop), plus the unset window (1:1 clamp) and the
 * cold seams.
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

static bool isGreenBg(Image *fb, int x, int y) {
    return px(fb, x, y, 0) == 0x00 && px(fb, x, y, 1) == 0xFF && px(fb, x, y, 2) == 0x00;
}
static bool isRed(Image *fb, int x, int y) {
    return px(fb, x, y, 0) == 0xFF && px(fb, x, y, 2) == 0x00;
}
static bool isBlue(Image *fb, int x, int y) {
    return px(fb, x, y, 2) == 0xFF && px(fb, x, y, 0) == 0x00;
}

int main(void) {
    int failures = 0;

    CHECK(Graphics_registerRow(RasterGraphics_getRow()));
    CHECK(Graphics_setGraphics(LANG_BACKEND_RASTER));
    CHECK(Graphics_resize(40, 40));
    Image *fb = RasterGraphics_getFramebuffer();
    Rectangle rect = { 0.0f, 0.0f, 40.0f, 40.0f };

    Image *half = makeHalfImage();
    CHECK(half != nullptr);

    // A picture: green background (Strict 0xRRGGBBAA) + the half image inside.
    Picture *pic = Picture_1(half);
    CHECK(pic != nullptr);
    Picture_setBackgroundColor(pic, 0x00FF00FFu);
    CHECK(Picture_getImage(pic) == half);
    CHECK(Picture_getBackgroundColor(pic) == 0x00FF00FFu);
    CHECK(Picture_getMode(pic) == PICTURE_MODE_FIT);   // the legacy default

    // FIT (stretch): the whole image fills the panel, no background survives.
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isRed(fb, 5, 20));
    CHECK(isBlue(fb, 35, 20));
    CHECK(!isGreenBg(fb, 2, 2));

    // ZOOM_FIT (contain): the image is letterboxed, background above and below.
    Picture_setMode(pic, PICTURE_MODE_ZOOM_FIT);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isGreenBg(fb, 20, 2));       // letterbox row
    CHECK(isRed(fb, 5, 20));
    CHECK(isBlue(fb, 35, 20));
    CHECK(isGreenBg(fb, 20, 38));      // letterbox row

    // ZOOM_FILL (cover): full-bleed, the overflow cropped away.
    Picture_setMode(pic, PICTURE_MODE_ZOOM_FILL);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(!isGreenBg(fb, 2, 2));       // corner is image, not background
    CHECK(isRed(fb, 2, 20));
    CHECK(isBlue(fb, 35, 20));

    // WINDOW top-left, 2 source px across: only the red half shows (2x scale).
    Picture_setMode(pic, PICTURE_MODE_FILL_TOP_LEFT);
    Picture_setFillWidth(pic, 2.0f);
    CHECK(Picture_getFillWidth(pic) == 2.0f);
    CHECK(Picture_getFillHeight(pic) == 0.0f);   // one window, two doors
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isRed(fb, 5, 20));
    CHECK(isRed(fb, 35, 20));          // still red: the window crops the right half

    // WINDOW top-right: the same 2 px window, anchored to the far edge (blue).
    Picture_setMode(pic, PICTURE_MODE_FILL_TOP_RIGHT);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isBlue(fb, 5, 20));
    CHECK(isBlue(fb, 35, 20));

    // The height door names the same window: 1 source px down (the panel is
    // square) is a 1x1 source window — the top-left source pixel only.
    Picture_setMode(pic, PICTURE_MODE_FILL_TOP_LEFT);
    Picture_setFillHeight(pic, 1.0f);
    CHECK(Picture_getFillWidth(pic) == 0.0f);    // the height door cleared the width
    CHECK(Picture_getFillHeight(pic) == 1.0f);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isRed(fb, 5, 20));
    CHECK(isRed(fb, 35, 20));

    // Cleared window = unset = widget pixels (1:1), clamped to the image (2 px).
    Picture_clearFillWindow(pic);
    CHECK(Picture_getFillWidth(pic) == 0.0f && Picture_getFillHeight(pic) == 0.0f);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));
    CHECK(isRed(fb, 5, 20));           // the clamped window is the cover width
    CHECK(isRed(fb, 35, 20));

    // An unbound picture shows only its background.
    Picture_setImage(pic, nullptr);
    CHECK(Picture_getImage(pic) == nullptr);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(Panel_paintParts(&(*pic).base, &rect));   // stage 0 still draws
    CHECK(isGreenBg(fb, 20, 20));
    Picture_setImage(pic, half);

    // Mode cycling walks all eight and wraps.
    Picture_setMode(pic, PICTURE_MODE_FIT);
    for (int i = 0; i < PICTURE_MODE_COUNT; i++)
        Picture_cycleMode(pic);
    CHECK(Picture_getMode(pic) == PICTURE_MODE_FIT);
    CHECK(Picture_getModeName(PICTURE_MODE_FILL_BOTTOM_RIGHT)[0] == 'F');

    // Cold seams (the Cold-Strict, Hot-Minimal Validation Law).
    CHECK(Panel_paintParts(nullptr, &rect) == false);
    CHECK(Panel_paintParts(&(*pic).base, nullptr) == false);

    Picture_free(pic);
    Image_destroy(half);   // the image is borrowed — the caller frees it
    RasterGraphics_shutdown();

    if (failures == 0)
        printf("PASS picture_test: bg + image inside, FIT/ZOOM_FIT/ZOOM_FILL + anchored window (fillWidth/fillHeight)\n");
    else
        fprintf(stderr, "FAIL picture_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
