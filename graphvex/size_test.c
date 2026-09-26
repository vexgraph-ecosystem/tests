#include <stdint.h>
#include <stdio.h>

#include "annotation/overview.h"
#include "lang/graphics_component.h"
#include "lang/graphics_panel.h"
#include "lang/size.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: SizeTest (tests/graphvex/size_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the AUTO model: the åuto FourCC value + bytes, the int/float
 * predicates, AUTO-by-default, the AUTO equivalence (declared sentinel ->
 * owner-supplied measured size; 0 for a dumb element), concrete-clears-AUTO,
 * and the GraphicsPanel equivalence (0).
 *
 * STRUCT FIELDS: none — procedural test harness.
 * ============================================================================
 */

#define CHECK(cond)                                                          \
    do {                                                                     \
        if(!(cond)) {                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
            failures++;                                                      \
        }                                                                    \
    } while(0)

int main(void) {
    int failures = 0;

    // 1. The FourCC: "åuto" packed first-char-in-MSB, negative as int32.
    CHECK(SIZE_AUTO < 0);
    CHECK(SIZE_AUTO == -445287313);
    CHECK(LENGTH_AUTO == SIZE_AUTO);          // one sentinel, two names
    uint32_t magic = (uint32_t) SIZE_AUTO;
    CHECK(((magic >> 24) & 0xFFu) == 0xE5u);  // å (229)
    CHECK(((magic >> 16) & 0xFFu) == 0x75u);  // u
    CHECK(((magic >> 8) & 0xFFu) == 0x74u);   // t
    CHECK((magic & 0xFFu) == 0x6Fu);          // o

    // 2. Predicates: exact for ints, negativity for floats.
    CHECK(Size_isAuto(SIZE_AUTO) == true);
    CHECK(Size_isAuto(LENGTH_AUTO) == true);
    CHECK(Size_isAuto(0) == false);
    CHECK(Size_isAutoF((float) SIZE_AUTO) == true);
    CHECK(Size_isAutoF(-1.0f) == true);        // any negative reads as AUTO
    CHECK(Size_isAutoF(0.0f) == false);

    // 3. AUTO by default + the dumb equivalence (0): a fresh component is AUTO
    //    on both dims and resolves to 0, never garbage.
    GraphicsComponent *gc = GraphicsComponent_0();
    CHECK(gc != nullptr);
    CHECK(GraphicsComponent_isAutoWidth(gc) == true);
    CHECK(GraphicsComponent_isAutoHeight(gc) == true);
    CHECK(GraphicsComponent_getResolvedWidth(gc) == 0.0f);
    CHECK(GraphicsComponent_getResolvedHeight(gc) == 0.0f);
    CHECK(GraphicsComponent_getAbsW(gc) == 0.0f);
    CHECK(GraphicsComponent_getAbsH(gc) == 0.0f);

    // 4. The AUTO equivalence: the declared sentinel survives while the owner
    //    supplies the measured size; the abs resolves to it (scale absorbed).
    GraphicsComponent_setParentAbs(gc, 0.0f, 0.0f, 200.0f, 200.0f);
    GraphicsComponent_setMeasuredSize(gc, 30.0f, 10.0f);
    CHECK(GraphicsComponent_isAutoWidth(gc) == true);   // still the sentinel
    CHECK(GraphicsComponent_getResolvedWidth(gc) == 30.0f);
    CHECK(GraphicsComponent_getResolvedHeight(gc) == 10.0f);
    CHECK(GraphicsComponent_getAbsW(gc) == 30.0f);
    GraphicsComponent_setScale(gc, 2.0f, 3.0f);
    CHECK(GraphicsComponent_getAbsW(gc) == 60.0f);      // 30 * 2
    CHECK(GraphicsComponent_getAbsH(gc) == 30.0f);      // 10 * 3

    // 5. Concrete clears AUTO per dim.
    GraphicsComponent_setWidth(gc, 50.0f);
    CHECK(GraphicsComponent_isAutoWidth(gc) == false);
    CHECK(GraphicsComponent_isAutoHeight(gc) == true);
    CHECK(GraphicsComponent_getResolvedWidth(gc) == 50.0f);
    CHECK(GraphicsComponent_getResolvedHeight(gc) == 10.0f);  // still measured
    GraphicsComponent_free(gc);

    // 6. GraphicsPanel equivalence is 0 (a dumb panel has no intrinsic content).
    GraphicsPanel *panel = GraphicsPanel_1(0x000000FFu);
    CHECK(panel != nullptr);
    GraphicsComponent *pg = Component_graphics(GraphicsPanel_component(panel), 0);
    CHECK(GraphicsComponent_isAutoWidth(pg) == true);   // AUTO by default
    CHECK(GraphicsComponent_getResolvedWidth(pg) == 0.0f);
    CHECK(GraphicsComponent_getResolvedHeight(pg) == 0.0f);
    GraphicsPanel_setSize(panel, 120.0f, 40.0f);                // concrete wins
    CHECK(GraphicsComponent_isAutoWidth(pg) == false);
    CHECK(GraphicsComponent_getAbsW(pg) == 120.0f);
    GraphicsPanel_free(panel);

    // 7. Cold seams.
    CHECK(GraphicsComponent_isAutoWidth(nullptr) == false);
    CHECK(GraphicsComponent_getResolvedWidth(nullptr) == 0.0f);
    GraphicsComponent_setMeasuredSize(nullptr, 1.0f, 1.0f);

    if (failures == 0)
        printf("PASS size_test: AUTO FourCC, default AUTO, equivalence, GraphicsPanel=0\n");
    else
        fprintf(stderr, "FAIL size_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
