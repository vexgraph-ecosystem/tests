// tests/graphvex/graphics/render_loop_test.c — mirrors src/graphics/render_loop.c
//
// Present-on-demand: a client presents only when it has demand (dirty or a
// newer generation). A resting client presents ZERO times.

#include <stdio.h>

#include "graphics/render_loop.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

static int g_presented = 0;
static int g_probed = 0;
static int g_window = 0;

static void probe(void *ud, double dt) { (void)ud; (void)dt; g_probed++; }
static bool present(void *window, double dt, void *ud) {
    (void)window; (void)dt; (void)ud;
    g_presented++;
    return true;
}

int main(void) {
    RenderLoop *loop = RenderLoop_0();
    CHECK(loop != NULL);
    CHECK(RenderLoop_findClient(loop, &g_window) == NULL);

    Client c = {0};
    c.window = &g_window;
    c.frameFn = probe;
    c.presentFn = present;
    CHECK(RenderLoop_addClient(loop, &c));
    CHECK(RenderLoop_findClient(loop, &g_window) != NULL);

    // no demand -> no present, but the probe still runs
    CHECK(!RenderLoop_step(loop));
    CHECK(g_presented == 0);
    CHECK(g_probed == 1);

    // markDirty -> exactly one present
    RenderLoop_markDirty(loop, &g_window);
    CHECK(RenderLoop_step(loop));
    CHECK(g_presented == 1);

    // demand consumed -> resting again
    CHECK(!RenderLoop_step(loop));
    CHECK(g_presented == 1);

    // a newer content generation alone demands a frame
    RenderLoop_setContentGen(loop, &g_window, 7);
    CHECK(RenderLoop_step(loop));
    CHECK(g_presented == 2);
    CHECK(!RenderLoop_step(loop));            // same generation, no demand

    // markDirty on an unknown window is a no-op
    int other = 0;
    RenderLoop_markDirty(loop, &other);
    CHECK(!RenderLoop_step(loop));

    CHECK(RenderLoop_removeClient(loop, &g_window));
    CHECK(RenderLoop_findClient(loop, &g_window) == NULL);
    CHECK(!RenderLoop_removeClient(loop, &g_window));   // already gone

    // null-safe
    CHECK(!RenderLoop_step(NULL));
    CHECK(RenderLoop_findClient(NULL, &g_window) == NULL);
    RenderLoop_notify(NULL);
    RenderLoop_free(NULL);
    RenderLoop_free(loop);

    printf("render_loop_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
