#include "annotation/overview.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "c23/darling-type.h"
#include "darling/component.h"
#include "darling/container.h"
#include "oop/type.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ComponentTest (tests/darling/component_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for the new-architecture Component leaf (immediate on-demand
 * rendering): the eager abs cascade (every geometry setter recomputes abs
 * immediately), the 9-grid anchor lattice against independent expectations,
 * pivot/margin/padding/clamp contracts, and the visible-gated render hooks
 * feeding the Present-On-Demand Law.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

static void expectAbs(const Component *c, float ex, float ey, float ew, float eh) {
    assert(Component_getAbsX(c) == ex);
    assert(Component_getAbsY(c) == ey);
    assert(Component_getAbsW(c) == ew);
    assert(Component_getAbsH(c) == eh);
}




int main(void) {
    printf("=== Running Component Test Suite ===\n");

    // 1 Null safety: every core function and getter degrades, never crashes.
    Component_recompute(nullptr);
    Component_setParentAbs(nullptr, 1.0f, 2.0f, 3.0f, 4.0f);
    assert(Component_hitTest(nullptr, 0.0f, 0.0f) == false);
    Component_getContentRect(nullptr, nullptr, nullptr, nullptr, nullptr);
    float l, t, r, b;
    Component_getMargin(nullptr, &l, &t, &r, &b);
    assert(l == 0.0f && t == 0.0f && r == 0.0f && b == 0.0f);
    Component_getPadding(nullptr, &l, &t, &r, &b);
    assert(l == 0.0f && t == 0.0f && r == 0.0f && b == 0.0f);
    Vec4 rect;
    Component_getAbsRect(nullptr, &rect);
    assert(rect.x == 0.0f && rect.y == 0.0f && rect.z == 0.0f && rect.w == 0.0f);
    Component_getParentAbsRect(nullptr, &rect);
    assert(rect.x == 0.0f && rect.y == 0.0f && rect.z == 0.0f && rect.w == 0.0f);
    assert(Component_getX(nullptr) == 0.0f && Component_getY(nullptr) == 0.0f);
    assert(Component_getWidth(nullptr) == 0.0f && Component_getHeight(nullptr) == 0.0f);
    assert(Component_getAbsX(nullptr) == 0.0f && Component_getAbsY(nullptr) == 0.0f);
    assert(Component_getMinWidth(nullptr) == 0.0f && Component_getMaxHeight(nullptr) == 0.0f);
    assert(Component_getAnchor(nullptr) == COMPONENT_ANCHOR_TOP_LEFT);
    assert(Component_getPivot(nullptr) == COMPONENT_PIVOT_TOP_LEFT);
    assert(Component_getOrigin(nullptr) == COMPONENT_ORIGIN_TOP_LEFT);
    assert(Component_getOpacity(nullptr) == 1.0f);
    assert(Component_getZ(nullptr) == 0);
    assert(Component_isVisible(nullptr) == false);
    Component_setOrigin(nullptr, COMPONENT_ORIGIN_BOTTOM_RIGHT);
    Component_setX(nullptr, 5.0f);
    Component_setSize(nullptr, 5.0f, 5.0f);
    Component_setVisible(nullptr, false);
    printf("  [ok] null safety\n");

    // 2 Defaults: detached leaf at origin, TOP_LEFT, visible, opaque, clear.
    Component *c = Component_0();
    assert(c != nullptr);
    assert(Component_getX(c) == 0.0f && Component_getY(c) == 0.0f);
    assert(Component_getWidth(c) == 0.0f && Component_getHeight(c) == 0.0f);
    expectAbs(c, 0.0f, 0.0f, 0.0f, 0.0f);
    assert(Component_getOrigin(c) == COMPONENT_ORIGIN_TOP_LEFT);
    assert(Component_getAnchor(c) == COMPONENT_ANCHOR_TOP_LEFT);
    assert(Component_getPivot(c) == COMPONENT_PIVOT_TOP_LEFT);
    assert(Component_isVisible(c) == true);
    assert(Component_getOpacity(c) == 1.0f);
    Component_getMargin(c, &l, &t, &r, &b);
    assert(l == 0.0f && t == 0.0f && r == 0.0f && b == 0.0f);
    Component_getPadding(c, &l, &t, &r, &b);
    assert(l == 0.0f && t == 0.0f && r == 0.0f && b == 0.0f);
    assert(Component_getBorderWidth(c) == 0.0f);
    assert(Component_getBorderColor(c) == COMPONENT_COLOR_CLEAR);
    assert(Component_getBackgroundColor(c) == COMPONENT_COLOR_CLEAR);
    assert(Component_getRadius(c) == 0.0f);
    assert(Component_getRadiusMode(c) == COMPONENT_CORNER_ARC);
    printf("  [ok] constructor defaults\n");

    // 3 Type identity: COMPONENT is a darling singleton, class 56, root.
    assert(Type_arch(TYPE_COMPONENT_SINGLETON) == ARCH_DARLING);
    assert(Type_class(TYPE_COMPONENT_SINGLETON) == ID_COMPONENT);
    printf("  [ok] type identity (ID_COMPONENT = %u)\n", ID_COMPONENT);

    // 4 Eager abs on a parentless leaf: every geometry setter recomputes now.
    Component_setLocation(c, 5.0f, 10.0f);
    expectAbs(c, 5.0f, 10.0f, 0.0f, 0.0f);
    Component_setSize(c, 100.0f, 40.0f);
    expectAbs(c, 5.0f, 10.0f, 100.0f, 40.0f);
    Component_setX(c, 9.0f);
    expectAbs(c, 9.0f, 10.0f, 100.0f, 40.0f);
    Component_setY(c, 12.0f);
    expectAbs(c, 9.0f, 12.0f, 100.0f, 40.0f);
    printf("  [ok] parentless eager abs\n");

    // 5 Parent box: child placement resolves against the parent's abs.
    Component *child = Component_0();
    Component_setParentAbs(child, 10.0f, 20.0f, 400.0f, 300.0f);
    Component_setLocation(child, 5.0f, 5.0f);
    Component_setSize(child, 50.0f, 50.0f);
    expectAbs(child, 15.0f, 25.0f, 50.0f, 50.0f);
    // Parent moves -> parent reports its new abs -> child recomputes eagerly.
    Component_setParentAbs(child, 100.0f, 200.0f, 400.0f, 300.0f);
    expectAbs(child, 105.0f, 205.0f, 50.0f, 50.0f);
    Component_getParentAbsRect(child, &rect);
    assert(rect.x == 100.0f && rect.y == 200.0f && rect.z == 400.0f && rect.w == 300.0f);
    printf("  [ok] parent abs cascade (eager)\n");

    // 6 Anchor + Pivot + Origin combinations:
    // Testing corner & center anchors with matching origin and pivot.
    const float W = 400.0f, H = 300.0f, S = 50.0f, mx = 16.0f, my = 16.0f;
    struct LatticeRow { int origin; int anchor; int pivot; float ex, ey; };
    struct LatticeRow lattice[5] = {
        { COMPONENT_ORIGIN_TOP_LEFT,      COMPONENT_ANCHOR_TOP_LEFT,      COMPONENT_PIVOT_TOP_LEFT,     mx,         my },
        { COMPONENT_ORIGIN_TOP_RIGHT,     COMPONENT_ANCHOR_TOP_RIGHT,     COMPONENT_PIVOT_TOP_RIGHT,    W - S - mx, my },
        { COMPONENT_ORIGIN_TOP_LEFT,      COMPONENT_ANCHOR_MIDDLE_CENTER, COMPONENT_PIVOT_CENTER,       W*0.5f - S*0.5f + mx, H*0.5f - S*0.5f + my },
        { COMPONENT_ORIGIN_BOTTOM_LEFT,   COMPONENT_ANCHOR_BOTTOM_LEFT,   COMPONENT_PIVOT_BOTTOM_LEFT,  mx,         H - S - my },
        { COMPONENT_ORIGIN_BOTTOM_RIGHT,  COMPONENT_ANCHOR_BOTTOM_RIGHT,  COMPONENT_PIVOT_BOTTOM_RIGHT, W - S - mx, H - S - my },
    };
    Component *cell = Component_0();
    Component_setParentAbs(cell, 0.0f, 0.0f, W, H);
    Component_setLocation(cell, mx, my);
    Component_setSize(cell, S, S);
    for (size_t i = 0; i < 5; i++) {
        Component_setOrigin(cell, lattice[i].origin);
        Component_setAnchor(cell, lattice[i].anchor);
        Component_setPivot(cell, lattice[i].pivot);
        expectAbs(cell, lattice[i].ex, lattice[i].ey, S, S);
    }
    printf("  [ok] 5-anchor corner & center matrix\n");

    // 6b Decoupled 9-grid anchor lattice (with pivot TOP_LEFT and origin TOP_LEFT)
    // Proves anchor point alone places the element at (Ua * W, Va * H) + loc
    struct AnchorRow { int anchor; float ex, ey; };
    struct AnchorRow anchorGrid[9] = {
        { COMPONENT_ANCHOR_TOP_LEFT,       mx,                 my },
        { COMPONENT_ANCHOR_TOP_CENTER,     W * 0.5f + mx,      my },
        { COMPONENT_ANCHOR_TOP_RIGHT,      W + mx,             my },
        { COMPONENT_ANCHOR_MIDDLE_LEFT,    mx,                 H * 0.5f + my },
        { COMPONENT_ANCHOR_MIDDLE_CENTER,  W * 0.5f + mx,      H * 0.5f + my },
        { COMPONENT_ANCHOR_MIDDLE_RIGHT,   W + mx,             H * 0.5f + my },
        { COMPONENT_ANCHOR_BOTTOM_LEFT,    mx,                 H + my },
        { COMPONENT_ANCHOR_BOTTOM_CENTER,  W * 0.5f + mx,      H + my },
        { COMPONENT_ANCHOR_BOTTOM_RIGHT,   W + mx,             H + my },
    };
    Component_setOrigin(cell, COMPONENT_ORIGIN_TOP_LEFT);
    Component_setPivot(cell, COMPONENT_PIVOT_TOP_LEFT);
    for (size_t i = 0; i < 9; i++) {
        Component_setAnchor(cell, anchorGrid[i].anchor);
        expectAbs(cell, anchorGrid[i].ex, anchorGrid[i].ey, S, S);
    }
    printf("  [ok] 9-grid anchor lattice (decoupled)\n");

    // 6c User Case Proof: origin BOTTOM_RIGHT + pivot BOTTOM_RIGHT + anchor BOTTOM_RIGHT + loc(10,10)
    Component *userHud = Component_0();
    Component_setParentAbs(userHud, 0.0f, 0.0f, 480.0f, 320.0f);
    Component_setSize(userHud, 130.0f, 40.0f);
    Component_setOrigin(userHud, COMPONENT_ORIGIN_BOTTOM_RIGHT);
    Component_setAnchor(userHud, COMPONENT_ANCHOR_BOTTOM_RIGHT);
    Component_setPivot(userHud, COMPONENT_PIVOT_BOTTOM_RIGHT);
    Component_setLocation(userHud, 10.0f, 10.0f);
    // 480 - 130 - 10 = 340; 320 - 40 - 10 = 270
    expectAbs(userHud, 340.0f, 270.0f, 130.0f, 40.0f);
    assert(480.0f - (Component_getAbsX(userHud) + Component_getAbsW(userHud)) == 10.0f);
    assert(320.0f - (Component_getAbsY(userHud) + Component_getAbsH(userHud)) == 10.0f);
    printf("  [ok] user hud origin/anchor/pivot/loc verification\n");

    // 7 Pivot: CENTER pivot shifts the placement point universally across anchors.
    Component *piv = Component_0();
    Component_setParentAbs(piv, 0.0f, 0.0f, 400.0f, 300.0f);
    Component_setPivot(piv, COMPONENT_PIVOT_CENTER);
    Component_setSize(piv, 50.0f, 50.0f);
    expectAbs(piv, -25.0f, -25.0f, 50.0f, 50.0f);
    Component_setCenter(piv);
    expectAbs(piv, -25.0f, -25.0f, 50.0f, 50.0f);
    // Center pivot on MIDDLE_CENTER anchor: (200 - 25, 150 - 25) = (175, 125)
    Component_setAnchor(piv, COMPONENT_ANCHOR_MIDDLE_CENTER);
    expectAbs(piv, 175.0f, 125.0f, 50.0f, 50.0f);
    printf("  [ok] pivot center placement across anchors\n");

    // 8 Margin: additively offsets the resolved placement; TL +x/+y inward.
    Component *mg = Component_0();
    Component_setParentAbs(mg, 0.0f, 0.0f, 400.0f, 300.0f);
    Component_setLocation(mg, 5.0f, 5.0f);
    Component_setMargin(mg, 8.0f, 10.0f, 2.0f, 3.0f);
    expectAbs(mg, 13.0f, 15.0f, 0.0f, 0.0f);
    Component_getMargin(mg, &l, &t, &r, &b);
    assert(l == 8.0f && t == 10.0f && r == 2.0f && b == 3.0f);
    // TR parity: origin TOP_RIGHT, anchor TOP_RIGHT, pivot TOP_RIGHT; x/y is edge inset; margin adds.
    Component *mt = Component_0();
    Component_setParentAbs(mt, 0.0f, 0.0f, 400.0f, 300.0f);
    Component_setOrigin(mt, COMPONENT_ORIGIN_TOP_RIGHT);
    Component_setAnchor(mt, COMPONENT_ANCHOR_TOP_RIGHT);
    Component_setPivot(mt, COMPONENT_PIVOT_TOP_RIGHT);
    Component_setLocation(mt, 8.0f, 8.0f);
    Component_setMargin(mt, 4.0f, 4.0f, 0.0f, 0.0f);
    Component_setSize(mt, 50.0f, 50.0f);
    expectAbs(mt, 346.0f, 12.0f, 50.0f, 50.0f);
    printf("  [ok] margin placement (TL additive + TR parity)\n");

    // 9 Size constraints: setSize clamps [min, max]; min/max re-clamp current.
    Component *sz = Component_0();
    Component_setMaxSize(sz, 100.0f, 100.0f);
    Component_setSize(sz, 200.0f, 200.0f);
    assert(Component_getWidth(sz) == 100.0f && Component_getHeight(sz) == 100.0f);
    Component_setMinSize(sz, 50.0f, 50.0f);
    Component_setSize(sz, 10.0f, 10.0f);
    assert(Component_getWidth(sz) == 50.0f && Component_getHeight(sz) == 50.0f);
    Component_setSize(sz, 30.0f, 30.0f);
    assert(Component_getWidth(sz) == 50.0f && Component_getHeight(sz) == 50.0f); // min wins
    Component *sm = Component_0();
    Component_setSize(sm, 20.0f, 20.0f);
    Component_setMinSize(sm, 50.0f, 50.0f);
    assert(Component_getWidth(sm) == 50.0f && Component_getHeight(sm) == 50.0f); // re-clamp
    Component_setMaxSize(sm, 30.0f, 30.0f);
    // clamp(min, size, max): min wins over a contradictory max, mirroring Container.
    assert(Component_getWidth(sm) == 50.0f && Component_getHeight(sm) == 50.0f);
    printf("  [ok] min/max clamping\n");

    // 10 Padding: content box = abs + padding, clamped at zero.
    Component *pd = Component_0();
    Component_setParentAbs(pd, 10.0f, 20.0f, 400.0f, 300.0f);
    Component_setLocation(pd, 5.0f, 5.0f);
    Component_setSize(pd, 100.0f, 100.0f);
    Component_setPadding(pd, 10.0f, 10.0f, 10.0f, 10.0f);
    float cx, cy, cw, ch;
    Component_getContentRect(pd, &cx, &cy, &cw, &ch);
    assert(cx == 25.0f && cy == 35.0f && cw == 80.0f && ch == 80.0f);
    Component_setPadding(pd, 60.0f, 60.0f, 60.0f, 60.0f);
    Component_getContentRect(pd, &cx, &cy, &cw, &ch);
    assert(cx == 75.0f && cy == 85.0f && cw == 0.0f && ch == 0.0f);
    Component_setPadding(pd, -5.0f, -5.0f, -5.0f, -5.0f); // negatives clamp to 0
    Component_getContentRect(pd, &cx, &cy, &cw, &ch);
    assert(cx == 15.0f && cy == 25.0f && cw == 100.0f && ch == 100.0f);
    printf("  [ok] padding content rect\n");

    // 11 Presentation round-trips and clamps.
    Component_setBorderWidth(c, -3.0f);
    assert(Component_getBorderWidth(c) == 0.0f);
    Component_setBorderWidth(c, 4.0f);
    assert(Component_getBorderWidth(c) == 4.0f);
    Component_setBorderColor(c, 0xFF112233u);
    assert(Component_getBorderColor(c) == 0xFF112233u);
    Component_setBackgroundColor(c, 0xFFAABBCCu);
    assert(Component_getBackgroundColor(c) == 0xFFAABBCCu);
    Component_setRadius(c, -5.0f);
    assert(Component_getRadius(c) == 0.0f);
    Component_setRadius(c, 8.0f);
    assert(Component_getRadius(c) == 8.0f);
    Component_setRadiusMode(c, COMPONENT_CORNER_SUPERELLIPSE);
    assert(Component_getRadiusMode(c) == COMPONENT_CORNER_SUPERELLIPSE);
    Component_setRadiusMode(c, 7);
    assert(Component_getRadiusMode(c) == COMPONENT_CORNER_SUPERELLIPSE); // invalid rejected
    Component_setOpacity(c, 1.5f);
    assert(Component_getOpacity(c) == 1.0f);
    Component_setOpacity(c, -0.5f);
    assert(Component_getOpacity(c) == 0.0f);
    Component_setOpacity(c, 0.5f);
    assert(Component_getOpacity(c) == 0.5f);
    Component_setZ(c, 42);
    assert(Component_getZ(c) == 42);
    Component_setVisible(c, false);
    assert(Component_isVisible(c) == false);
    Component_setVisible(c, true);
    assert(Component_isVisible(c) == true);
    printf("  [ok] presentation state round-trips\n");

    // 12 ComponentView: pure device mapping (scale + origin translation).
    ComponentView viewA = { .scaleX = 1.0f, .scaleY = 1.0f, .originX = 0.0f, .originY = 0.0f };
    ComponentView viewB = { .scaleX = 2.0f, .scaleY = 2.0f, .originX = 4.0f, .originY = 6.0f };
    float vx, vy, vw, vh;
    Component_viewMap(&viewA, 10.0f, 20.0f, 30.0f, 40.0f, &vx, &vy, &vw, &vh);
    assert(vx == 10.0f && vy == 20.0f && vw == 30.0f && vh == 40.0f);
    Component_viewMap(&viewB, 10.0f, 20.0f, 30.0f, 40.0f, &vx, &vy, &vw, &vh);
    assert(vx == 12.0f && vy == 28.0f && vw == 60.0f && vh == 80.0f);
    printf("  [ok] ComponentView pure mapping\n");

    // 13 hitTest: point-in-abs-rect, half-open, visible-gated.
    Component *ht = Component_0();
    Component_setParentAbs(ht, 10.0f, 20.0f, 400.0f, 300.0f);
    Component_setLocation(ht, 5.0f, 5.0f);
    Component_setSize(ht, 100.0f, 100.0f);
    assert(Component_hitTest(ht, 15.0f, 25.0f) == true);    // abs origin corner
    assert(Component_hitTest(ht, 50.0f, 50.0f) == true);
    assert(Component_hitTest(ht, 114.0f, 124.0f) == true);  // half-open inside
    assert(Component_hitTest(ht, 14.0f, 25.0f) == false);
    assert(Component_hitTest(ht, 15.0f, 24.0f) == false);
    assert(Component_hitTest(ht, 115.0f, 125.0f) == false);
    Component_setVisible(ht, false);
    assert(Component_hitTest(ht, 50.0f, 50.0f) == false);
    printf("  [ok] hitTest bounds\n");

    // 14 Container node: Component[] ownership with owner-fed cascade.
    // The Container owns the items; the owner feeds each item its parent
    // box via Component_setParentAbs once per layout.
    Component *root = Component_0();
    Component_setSize(root, 800.0f, 600.0f);
    Component_setParentAbs(root, 0.0f, 0.0f, 800.0f, 600.0f);

    Component *ch1 = Component_0();
    Component_setAnchor(ch1, COMPONENT_ANCHOR_TOP_LEFT);
    Component_setLocation(ch1, 10.0f, 10.0f);
    Component_setSize(ch1, 100.0f, 50.0f);

    Component *ch2 = Component_0();
    Component_setAnchor(ch2, COMPONENT_ANCHOR_BOTTOM_RIGHT);
    Component_setOrigin(ch2, COMPONENT_ORIGIN_BOTTOM_RIGHT);
    Component_setPivot(ch2, COMPONENT_PIVOT_BOTTOM_RIGHT);
    Component_setLocation(ch2, 20.0f, 30.0f);
    Component_setSize(ch2, 120.0f, 40.0f);

    Container *node = Container_0();
    assert(Container_count(node) == 0u);
    assert(Container_add(node, ch1) == true);
    assert(Container_add(node, ch2) == true);
    assert(Container_count(node) == 2u);

    // Owner feeds the root content box into each item (the cascade).
    float rx, ry, rw, rh;
    Component_getContentRect(root, &rx, &ry, &rw, &rh);
    Component *item0 = Container_get(node, 0u);
    Component *item1 = Container_get(node, 1u);
    Component_setParentAbs(item0, rx, ry, rw, rh);
    Component_setParentAbs(item1, rx, ry, rw, rh);

    // ch1: (10, 10, 100, 50)
    expectAbs(item0, 10.0f, 10.0f, 100.0f, 50.0f);
    // ch2: anchor BR (800, 600), pivot BR (120, 40), origin BR (dir -1, -1), loc (20, 30)
    // absX = 800 - 120 - 20 = 660; absY = 600 - 40 - 30 = 530
    expectAbs(item1, 660.0f, 530.0f, 120.0f, 40.0f);

    // Owner re-cascades on root resize:
    Component_setSize(root, 1000.0f, 800.0f);
    Component_setParentAbs(root, 0.0f, 0.0f, 1000.0f, 800.0f);
    Component_getContentRect(root, &rx, &ry, &rw, &rh);
    Component_setParentAbs(item0, rx, ry, rw, rh);
    Component_setParentAbs(item1, rx, ry, rw, rh);
    expectAbs(item0, 10.0f, 10.0f, 100.0f, 50.0f);
    // ch2: 1000 - 120 - 20 = 860, 800 - 40 - 30 = 730
    expectAbs(item1, 860.0f, 730.0f, 120.0f, 40.0f);

    // Remove item
    assert(Container_removeAt(node, 0u) == true);
    assert(Container_count(node) == 1u);
    printf("  [ok] Container node with owner-fed cascade\n");

    // 15 Component_viewMap: identity (null view) + scaled provably-gapless map.
    float mx0, my0, mw, mh;
    Component_viewMap(nullptr, 4.5f, 2.25f, 3.0f, 3.75f, &mx0, &my0, &mw, &mh);
    assert(mx0 == 4.5f && my0 == 2.25f && mw == 3.0f && mh == 3.75f); // identity
    ComponentView view2x = { .scaleX = 2.0f, .scaleY = 2.0f, .originX = 0.0f, .originY = 0.0f };
    Component_viewMap(&view2x, 4.5f, 2.25f, 3.0f, 3.75f, &mx0, &my0, &mw, &mh);
    // floor(9.0) = 9, floor(4.5) = 4, ceil(15.0) = 15, ceil(12.0) = 12
    assert(mx0 == 9.0f && my0 == 4.0f && mw == 6.0f && mh == 8.0f);
    printf("  [ok] Component_viewMap identity + scaled\n");

    // 16 Padding insets children: owner re-cascades the content box.
    Component_setPadding(root, 10.0f, 20.0f, 30.0f, 40.0f);
    Component_getContentRect(root, &rx, &ry, &rw, &rh);
    assert(rx == 10.0f && ry == 20.0f && rw == 960.0f && rh == 740.0f);
    Component_setParentAbs(item1, rx, ry, rw, rh);
    Component_getParentAbsRect(item1, &rect);
    assert(rect.x == 10.0f && rect.y == 20.0f && rect.z == 960.0f && rect.w == 740.0f);
    // ch2 re-resolves against the content box: BR(970,760) - pivot(120,40) - loc(20,30)
    expectAbs(item1, 830.0f, 690.0f, 120.0f, 40.0f);
    Component_setPadding(root, 0.0f, 0.0f, 0.0f, 0.0f);
    Component_getContentRect(root, &rx, &ry, &rw, &rh);
    Component_setParentAbs(item1, rx, ry, rw, rh);
    Component_getParentAbsRect(item1, &rect);
    assert(rect.x == 0.0f && rect.y == 0.0f && rect.z == 1000.0f && rect.w == 800.0f);
    expectAbs(item1, 860.0f, 730.0f, 120.0f, 40.0f);
    printf("  [ok] padding insets children (content box cascade)\n");

    printf("=== Component Test Suite: ALL CHECKS PASSED ===\n");
    return 0;
}