// tests/graphvex/vulkan/vk_renderer_test.c — mirrors src/vulkan/vk_renderer.c
//
// The Vulkan Backend row. Without a device it still records the display-list
// verbs into the CPU quad batch (the part that must never be wrong); present()
// is the host seam and returns false until it lands.

#include <stdio.h>

#include "graphics/graphics.h"
#include "vulkan/vulkan_backend.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

int main(void) {
    CHECK(Graphics_register(VulkanBackend_row()));
    CHECK(Graphics_use(BACKEND_VULKAN));
    CHECK(Graphics_backendId() == BACKEND_VULKAN);

    CHECK(Graphics_resize(64, 64));
    CHECK(Graphics_begin());
    CHECK(Graphics_clear(COLOR_BLACK));                       // full-viewport quad
    CHECK(Graphics_fillRect(&(Rect){4, 4, 10, 10}, &(Brush){COLOR_WHITE, 0, 0, 0}));

    const VkBatch *batch = VulkanBackend_batch();
    CHECK(batch != NULL);
    CHECK(batch->count == 2);                                  // clear + rect

    CHECK(Graphics_end());
    // present is the (unimplemented) host seam -> false, never a swapchain
    CHECK(!Graphics_present());
    CHECK(VulkanBackend_lastError() != NULL);

    // clip intersects into the recorded quad
    CHECK(Graphics_begin());
    CHECK(Graphics_clip(&(Rect){0, 0, 4, 4}));
    CHECK(Graphics_fillRect(&(Rect){0, 0, 100, 100}, &(Brush){COLOR_WHITE, 0, 0, 0}));
    const VkBatch *b2 = VulkanBackend_batch();
    CHECK(b2->count == 1);
    CHECK(b2->quads[0].w == 4.0f && b2->quads[0].h == 4.0f);   // clipped
    CHECK(Graphics_end());

    VulkanBackend_unbind();
    printf("vk_renderer_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
