// raster_graphics_test.c — Unified Graphics seam + RasterGraphics row tests.
//
// Headless cold-boundary matrix (the Cold-Strict, Hot-Minimal Validation
// Law): nullptr/unknown-backend/zero-extent settings all degrade to false
// without crashing; the software row then proves pixel-exact output for
// clear, fillRect, drawRect, circles, clip, and image blit. No GPU, no
// AppKit — pure graphvex + vexspoke.

#include <stdio.h>
#include <string.h>

#include "graphics/graphics.h"
#include "raster/raster_graphics.h"
#include "vulkan/vk_graphics.h"
#include "metal/metal_graphics.h"
#include "buffer/buffer.h"
#include "lang/rect/rectangle.h"
#include "paint/brush.h"
#include "paint/stroke.h"
#include "image/image.h"
#include "vector/shape.h"

static int failures = 0;

static void expect(bool cond, const char *what) {
    if (!cond) {
        printf("FAIL: %s\n", what);
        failures++;
    } else {
        printf("ok:   %s\n", what);
    }
}

// Top-left origin pixel probe. Channels follow the Strict 0xRRGGBBAA Color
// Law: channel 0 = RED, 1 = GREEN, 2 = BLUE, 3 = ALPHA (putPixel stores
// (rgba >> 24) in channel 0 — red).
static uint32_t pixelAt(uint32_t x, uint32_t y) {
    Buffer *fb = RasterGraphics_getFramebuffer();
    if (!fb)
        return 0u;
    uint32_t ch0 = (uint32_t) Buffer_getPixel(fb, x, y, 0u); // red
    uint32_t ch1 = (uint32_t) Buffer_getPixel(fb, x, y, 1u); // green
    uint32_t ch2 = (uint32_t) Buffer_getPixel(fb, x, y, 2u); // blue
    uint32_t ch3 = (uint32_t) Buffer_getPixel(fb, x, y, 3u); // alpha
    return (ch0 << 24) | (ch1 << 16) | (ch2 << 8) | ch3; // 0xRRGGBBAA
}

int main(void) {
    // --- seam cold matrix ---------------------------------------------------
    expect(Graphics_getGraphicsId() == 0u, "no backend selected initially");
    expect(Graphics_getCurrent() != nullptr, "getCurrent never null");
    expect(Graphics_begin() == false, "forwarders cold-return false with no row");
    expect(Graphics_fillRect(nullptr, nullptr) == false, "fillRect cold-false with no row");
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_VULKAN) == false,
           "VULKAN row not registered yet -> setGraphics false");
    expect(Graphics_setGraphics(99u) == false, "unknown backend id -> false");

    // --- select Raster ------------------------------------------------------
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_RASTER) == true, "setGraphics(RASTER) succeeds");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_RASTER, "getGraphicsId reports RASTER");
    expect((*Graphics_getCurrent()).backendId == GRAPHICS_BACKEND_RASTER,
           "current row backendId is RASTER");

    // --- cold behavior before resize ---------------------------------------
    expect(Graphics_begin() == false, "begin false before resize");
    expect(Graphics_fillRect(nullptr, nullptr) == false, "verb null args false");
    Rectangle *r0 = Rectangle_4(0.0f, 0.0f, 10.0f, 10.0f);
    Brush *b0 = Brush_2(0xFF0000FFu, 1.0f);
    expect(Graphics_fillRect(r0, b0) == false, "fillRect false before resize");
    expect(Graphics_clear(0x000000FFu) == false, "clear false before resize");

    // --- bind the drawable ---------------------------------------------------
    expect(Graphics_resize(0u, 0u) == false, "resize rejects zero extent");
    expect(Graphics_resize(8u, 8u) == true, "resize 8x8 succeeds");
    expect(RasterGraphics_isReady() == true, "RasterGraphics ready");
    expect(RasterGraphics_getWidth() == 8u && RasterGraphics_getHeight() == 8u,
           "extent reported");
    expect(Graphics_begin() == true, "begin true after resize");
    expect(Graphics_end() == true, "end true after resize");
    expect(Graphics_present() == true, "present true after resize");

    // --- clear + fillRect ---------------------------------------------------
    expect(Graphics_clear(0x000000FFu) == true, "clear to black");
    expect(pixelAt(0u, 0u) == 0x000000FFu, "corner is black after clear");

    Rectangle *redRect = Rectangle_4(2.0f, 2.0f, 4.0f, 4.0f);
    expect(Graphics_fillRect(redRect, b0) == true, "fillRect ok");
    expect(pixelAt(3u, 3u) == 0xFF0000FFu, "inside red rect is red");
    expect(pixelAt(1u, 1u) == 0x000000FFu, "outside red rect stays black");

    // semi-transparent opacity: alpha channel scaled
    Brush *half = Brush_2(0x00FF00FFu, 0.5f);
    Rectangle *greenRect = Rectangle_4(0.0f, 0.0f, 8u, 1u);
    Graphics_fillRect(greenRect, half);
    uint32_t gpx = pixelAt(4u, 0u);
    expect(((gpx & 0xFFu) == 0x80u), "opacity 0.5 scales alpha to 0x80");

    // --- clip ---------------------------------------------------------------
    expect(Graphics_clip(nullptr) == true, "clip reset ok");
    Rectangle *clipRect = Rectangle_4(0.0f, 0.0f, 2.0f, 2.0f);
    expect(Graphics_clip(clipRect) == true, "clip set ok");
    Graphics_clear(0x000000FFu);
    Rectangle *big = Rectangle_4(-1.0f, -1.0f, 16.0f, 16.0f);
    Graphics_fillRect(big, b0);
    expect(pixelAt(0u, 0u) == 0xFF0000FFu, "pixel within clip painted");
    expect(pixelAt(7u, 7u) == 0x000000FFu, "pixel outside clip untouched");
    expect(Graphics_clip(nullptr) == true, "clip reset restores full drawable");

    // --- drawRect -----------------------------------------------------------
    Graphics_clear(0x000000FFu);
    Stroke *blackStroke = Stroke_2(2.0f, 0xFFFFFFFFu);
    Rectangle *frame = Rectangle_4(1.0f, 1.0f, 6.0f, 6.0f);
    expect(Graphics_drawRect(frame, blackStroke) == true, "drawRect ok");
    expect(pixelAt(2u, 1u) == 0xFFFFFFFFu, "top band painted");
    expect(pixelAt(1u, 2u) == 0xFFFFFFFFu, "left band painted");
    expect(pixelAt(4u, 4u) == 0x000000FFu, "interior stays black");
    expect(pixelAt(0u, 0u) == 0x000000FFu, "outside stays black");

    // --- circles ------------------------------------------------------------
    Graphics_clear(0x000000FFu);
    expect(Graphics_fillCircle(4.0f, 4.0f, 2.0f, b0) == true, "fillCircle ok");
    expect(pixelAt(4u, 4u) == 0xFF0000FFu, "circle center painted");
    expect(pixelAt(0u, 0u) == 0x000000FFu, "far corner outside circle");

    Graphics_clear(0x000000FFu);
    expect(Graphics_drawCircle(4.0f, 4.0f, 2.0f, blackStroke) == true, "drawCircle ok");
    expect(pixelAt(4u, 1u) == 0xFFFFFFFFu, "ring top painted");
    expect(pixelAt(4u, 4u) == 0x000000FFu, "ring interior stays black");

    // --- path fill (rect path via Shape) ------------------------------------
    Graphics_clear(0x000000FFu);
    Shape *path = Shape_4(2.0f, 2.0f, 4.0f, 4.0f);
    expect(Graphics_fillPath(path, b0) == true, "fillPath ok");
    expect(pixelAt(3u, 3u) == 0xFF0000FFu, "path interior painted");
    expect(pixelAt(1u, 1u) == 0x000000FFu, "path exterior stays black");
    expect(Graphics_drawPath(path, blackStroke) == true, "drawPath ok");

    // --- image blit ----------------------------------------------------------
    Graphics_clear(0x000000FFu);
    Image *img = Image_2(2u, 2u);
    // Upload RGBA8 (byte0=red, byte1=green, byte2=blue, byte3=alpha) per the
    // Strict 0xRRGGBBAA Color Law; (0,0) = opaque red.
    uint8_t rgba[16];
    for (size_t i = 0; i < 16u; i++) rgba[i] = 0u;
    rgba[0] = 255u;      // red
    rgba[1] = 0u;        // green
    rgba[2] = 0u;        // blue
    rgba[3] = 255u;      // alpha
    Image_upload(rgba, 2u, 2u, img);
    Rectangle *dst = Rectangle_4(0.0f, 0.0f, 8.0f, 8.0f);
    expect(Graphics_drawImage(img, dst) == true, "drawImage ok");
    expect(pixelAt(0u, 0u) == 0xFF0000FFu, "image top-left blitted");
    expect(pixelAt(3u, 3u) == 0xFF0000FFu, "image scaled region red");

    // --- VkGraphics row (headless: no device/surface — selection only) ------
    expect(VkGraphics_0() != nullptr, "VkGraphics_0 registers the singleton");
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_VULKAN) == true,
           "setGraphics(VULKAN) succeeds once registered");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_VULKAN, "getGraphicsId reports VULKAN");
    expect((*Graphics_getCurrent()).backendId == GRAPHICS_BACKEND_VULKAN,
           "current row backendId is VULKAN");
    expect(Graphics_resize(640u, 480u) == true, "VK resize records extent without device");
    expect(VkGraphics_getWidth() == 640u && VkGraphics_getHeight() == 480u, "VK extent reported");
    expect(Graphics_begin() == false, "begin cold-false: no live device headless");
    expect(Graphics_clear(0x102030FFu) == false, "clear cold-false without device");
    expect(Graphics_present() == false, "present cold-false without device");
    expect(Graphics_end() == true, "VK end closes the frame window (bookkeeping)");
    expect(Graphics_fillRect(r0, b0) == false, "VK verbs draft-false (no pass yet)");
    expect(Graphics_drawImage(img, dst) == false, "VK drawImage draft-false");
    expect(Graphics_clip(clipRect) == false, "VK clip draft-false");
    // switching back to Raster keeps the software row fully live
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_RASTER) == true, "switch back to RASTER");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_RASTER, "getGraphicsId reports RASTER again");
    expect(Graphics_clear(0x000000FFu) == true, "RASTER row live after switch back");

    // --- MetalGraphics stub row (secondary backend; cold-false only) ------
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_METAL) == false,
           "METAL row not registered yet -> setGraphics false");
    expect(MetalGraphics_0() != nullptr, "MetalGraphics_0 registers the singleton");
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_METAL) == true,
           "setGraphics(METAL) succeeds once registered");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_METAL, "getGraphicsId reports METAL");
    expect((*Graphics_getCurrent()).backendId == GRAPHICS_BACKEND_METAL,
           "current row backendId is METAL");
    expect(Graphics_begin() == false, "METAL begin cold-false (;;INCOMPLETE)");
    expect(Graphics_resize(640u, 480u) == false, "METAL resize cold-false (;;INCOMPLETE)");
    expect(Graphics_clear(0x000000FFu) == false, "METAL clear cold-false (;;INCOMPLETE)");
    expect(Graphics_present() == false, "METAL present cold-false (;;INCOMPLETE)");
    expect(Graphics_fillRect(r0, b0) == false, "METAL verbs cold-false (;;INCOMPLETE)");
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_VULKAN) == true, "switch back to VULKAN");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_VULKAN, "getGraphicsId reports VULKAN again");
    expect(Graphics_setGraphics(GRAPHICS_BACKEND_RASTER) == true, "and back to RASTER");
    expect(Graphics_getGraphicsId() == GRAPHICS_BACKEND_RASTER, "getGraphicsId reports RASTER");
    expect(Graphics_clear(0x000000FFu) == true, "RASTER row still live after all switches");

    // --- cleanup -------------------------------------------------------------
    Stroke_free(blackStroke);
    Brush_free(half);
    Brush_free(b0);
    Rectangle_free(clipRect);
    Rectangle_free(big);
    Rectangle_free(frame);
    Rectangle_free(greenRect);
    Rectangle_free(redRect);
    Rectangle_free(r0);
    Shape_free(path);
    Image_free(img);

    printf(failures == 0 ? "raster_graphics_test: ALL PASS\n" : "raster_graphics_test: %d FAILURES\n", failures);
    return failures == 0 ? 0 : 1;
}