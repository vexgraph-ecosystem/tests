#include "scroll_scene.h"

#include "lang/device.h"
#include "lang/graphics.h"
#include "lang/image.h"
#include "raster/raster_graphics.h"
#include "vulkan/vk_device.h"
#include "vulkan/vk_graphics.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCENE_GPU_TEST_WIDTH 800u
#define SCENE_GPU_TEST_HEIGHT 600u
#define SCENE_GPU_TEST_BYTES ((size_t) SCENE_GPU_TEST_WIDTH * SCENE_GPU_TEST_HEIGHT * 4u)

static void compareScene(uint32_t width, uint32_t height, uint8_t *reference,
                         uint8_t *actual, int row) {
    size_t bytes = (size_t) width * height * 4u;
    uint64_t clockMs = (uint64_t) (row + 1) * 1000u;
    ScrollScene_setPageOffset(SCROLL_SCENE_ROW_Y(row) * ((float) width / SCENE_GPU_TEST_WIDTH), clockMs);
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    ScrollScene_paint((int) width, (int) height);
    Image *image = RasterGraphics_getFramebuffer();
    assert(image != nullptr);
    memcpy(reference, Image_pixels(image), bytes);
    assert(Graphics_setGraphics(LANG_BACKEND_VULKAN));
    ScrollScene_paint((int) width, (int) height);
    assert(VkGraphics_readback(bytes, actual));
    for (size_t pixel = 0; pixel < bytes; pixel++) {
        int difference = (int) actual[pixel] - (int) reference[pixel];
        if (difference < 0)
            difference = -difference;
        if (difference > 1) {
            fprintf(stderr, "row %d %ux%u GPU mismatch at byte %zu: GPU %u CPU %u\n",
                    row, width, height, pixel, actual[pixel], reference[pixel]);
            abort();
        }
    }
}

int main(void) {
    assert(Graphics_registerRow(RasterGraphics_getRow()));
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    ScrollScene_build();
    uint8_t *reference = (uint8_t*) malloc(SCENE_GPU_TEST_BYTES * 4u);
    uint8_t *actual = (uint8_t*) malloc(SCENE_GPU_TEST_BYTES * 4u);
    assert(reference != nullptr && actual != nullptr);

    assert(Device_registerRow(Vulkan_row()));
    DeviceDesc desc = { .backend = LANG_BACKEND_VULKAN };
    Device *device = Device_new(&desc);
    assert(device != nullptr);
    assert(VkGraphics_bind(device));
    assert(Graphics_registerRow(VkGraphics_getRow()));
    for (int row = 0; row < SCROLL_SCENE_MAX_NESTED; row++)
        compareScene(SCENE_GPU_TEST_WIDTH, SCENE_GPU_TEST_HEIGHT, reference, actual, row);
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    ScrollScene_rescale(2.0f, 2.0f, 15000u);
    compareScene(2u * SCENE_GPU_TEST_WIDTH, 2u * SCENE_GPU_TEST_HEIGHT,
                 reference, actual, 2);
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    ScrollScene_free();
    Device_destroy(device);
    RasterGraphics_shutdown();
    free(reference);
    free(actual);
    puts("PASS scroll_scene_vulkan_test");
    return 0;
}
