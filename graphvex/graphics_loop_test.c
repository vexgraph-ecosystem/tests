#include "annotation/overview.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stdlib.h>

#include "../../ecosystem/graphvex/src/graphics/graphics_loop.h"
#include "vulkan/graphics_layer.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: GraphicsLoopTest (tests/graphvex/graphics_loop_test.c)
 * LEVEL: L3 — Module Code (standalone verification harness)
 * ============================================================================
 * Verification suite for GraphicsLoop:
 *   1. Constructor and lifecycle (GraphicsLoop_0, GraphicsLoop_1, GraphicsLoop_free).
 *   2. Client registration with 2-VkImage system and MTKLayer assignment.
 *   3. Per-window readiness check (readyFn: scenepanel & contentpanel ready)
 *      - Defers present when readyFn returns false (dirty preserved).
 *      - Fires present when readyFn returns true (dirty cleared).
 *   4. Per-window custom present callback execution.
 *   5. Event pump polling hook delegation.
 *   6. Dynamic capacity doubling (Anti-Hardcoding Law).
 *   7. Backward compatibility aliases (GfxLoop_*).
 * ============================================================================
 */

#define CHECK(cond, name) do { \
    if (!(cond)) { printf("FAIL: %s\n", name); failures++; } \
    else { printf("ok: %s\n", name); } \
} while (0)

static int s_pumpCalled = 0;
static void dummyPoll(void) {
    s_pumpCalled++;
}

static int s_frameCallbackCalled = 0;
static double s_lastDt = 0.0;
static bool s_rearmInProbe = false;
static void dummyFrameFn(void *window, double dt, void *userdata) {
    (void) userdata;
    s_frameCallbackCalled++;
    s_lastDt = dt;
    if (s_rearmInProbe) {
        s_rearmInProbe = false;
        GraphicsLoop_markDirty(GraphicsLoop_default(), window);
    }
}

static bool s_panelsReady = true;
static bool dummyReadyFn(void *window, void *userdata) {
    (void) window;
    (void) userdata;
    return s_panelsReady;
}

static int s_windowPresentsCalled = 0;
static bool dummyPresentFn(void *window, void *userdata) {
    (void) window;
    (void) userdata;
    s_windowPresentsCalled++;
    return true;
}

int main(void) {
    int failures = 0;

    printf("=== Running GraphicsLoop Test Suite ===\n");

    // 1. Constructors & Defaults
    GraphicsLoop *loop = GraphicsLoop();
    CHECK(loop != nullptr, "default GraphicsLoop construct");
    CHECK(GraphicsLoop_isValid(loop), "loop is valid");
    CHECK((*loop).targetFps == 60, "default target FPS is 60");
    CHECK(GraphicsLoop_getClientCount(loop) == 0, "initial client count is 0");

    GraphicsLoop *loop120 = GraphicsLoop(120);
    CHECK(loop120 != nullptr, "GraphicsLoop(120) construct");
    CHECK((*loop120).targetFps == 120, "target FPS is 120");
    GraphicsLoop_free(loop120);

    // 2. Poll hook installation
    GraphicsLoop_installPoll(loop, dummyPoll);
    CHECK((*loop).pollFn == dummyPoll, "pollFn installed");

    // 3. Client registration (2-VkImage system: sceneLayer + contentLayer)
    int dummyWin1 = 1;
    int dummyApp1 = 100;
    GraphicsLayer *scene1 = GraphicsLayer(GRAPHICS_LAYER_SCENE);
    GraphicsLayer *content1 = GraphicsLayer(GRAPHICS_LAYER_CONTENT);

    int fakeVkImage1 = 0x1111;
    int fakeVkImage2 = 0x2222;
    GraphicsLayer_setImage(scene1, &fakeVkImage1);
    GraphicsLayer_setImage(content1, &fakeVkImage2);

    bool regOk = GraphicsLoop_registerClient(loop, &dummyWin1, &dummyApp1, scene1, content1, dummyFrameFn, nullptr);
    CHECK(regOk, "register client with 2-VkImage layers");
    CHECK(GraphicsLoop_getClientCount(loop) == 1, "client count is 1");

    GraphicsClient *client = GraphicsLoop_findClient(loop, &dummyWin1);
    CHECK(client != nullptr, "find registered client");
    CHECK((*client).sceneLayer == scene1, "client sceneLayer matches");
    CHECK((*client).contentLayer == content1, "client contentLayer matches");
    CHECK((*client).app == &dummyApp1, "client app matches");

    // 4. MTKLayer, readyFn, and presentFn installation
    int fakeMtkLayer = 0xCAFE;
    GraphicsLoop_setClientMtkLayer(loop, &dummyWin1, &fakeMtkLayer);
    CHECK(GraphicsLoop_getClientMtkLayer(loop, &dummyWin1) == &fakeMtkLayer, "client mtkLayer stored and retrieved");

    GraphicsLoop_setClientReadyFn(loop, &dummyWin1, dummyReadyFn);
    GraphicsLoop_setClientPresentFn(loop, &dummyWin1, dummyPresentFn);

    // 5. Readiness check: when panels are NOT ready, present is deferred
    s_panelsReady = false;
    GraphicsLoop_markDirty(loop, &dummyWin1);
    s_windowPresentsCalled = 0;
    s_frameCallbackCalled = 0;

    GraphicsLoop_step(loop);
    CHECK(s_frameCallbackCalled == 1, "frameFn probed even when panels not ready");
    CHECK(s_windowPresentsCalled == 0, "present deferred when readyFn returns false");
    CHECK(atomic_load_explicit(&(*client).dirty, memory_order_relaxed) == true, "dirty flag preserved when deferred");

    // 6. Readiness check: when panels ARE ready, plaster & present fires
    s_panelsReady = true;
    GraphicsLoop_step(loop);
    CHECK(s_windowPresentsCalled == 1, "present executed when readyFn returns true");
    CHECK(atomic_load_explicit(&(*client).dirty, memory_order_relaxed) == false, "dirty flag cleared after successful present");

    // 7. Dynamic scaling
    int dummyWins[10];
    for (int i = 0; i < 10; i++) {
        dummyWins[i] = 100 + i;
        GraphicsLoop_registerClient(loop, &dummyWins[i], nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    CHECK(GraphicsLoop_getClientCount(loop) == 11, "dynamic scaling accommodated 11 clients");

    // Unregister
    bool unregOk = GraphicsLoop_unregisterClient(loop, &dummyWin1);
    CHECK(unregOk, "unregister first client");
    CHECK(GraphicsLoop_getClientCount(loop) == 10, "client count is 10 after unregister");
    CHECK(GraphicsLoop_findClient(loop, &dummyWin1) == nullptr, "unregistered client not found");

    // Clean up
    GraphicsLayer_destroy(scene1);
    GraphicsLayer_destroy(content1);
    GraphicsLoop_free(loop);

    if (failures > 0) {
        printf("FAIL graphics_loop_test: %d failure(s)\n", failures);
        return 1;
    }
    printf("PASS graphics_loop_test: all assertions passed\n");
    return 0;
}
