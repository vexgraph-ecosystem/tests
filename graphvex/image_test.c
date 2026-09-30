// tests/graphvex/image_test.c — mirrors src/image.c
//
// RGBA8 pixel buffer with a CPU shadow: construct, resize, upload, fill,
// stride, layer, and the opaque native/IOSurface slots.

#include <stdio.h>
#include <string.h>

#include "image.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

int main(void) {
    // defaults: 1x1 RGBA8, no shadow, no native handle
    Image *img = Image_0();
    CHECK(img != NULL);
    CHECK(Image_width(img) == 1 && Image_height(img) == 1);
    CHECK(Image_format(img) == IMAGE_FORMAT_RGBA8);
    CHECK(Image_usage(img) == IMAGE_USAGE_NONE);
    CHECK(Image_pixels(img) == NULL);
    CHECK(Image_isValid(img));
    CHECK(Image_native(img) == NULL && Image_iosurface(img) == NULL);

    // 0 dimensions clamp to 1
    Image *z = Image_2(0, 0);
    CHECK(Image_width(z) == 1 && Image_height(z) == 1);
    Image_destroy(z);

    // shadow allocation + stride
    CHECK(Image_ensureShadow(img, 4, 3));
    CHECK(Image_pixels(img) != NULL);
    CHECK(Image_stride(img) == 16);
    CHECK(Image_width(img) == 4 && Image_height(img) == 3);

    // fill uses monotonic 0xRRGGBBAA byte order
    Image_fill(img, COLOR_RGBA(10, 20, 30, 255));
    const uint8_t *p = Image_pixels(img);
    CHECK(p[0] == 10 && p[1] == 20 && p[2] == 30 && p[3] == 255);

    // upload replaces content and dims
    uint8_t src[2 * 2 * 4];
    for (int i = 0; i < 16; i++) src[i] = (uint8_t)i;
    CHECK(Image_upload(src, 2, 2, img));
    CHECK(Image_width(img) == 2 && Image_height(img) == 2);
    CHECK(Image_pixels(img)[3] == 3);

    // resize preserves a block and re-zeroes
    CHECK(Image_resize(img, 3, 3));
    CHECK(Image_width(img) == 3 && Image_height(img) == 3);

    // usage as declared
    Image *u = Image_4(2, 2, IMAGE_FORMAT_BGRA8, IMAGE_USAGE_RENDER | IMAGE_USAGE_TRANSFER);
    CHECK(Image_format(u) == IMAGE_FORMAT_BGRA8);
    CHECK(Image_usage(u) == (IMAGE_USAGE_RENDER | IMAGE_USAGE_TRANSFER));
    Image_destroy(u);

    // layer + native slots
    CHECK(Image_layer(img) == 0u);
    Image_setLayer(img, 3);
    CHECK(Image_layer(img) == 3u);
    Image_setNative(img, (void *)0x1234);
    CHECK(Image_native(img) == (void *)0x1234);
    Image_setIOSurface(img, (void *)0x5678);
    CHECK(Image_iosurface(img) == (void *)0x5678);

    Image_destroy(img);
    Image_destroy(NULL);   // null-safe
    CHECK(!Image_isValid(NULL));

    // null-safe reads
    CHECK(Image_width(NULL) == 0u);
    CHECK(Image_pixels(NULL) == NULL);
    CHECK(Image_native(NULL) == NULL);
    Image_fill(NULL, COLOR_WHITE);              // must not crash
    CHECK(!Image_upload(NULL, 1, 1, NULL));

    printf("image_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
