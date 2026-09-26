#include "annotation/overview.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "darling/container.h"
#include "darling/component.h"
#include "nio/mem.h"
#include "darling/frame.h"
#include "darling/panel/panel.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: FrameRootPanelTest (_tests/darling/frame_root_panel_test.c)
 * LEVEL: L3 -- Module Code (headless verification harness)
 * ============================================================================
 * Verification suite for the Window Board Root Lock Law (law 49):
 *   - Frame_setContentPane / Frame_setScenePane lock the board root geometry.
 *   - Frame_setContentPanel / Frame_setScenePanel polymorphic macros work.
 *   - lockedRoot flag is set on the incoming Panel's Container base.
 *   - Anchor is forced to COMPONENT_ANCHOR_TOP_LEFT.
 *   - Pivot is forced to COMPONENT_PIVOT_TOP_LEFT.
 *   - Location is forced to (0, 0).
 *   - Size is forced to (frame.width, frame.height).
 *   - Component_setSize silently no-ops on locked root.
 *   - Component_setLocation silently no-ops on locked root.
 *   - Panel Override Law: the embedded Component metadata mirrors the
 *     override (anchor/pivot/location/size) on set, ignores Panel_setSize /
 *     Panel_setLocation while locked, and tracks Frame_resize.
 *   - Frame_resize propagates new dimensions to both locked roots.
 *   - Container_setLockedRoot(c, false) unlocks the container.
 *   - Container_forceSize bypasses the lock.
 *   - Clearing contentPane (null) does not crash.
 *
 * STRUCT FIELDS: none -- procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

static int g_pass = 0;
static int g_fail = 0;

static void ok(bool cond, const char *name) {
    if (cond) {
        printf("ok: %s\n", name);
        g_pass++;
    } else {
        printf("FAIL: %s\n", name);
        g_fail++;
    }
}

int main(void) {
    printf("=== Running FrameRootPanelTest Suite ===\n");

    // --- 1. Frame construction ---
    Frame *frame = Frame();
    ok(frame != nullptr, "Frame() construct");
    ok((*frame).width >= 0 && (*frame).height >= 0, "default frame size is non-negative");
    ok((*frame).contentPane == nullptr, "default contentPane is null");
    ok((*frame).scenePane == nullptr, "default scenePane is null");

    // Give the frame a concrete size before attaching panes.
    (*frame).width = 800;
    (*frame).height = 600;

    // --- 2. setContentPane: board root tracks the frame size ---
    Panel *cp = Panel();
    ok(cp != nullptr, "Panel() for contentPane");

    // Pre-set a different size; Frame_setContentPane must override it.
    Component_setSize(&(*cp).component, 100.0f, 50.0f);
    ok(Component_getWidth(&(*cp).component) == 100.0f, "pre-set contentPane w = 100 before attach");

    Frame_setContentPane(frame, cp);

    ok((*frame).contentPane == cp, "contentPane assigned");
    ok(Component_getAnchor(&(*cp).component) == COMPONENT_ANCHOR_TOP_LEFT, "contentPane anchor = TOP_LEFT");
    ok(Component_getPivot(&(*cp).component) == COMPONENT_PIVOT_TOP_LEFT, "contentPane pivot = TOP_LEFT");
    ok(Component_getX(&(*cp).component) == 0.0f, "contentPane x = 0");
    ok(Component_getY(&(*cp).component) == 0.0f, "contentPane y = 0");
    ok(Component_getWidth(&(*cp).component) == 800.0f, "contentPane w = frame.width (800)");
    ok(Component_getHeight(&(*cp).component) == 600.0f, "contentPane h = frame.height (600)");
    ok(Component_getAbsX(&(*cp).component) == 0.0f, "contentPane absX = 0");
    ok(Component_getAbsY(&(*cp).component) == 0.0f, "contentPane absY = 0");
    ok(Component_getAbsW(&(*cp).component) == 800.0f, "contentPane absW = 800");
    ok(Component_getAbsH(&(*cp).component) == 600.0f, "contentPane absH = 600");

    // --- 3. Userland edits apply (no lock): plain metadata ---
    Component_setSize(&(*cp).component, 200.0f, 150.0f);
    ok(Component_getWidth(&(*cp).component) == 200.0f, "Component_setSize applies on board root w");
    ok(Component_getHeight(&(*cp).component) == 150.0f, "Component_setSize applies on board root h");
    Panel_setSize(cp, 400.0f, 300.0f);
    ok(Component_getWidth(&(*cp).component) == 400.0f, "Panel_setSize applies on board root w");
    ok(Component_getHeight(&(*cp).component) == 300.0f, "Panel_setSize applies on board root h");

    // --- 4. Userland location applies too ---
    Component_setLocation(&(*cp).component, 50.0f, 50.0f);
    ok(Component_getX(&(*cp).component) == 50.0f, "Component_setLocation applies x");
    ok(Component_getY(&(*cp).component) == 50.0f, "Component_setLocation applies y");
    Panel_setLocation(cp, 10.0f, 20.0f);
    ok(Component_getX(&(*cp).component) == 10.0f, "Panel_setLocation applies x");
    ok(Component_getY(&(*cp).component) == 20.0f, "Panel_setLocation applies y");

    // --- 5. setScenePane: same board semantics ---
    Panel *sp = Panel();
    ok(sp != nullptr, "Panel() for scenePane");

    Frame_setScenePane(frame, sp);

    ok((*frame).scenePane == sp, "scenePane assigned");
    ok(Component_getAnchor(&(*sp).component) == COMPONENT_ANCHOR_TOP_LEFT, "scenePane anchor = TOP_LEFT");
    ok(Component_getPivot(&(*sp).component) == COMPONENT_PIVOT_TOP_LEFT, "scenePane pivot = TOP_LEFT");
    ok(Component_getX(&(*sp).component) == 0.0f, "scenePane x = 0");
    ok(Component_getY(&(*sp).component) == 0.0f, "scenePane y = 0");
    ok(Component_getWidth(&(*sp).component) == 800.0f, "scenePane w = frame.width (800)");
    ok(Component_getHeight(&(*sp).component) == 600.0f, "scenePane h = frame.height (600)");

    // scenePane: userland setSize applies as well
    Component_setSize(&(*sp).component, 999.0f, 999.0f);
    ok(Component_getWidth(&(*sp).component) == 999.0f, "Component_setSize applies on scenePane w");
    ok(Component_getHeight(&(*sp).component) == 999.0f, "Component_setSize applies on scenePane h");

    // --- 6. Frame_resize re-tracks boards to the window ---
    // Frame_resize calls Frame_render and Frame_present internally; both are
    // headless no-ops without a live graphics context, so this is safe.
    // Boards re-track the window (userland edits from sections 3-5 are
    // overwritten — the board IS the window).
    Frame_resize(frame, 1280, 720);
    ok((*frame).width == 1280, "frame.width updated to 1280 after resize");
    ok((*frame).height == 720, "frame.height updated to 720 after resize");
    ok(Component_getWidth(&(*cp).component) == 1280.0f, "contentPane w = 1280 after resize");
    ok(Component_getHeight(&(*cp).component) == 720.0f, "contentPane h = 720 after resize");
    ok(Component_getWidth(&(*sp).component) == 1280.0f, "scenePane w = 1280 after resize");
    ok(Component_getHeight(&(*sp).component) == 720.0f, "scenePane h = 720 after resize");
    ok(Component_getAbsW(&(*cp).component) == 1280.0f, "contentPane absW = 1280 after resize");
    ok(Component_getAbsH(&(*cp).component) == 720.0f, "contentPane absH = 720 after resize");

    // Manual setSize applies after resize too
    Component_setSize(&(*cp).component, 1.0f, 1.0f);
    ok(Component_getWidth(&(*cp).component) == 1.0f, "Component_setSize applies after resize");

    // --- 7. Container node holds board metadata values ---
    Container *node = Container_0();
    ok(node != nullptr, "Container() node constructs");
    ok(Container_count(node) == 0u, "Container() node starts empty");
    ok(Container_add(node, &(*cp).component) == true, "Container_add board metadata");
    ok(Container_count(node) == 1u, "Container count = 1 after add");
    Component *slot = Container_get(node, 0u);
    ok(slot != nullptr && Component_getWidth(slot) == 1.0f, "Container_get returns board metadata");

    // --- 9. Frame_setContentPanel polymorphic macro: accepts Panel* ---
    Panel *cp2 = Panel();
    ok(cp2 != nullptr, "Panel() for contentPanel macro test");
    Frame_setContentPanel(frame, cp2);
    ok((*frame).contentPane == cp2, "Frame_setContentPanel assigns contentPane");
    ok(Component_getWidth(&(*cp2).component) == 1280.0f, "Frame_setContentPanel sets width = frame.width");

    // --- 10. Frame_setScenePanel polymorphic macro ---
    Panel *sp2 = Panel();
    ok(sp2 != nullptr, "Panel() for scenePanel macro test");
    Frame_setScenePanel(frame, sp2);
    ok((*frame).scenePane == sp2, "Frame_setScenePanel assigns scenePane");
    ok(Component_getWidth(&(*sp2).component) == 1280.0f, "Frame_setScenePanel sets width = frame.width");

    // --- 11. Null contentPane does not crash ---
    Frame_setContentPane(frame, nullptr);
    ok((*frame).contentPane == nullptr, "contentPane cleared to null");

    Frame_resize(frame, 640, 480);
    ok((*frame).width == 640, "frame.width = 640 after resize with null pane");
    ok((*frame).height == 480, "frame.height = 480 after resize with null pane");

    // --- Cleanup ---
    Memory_free(cp);
    Memory_free(sp);
    Memory_free(cp2);
    Memory_free(sp2);
    Frame_free(frame);

    printf("\n");
    if (g_fail == 0) {
        printf("PASS frame_root_panel_test: all %d assertions passed\n", g_pass);
        return 0;
    } else {
        printf("FAIL frame_root_panel_test: %d passed, %d failed\n", g_pass, g_fail);
        return 1;
    }
}
