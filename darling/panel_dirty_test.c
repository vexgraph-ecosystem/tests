#include "annotation/overview.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "darling/container.h"
#include "darling/panel/panel.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: PanelDirtyTest (darling/panel/panel_dirty_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for the Panel tree + Container node model: null safety,
 * tree attach/detach structure, Container membership (add/remove/count/get),
 * and the retired dirtiness stubs (always clean, never crash).
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
    printf("=== Running Panel Dirty Test Suite ===\n");

    // section 1 Null safety (retired dirtiness stubs never crash, always clean)
    assert(Panel_isTreeDirty(nullptr) == false);
    Panel_clearTreeDirty(nullptr);

    // section 2 Single panel: always clean, clear is a safe no-op
    Panel *root = Panel_0();
    assert(root != nullptr);
    Panel_clearTreeDirty(root);
    assert(Panel_isTreeDirty(root) == false);

    // section 3 Multi-level hierarchy structure:
    // root -> child1 -> grandchild
    //      -> child2
    Panel *child1 = Panel_0();
    Panel *child2 = Panel_0();
    Panel *grandchild = Panel_0();
    assert(child1 != nullptr && child2 != nullptr && grandchild != nullptr);

    Panel_addContainer(root, child1);
    Panel_addContainer(root, child2);
    Panel_addContainer(child1, grandchild);

    assert(Panel_childCount(root) == 2u);
    assert(Panel_childCount(child1) == 1u);
    assert(Panel_childCount(child2) == 0u);
    assert(Panel_getChild(root, 0u) == child1);
    assert(Panel_getChild(root, 1u) == child2);
    assert(Panel_getChild(child1, 0u) == grandchild);
    assert(Panel_getParent(child1) == root);
    assert(Panel_getParent(grandchild) == child1);
    assert(Panel_containsChild(root, child2) == true);
    assert(Panel_containsChild(child2, grandchild) == false);

    // Detach keeps the tree consistent
    assert(Panel_removeChild(root, child2) == true);
    assert(Panel_childCount(root) == 1u);
    assert(Panel_getParent(child2) == nullptr);
    assert(Panel_containsChild(root, child2) == false);

    // Clearing the tree is a safe no-op at every level
    Panel_clearTreeDirty(root);
    assert(Panel_isTreeDirty(grandchild) == false);
    assert(Panel_isTreeDirty(child1) == false);
    assert(Panel_isTreeDirty(child2) == false);
    assert(Panel_isTreeDirty(root) == false);

    // section 4 Container node membership mirrors the tree metadata
    Container *node = Container_0();
    assert(node != nullptr);
    assert(Container_count(node) == 0u);
    assert(Container_add(node, &(*child1).component) == true);
    assert(Container_add(node, &(*grandchild).component) == true);
    assert(Container_count(node) == 2u);
    assert(Container_get(node, 0u) != nullptr);
    assert(Container_get(node, 2u) == nullptr);
    assert(Container_removeAt(node, 0u) == true);
    assert(Container_count(node) == 1u);
    Container_clear(node);
    assert(Container_count(node) == 0u);

    // section 5 Component metadata edits apply (opacity round-trips on the leaf)
    Component_setOpacity(&(*child2).component, 0.75f);
    assert(Component_getOpacity(&(*child2).component) == 0.75f);

    printf("=== Panel Dirty Test Suite: ALL PASS ===\n");
    return 0;
}
