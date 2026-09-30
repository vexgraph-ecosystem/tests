// tests/graphvex/vulkan/surface_test.c — mirrors src/vulkan/surface.c
//
// A Surface is a host-borrowed destination + ONE retained present Image.
// There is NO swapchain; present() hands the image to the host seam.

#include <stdio.h>

#include "vulkan/surface.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

int main(void) {
    Surface *s = Surface_2((void *)0xCAFE, 100, 50);
    CHECK(s != NULL);
    CHECK(Surface_isValid(s));
    CHECK(Surface_width(s) == 100 && Surface_height(s) == 50);
    CHECK(Surface_handle(s) == (void *)0xCAFE);   // borrowed, returned verbatim

    // the retained present image exists and is renderable
    Image *img = Surface_presentImage(s);
    CHECK(img != NULL);
    CHECK(Image_width(img) == 100 && Image_height(img) == 50);
    CHECK((Image_usage(img) & IMAGE_USAGE_RENDER) != 0u);

    // present is the (unimplemented) host seam -> false, never a swapchain call
    CHECK(!Surface_present(s));

    CHECK(Surface_resize(s, 200, 100));
    CHECK(Surface_width(s) == 200 && Surface_height(s) == 100);
    CHECK(Image_width(Surface_presentImage(s)) == 200);

    // a null native handle is a valid offscreen surface
    Surface *off = Surface_0();
    CHECK(Surface_isValid(off));
    CHECK(Surface_handle(off) == NULL);
    CHECK(!Surface_present(off));       // still false without a host seam
    Surface_destroy(off);

    // null-safe
    CHECK(!Surface_isValid(NULL));
    CHECK(Surface_width(NULL) == 0u);
    CHECK(Surface_handle(NULL) == NULL);
    CHECK(Surface_presentImage(NULL) == NULL);
    CHECK(!Surface_present(NULL));
    CHECK(!Surface_resize(NULL, 4, 4));
    Surface_destroy(NULL);

    Surface_destroy(s);
    printf("surface_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
