#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"
#include "device/device.h"
#include "filter/filter.h"
#include "lang/filter_stack.h"
#include "vulkan/vk_device.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: DeviceFilterTest (tests/graphvex/device_filter_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the renovation's two row families end-to-end, headless:
 *   - the Device dialect registry (vulkan row registers + creates a device
 *     when the loader is present; skipped gracefully when it is not);
 *   - the Filter algorithm registry + the ordered stack (order is meaning:
 *     blur -> contrast != contrast -> blur);
 *   - gaussian vs box (peaked vs flat).
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - makeInput(void) : a mid-grey field with one bright pixel
 *   - main(void)
 * ============================================================================
 */

extern const DeviceRow *Vulkan_row(void);
extern const FilterRow *BoxBlur_row(void);
extern const FilterRow *GaussianBlur_row(void);
extern const FilterRow *Contrast_row(void);

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

static Image *makeInput(void) {
    Image *in = Image_2(16, 16);
    if (in == nullptr)
        return nullptr;
    uint8_t *px = Image_pixels(in);
    memset(px, 0, 16 * 16 * 4);
    for (size_t i = 0; i < 16 * 16; i++) {
        px[i * 4 + 0] = 128; px[i * 4 + 1] = 128; px[i * 4 + 2] = 128; px[i * 4 + 3] = 255;
    }
    uint8_t *c = px + ((size_t) 8 * 16 + 8) * 4u;
    c[0] = c[1] = c[2] = 255;
    return in;
}

int main(void) {
    int failures = 0;

    // 1. Register the dialect + algorithm rows (exactly what boot will do).
    CHECK(Device_registerRow(Vulkan_row()));
    CHECK(Filter_registerRow(BoxBlur_row()));
    CHECK(Filter_registerRow(GaussianBlur_row()));
    CHECK(Filter_registerRow(Contrast_row()));
    CHECK(Device_rowCount() == 1);
    CHECK(Filter_rowCount() == 3);
    CHECK(strcmp(Device_backendName(LANG_BACKEND_VULKAN), "vulkan") == 0);
    CHECK(strcmp(Filter_kindName(FILTER_BLUR_GAUSSIAN), "blur.gaussian") == 0);

    // 2. Device: created through the contract when the loader is present.
    Device *dev = Device(LANG_BACKEND_VULKAN, nullptr);
    if (Device_isValid(dev)) {
        CHECK(Device_backend(dev) == LANG_BACKEND_VULKAN);
        CHECK(Device_isReady(dev));
        CHECK(Device_native(dev) != nullptr);
        printf("device: vulkan live (native=%p)\n", Device_native(dev));
        Device_destroy(dev);
    } else {
        printf("device: vulkan loader absent — device half skipped\n");
    }

    // 3. Ordered stack: blur -> contrast != contrast -> blur over one input.
    Image *in = makeInput();
    CHECK(in != nullptr);
    Image *outA = Image_2(16, 16);
    Image *outB = Image_2(16, 16);

    Filter *blurA = Filter_new(&(FilterDesc){ .kind = FILTER_BLUR_BOX, .param = {2.0f} });
    Filter *contrastA = Filter_new(&(FilterDesc){ .kind = FILTER_CONTRAST, .param = {2.0f} });
    FilterStack *a = FilterStack_1(2);
    CHECK(FilterStack_add(a, blurA));
    CHECK(FilterStack_add(a, contrastA));

    Filter *contrastB = Filter_new(&(FilterDesc){ .kind = FILTER_CONTRAST, .param = {2.0f} });
    Filter *blurB = Filter_new(&(FilterDesc){ .kind = FILTER_BLUR_BOX, .param = {2.0f} });
    FilterStack *b = FilterStack_1(2);
    CHECK(FilterStack_add(b, contrastB));
    CHECK(FilterStack_add(b, blurB));

    CHECK(FilterStack_count(a) == 2);
    CHECK(FilterStack_apply(a, nullptr, in, outA));
    CHECK(FilterStack_apply(b, nullptr, in, outB));
    CHECK(memcmp(Image_pixels(outA), Image_pixels(outB), 16 * 16 * 4) != 0);

    // 4. Gaussian is peaked where box is flat.
    Image *box = Image_2(16, 16);
    Image *gauss = Image_2(16, 16);
    Filter *blurBox = Filter_new(&(FilterDesc){ .kind = FILTER_BLUR_BOX, .param = {2.0f} });
    Filter *blurGauss = Filter_new(&(FilterDesc){ .kind = FILTER_BLUR_GAUSSIAN, .param = {2.0f} });
    CHECK(Filter_apply(blurBox, nullptr, in, box));
    CHECK(Filter_apply(blurGauss, nullptr, in, gauss));
    const uint8_t *pb = Image_pixels(box);
    const uint8_t *pg = Image_pixels(gauss);
    size_t center = ((size_t) 8 * 16 + 8) * 4u;
    size_t n2 = ((size_t) 8 * 16 + 10) * 4u;
    CHECK(pb[center] > 0 && pg[center] > 0);
    CHECK(pg[n2] < pb[n2]);   // gaussian falls off; box is flat
    CHECK(memcmp(pb, pg, 16 * 16 * 4) != 0);

    // 5. Empty stack = identity (copy input -> output).
    Image *idOut = Image_2(16, 16);
    FilterStack *empty = FilterStack_0();
    CHECK(FilterStack_apply(empty, nullptr, in, idOut));
    CHECK(memcmp(Image_pixels(in), Image_pixels(idOut), 16 * 16 * 4) == 0);

    // 6. Cold seams: null-safe, no crash.
    CHECK(FilterStack_apply(nullptr, nullptr, in, outA) == false);
    CHECK(FilterStack_apply(a, nullptr, nullptr, outA) == false);
    CHECK(Filter_apply(nullptr, nullptr, in, outA) == false);
    CHECK(Filter_getParam(nullptr, 0) == 0.0f);
    Filter_destroy(nullptr);
    FilterStack_destroy(nullptr);
    Image_destroy(nullptr);
    Device_destroy(nullptr);

    Image_destroy(in);
    Image_destroy(outA);
    Image_destroy(outB);
    Image_destroy(box);
    Image_destroy(gauss);
    Image_destroy(idOut);
    FilterStack_destroy(a);
    FilterStack_destroy(b);
    FilterStack_destroy(empty);
    Filter_destroy(blurA);
    Filter_destroy(contrastA);
    Filter_destroy(contrastB);
    Filter_destroy(blurB);
    Filter_destroy(blurBox);
    Filter_destroy(blurGauss);

    if (failures == 0)
        printf("PASS device_filter_test: rows, device, ordered stack, cold seams\n");
    else
        fprintf(stderr, "FAIL device_filter_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
