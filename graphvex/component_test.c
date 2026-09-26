#include <math.h>
#include <stdio.h>

#include "annotation/overview.h"
#include "lang/component.h"
#include "lang/element_node.h"
#include "lang/graphics_panel.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ComponentTest (tests/graphvex/component_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the element model headless: Element_add links the tree (child->parent),
 * Component_layout resolves the absolute layout (recursing into children), the
 * dials track across resize steps, and the parent navigation
 * Component_getParent(ElementNode_getChildren(node, n)) reads back the owner.
 *
 * STRUCT FIELDS: none — procedural test harness.
 * ============================================================================
 */

#define EPS 0.001f

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

static bool approx(float a, float b) {
    return fabsf(a - b) <= EPS;
}

int main(void) {
    int failures = 0;

    // 1. A root GraphicsPanel with one anchored child GraphicsPanel.
    GraphicsPanel *root = GraphicsPanel_2("root", 0x1E1E24FFu);
    CHECK(GraphicsPanel_isValid(root));
    CHECK(Component_getName(GraphicsPanel_component(root))[0] == 'r');

    GraphicsPanel *br = GraphicsPanel_1(0xE0C050FFu);
    GraphicsPanel_setAnchor(br, GRAPHICS_COMPONENT_ANCHOR_BOTTOM_RIGHT);
    GraphicsPanel_setPivot(br, GRAPHICS_COMPONENT_PIVOT_BOTTOM_RIGHT);
    GraphicsPanel_setOrigin(br, GRAPHICS_COMPONENT_ORIGIN_BOTTOM_RIGHT);
    GraphicsPanel_setSize(br, 170.0f, 36.0f);
    GraphicsPanel_setLocation(br, 24.0f, 24.0f);

    CHECK(GraphicsPanel_add(root, GraphicsPanel_component(br)));   // root now owns br
    CHECK(Component_childCount(GraphicsPanel_component(root)) == 1);

    // 2. The tree link: child->parent is the root's Component.
    Component *child = ElementNode_getChildren(Component_getChildren(GraphicsPanel_component(root)), 0);
    CHECK(child == GraphicsPanel_component(br));
    CHECK(Component_getParent(child) == GraphicsPanel_component(root));

    // 3. Absolute layout tracks the parent's bottom-right across resize steps.
    Component_layout(GraphicsPanel_component(root), 0.0f, 0.0f, 1000.0f, 800.0f);
    CHECK(approx(GraphicsPanel_getAbsX(br), 1000.0f - 170.0f - 24.0f));
    CHECK(approx(GraphicsPanel_getAbsY(br), 800.0f - 36.0f - 24.0f));

    Component_layout(GraphicsPanel_component(root), 0.0f, 0.0f, 600.0f, 400.0f);
    CHECK(approx(GraphicsPanel_getAbsX(br), 600.0f - 170.0f - 24.0f));
    CHECK(approx(GraphicsPanel_getAbsY(br), 400.0f - 36.0f - 24.0f));

    // 4. Corner radius forwards to the primary GraphicsComponent.
    GraphicsPanel_setCornerRadius(br, 30.0f);
    CHECK(approx(GraphicsPanel_getCornerRadius(br), 30.0f));
    CHECK(approx(GraphicsComponent_getCornerRadius(Component_graphics(GraphicsPanel_component(br), 0)), 30.0f));

    // 5. hitTest resolves the child by its abs AABB (br is at 406,340 .. 576,376).
    uint32_t hit = ElementNode_hitTest(Component_getChildren(GraphicsPanel_component(root)),
                                       450.0f, 350.0f);
    CHECK(hit == 0);
    CHECK(ElementNode_hitTest(Component_getChildren(GraphicsPanel_component(root)), 0.0f, 0.0f) == UINT32_MAX);

    // 6. Component_zero() is the empty element.
    Component *zero = Component_zero();
    CHECK(zero != nullptr && Component_graphicsCount(zero) == 0);
    CHECK(Component_getParent(zero) == nullptr);
    Component_free(zero);

    // 7. viewMap maps an abs rect through a view (scale + origin) into native
    //    pixel currency: (abs - origin) * scale, floor/ceil on the span.
    float vx = 0.0f, vy = 0.0f, vw = 0.0f, vh = 0.0f;
    GraphicsComponentView view = { 2.0f, 2.0f, 10.0f, 10.0f };
    GraphicsComponent_viewMap(&view, 10.0f, 10.0f, 8.0f, 8.0f, &vx, &vy, &vw, &vh);
    CHECK(vx == 0.0f && vy == 0.0f && vw == 16.0f && vh == 16.0f);
    GraphicsComponent_viewMap(&view, 5.0f, 5.0f, 8.0f, 8.0f, &vx, &vy, &vw, &vh);
    CHECK(vx == -10.0f && vy == -10.0f && vw == 16.0f && vh == 16.0f);
    GraphicsComponent_viewMap(nullptr, 4.0f, 6.0f, 2.0f, 3.0f, &vx, &vy, &vw, &vh);
    CHECK(vx == 4.0f && vy == 6.0f && vw == 2.0f && vh == 3.0f);

    // 8. Cold seams.
    CHECK(Element_add(nullptr, child) == false);
    CHECK(Element_add(GraphicsPanel_component(root), nullptr) == false);
    CHECK(Component_getParent(nullptr) == nullptr);
    CHECK(ElementNode_count(nullptr) == 0);
    CHECK(GraphicsPanel_isValid(nullptr) == false);
    GraphicsPanel_free(nullptr);

    GraphicsPanel_free(root);   // frees br too (the root owns it)

    if (failures == 0)
        printf("PASS component_test: element model (add/parent/layout/forward/hit-test/viewMap)\n");
    else
        fprintf(stderr, "FAIL component_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
