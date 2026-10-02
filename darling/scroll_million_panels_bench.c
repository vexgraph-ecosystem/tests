// Opt-in headless probe. One ScrollPanel, one content Panel, N real attached
// Panel children. The indexed path changes traversal only, not the objects.
// Usage: scroll_million_panels_bench 10000|100000|1000000 (or any 1..1000000)
// At large N, float coordinates near the end have coarser subpixel precision.
#include "darling/panel/panel.h"
#include "darling/panel/scroll_panel.h"
#include "lang/device.h"
#include "lang/graphics.h"
#include "lang/image.h"
#include "lang/graphics_component.h"
#include "raster/raster_graphics.h"
#include "struct/list.h"
#include "vulkan/vk_device.h"
#include "vulkan/vk_graphics.h"

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

#define BENCH_MAX 1000000u
#define BENCH_WIDTH 600u
#define BENCH_HEIGHT 600u
#define BENCH_PITCH 32u
#define BENCH_CARD_HEIGHT 24u
#define BENCH_WAIT_ATTEMPTS 20
#define BENCH_BYTES ((size_t) BENCH_WIDTH * BENCH_HEIGHT * 4u)

static double nowMs(void) {
    struct timespec t = { 0, 0 };
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double) t.tv_sec * 1000.0 + (double) t.tv_nsec / 1000000.0;
}

static long peakRssBytes(void) {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0)
        return -1;
#ifdef __APPLE__
    return usage.ru_maxrss;
#else
    return usage.ru_maxrss * 1024L;
#endif
}

static bool frameStart(void) {
    return Graphics_resize(BENCH_WIDTH, BENCH_HEIGHT)
        && Graphics_begin() && Graphics_clear(PANEL_COLOR_BLACK);
}

// Mirrors ScrollPanel_paint for this flat, opaque, top-left-anchored scene:
// viewport stage, clipped content (with ScrollPanel's placed content offset),
// and the same ScrollPanel bars. Only the child traversal is indexed.
static bool paintIndexed(ScrollPanel *sp, Panel *content, size_t count,
                         const Rectangle *viewport, size_t *visited) {
    Panel *base = &(*sp).base;
    bool drew = Panel_paintParts(base, viewport);
    Rectangle saved;
    bool hadClip = Graphics_getClip(&saved);
    Rectangle clip = *viewport;
    if (hadClip)
        Rectangle_intersection(&clip, &saved, &clip);
    *visited = 0u;
    if (!Rectangle_isEmpty(&clip)) {
        if (!Graphics_clip(&clip))
            return false;
        Component *component = &(*content).component;
        float cx = (*viewport).x + GraphicsComponent_getX(component);
        float cy = (*viewport).y + GraphicsComponent_getY(component);
        Rectangle contentRect = { cx, cy, Component_getWidth(component),
                                  Component_getHeight(component) };
        drew = Panel_paintParts(content, &contentRect) || drew;
        // Widen by one pitch on both sides for float rounding at ~32M px.
        double top = (double) clip.y - (double) cy;
        double bottom = (double) clip.y + (double) clip.height - (double) cy;
        size_t first = top > 0.0 ? (size_t) (top / BENCH_PITCH) : 0u;
        size_t last = bottom > 0.0 ? (size_t) (bottom / BENCH_PITCH) : 0u;
        if (first > 0u)
            first--;
        if (last < count)
            last++;
        if (last > count)
            last = count;
        for (size_t i = first; i < last; i++) {
            Panel *child = Panel_getChild(content, i);
            if (!child || !Panel_isVisible(child))
                continue;
            Component *c = &(*child).component;
            Rectangle r = { cx + GraphicsComponent_getX(c), cy + GraphicsComponent_getY(c),
                            Component_getWidth(c), Component_getHeight(c) };
            drew = Panel_paintParts(child, &r) || drew;
            (*visited)++;
        }
        if (!Graphics_clip(hadClip ? &saved : nullptr))
            return false;
    }
    return ScrollPanel_paintBars(sp, viewport) || drew;
}

static bool paintFrame(ScrollPanel *sp, Panel *content, size_t count,
                       const Rectangle *viewport, bool indexed, size_t *visited) {
    if (!frameStart())
        return false;
    if (indexed) {
        if (!paintIndexed(sp, content, count, viewport, visited))
            return false;
    } else {
        *visited = count;
        // ScrollPanel_paint returns only viewport/chrome draw status: its
        // paintSubtree is void and does not propagate child draw status.
        (void) ScrollPanel_paint(sp, viewport);
    }
    // Offscreen Vulkan has no swapchain: Graphics_end submits, while present
    // deliberately returns false. Raster end likewise completes the frame.
    return Graphics_end();
}

static bool readGpu(uint8_t *pixels) {
    for (int i = 0; i < BENCH_WAIT_ATTEMPTS; i++) {
        if (VkGraphics_readback(BENCH_BYTES, pixels))
            return true;
    }
    return false;
}

static bool comparePixels(const uint8_t *a, const uint8_t *b, size_t bytes, unsigned tolerance) {
    for (size_t i = 0; i < bytes; i++) {
        int delta = (int) a[i] - (int) b[i];
        if (abs(delta) > (int) tolerance) {
            fprintf(stderr, "pixel mismatch byte %zu: %u vs %u\n", i, a[i], b[i]);
            return false;
        }
    }
    return true;
}

static bool measure(ScrollPanel *sp, Panel *content, size_t count,
                    const Rectangle *view, const char *position, const char *path,
                    bool indexed, bool gpu, uint8_t *reference, uint8_t *actual) {
    size_t visited = 0u;
    if (!Graphics_setGraphics(gpu ? LANG_BACKEND_VULKAN : LANG_BACKEND_RASTER))
        return false;
    double start = nowMs();
    if (!paintFrame(sp, content, count, view, indexed, &visited))
        return false;
    double submitMs = nowMs() - start;
    uint32_t draws = gpu ? VkGraphics_getDrawCount() : 0u;
    double completeMs = 0.0;
    if (gpu) {
        start = nowMs();
        if (!readGpu(actual)) {
            fputs("GPU readback timed out (bounded attempts)\n", stderr);
            return false;
        }
        completeMs = nowMs() - start;
        // Compare GPU traversal paths against each other, independently of
        // raster parity (GPU blends can differ from CPU by one channel unit).
        if (!indexed)
            memcpy(reference, actual, BENCH_BYTES);
        else if (!comparePixels(reference, actual, BENCH_BYTES, 1u))
            return false;
    } else {
        Image *image = RasterGraphics_getFramebuffer();
        if (!image || !Image_pixels(image))
            return false;
        if (!indexed && strcmp(position, "first") == 0) {
            const uint8_t *pixel = Image_pixels(image) + ((size_t) 10u * BENCH_WIDTH + 10u) * 4u;
            if (pixel[0] != 0xCFu || pixel[1] != 0x58u || pixel[2] != 0x3Au
                || pixel[3] != 0xFFu) {
                fputs("first real Panel was not painted\n", stderr);
                return false;
            }
        }
        if (!indexed)
            memcpy(reference, Image_pixels(image), BENCH_BYTES);
        else if (!comparePixels(reference, Image_pixels(image), BENCH_BYTES, 0u))
            return false;
    }
    printf("N=%zu position=%s path=%s backend=%s visited=%zu paint+submit=%.3fms "
           "completion+readback=%.3fms gpu_draw_calls=%u\n",
           count, position, path, gpu ? "vulkan" : "raster", visited,
           submitMs, completeMs, draws);
    return true;
}

int main(int argc, char **argv) {
    char *end = nullptr;
    errno = 0;
    unsigned long value = argc == 2 ? strtoul(argv[1], &end, 10) : 0u;
    if (argc != 2 || errno || end == argv[1] || *end != '\0'
        || value == 0u || value > BENCH_MAX) {
        fputs("usage: scroll_million_panels_bench N (1..1000000; try 10000, 100000 first)\n", stderr);
        return 2;
    }
    size_t count = (size_t) value;
    int status = 1;
    Device *device = nullptr;
    uint8_t *reference = (uint8_t*) malloc(BENCH_BYTES);
    uint8_t *actual = (uint8_t*) malloc(BENCH_BYTES);
    if (!reference || !actual || !Graphics_registerRow(RasterGraphics_getRow())
        || !Graphics_setGraphics(LANG_BACKEND_RASTER))
        goto done;
    double start = nowMs();
    ScrollPanel *sp = ScrollPanel_2((float) BENCH_WIDTH, (float) BENCH_HEIGHT);
    Panel *content = Panel_0();
    if (!sp || !content)
        goto done;
    // Pre-size the actual Panel child List; repeated 1024-slot growth copies
    // ~4 GB at N=1M. This is not a second index or a virtualized node tree.
    (*content).children = List(ID_LONG, count);
    if (!(*content).children)
        goto done;
    Panel_setSize(content, (float) BENCH_WIDTH, (float) (count * BENCH_PITCH));
    ScrollPanel_setContent(sp, content);
    if (ScrollPanel_getContentPanel(sp) != content)
        goto done;
    ScrollPanel_setBarVisible(sp, true, false);
    ScrollPanel_setBarVisible(sp, false, false);
    for (size_t i = 0; i < count; i++) {
        Panel *child = Panel_0();
        if (!child) {
            fprintf(stderr, "Panel allocation failed at %zu/%zu\n", i, count);
            goto done;
        }
        Panel_setLocation(child, 0.0f, (float) (i * BENCH_PITCH));
        Panel_setSize(child, (float) BENCH_WIDTH, (float) BENCH_CARD_HEIGHT);
        Panel_setBackgroundColor(child, (i & 1u) ? 0x287BC7FFu : 0xCF583AFFu);
        // List_add currently increments activeCount AFTER calling the checked
        // Collection_writeSlot, which rejects that index. Fill the public List
        // slot instead and mirror Panel_addContainer's parent/geometry edge.
        uint8_t *slot = List_addSlot((*content).children);
        if (!slot) {
            fprintf(stderr, "child List allocation failed at %zu/%zu\n", i, count);
            goto done;
        }
        uint64_t pointer = (uint64_t) (uintptr_t) child;
        memcpy(slot, &pointer, sizeof(pointer));
        (*child).parent = content;
        float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
        GraphicsComponent_getContentRect(&(*content).component, &x, &y, &w, &h);
        GraphicsComponent_setParentAbs(&(*child).component, x, y, w, h);
        if (Panel_childCount(content) != i + 1u || Panel_getChild(content, i) != child
            || Panel_getParent(child) != content) {
            fprintf(stderr, "Panel attachment failed at %zu/%zu\n", i, count);
            goto done;
        }
    }
    printf("N=%zu real_children=%zu sizeof(Panel)=%zu build=%.3fms peak_RSS=%.2fMiB\n",
           count, Panel_childCount(content), sizeof(Panel), nowMs() - start,
           (double) peakRssBytes() / (1024.0 * 1024.0));
    fflush(stdout);
    Rectangle view = { 0.0f, 0.0f, (float) BENCH_WIDTH, (float) BENCH_HEIGHT };
    const char *positions[] = { "first", "middle", "last", "scroll+64", "scroll-32" };
    float maxOffset = count * BENCH_PITCH > BENCH_HEIGHT
        ? (float) (count * BENCH_PITCH - BENCH_HEIGHT) : 0.0f;
    float offsets[] = { 0.0f, maxOffset * 0.5f, maxOffset,
                        fminf(maxOffset, 64.0f), 32.0f };
    bool gpu = Device_registerRow(Vulkan_row());
    if (gpu) {
        DeviceDesc desc = { .backend = LANG_BACKEND_VULKAN };
        device = Device_new(&desc);
        gpu = device && VkGraphics_bind(device) && Graphics_registerRow(VkGraphics_getRow());
    }
    if (!gpu)
        fputs("Vulkan unavailable; raster parity and timing only\n", stderr);
    else
        puts("gpu_draw_calls are batched vkCmdDraw calls, not visible Panel count");
    for (size_t p = 0; p < sizeof(positions) / sizeof(positions[0]); p++) {
        double movementStart = nowMs();
        if (p == 3u)
            ScrollPanel_setOffsetAt(sp, 0.0f, 0.0f, 100u);
        if (p < 3u)
            ScrollPanel_setOffsetAt(sp, 0.0f, offsets[p], (uint64_t) (p + 1u) * 100u);
        else
            ScrollPanel_scrollByAt(sp, 0.0f, p == 3u ? 64.0f : -32.0f,
                                   (uint64_t) (p + 1u) * 100u);
        float x = 0.0f, y = 0.0f;
        ScrollPanel_getOffset(sp, &x, &y);
        printf("N=%zu position=%s scroll_update=%.3fms offsetY=%.1f\n",
               count, positions[p], nowMs() - movementStart, (double) y);
        if (!measure(sp, content, count, &view, positions[p], "full", false, false, reference, actual)
            || !measure(sp, content, count, &view, positions[p], "indexed", true, false, reference, actual))
            goto done;
        if (gpu && (!measure(sp, content, count, &view, positions[p], "full", false, true, reference, actual)
            || !measure(sp, content, count, &view, positions[p], "indexed", true, true, reference, actual)))
            goto done;
    }
    status = 0;
done:
    if (status)
        fputs("benchmark failed (allocation, frame, or parity)\n", stderr);
    if (device) {
        VkGraphics_unbind();
        Device_destroy(device);
    }
    RasterGraphics_shutdown();
    free(reference);
    free(actual);
    return status;
}
