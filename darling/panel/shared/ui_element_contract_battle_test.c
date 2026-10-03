// Exercise every public Element operation, including borrowed/private bounds,
// dirty branches, tree ordering, paint command state, nulls and pool lifetimes.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame/frame.h"
#include "nio/property_pool.h"

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); exit(1); } } while (0)




#include "darling/test_application.h"
int main(void) {
    uint32_t baseline = PropertyPool_live(PropertyPool_default());
    Element *root = Element();
    ElementDesc desc1 = {.width = 40, .height = 30, .tag = "a"};
    Element *a = Element(&desc1);
    ElementDesc desc2 = {.width = 20, .height = 10, .tag = "b"};
    Element *b = Element(&desc2);
    CHECK(root && a && b);
    CHECK(Element_isValid(root) && Element_isVisible(root) && !Element_isPressed(root));
    CHECK(Element_add(root, a) == a && Element_addAt(root, b, 0) == b);
    CHECK(Element_count(root) == 2 && Element_child(root, 0) == b && Element_child(root, 1) == a);
    CHECK(Element_child(root, -1) == NULL && Element_child(root, 2) == NULL);
    CHECK(Element_parent(a) == root && Element_root(a) == root);
    CHECK(Element_find(root, "a") == a && Element_find(root, "missing") == NULL);
    CHECK(Element_setSize(root, 100, 80) == root);
    Element_revalidate(root);
    CHECK(!Element_isDirty(root) && !Element_isDirty(a) && !Element_isDirty(b));
    CHECK(Element_setOffset(a, 3, 4) == a);
    CHECK(Element_isDirty(root) && Element_isDirty(a) && !Element_isDirty(b));
    Element_revalidate(root);
    Element_markDirty(b);
    CHECK(Element_isDirty(root) && Element_isDirty(b) && !Element_isDirty(a));
    Element_revalidate(root);
    CHECK(Element_setAnchor(a, PART_CENTER) == a && Element_anchor(a) == PART_CENTER);
    CHECK(Element_setPivot(a, PART_CENTER) == a && Element_pivot(a) == PART_CENTER);
    Rect r = Element_resolve(a, (Rect){10, 20, 100, 80});
    CHECK(r.x == 43 && r.y == 49 && r.w == 40 && r.h == 30);
    Element_setAnchor(a, 999); Element_setPivot(a, -1);
    CHECK(Element_anchor(a) == PART_CENTER && Element_pivot(a) == PART_CENTER);
    CHECK(Element_setTag(a, "renamed") == a && !strcmp(Element_tag(a), "renamed"));
    CHECK(Element_find(root, "a") == NULL && Element_find(root, "renamed") == a);
    CHECK(Element_setRadius(a, -2) == a && Element_radius(a) == 0);
    CHECK(Element_setRadius(a, 5) == a && Element_radius(a) == 5);
    CHECK(Element_setBackground(a, COLOR_WHITE) == a);
    CHECK(Element_setBorder(a, COLOR_BLACK, -2) == a);
    CHECK((*Element_property(a)).borderWidth == 0);
    CHECK(Element_setBorder(a, COLOR_BLACK, 2) == a);
    CHECK(Element_setShadow(a, 3, 4, -2) == a);
    CHECK((*Element_property(a)).shadowBlur == 0);
    CHECK(Color_alpha((*Element_property(a)).shadow) > 0);
    CHECK(Element_setShadowColor(a, COLOR_RGBA(0, 0, 0, 100)) == a);
    CHECK(Element_setShadow(a, 3, 4, 2) == a);
    CHECK(Element_setBlur(a, -2) == a && (*Element_property(a)).blur == 0);
    CHECK(Element_setBlur(a, 3) == a);
    Rect bounds = Element_bounds(a, (Rect){10, 20, 100, 80});
    CHECK(bounds.x == 40 && bounds.y == 46 && bounds.w == 48 && bounds.h == 39);
    CHECK(Element_setClip(a, true) == a && (*Element_property(a)).clip);
    CHECK(Element_setPressed(a, true) == a && Element_isPressed(a));
    DisplayList *dl = DisplayList_0(); CHECK(dl);
    Element_paint(a, r, dl);
    CHECK(DisplayList_count(dl) == 3); // shadow + body + pressed overlay
    const DrawCmd *cmd = DisplayList_cmds(dl);
    CHECK(cmd[1].radius == 5 && cmd[1].border == 2 && cmd[1].blur == 3);
    CHECK(cmd[1].color == COLOR_WHITE && cmd[1].borderColor == COLOR_BLACK);
    CHECK(Element_setVisible(a, false) == a && !Element_isVisible(a));
    DisplayList_clear(dl); Element_paint(a, r, dl); CHECK(DisplayList_count(dl) == 0);
    Element_setVisible(a, true); Element_setPressed(a, false);
    CHECK(Element_remove(b) && !Element_remove(b));
    CHECK(Element_parent(b) == NULL && Element_root(b) == b && Element_count(root) == 1);
    CHECK(Element_addAt(root, b, 999) == b && Element_child(root, 1) == b);
    Element_setAnchor(a, PART_TOP_LEFT); Element_setPivot(a, PART_TOP_LEFT); Element_setOffset(a, 0, 0);
    CHECK(Element_hit(root, 5, 5) == b);
    Element_setVisible(b, false); CHECK(Element_hit(root, 5, 5) == a);
    Element_setVisible(a, false); CHECK(Element_hit(root, 5, 5) == root);
    CHECK(Element_hit(root, 100, 80) == NULL);
    Property shared = Property_default(); shared.w = 17; shared.h = 19;
    CHECK(Element_setProperty(a, &shared) == a && Element_property(a) == &shared);
    CHECK(Element_setProperty(b, &shared) == b);
    Element_setSize(a, 23, 29); CHECK(Element_width(b) == 23 && Element_height(b) == 29);
    CHECK(Element_ownProperty(b) == b && Element_property(b) != &shared);
    Element_setSize(a, 31, 37); CHECK(Element_width(b) == 23 && Element_height(b) == 29);
    CHECK(Element_setProperty(b, NULL) == b && !Element_isValid(b));
    CHECK(Element_ownProperty(b) == b && Element_isValid(b));
    Element_setSize(b, -1, 1); CHECK(!Element_isValid(b));
    CHECK(Element_width(NULL) == 0 && Element_height(NULL) == 0 && Element_radius(NULL) == 0);
    CHECK(Element_anchor(NULL) == PART_TOP_LEFT && Element_pivot(NULL) == PART_TOP_LEFT);
    CHECK(!Element_isValid(NULL) && !Element_isVisible(NULL) && !Element_isPressed(NULL) && !Element_isDirty(NULL));
    CHECK(!Element_property(NULL) && !Element_tag(NULL) && !Element_parent(NULL) && !Element_root(NULL));
    CHECK(!Element_find(NULL, "x") && !Element_find(root, NULL) && !Element_hit(NULL, 0, 0));
    CHECK(!Element_child(NULL, 0) && Element_count(NULL) == 0 && !Element_remove(NULL));
    CHECK(Element_resolve(NULL, r).w == 0 && Element_bounds(NULL, r).w == 0);
    CHECK(!Element_setSize(NULL, 1, 1) && !Element_setOffset(NULL, 1, 1));
    CHECK(!Element_setAnchor(NULL, 0) && !Element_setPivot(NULL, 0) && !Element_setTag(NULL, "x"));
    CHECK(!Element_setRadius(NULL, 1) && !Element_setBackground(NULL, COLOR_WHITE));
    CHECK(!Element_setBorder(NULL, COLOR_WHITE, 1) && !Element_setShadow(NULL, 1, 1, 1));
    CHECK(!Element_setShadowColor(NULL, COLOR_WHITE) && !Element_setBlur(NULL, 1));
    CHECK(!Element_setClip(NULL, true) && !Element_setPressed(NULL, true) && !Element_setVisible(NULL, true));
    CHECK(!Element_setProperty(NULL, &shared) && !Element_ownProperty(NULL));
    CHECK(!Element_add(root, NULL) && !Element_addAt(root, NULL, 0));
    CHECK(Element_add(NULL, b) == b && Element_addAt(NULL, b, 0) == b);
    Element_markDirty(NULL); Element_revalidate(NULL); Element_paint(NULL, r, dl); Element_paint(root, r, NULL);
    Point point = Part_point((Rect){10, 20, 100, 80}, PART_BOTTOM_RIGHT);
    CHECK(point.x == 110 && point.y == 100);
    CHECK(Part_point(r, -1).x == r.x && Part_point(r, 999).y == r.y);
    DisplayList_free(dl); Element_destroy(root); Element_destroy(NULL);
    CHECK(shared.w == 31 && shared.h == 37); // borrowed property survives destruction
    CHECK(PropertyPool_live(PropertyPool_default()) == baseline);
    puts("ui_element_contract_battle_test: PASS (entire Element public surface)");
    return 0;
}
