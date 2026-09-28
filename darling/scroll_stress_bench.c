// Opt-in performance probe: build once, measure CPU and Vulkan frames separately.
// Usage: scroll_stress_bench 1000|50000 [1|2] (native backing scale)
#include "scroll_scene.h"

#include "lang/device.h"
#include "lang/graphics.h"
#include "raster/raster_graphics.h"
#include "vulkan/vk_device.h"
#include "vulkan/vk_graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRESS_BENCH_WIDTH 800
#define STRESS_BENCH_HEIGHT 600
#define STRESS_BENCH_FRAME_COUNT 3
#define STRESS_BENCH_WAIT_SLICES 20

static double nowMs(void) {
    struct timespec time = { 0, 0 };
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double) time.tv_sec * 1000.0 + (double) time.tv_nsec / 1000000.0;
}

int main(int argc, char **argv) {
    if ((argc != 2 && argc != 3)
        || (strcmp(argv[1], "1000") != 0 && strcmp(argv[1], "50000") != 0)
        || (argc == 3 && strcmp(argv[2], "1") != 0 && strcmp(argv[2], "2") != 0)) {
        fprintf(stderr, "usage: scroll_stress_bench 1000|50000 [1|2]\n");
        return 2;
    }
    int scale = argc == 3 && strcmp(argv[2], "2") == 0 ? 2 : 1;
    int width = STRESS_BENCH_WIDTH * scale;
    int height = STRESS_BENCH_HEIGHT * scale;
    int stress = strcmp(argv[1], "50000") == 0
        ? SCROLL_SCENE_STRESS_DENSE : SCROLL_SCENE_STRESS_LINES;
    if (!Graphics_registerRow(RasterGraphics_getRow())
        || !Graphics_setGraphics(LANG_BACKEND_RASTER)
        || !Device_registerRow(Vulkan_row()))
        return 1;
    DeviceDesc desc = { .backend = LANG_BACKEND_VULKAN };
    Device *device = Device_new(&desc);
    if (!device || !VkGraphics_bind(device)
        || !Graphics_registerRow(VkGraphics_getRow()))
        return 1;
    size_t bytes = (size_t) width * (size_t) height * 4u;
    uint8_t *pixels = (uint8_t*) malloc(bytes);
    if (!pixels)
        return 1;
    ScrollScene_setStress(stress);
    double begin = nowMs();
    ScrollScene_buildScaled((float) scale, (float) scale);
    double buildMs = nowMs() - begin;
    double cpuMs = 0.0, gpuRecordMs = 0.0, gpuCompleteMs = 0.0;
    for (int i = 0; i < STRESS_BENCH_FRAME_COUNT; i++) {
        if (!Graphics_setGraphics(LANG_BACKEND_RASTER))
            return 1;
        begin = nowMs();
        ScrollScene_paint(width, height);
        cpuMs += nowMs() - begin;
        if (!Graphics_setGraphics(LANG_BACKEND_VULKAN))
            return 1;
        begin = nowMs();
        ScrollScene_paint(width, height);
        gpuRecordMs += nowMs() - begin;
        begin = nowMs();
        bool completed = false;
        for (int slice = 0; slice < STRESS_BENCH_WAIT_SLICES && !completed; slice++)
            completed = VkGraphics_readback(bytes, pixels);
        gpuCompleteMs += nowMs() - begin;
        if (!completed) {
            fprintf(stderr, "GPU frame did not complete within bounded wait slices\n");
            return 1;
        }
    }
    printf("mode=%d rectangles=%d frame=%dx%d build=%.2fms CPU=%.2fms/frame "
           "GPU record+submit=%.2fms/frame GPU completion+readback=%.2fms/frame\n",
           stress, stress == SCROLL_SCENE_STRESS_DENSE
               ? SCROLL_SCENE_STRESS_DENSE + (SCROLL_SCENE_MAX_NESTED - 1) * SCROLL_SCENE_STRESS_LINES
               : SCROLL_SCENE_MAX_NESTED * SCROLL_SCENE_STRESS_LINES,
           width, height, buildMs,
           cpuMs / STRESS_BENCH_FRAME_COUNT,
           gpuRecordMs / STRESS_BENCH_FRAME_COUNT,
           gpuCompleteMs / STRESS_BENCH_FRAME_COUNT);
    ScrollScene_free();
    VkGraphics_unbind();
    Device_destroy(device);
    RasterGraphics_shutdown();
    free(pixels);
    return 0;
}
