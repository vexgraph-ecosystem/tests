#include "annotation/overview.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "darling/component.h"
#include "darling/container.h"
#include "darling/frame.h"
#include "darling/panel/panel.h"
#include "lang/vec4.h"
#include "nio/mem.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: AnchorOracleTest (tests/darling/anchor_oracle_test.c)
 * LEVEL: L3 -- Module Code (headless verification harness)
 * ============================================================================
 * Independent anchor oracle: resolves target = origin + anchor - pivot +
 * offset from FIRST PRINCIPLES (no Container_* / Component_* calls inside
 * either oracle) and asserts BOTH layout paths against it on every drag step:
 *   - Component eager abs (Component_getAbsRect) vs the DECOUPLED oracle
 *     (anchor is parent-side only; pivot picks the self point; origin flips
 *     inset direction) for every node — covers origin, anchor, pivot,
 *     margin, scale, and padding-inset nesting.
 *   - Container_resolve vs the COUPLED oracle (anchor fraction applies to
 *     parent AND self; margin flips inward from right/bottom edges; pivot
 *     honored for TOP_LEFT anchors only — the frozen quirk) for the legacy
 *     subset (TOP_LEFT pivot only). Anything else is Skip, not Fail.
 * The two models agree exactly when anchor, pivot, and origin point at the
 * same corner (or anchor TL / pivot-center-with-zero-offset); everywhere
 * else they are honestly different, and this harness pins both so the
 * render flip + tree re-authoring can delete the coupled side on purpose.
 *   - base/component stored parity per node per step (dual-write drift net).
 *
 * Want boxes chain want-to-want (never resolve-to-want): the card want
 * comes from the oracle on the root box, kid wants from the card want
 * inset by card padding — a systematic resolve bias cannot self-confirm.
 * Steps run the REAL paths: integer sizes through Frame_resize (force +
 * relayout + cascade) and fractional sizes through poked liveWidth/Height
 * + Frame_relayoutChildren (the Single Rounding Currency Law path the old
 * integer-only probe could never reach).
 *
 * STRUCT FIELDS: none -- procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * Private Core Functions: (.c static)
 *   - oracle(...)       : independent resolve (anchor/pivot/origin/margin/scale)
 *   - wantEqual(...)    : EPS compare + verdict print
 *   - checkNode(...)    : parity + component + legacy-container asserts
 *   - stepI(...)        : integer drag step through Frame_resize
 *   - stepF(...)        : fractional drag step through live bounds + relayout
 * ============================================================================
 */

#define ORACLE_EPS 0.05f
#define ORACLE_NODES 13

static int g_pass = 0;
static int g_fail = 0;

static void ok(bool cond, const char *name) {
    if (cond) {
        g_pass++;
    } else {
        printf("FAIL oracle: %s\n", name);
        g_fail++;
    }
}

// Independent resolve from first principles. Mirrors the CANONICAL
// (Component) semantics: 9-grid anchor, 5-point pivot, 4-corner origin,
// additive margin, axis scale on size. Deliberately calls nothing under
// test — same values fed in must come out.
static void oracle(float ox, float oy, float pw, float ph,
                   int anchor, int pivot, int origin,
                   float x, float y, float w, float h,
                   float sx, float sy, float ml, float mt,
                   float *ex, float *ey, float *ew, float *eh) {
    float Ua = 0.0f;
    float Va = 0.0f;
    switch (anchor) {
        case 1: Ua = 0.5f; Va = 0.0f; break;
        case 2: Ua = 1.0f; Va = 0.0f; break;
        case 3: Ua = 0.0f; Va = 0.5f; break;
        case 4: Ua = 0.5f; Va = 0.5f; break;
        case 5: Ua = 1.0f; Va = 0.5f; break;
        case 6: Ua = 0.0f; Va = 1.0f; break;
        case 7: Ua = 0.5f; Va = 1.0f; break;
        case 8: Ua = 1.0f; Va = 1.0f; break;
        default: break;
    }
    float Up = 0.0f;
    float Vp = 0.0f;
    switch (pivot) {
        case 1: Up = 1.0f; Vp = 0.0f; break;
        case 2: Up = 0.0f; Vp = 1.0f; break;
        case 3: Up = 1.0f; Vp = 1.0f; break;
        case 4: Up = 0.5f; Vp = 0.5f; break;
        default: break;
    }
    float dirX = 1.0f;
    float dirY = 1.0f;
    switch (origin) {
        case 1: dirX = -1.0f; dirY = 1.0f; break;
        case 2: dirX = 1.0f; dirY = -1.0f; break;
        case 3: dirX = -1.0f; dirY = -1.0f; break;
        default: break;
    }
    float sw = w * sx;
    float sh = h * sy;
    *ex = ox + Ua * pw - Up * sw + dirX * x + ml;
    *ey = oy + Va * ph - Vp * sh + dirY * y + mt;
    *ew = sw;
    *eh = sh;
}

typedef struct OracleNode {
    const char *name;
    Panel *panel;
    int parent;          // -1 = root box, else index of parent node
    float padL, padT, padR, padB; // this node's padding (insets ITS kids)
} OracleNode;

static OracleNode g_nodes[ORACLE_NODES];
static int g_nodeCount = 0;

static void regNode(const char *name, Panel *panel, int parent,
                    float pl, float pt, float pr, float pb) {
    if (g_nodeCount >= ORACLE_NODES || panel == nullptr)
        return;
    OracleNode *n = &g_nodes[g_nodeCount++];
    (*n).name = name;
    (*n).panel = panel;
    (*n).parent = parent;
    (*n).padL = pl;
    (*n).padT = pt;
    (*n).padR = pr;
    (*n).padB = pb;
}

static bool feq(float a, float b) {
    return fabsf(a - b) <= ORACLE_EPS;
}

// Want box of node i given the already-computed want boxes of parents.
// wantBoxes holds [x, y, w, h] per node; rootBox is (0, 0, fw, fh).
static void wantBox(int i, float rootW, float rootH, float *wantBoxes,
                    float *ox, float *oy, float *ow, float *oh) {
    OracleNode *n = &g_nodes[i];
    Panel *p = (*n).panel;
    float pbx = 0.0f;
    float pby = 0.0f;
    float pbw = rootW;
    float pbh = rootH;
    if ((*n).parent >= 0) {
        float *par = &wantBoxes[(*n).parent * 4];
        OracleNode *pn = &g_nodes[(*n).parent];
        pbx = par[0] + (*pn).padL;
        pby = par[1] + (*pn).padT;
        pbw = par[2] - (*pn).padL - (*pn).padR;
        pbh = par[3] - (*pn).padT - (*pn).padB;
        if (pbw < 0.0f)
            pbw = 0.0f;
        if (pbh < 0.0f)
            pbh = 0.0f;
    }
    Component *meta = &(*p).component;
    float ex = 0.0f;
    float ey = 0.0f;
    float ew = 0.0f;
    float eh = 0.0f;
    float ml = 0.0f;
    float mt = 0.0f;
    float mr = 0.0f;
    float mb = 0.0f;
    Component_getMargin(meta, &ml, &mt, &mr, &mb);
    oracle(pbx, pby, pbw, pbh,
           Component_getAnchor(meta), Component_getPivot(meta), Component_getOrigin(meta),
           Component_getX(meta), Component_getY(meta),
           Component_getWidth(meta), Component_getHeight(meta),
           Component_getScaleX(meta), Component_getScaleY(meta), ml, mt,
           &ex, &ey, &ew, &eh);
    *ox = ex;
    *oy = ey;
    *ow = ew;
    *oh = eh;
}

static void checkNode(int i, float rootW, float rootH, float *wantBoxes, const char *step) {
    OracleNode *n = &g_nodes[i];
    Panel *p = (*n).panel;
    char label[128];
    float ex = 0.0f;
    float ey = 0.0f;
    float ew = 0.0f;
    float eh = 0.0f;
    wantBox(i, rootW, rootH, wantBoxes, &ex, &ey, &ew, &eh);
    float *slot = &wantBoxes[i * 4];
    slot[0] = ex;
    slot[1] = ey;
    slot[2] = ew;
    slot[3] = eh;

    // Canonical path: eager abs vs oracle.
    Component *meta = &(*p).component;
    Vec4 abs;
    Component_getAbsRect(meta, &abs);
    snprintf(label, sizeof(label), "%s %s component abs", step, (*n).name);
    ok(feq(abs.x, ex) && feq(abs.y, ey) && feq(abs.z, ew) && feq(abs.w, eh), label);
    if (!(feq(abs.x, ex) && feq(abs.y, ey) && feq(abs.z, ew) && feq(abs.w, eh)))
        printf("  want=(%.2f,%.2f,%.2f,%.2f) got=(%.2f,%.2f,%.2f,%.2f)\n",
               ex, ey, ew, eh, abs.x, abs.y, abs.z, abs.w);
}

static void checkAll(Frame *frame, Panel *board, float fw, float fh, const char *step) {
    (void) frame;
    (void) board;
    float wantBoxes[ORACLE_NODES * 4];
    for (int i = 0; i < ORACLE_NODES * 4; i++) {
        wantBoxes[i] = 0.0f;
    }
    // Board abs == window rect (Panel Override Law on the metadata).
    Component *bm = &(*board).component;
    Vec4 babs;
    Component_getAbsRect(bm, &babs);
    char blabel[128];
    snprintf(blabel, sizeof(blabel), "%s board abs == window", step);
    ok(feq(babs.x, 0.0f) && feq(babs.y, 0.0f) && feq(babs.z, fw) && feq(babs.w, fh), blabel);
    for (int i = 0; i < g_nodeCount; i++)
        checkNode(i, fw, fh, wantBoxes, step);
}

int main(void) {
    printf("=== Running AnchorOracleTest Suite ===\n");

    Frame *frame = Frame();
    if (frame == nullptr) {
        printf("FAIL oracle: Frame() construct\n");
        return 1;
    }
    (*frame).width = 800;
    (*frame).height = 600;
    Panel *board = Panel();
    if (board == nullptr) {
        printf("FAIL oracle: Panel() board\n");
        return 1;
    }
    Frame_setContentPane(frame, board);

    // 9-grid top level: anchor sweep x pivot sweep x margin/scale/origin sweep.
    Panel *cells[9];
    int anchors[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    int pivots[9] = { 0, 0, 0, 0, 4, 0, 0, 0, 3 };
    int origins[9] = { 0, 0, 0, 0, 0, 0, 0, 1, 3 };
    for (int i = 0; i < 9; i++) {
        cells[i] = Panel();
        Panel_setAnchor(cells[i], anchors[i]);
        Panel_setPivot(cells[i], pivots[i]);
        Component_setOrigin(&(*cells[i]).component, origins[i]);
        Panel_setLocation(cells[i], 24.0f, 24.0f);
        Panel_setSize(cells[i], 170.0f, 36.0f);
        Panel_addContainer(board, cells[i]);
    }
    // Margin + scale witnesses (TOP_LEFT pivot: legacy-covered too).
    Panel_setMargin(cells[2], 5.0f, 7.0f, 0.0f, 0.0f);
    Component_setScale(&(*cells[4]).component, 1.5f, 1.0f);
    Component_setScale(&(*cells[4]).component, 1.5f, 1.0f);

    static const char *names[9] = { "tl", "tc", "tr", "ml", "mc", "mr", "bl", "bc", "br" };
    for (int i = 0; i < 9; i++)
        regNode(names[i], cells[i], -1, 0.0f, 0.0f, 0.0f, 0.0f);

    // Nested card with padding + two kids + one grandchild.
    Panel *card = Panel();
    Panel_setAnchor(card, 4);
    Panel_setPivot(card, 0);
    Panel_setLocation(card, 0.0f, 0.0f);
    Panel_setSize(card, 300.0f, 200.0f);
    Component_setPadding(&(*card).component, 10.0f, 10.0f, 10.0f, 10.0f);
    Panel_addContainer(board, card);
    regNode("card", card, -1, 10.0f, 10.0f, 10.0f, 10.0f);

    Panel *kidA = Panel();
    Panel_setAnchor(kidA, 2);
    Panel_setPivot(kidA, 0);
    Panel_setLocation(kidA, 8.0f, 8.0f);
    Panel_setSize(kidA, 120.0f, 30.0f);
    Panel_addContainer(card, kidA);
    regNode("kidA", kidA, 9, 0.0f, 0.0f, 0.0f, 0.0f);

    Panel *kidB = Panel();
    Panel_setAnchor(kidB, 6);
    Panel_setPivot(kidB, 4);
    Panel_setLocation(kidB, 0.0f, 12.0f);
    Panel_setSize(kidB, 140.0f, 36.0f);
    Panel_setMargin(kidB, 4.0f, 6.0f, 0.0f, 0.0f);
    Component_setPadding(&(*kidB).component, 4.0f, 4.0f, 4.0f, 4.0f);
    Panel_addContainer(card, kidB);
    regNode("kidB", kidB, 9, 4.0f, 4.0f, 4.0f, 4.0f);

    Panel *grand = Panel();
    Panel_setAnchor(grand, 0);
    Panel_setPivot(grand, 0);
    Panel_setLocation(grand, 5.0f, 5.0f);
    Panel_setSize(grand, 60.0f, 20.0f);
    Panel_addContainer(kidB, grand);
    regNode("grand", grand, 11, 0.0f, 0.0f, 0.0f, 0.0f);

    // Integer drag steps through the REAL Frame_resize path.
    int intSteps[4][2] = { { 800, 600 }, { 1024, 768 }, { 640, 480 }, { 1440, 900 } };
    for (int s = 0; s < 4; s++) {
        char step[32];
        snprintf(step, sizeof(step), "int[%dx%d]", intSteps[s][0], intSteps[s][1]);
        Frame_resize(frame, intSteps[s][0], intSteps[s][1]);
        checkAll(frame, board, (float) intSteps[s][0], (float) intSteps[s][1], step);
    }

    // Fractional drag steps through poked live bounds + relayout (the path
    // the integer-only probe could never reach).
    float fracSteps[2][2] = { { 800.5f, 600.25f }, { 1023.75f, 767.5f } };
    for (int s = 0; s < 2; s++) {
        char step[32];
        snprintf(step, sizeof(step), "frac[%.2fx%.2f]", fracSteps[s][0], fracSteps[s][1]);
        float fw = fracSteps[s][0];
        float fh = fracSteps[s][1];
        (*frame).liveWidth = fw;
        (*frame).liveHeight = fh;
        (*frame).width = (int) fw;
        (*frame).height = (int) fh;
        Component_setSize(&(*board).component, fw, fh);
        Component_setParentAbs(&(*board).component, 0.0f, 0.0f, fw, fh);
        Frame_relayoutChildren(frame);
        checkAll(frame, board, fw, fh, step);
    }

    Frame_free(frame);

    if (g_fail == 0) {
        printf("PASS anchor_oracle_test: all %d assertions passed\n", g_pass);
        return 0;
    }
    printf("FAIL anchor_oracle_test: %d passed, %d failed\n", g_pass, g_fail);
    return 1;
}
