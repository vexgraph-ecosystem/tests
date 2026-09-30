// tests/graphvex/panel_test.c — mirrors src/panel.c
//
// The UI component: anchor/pivot placement across ALL 9 parts x 9 pivots, plus
// offsets, styles (radius/border/shadow) and the paint path.

#include <math.h>
#include <stdio.h>

#include "panel.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

static int closef(float a, float b) { return fabsf(a - b) < 0.001f; }

int main(void) {
    Rect parent = {100, 50, 200, 100};

    // --- the 9 parts themselves ---
    Rect q = {0, 0, 100, 60};
    CHECK((Part_point(q, PART_TOP_LEFT).x == 0 && Part_point(q, PART_TOP_LEFT).y == 0));
    CHECK((Part_point(q, PART_TOP_CENTER).x == 50 && Part_point(q, PART_TOP_CENTER).y == 0));
    CHECK((Part_point(q, PART_TOP_RIGHT).x == 100 && Part_point(q, PART_TOP_RIGHT).y == 0));
    CHECK((Part_point(q, PART_MIDDLE_LEFT).x == 0 && Part_point(q, PART_MIDDLE_LEFT).y == 30));
    CHECK((Part_point(q, PART_CENTER).x == 50 && Part_point(q, PART_CENTER).y == 30));
    CHECK((Part_point(q, PART_MIDDLE_RIGHT).x == 100 && Part_point(q, PART_MIDDLE_RIGHT).y == 30));
    CHECK((Part_point(q, PART_BOTTOM_LEFT).x == 0 && Part_point(q, PART_BOTTOM_LEFT).y == 60));
    CHECK((Part_point(q, PART_BOTTOM_CENTER).x == 50 && Part_point(q, PART_BOTTOM_CENTER).y == 60));
    CHECK((Part_point(q, PART_BOTTOM_RIGHT).x == 100 && Part_point(q, PART_BOTTOM_RIGHT).y == 60));
    // an out-of-range part clamps to TOP_LEFT, never a bad read
    CHECK((Part_point(q, 999).x == 0 && Part_point(q, -3).y == 0));

    PanelDesc d = {0};
    d.width = 20;
    d.height = 10;
    d.anchor = PART_TOP_LEFT;
    d.pivot = PART_TOP_LEFT;
    Panel *p = Panel_new(&d);
    CHECK(p != NULL && Panel_isValid(p));

    // --- specific known placements ---
    Rect r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 100) && closef(r.y, 50));

    Panel_setAnchor(p, PART_CENTER);
    Panel_setPivot(p, PART_CENTER);
    r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 190) && closef(r.y, 95));      // centred

    Panel_setAnchor(p, PART_BOTTOM_RIGHT);
    Panel_setPivot(p, PART_BOTTOM_RIGHT);
    r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 280) && closef(r.y, 140));

    Panel_setAnchor(p, PART_BOTTOM_RIGHT);
    Panel_setPivot(p, PART_TOP_LEFT);
    r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 300) && closef(r.y, 150));     // hangs off the corner

    Panel_setAnchor(p, PART_TOP_CENTER);
    Panel_setPivot(p, PART_BOTTOM_CENTER);
    r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 190) && closef(r.y, 40));

    // offset shifts the result by exactly the offset
    Panel_setAnchor(p, PART_TOP_LEFT);
    Panel_setPivot(p, PART_TOP_LEFT);
    Panel_setOffset(p, 5, 7);
    r = Panel_resolve(p, parent);
    CHECK(closef(r.x, 105) && closef(r.y, 57));
    CHECK(closef(r.w, 20) && closef(r.h, 10));

    // --- ALL 81 anchor x pivot combos: the panel's pivot lands on the anchor ---
    Panel_setOffset(p, 0, 0);
    for (int a = 0; a < PART_COUNT; a++) {
        for (int pv = 0; pv < PART_COUNT; pv++) {
            Panel_setAnchor(p, a);
            Panel_setPivot(p, pv);
            Rect rr = Panel_resolve(p, parent);
            Point anchor = Part_point(parent, a);
            Point pivot = Part_point(rr, pv);
            if (!(closef(anchor.x, pivot.x) && closef(anchor.y, pivot.y))) {
                printf("FAIL a=%d p=%d: anchor(%.1f,%.1f) != pivot(%.1f,%.1f)\n",
                       a, pv, anchor.x, anchor.y, pivot.x, pivot.y);
                g_fail++;
            }
        }
    }

    // --- style + paint ---
    Panel_setAnchor(p, PART_CENTER);
    Panel_setPivot(p, PART_CENTER);
    Panel_setBackground(p, COLOR_RGBA(1, 2, 3, 4));
    Panel_setBorder(p, COLOR_RGBA(5, 6, 7, 8), 2);
    Panel_setRadius(p, 8);
    Panel_setShadow(p, COLOR_RGBA(0, 0, 0, 128), 3, 4, 2);
    CHECK(Panel_radius(p) == 8.0f);
    CHECK(Panel_anchor(p) == PART_CENTER);

    DisplayList *dl = DisplayList_0();
    Panel_paint(p, (Rect){10, 10, 40, 20}, dl);
    CHECK(DisplayList_count(dl) == 2);               // shadow + body

    Panel_setShadow(p, COLOR_CLEAR, 0, 0, 0);
    DisplayList_clear(dl);
    Panel_paint(p, (Rect){10, 10, 40, 20}, dl);
    CHECK(DisplayList_count(dl) == 1);               // no shadow

    // the painted body carries the radius + border
    const DrawCmd *body = DisplayList_cmds(dl);
    CHECK(body[0].radius == 8.0f);
    CHECK(body[0].border == 2.0f);
    CHECK(body[0].color == COLOR_RGBA(1, 2, 3, 4));
    CHECK(body[0].borderColor == COLOR_RGBA(5, 6, 7, 8));

    // radius clamps negative; invalid parts are ignored
    Panel_setRadius(p, -5);
    CHECK(Panel_radius(p) == 0.0f);
    Panel_setAnchor(p, PART_TOP_LEFT);
    Panel_setAnchor(p, 99);
    CHECK(Panel_anchor(p) == PART_TOP_LEFT);
    Panel_setPivot(p, PART_CENTER);
    Panel_setPivot(p, -1);
    CHECK(Panel_pivot(p) == PART_CENTER);

    // null-safe
    CHECK(!Panel_isValid(NULL));
    CHECK(Panel_width(NULL) == 0.0f);
    CHECK(Panel_resolve(NULL, parent).w == 0.0f);
    Panel_paint(NULL, (Rect){0, 0, 1, 1}, dl);       // must not crash

    DisplayList_free(dl);
    Panel_destroy(p);
    Panel_destroy(NULL);

    if (g_fail == 0) printf("panel_test: ALL PASS (9 anchors x 9 pivots = 81 combos)\n");
    return g_fail == 0 ? 0 : 1;
}
