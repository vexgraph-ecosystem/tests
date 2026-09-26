#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"
#include "lang/component.h"
#include "lang/graphics_component.h"
#include "lang/graphics_panel.h"
#include "lang/transform.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ToStringTest (tests/graphvex/tostring_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Proves the toString Law headless: every class renders a value string and a
 * one-layer struct dump, null self writes "nullptr", the struct dump mirrors the
 * fields, and a truncation is flagged (never silent).
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
    char buf[512];
    bool trunc = false;

    // 1. Transform: value + struct.
    Transform t = Transform_translate(10.0f, 20.0f);
    Transform_toString(&t, buf, sizeof(buf), &trunc);
    CHECK(!trunc && strstr(buf, "Transform[") != nullptr);
    Transform_toStringStruct(&t, buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "m02: 10.000") != nullptr);
    Transform_toString(nullptr, buf, sizeof(buf), &trunc);
    CHECK(strcmp(buf, "nullptr") == 0);

    // 2. GraphicsComponent: value + struct mirror the fields.
    GraphicsComponent gc = {0};
    GraphicsComponent_init(&gc);
    GraphicsComponent_setSize(&gc, 20.0f, 10.0f);
    GraphicsComponent_setBackgroundColor(&gc, 0x3366CCFFu);
    GraphicsComponent_setCornerRadius(&gc, 30.0f);
    GraphicsComponent_toString(&gc, buf, sizeof(buf), &trunc);
    CHECK(!trunc && strstr(buf, "GraphicsComponent[") != nullptr);
    GraphicsComponent_toStringStruct(&gc, buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "backgroundColor: 0x3366CCFF") != nullptr);
    CHECK(strstr(buf, "cornerRadius: 30.0") != nullptr);

    // 3. Component: one layer — parent by name, children as a count (no recursion).
    GraphicsPanel *root = GraphicsPanel_2("root", 0x1E1E24FFu);
    GraphicsPanel *child = GraphicsPanel_1(0xE05050FFu);
    GraphicsPanel_add(root, GraphicsPanel_component(child));
    Component_toStringStruct(GraphicsPanel_component(root), buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "name: \"root\"") != nullptr);
    CHECK(strstr(buf, "children: 1") != nullptr);
    CHECK(strstr(buf, "Component {") != nullptr);
    CHECK(strstr(buf, "name: \"\"" ) == nullptr);   // no recursion into the child
    Component_toStringStruct(GraphicsPanel_component(child), buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "parent: \"root\"") != nullptr);   // one layer up, by name

    // 4. GraphicsPanel: value + struct (forwards to its component).
    GraphicsPanel_toString(root, buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "GraphicsPanel(\"root\", 0x1E1E24FF)") != nullptr);
    GraphicsPanel_toStringStruct(root, buf, sizeof(buf), &trunc);
    CHECK(strstr(buf, "Component {") != nullptr);

    // 5. Null self -> "nullptr"; truncation is flagged, never silent.
    Component_toString(nullptr, buf, sizeof(buf), &trunc);
    CHECK(strcmp(buf, "nullptr") == 0);
    GraphicsPanel_toStringStruct(nullptr, buf, sizeof(buf), &trunc);
    CHECK(strcmp(buf, "nullptr") == 0);

    char tiny[8];
    GraphicsComponent_toStringStruct(&gc, tiny, sizeof(tiny), &trunc);
    CHECK(trunc == true);            // cut flagged
    CHECK(tiny[sizeof(tiny) - 1] == '\0');   // always terminated

    GraphicsPanel_free(root);   // frees child too

    if (failures == 0)
        printf("PASS tostring_test: value + one-layer struct, nullptr, truncation flag\n");
    else
        fprintf(stderr, "FAIL tostring_test: %d assertion(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
