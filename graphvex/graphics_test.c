#include <stdio.h>

#include "annotation/overview.h"
#include "lang/brush.h"
#include "lang/graphics.h"
#include "lang/stroke.h"
#include "raster/raster_graphics.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: GraphicsTest (tests/graphvex/graphics_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the unified Graphics seam headless: register the software row, select
 * it, and paint rects/circles/images into an Image, reading pixels back.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION ROUTE:
 *   - px(img, x, y, c) : read a channel of a pixel
 *   - main(void)
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

    // 1. Register + select the software row.
    CHECK(Graphics_registerRow(RasterGraphics_getRow()));
    CHECK(Graphics_setGraphics(LANG_BACKEND_RASTER));
    CHECK(Graphics_getGraphicsId() == LANG_BACKEND_RASTER);
    CHECK(Graphics_getCurrent() != nullptr);

    // 2. Bind a drawable + clear it black.
    CHECK(Graphics_resize(64, 64));
    Image *fb = RasterGraphics_getFramebuffer();
    CHECK(fb != nullptr);
    CHECK(Graphics_clear(0x000000FFu));
    CHECK(px(fb, 0, 0, 0) == 0 && px(fb, 0, 0, 3) == 255);

    // 3. fillRect: opaque red at (10,10,20,20).
    Rectangle red = { 10.0f, 10.0f, 20.0f, 20.0f };
    Brush *brush = Brush_2(0xFF0000FFu, 1.0f);
    CHECK(Graphics_fillRect(&red, brush));
    CHECK(px(fb, 15, 15, 0) == 255 && px(fb, 15, 15, 1) == 0 && px(fb, 15, 15, 2) == 0);
    CHECK(px(fb, 5, 5, 0) == 0);   // outside untouched

    // 4. drawRect: a green border just inside the red rect's edge.
    Rectangle box = { 40.0f, 40.0f, 20.0f, 20.0f };
    Stroke *stroke = Stroke_2(3.0f, 0x00FF00FFu);
    CHECK(Graphics_drawRect(&box, stroke));
    CHECK(px(fb, 41, 41, 1) == 255);        // on the top edge
    CHECK(px(fb, 50, 50, 1) == 0);          // hollow interior

    // 5. fillCircle: blue disc centered at (32,32) r=8.
    Brush *blue = Brush_2(0x0000FFFFu, 1.0f);
    CHECK(Graphics_fillCircle(32.0f, 32.0f, 8.0f, blue));
    CHECK(px(fb, 32, 32, 2) == 255);        // center
    CHECK(px(fb, 32, 8, 2) == 0);           // outside the disc

    // 6. drawImage: blit a 4x4 white image into the top-left.
    Image *src = Image_2(4, 4);
    uint8_t *sp = Image_pixels(src);
    for (int i = 0; i < 4 * 4; i++) {
        sp[i * 4 + 0] = 255; sp[i * 4 + 1] = 255; sp[i * 4 + 2] = 255; sp[i * 4 + 3] = 255;
    }
    Rectangle dst = { 0.0f, 0.0f, 4.0f, 4.0f };
    CHECK(Graphics_drawImage(src, &dst));
    CHECK(px(fb, 2, 2, 0) == 255 && px(fb, 2, 2, 3) == 255);

    // 7. clip: a scissor to the top-left 8x8 stops a full-drawable fill.
    Rectangle scissor = { 0.0f, 0.0f, 8.0f, 8.0f };
    CHECK(Graphics_clip(&scissor));
    Rectangle all = { 0.0f, 0.0f, 64.0f, 64.0f };
    Brush *magenta = Brush_2(0xFF00FFFFu, 1.0f);
    CHECK(Graphics_fillRect(&all, magenta));
    CHECK(px(fb, 3, 3, 0) == 255 && px(fb, 3, 3, 2) == 255);   // inside clip
    CHECK(px(fb, 50, 50, 0) == 0);                              // outside clip: no magenta
    CHECK(Graphics_clip(nullptr));                             // reset

    // 8. Cold seams: unknown backend rejected, unselected forwarders false.
    CHECK(Graphics_setGraphics(999) == false);
    CHECK(Graphics_getGraphicsId() == LANG_BACKEND_RASTER);   // previous stays active
    CHECK(Graphics_fillRect(nullptr, nullptr) == false);
    CHECK(Graphics_resize(0, 0) == false);

    Brush_free(brush);
    Brush_free(blue);
    Brush_free(magenta);
    Stroke_free(stroke);
    Image_destroy(src);
    RasterGraphics_shutdown();

    if (failures == 0)
        printf("PASS graphics_test: table + software row (rect/circle/border/image/clip)\n");
    else
        fprintf(stderr, "FAIL graphics_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
