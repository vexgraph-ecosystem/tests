#ifndef DARLING_FILTER_GALLERY_FIXTURE_H
#define DARLING_FILTER_GALLERY_FIXTURE_H

// Deterministic picture and captions for the gallery, not production image/font
// loading. All three views execute Graphvex's real Vulkan scoped scatter pass.
#include "compositor/gpu_scope.h"
#include "image.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

enum { FILTER_GALLERY_WIDTH = 360, FILTER_GALLERY_HEIGHT = 300,
       FILTER_GALLERY_GPU_PIXEL_BUDGET = 262144 }; // explicit cold allocation/work safety budget

static inline void FilterGallery_pixel(Image *image, unsigned x, unsigned y, Color color) {
    if (x >= Image_width(image) || y >= Image_height(image))
        return;
    uint8_t *pixel = Image_pixels(image) + (size_t) y * Image_stride(image) + (size_t) x * 4;
    pixel[0] = Color_red(color);
    pixel[1] = Color_green(color);
    pixel[2] = Color_blue(color);
    pixel[3] = Color_alpha(color);
}

// Small uppercase fixture font; readable labels do not claim Label/text support.
static inline void FilterGallery_text(Image *image, unsigned x, unsigned y,
                                      const char *text, unsigned scale, Color color) {
    static const uint8_t letters[26][7] = {
        {14,17,17,31,17,17,17}, {30,17,17,30,17,17,30}, {14,17,16,16,16,17,14},
        {30,17,17,17,17,17,30}, {31,16,16,30,16,16,31}, {31,16,16,30,16,16,16},
        {14,17,16,23,17,17,15}, {17,17,17,31,17,17,17}, {14,4,4,4,4,4,14},
        {7,2,2,2,18,18,12}, {17,18,20,24,20,18,17}, {16,16,16,16,16,16,31},
        {17,27,21,21,17,17,17}, {17,25,21,19,17,17,17}, {14,17,17,17,17,17,14},
        {30,17,17,30,16,16,16}, {14,17,17,17,21,18,13}, {30,17,17,30,20,18,17},
        {15,16,16,14,1,1,30}, {31,4,4,4,4,4,4}, {17,17,17,17,17,17,14},
        {17,17,17,17,17,10,4}, {17,17,17,21,21,21,10}, {17,17,10,4,10,17,17},
        {17,17,10,4,4,4,4}, {31,1,2,4,8,16,31}
    };
    for (size_t i = 0; text[i]; ++i) {
        if (text[i] < 'A' || text[i] > 'Z')
            continue;
        const uint8_t *glyph = letters[(unsigned) (text[i] - 'A')];
        for (unsigned row = 0; row < 7; ++row)
            for (unsigned column = 0; column < 5; ++column)
                if (glyph[row] & (1u << (4 - column)))
                    for (unsigned iy = 0; iy < scale; ++iy)
                        for (unsigned ix = 0; ix < scale; ++ix)
                            FilterGallery_pixel(image, x + (unsigned) i * 6 * scale + column * scale + ix,
                                                y + row * scale + iy, color);
    }
}

static inline Image *FilterGallery_landscape(unsigned width, unsigned height) {
    Image *image = Image_2(width, height);
    if (!image || !Image_ensureShadow(image, width, height)) {
        Image_destroy(image);
        return nullptr;
    }
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            unsigned nx = x * 360 / width, ny = y * 300 / height;
            Color color = COLOR_RGBA(56 + ny / 4, 132 + ny / 5, 204, 255);
            int dx = (int) nx - 270, dy = (int) ny - 56;
            if (dx * dx + dy * dy < 23 * 23)
                color = COLOR_RGBA(255, 216, 104, 255);
            unsigned mountain = nx < 150 ? (nx > 70 ? nx - 70 : 70 - nx) :
                                 (nx > 250 ? nx - 250 : 250 - nx);
            if (ny > 76 + mountain / 2 && ny < 178)
                color = COLOR_RGBA(35, 67, 98, 255);
            if (ny >= 178)
                color = (ny / 6 + nx / 26) % 3 ? COLOR_RGBA(37, 133, 149, 255) :
                                               COLOR_RGBA(122, 205, 202, 255);
            if (ny > 242)
                color = COLOR_RGBA(27, 72, 57, 255);
            if ((nx % 36 < 5 && ny > 215) || (ny > 230 && ny < 235))
                color = COLOR_RGBA(231, 161, 85, 255);
            if (nx < 76 && ny > 35 && ny < 100)
                color = (nx / 8 + ny / 8) % 2 ? COLOR_WHITE : COLOR_RGBA(24, 39, 61, 255);
            FilterGallery_pixel(image, x, y, color);
        }
    }
    return image;
}

static inline bool FilterGallery_shaderDirectory(char *dest,size_t cap) {
    const char *home=getenv("B_HOME");
    int length;
    if (home)
        length=snprintf(dest,cap,"%s/out/debug/shader/compositor",home);
    else
        length=snprintf(dest,cap,"%s/Library/Application Support/vexgraph/b/out/debug/shader/compositor",getenv("HOME"));
    return length>=0 && (size_t) length<cap;
}

static inline bool FilterGallery_render(GpuScope *gpu,unsigned which,Image **out) {
    if (which > 2 || !out || !gpu)
        return false;
    Image *baseline = FilterGallery_landscape(FILTER_GALLERY_WIDTH, FILTER_GALLERY_HEIGHT);
    Image *photo = nullptr, *decorationImage = nullptr;
    if (!baseline)
        return false;
    if (which) {
        for (unsigned y = 0; y < FILTER_GALLERY_HEIGHT; ++y)
            for (unsigned x = 0; x < FILTER_GALLERY_WIDTH; ++x)
                FilterGallery_pixel(baseline, x, y, (x / 12 + y / 12) % 2 ?
                    COLOR_RGBA(39, 45, 61, 255) : COLOR_RGBA(27, 33, 47, 255));
    }
    bool status=false;
    decorationImage = Image_2(296, 180);
    if (!decorationImage || !Image_ensureShadow(decorationImage, 296, 180)) {
        goto cleanup;
    }
    Image_fill(decorationImage, which == 0 ? COLOR_RGBA(225, 239, 255, 80) :
               which == 1 ? COLOR_RGBA(69, 93, 141, 255) : COLOR_RGBA(138, 76, 119, 255));
    if (which) {
        photo = FilterGallery_landscape(296, 132);
        if (!photo) {
            goto cleanup;
        }
    } else {
        photo = Image_2(154, 24);
        if (!photo || !Image_ensureShadow(photo, 154, 24)) {
            goto cleanup;
        }
        Image_fill(photo, COLOR_CLEAR);
        FilterGallery_text(photo, 2, 2, "GLASS", 3, COLOR_RGBA(12, 37, 65, 255));
    }
    status=GpuScope_render(gpu,which,baseline,decorationImage,32,78,
        photo,which ? 32 : 110,which ? 100 : 154,8,out);
cleanup:
    Image_destroy(photo);
    Image_destroy(decorationImage);
    Image_destroy(baseline);
    return status;
}

static inline Image *FilterGallery_caption(unsigned which) {
    static const char *titles[] = {"GPU BACKDROP", "GPU FOREGROUND", "GPU ELEMENT"};
    static const char *details[] = {"BEHIND PANEL", "CHILD BLUR CLIPPED", "WHOLE PANEL BLUR"};
    if (which > 2)
        return nullptr;
    Image *image = Image_2(FILTER_GALLERY_WIDTH, 70);
    if (!image || !Image_ensureShadow(image, FILTER_GALLERY_WIDTH, 70)) {
        Image_destroy(image);
        return nullptr;
    }
    Image_fill(image, COLOR_RGBA(18, 23, 35, 255));
    FilterGallery_text(image, 12, 6, titles[which], 4, COLOR_RGBA(224, 235, 255, 255));
    FilterGallery_text(image, 12, 44, details[which], 2, COLOR_RGBA(145, 181, 220, 255));
    return image;
}

#endif
