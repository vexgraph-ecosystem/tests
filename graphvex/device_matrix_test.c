#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"
#include "device/device.h"
#include "lang/image.h"
#include "null/null_device.h"
#include "raster/raster_device.h"
#include "vulkan/vk_device.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: DeviceMatrixTest (tests/graphvex/device_matrix_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the dialect matrix: vulkan + raster + null registered at once, each
 * created through the one Device contract and coexisting in the same process.
 * raster is fully headless (owns a CPU framebuffer Image); null is a no-op;
 * vulkan is created only when its loader is present.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

extern const DeviceRow *Vulkan_row(void);
extern const DeviceRow *Raster_row(void);
extern const DeviceRow *Null_row(void);

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

int main(void) {
    int failures = 0;

    // Register three dialects at once — they must coexist behind one contract.
    CHECK(Device_registerRow(Vulkan_row()));
    CHECK(Device_registerRow(Raster_row()));
    CHECK(Device_registerRow(Null_row()));
    CHECK(Device_rowCount() == 3);
    CHECK(Device_findRow(LANG_BACKEND_VULKAN) != nullptr);
    CHECK(Device_findRow(LANG_BACKEND_RASTER) != nullptr);
    CHECK(Device_findRow(LANG_BACKEND_NULL) != nullptr);
    CHECK(strcmp(Device_backendName(LANG_BACKEND_RASTER), "raster") == 0);
    CHECK(strcmp(Device_backendName(LANG_BACKEND_NULL), "null") == 0);

    // Raster: headless, owns a CPU framebuffer Image, presents by marking done.
    Device *raster = Device_new(&(DeviceDesc){ .backend = LANG_BACKEND_RASTER,
                                               .width = 64, .height = 48 });
    CHECK(Device_isValid(raster));
    CHECK(Device_backend(raster) == LANG_BACKEND_RASTER);
    CHECK(Device_isReady(raster));
    CHECK(Device_width(raster) == 64 && Device_height(raster) == 48);
    Image *fb = (Image*) Device_native(raster);
    CHECK(fb != nullptr);
    CHECK(Image_width(fb) == 64 && Image_height(fb) == 48);
    CHECK(Image_pixels(fb) != nullptr);
    CHECK(Device_present(raster));
    CHECK(Device_resize(raster, 128, 96));
    CHECK(Device_width(raster) == 128 && Device_height(raster) == 96);
    CHECK(Image_width((Image*) Device_native(raster)) == 128);

    // Null: no-op, always ready, no native handle, present succeeds.
    Device *null = Device(LANG_BACKEND_NULL, nullptr);
    CHECK(Device_isValid(null));
    CHECK(Device_backend(null) == LANG_BACKEND_NULL);
    CHECK(Device_isReady(null));
    CHECK(Device_native(null) == nullptr);
    CHECK(Device_present(null));
    CHECK(Device_resize(null, 10, 20));
    CHECK(Device_width(null) == 10 && Device_height(null) == 20);

    // Vulkan: created when the loader is present, skipped gracefully otherwise.
    Device *vk = Device(LANG_BACKEND_VULKAN, nullptr);
    if (Device_isValid(vk)) {
        CHECK(Device_backend(vk) == LANG_BACKEND_VULKAN);
        CHECK(Device_isReady(vk));
        printf("matrix: vulkan live alongside raster + null\n");
    } else {
        printf("matrix: vulkan loader absent — 2/3 dialects live\n");
    }

    // All coexist: destroy in any order, no cross-talk.
    Device_destroy(vk);
    CHECK(Device_isValid(raster));   // raster unaffected by vulkan teardown
    Device_destroy(raster);
    Device_destroy(null);

    // Cold seams.
    CHECK(Device_backend(nullptr) == LANG_BACKEND_NONE);
    CHECK(Device_isValid(nullptr) == false);
    CHECK(Device_isReady(nullptr) == false);
    CHECK(Device_width(nullptr) == 0);
    CHECK(Device_native(nullptr) == nullptr);
    CHECK(Device_present(nullptr) == false);
    CHECK(Device_resize(nullptr, 1, 1) == false);
    Device_destroy(nullptr);

    if (failures == 0)
        printf("PASS device_matrix_test: 3 dialects coexist behind one contract\n");
    else
        fprintf(stderr, "FAIL device_matrix_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
