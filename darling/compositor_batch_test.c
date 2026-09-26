#include "annotation/overview.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "darling/compositor.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: CompositorBatchTest (darling/compositor_batch_test.c — layer-flight contract)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless contract suite for the Compositor layer-flight queries (Rule 39):
 * with no device and no flight the composer reports settled + idle-for-resize;
 * shutdown with no flight touches no driver (Rule 26/27 teardown proof,
 * headless half).
 *
 * Flight-submit waits (bounded 100ms) run at tick seams WITH a live device
 * (integration); here the suite pins the degrade-idle half: no flight =>
 * settled, never a hang, never an unbounded wait.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

int main(void) {
    printf("=== Running Compositor Layer-Flight Test Suite ===\n");

    // #1 Headless flight defaults: no flight => settled + idle.
    assert(Darling_compositorSettled() == true);
    assert(Darling_compositorIdleForResize() == true);

    // #2 Shutdown-with-flight contract: shutdown with no device drains
    // nothing, touches no driver (Rule 26/27, headless half); double
    // shutdown stays safe.
    Darling_shutdownCompositor();
    assert(Darling_compositorSettled() == true);
    assert(Darling_compositorIdleForResize() == true);
    Darling_shutdownCompositor();
    assert(Darling_compositorSettled() == true);

    // #3 Resize-gate coherence: idle-for-resize tracks settled exactly —
    // resize-class work may proceed only when the drain query agrees.
    assert(Darling_compositorIdleForResize() == Darling_compositorSettled());

    printf("=== Compositor Layer-Flight Test Suite: ALL PASS ===\n");
    return 0;
}
