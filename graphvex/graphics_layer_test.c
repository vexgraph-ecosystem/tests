#include "annotation/overview.h"

#include <stdio.h>

#include "vulkan/graphics_layer.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: GraphicsLayerTest (tests/graphics_layer_test.c — board shim seam test)
 * LEVEL: L3 — Module Code (standalone verification harness)
 * ============================================================================
 * Cold-boundary matrix for the GraphicsLayer board shim (Rule 35.4): nullptr
 * guards, hostile sizes, role validation, attach/detach, and the resize
 * pulse (setPointSize raises, setPixelSize lowers). Headless — never touches
 * AppKit (the cocoa factory is exercised on-device, not here).
 *
 * STRUCT FIELDS: none — procedural (allocates, checks, frees, returns).
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

#define CHECK(cond, name) do { \
    if (!(cond)) { printf("FAIL: %s\n", name); failures++; } \
    else { printf("ok: %s\n", name); } \
} while (0)

static void *s_stubTopWindow = nullptr;
static void *s_stubTopLayer = nullptr;
static void *s_stubBottomWindow = nullptr;
static void *s_stubBottomLayer = nullptr;

static void stubSetTop(void *window, void *layer) {
    s_stubTopWindow = window;
    s_stubTopLayer = layer;
}

static void stubSetBottom(void *window, void *layer) {
    s_stubBottomWindow = window;
    s_stubBottomLayer = layer;
}

int main(void) {
    int failures = 0;
    int w = 0;
    int h = 0;

    GraphicsLayer *scene = GraphicsLayer();
    CHECK(scene != nullptr, "default construct scene");
    CHECK(GraphicsLayer_getRole(scene) == GRAPHICS_LAYER_SCENE, "default role scene");
    CHECK(!GraphicsLayer_isAttached(scene), "detached on construct");
    CHECK(!GraphicsLayer_needsResize(scene), "no resize pulse on construct");
    CHECK(GraphicsLayer_getLayer(scene) == nullptr, "null layer on construct");
    CHECK(GraphicsLayer_getDevice(scene) == nullptr, "null device on construct");

    GraphicsLayer *content = GraphicsLayer(GRAPHICS_LAYER_CONTENT);
    CHECK(content != nullptr, "content construct");
    CHECK(GraphicsLayer_getRole(content) == GRAPHICS_LAYER_CONTENT, "content role");

    GraphicsLayer *bad = GraphicsLayer(99);
    CHECK(bad != nullptr && GraphicsLayer_getRole(bad) == GRAPHICS_LAYER_SCENE, "bad role clamps scene");
    GraphicsLayer_destroy(bad);

    CHECK(!GraphicsLayer_attach(nullptr, (void*) 0x1), "attach null self false");
    CHECK(!GraphicsLayer_attach(scene, nullptr), "attach null layer false");
    CHECK(!GraphicsLayer_isAttached(scene), "still detached after null attach");

    CHECK(GraphicsLayer_isValid(scene), "live handle valid");
    CHECK(!GraphicsLayer_isValid(nullptr), "null handle invalid");
    int foreign = 42;
    CHECK(!GraphicsLayer_isValid((GraphicsLayer*) (void*) &foreign), "foreign struct invalid");

    int fakeWindow = 7;
    CHECK(!GraphicsLayer_bindWindow(scene, &fakeWindow, GRAPHICS_LAYER_CONTENT), "bind pre-seam fails closed");
    CHECK(!GraphicsLayer_bindWindow(nullptr, &fakeWindow, GRAPHICS_LAYER_CONTENT), "bind null self false");

    int fake = 42;
    CHECK(GraphicsLayer_attach(scene, &fake), "attach opaque handle");
    CHECK(GraphicsLayer_isAttached(scene), "attached after attach");
    CHECK(GraphicsLayer_getLayer(scene) == &fake, "layer round-trips opaque");
    CHECK(GraphicsLayer_needsResize(scene), "attach raises resize pulse");

    CHECK(GraphicsLayer_setPixelSize(scene, 1280, 800), "pixel commit");
    CHECK(!GraphicsLayer_needsResize(scene), "pixel commit lowers pulse");
    GraphicsLayer_getPixelSize(scene, &w, &h);
    CHECK(w == 1280 && h == 800, "pixel size round-trip");

    CHECK(!GraphicsLayer_setPixelSize(scene, 0, 800), "zero pixel width rejected");
    CHECK(!GraphicsLayer_setPixelSize(scene, 1280, -1), "negative pixel height rejected");
    GraphicsLayer_getPixelSize(scene, &w, &h);
    CHECK(w == 1280 && h == 800, "rejected commit keeps extent");

    CHECK(GraphicsLayer_setPointSize(scene, 640, 400), "point size chase");
    CHECK(GraphicsLayer_needsResize(scene), "point move raises pulse");
    GraphicsLayer_getPointSize(scene, &w, &h);
    CHECK(w == 640 && h == 400, "point size round-trip");
    CHECK(GraphicsLayer_setPointSize(scene, 640, 400), "same point size idempotent");
    CHECK(!GraphicsLayer_setPointSize(scene, 0, 400), "zero point width rejected");
    CHECK(!GraphicsLayer_setPointSize(nullptr, 640, 400), "null self rejected");

    GraphicsLayer_setScale(scene, 2.0f);
    CHECK(GraphicsLayer_getScale(scene) == 2.0f, "scale round-trip");
    GraphicsLayer_setScale(scene, 0.0f);
    CHECK(GraphicsLayer_getScale(scene) == 2.0f, "zero scale rejected");
    GraphicsLayer_setScale(scene, -1.0f);
    CHECK(GraphicsLayer_getScale(scene) == 2.0f, "negative scale rejected");

    GraphicsLayer_setDevice(scene, &fake);
    CHECK(GraphicsLayer_getDevice(scene) == &fake, "device round-trips opaque");

    GraphicsLayer_setRole(scene, GRAPHICS_LAYER_CONTENT);
    CHECK(GraphicsLayer_getRole(scene) == GRAPHICS_LAYER_CONTENT, "role setter");
    GraphicsLayer_setRole(scene, 99);
    CHECK(GraphicsLayer_getRole(scene) == GRAPHICS_LAYER_CONTENT, "bad role rejected");

    GraphicsLayer_installWindowBind(stubSetTop, stubSetBottom);
    CHECK(!GraphicsLayer_bindWindow(content, &fakeWindow, GRAPHICS_LAYER_CONTENT), "bind unattached false");
    CHECK(!GraphicsLayer_bindWindow(scene, nullptr, GRAPHICS_LAYER_CONTENT), "bind null window false");
    CHECK(!GraphicsLayer_bindWindow(scene, &fakeWindow, 99), "bind bad slot false");
    int fakeContent = 43;
    CHECK(GraphicsLayer_attach(content, &fakeContent), "content attach for bind");
    CHECK(GraphicsLayer_bindWindow(content, &fakeWindow, GRAPHICS_LAYER_CONTENT), "bind content routes top");
    CHECK(s_stubTopWindow == &fakeWindow && s_stubTopLayer == &fakeContent, "top seam got handle");
    CHECK(GraphicsLayer_bindWindow(scene, &fakeWindow, GRAPHICS_LAYER_SCENE), "bind scene routes bottom");
    CHECK(s_stubBottomWindow == &fakeWindow && s_stubBottomLayer == &fake, "bottom seam got handle");
    CHECK(!GraphicsLayer_isValid((GraphicsLayer*) (void*) &foreign), "foreign still invalid");

    GraphicsLayer_detach(scene);
    CHECK(!GraphicsLayer_isAttached(scene), "detached after detach");
    CHECK(GraphicsLayer_getLayer(scene) == nullptr, "null layer after detach");
    GraphicsLayer_detach(nullptr);

    CHECK(GraphicsLayer_getRole(nullptr) == GRAPHICS_LAYER_SCENE, "null role default");
    CHECK(GraphicsLayer_getScale(nullptr) == 1.0f, "null scale default");
    CHECK(!GraphicsLayer_isAttached(nullptr), "null attached false");
    CHECK(!GraphicsLayer_needsResize(nullptr), "null pulse false");
    CHECK(GraphicsLayer_getLayer(nullptr) == nullptr, "null layer null");
    GraphicsLayer_getPointSize(nullptr, &w, &h);
    CHECK(w == 0 && h == 0, "null point size zeros");
    GraphicsLayer_destroy(nullptr);

    GraphicsLayer_destroy(scene);
    GraphicsLayer_destroy(content);

    if (failures > 0) {
        printf("graphics_layer_test: %d failures\n", failures);
        return 1;
    }
    printf("graphics_layer_test: all pass\n");
    return 0;
}
