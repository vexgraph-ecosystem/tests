#include "darling/compositor/filter_gallery_fixture.h"

#include <assert.h>
#include "test_support.h"

// Numeric fixture/scope proof only: gallery appearance remains user-owned.
static Color pixel(const Image *image, unsigned x, unsigned y) {
    const uint8_t *p = Image_pixels(image) + (size_t) y * Image_stride(image) + (size_t) x * 4;
    return COLOR_RGBA(p[0], p[1], p[2], p[3]);
}

int main(void) {
    assert(!FilterGallery_photoFromPath("/nonexistent-filter-gallery-photo.png", 1, 1));
    assert(!FilterGallery_photoFromPath(nullptr, 0, 1));
    assert(!FilterGallery_photoFromPath(nullptr, UINT32_MAX, UINT32_MAX));
    Device *device=Device_create(false);
    if (!Device_isValid(device)) {
        fprintf(stderr,"filter_gallery_fixture_test: SKIP Vulkan unavailable\n");
        Device_destroy(device); return B_TEST_SKIP;
    }
    char directory[2048];
    assert(FilterGallery_shaderDirectory(directory,sizeof directory));
    GpuScope *gpu=GpuScope(device,directory,FILTER_GALLERY_GPU_PIXEL_BUDGET);
    assert(gpu);
    Image *views[3] = {nullptr};
    Image *original = FilterGallery_photo(FILTER_GALLERY_WIDTH, FILTER_GALLERY_HEIGHT);
    assert(original);
    for (unsigned i = 0; i < 3; ++i) {
        assert(FilterGallery_render(gpu,i,&views[i]));
        assert(Image_width(views[i]) == FILTER_GALLERY_WIDTH);
        assert(Image_height(views[i]) == FILTER_GALLERY_HEIGHT);
        Image *caption = FilterGallery_caption(i);
        assert(caption && Image_width(caption) == FILTER_GALLERY_WIDTH);
        assert(pixel(caption, 12, 6) != COLOR_CLEAR);
        Image_destroy(caption);
    }
    // Backdrop blur affects only covered prefix; the picture outside stays sharp.
    assert(pixel(views[0], 12, 120) == pixel(original, 12, 120));
    assert(pixel(views[0], 44, 86) != pixel(original, 44, 86));
    // Foreground blur is clipped by the panel; whole-element blur spills beyond it.
    Color outside = (31 / 12 + 120 / 12) % 2 ? COLOR_RGBA(39, 45, 61, 255) :
                                              COLOR_RGBA(27, 33, 47, 255);
    assert(pixel(views[1], 31, 120) == outside);
    assert(pixel(views[2], 31, 120) != outside);
    assert(pixel(views[1], 32, 130) != pixel(views[2], 32, 130));
    Image *unchanged = original;
    assert(!FilterGallery_render(gpu,3,&unchanged));
    assert(unchanged == original);
    assert(!FilterGallery_render(gpu,0,nullptr));
    assert(!FilterGallery_caption(3));
    for (unsigned i = 0; i < 3; ++i)
        Image_destroy(views[i]);
    Image_destroy(original);
    assert(GpuScope_destroy(gpu));
    assert(Device_isValid(device));
    Device_destroy(device);
    puts("filter_gallery_fixture_test: PASS actual Vulkan three-scope scatter gallery pixels");
    return 0;
}
