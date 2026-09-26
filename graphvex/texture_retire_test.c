#include "annotation/overview.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "vulkan/texture/texture.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: TextureRetireTest (graphvex/texture_retire_test.c — retire-ring contract)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless contract suite for the Texture bounded retire ring (Rule 39):
 * cold-validation rejects (nullptr / wrong-id / zero-size / overflow) must
 * degrade-false without a device; the ring capacity/depth/frame clock must
 * report their headless defaults; shutdown with no flight (and double
 * shutdown) must return promptly without touching the driver.
 *
 * Same-size fast-path vs resize-retire-path vs timeout-cancelled classification
 * runs at cold seams WITH a live device (integration); here the suite pins
 * the degrade-false half of the contract: no device => -1/false/0, never a
 * crash, never a hang, never an unbounded wait (Rule 27).
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
    printf("=== Running Texture Retire Test Suite ===\n");

    // section 1 Headless ring defaults: no device => depth 0, capacity 8, seq 0.
    assert(Texture_retireCapacity() == 8);
    assert(Texture_retireDepth() == 0);
    assert(Texture_frameSeq() == 0);
    assert(Texture_maxBoundId() >= 0);
    assert(Texture_isReady() == false);

    // section 2 nullptr safety: every entry degrades-false, never dereferences.
    uint32_t w = 0;
    uint32_t h = 0;
    assert(Texture_getSize(-1, &w, &h) == false);
    assert(Texture_updateSubRaw(-1, nullptr, 0, 0, 4, 4) == false);
    assert(Texture_replaceRaw(-1, nullptr, 4, 4) == -1);
    assert(Texture_loadRaw(nullptr, 4, 4) == -1);
    assert(Texture_load(nullptr) == -1);
    Texture_free(-1);

    // section 3 wrong-id safety: far-OOB ids never reach the bindless array.
    assert(Texture_getSize(9999, &w, &h) == false);
    assert(Texture_updateSubRaw(9999, nullptr, 0, 0, 4, 4) == false);
    assert(Texture_replaceRaw(9999, nullptr, 4, 4) == -1);
    Texture_free(9999);

    // section 4 zero-size / overflow rejects (cold validation, Rule 35): a live
    // 4x4 white probe with hostile extents must fail loudly (-1), and a
    // zero extent must fail even with valid bytes.
    static uint8_t probe[4 * 4 * 4] = {0};
    assert(Texture_loadRaw(probe, 0, 4) == -1);
    assert(Texture_loadRaw(probe, 4, 0) == -1);
    assert(Texture_loadRaw(probe, UINT32_MAX, UINT32_MAX) == -1);
    assert(Texture_replaceRaw(0, probe, 0, 4) == -1);
    assert(Texture_replaceRaw(0, probe, UINT32_MAX, UINT32_MAX) == -1);
    assert(Texture_updateSubRaw(0, probe, 0, 0, UINT32_MAX, 4) == false);

    // section 5 same-size vs resize-while-pending contract without a device: both
    // paths degrade-false (-1) and leave the ring empty — nothing published,
    // nothing retired, nothing wedged.
    assert(Texture_replaceRaw(0, probe, 4, 4) == -1);
    assert(Texture_retireDepth() == 0);
    assert(Texture_frameSeq() == 0);

    // section 6 shutdown-with-flight contract: shutdown with no device drains
    // nothing, touches no driver, and resets the clock; double shutdown
    // stays safe (Rule 26 teardown order proof, headless half).
    Texture_shutdown();
    assert(Texture_retireDepth() == 0);
    assert(Texture_frameSeq() == 0);
    Texture_shutdown();
    assert(Texture_retireDepth() == 0);
    Texture_free(0);

    printf("=== Texture Retire Test Suite: ALL PASS ===\n");
    return 0;
}
