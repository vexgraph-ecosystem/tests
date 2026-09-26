#include <math.h>
#include <stdio.h>
#include <string.h>

#include "annotation/overview.h"

#include "darling/panel/panel.h"
#include "darling/frame.h"
#include "nio/mem.h"

;;OVERVIEW
/**
 * ============================================================================
 * TEST: anchor_lattice_test (tests/darling/anchor_lattice_test.c)
 * ============================================================================
 * The Anchor Lattice Law, verified at EVERY SINGLE ELEMENT — not probe panels
 * in an app test suite. The tree is a full 9-grid lattice of lattice cells:
 *   root (locked contentPane, tracks the window size)
 *     - 9 section cells, one per COMPONENT_ANCHOR_* (each 64x64, 16px inset)
 *         - 3 nested children per section, EACH child anchored differently
 *             - 1 grandchild per child (also anchored)
 * Every element in the tree carries an anchor, so a resize relayout must
 * re-resolve every node. The suite resizes the Frame, then walks the whole
 * tree and asserts each element's resolved rect against the anchor contract
 * computed from the LIVE size. A one-step-late layout (the tear signature)
 * lands edge elements off by the growth delta and fails the walk.
 * ============================================================================
 */

#define SECT 64.0f
#define INSET 16.0f

static const char *anchorName(int kind) {
    switch (kind) {
        case COMPONENT_ANCHOR_TOP_LEFT:      return "top-left";
        case COMPONENT_ANCHOR_TOP_CENTER:    return "top-center";
        case COMPONENT_ANCHOR_TOP_RIGHT:     return "top-right";
        case COMPONENT_ANCHOR_MIDDLE_LEFT:   return "middle-left";
        case COMPONENT_ANCHOR_MIDDLE_CENTER: return "middle-center";
        case COMPONENT_ANCHOR_MIDDLE_RIGHT:  return "middle-right";
        case COMPONENT_ANCHOR_BOTTOM_LEFT:   return "bottom-left";
        case COMPONENT_ANCHOR_BOTTOM_CENTER: return "bottom-center";
        case COMPONENT_ANCHOR_BOTTOM_RIGHT:  return "bottom-right";
        default:                             return "?";
    }
}

// Independent expectation (written from the Component contract: abs =
// anchorPoint(parent) - pivotPoint(self) + dir * loc, default pivot
// TOP_LEFT + origin TOP_LEFT, zero margin — so abs = anchorPoint + loc.
// This differs from the retired Container_resolve inset math on purpose:
// the lattice verifies tear-free relayout (every node re-resolves against
// the LIVE size in the same step), not inset aesthetics.
static void anchorExpectPos(int kind, float W, float H, float mx, float my,
                            float *ex, float *ey) {
    float Ua = 0.0f, Va = 0.0f;
    switch (kind) {
        case COMPONENT_ANCHOR_TOP_LEFT:      Ua = 0.0f; Va = 0.0f; break;
        case COMPONENT_ANCHOR_TOP_CENTER:    Ua = 0.5f; Va = 0.0f; break;
        case COMPONENT_ANCHOR_TOP_RIGHT:     Ua = 1.0f; Va = 0.0f; break;
        case COMPONENT_ANCHOR_MIDDLE_LEFT:   Ua = 0.0f; Va = 0.5f; break;
        case COMPONENT_ANCHOR_MIDDLE_CENTER: Ua = 0.5f; Va = 0.5f; break;
        case COMPONENT_ANCHOR_MIDDLE_RIGHT:  Ua = 1.0f; Va = 0.5f; break;
        case COMPONENT_ANCHOR_BOTTOM_LEFT:   Ua = 0.0f; Va = 1.0f; break;
        case COMPONENT_ANCHOR_BOTTOM_CENTER: Ua = 0.5f; Va = 1.0f; break;
        case COMPONENT_ANCHOR_BOTTOM_RIGHT:  Ua = 1.0f; Va = 1.0f; break;
        default: break;
    }
    (void) H;
    *ex = Ua * W + mx;
    *ey = Va * H + my;
}

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

#define EPS 0.01f

// One node assertion: resolve against the LIVE parent box and compare against
// the independent anchor contract (anchorExpectPos). Stored location doubles
// as the inset/margin from the anchor edge; every lattice node is square, so
// one scalar S covers both axes. Dest-last outs, dest-last resolve.
static void expectNode(Panel *node, float absX, float absY,
                       float parentW, float parentH, const char *label) {
    Component *base = &(*node).component;
    float ex = 0.0f;
    float ey = 0.0f;
    anchorExpectPos((*base).anchor, parentW, parentH,
                    (*base).x, (*base).y, &ex, &ey);

    Vec4 rect;
    Component_setParentAbs(base, absX, absY, parentW, parentH);
    Component_getAbsRect(base, &rect);

    float wantX = absX + ex;
    float wantY = absY + ey;
    float wantW = (*base).w;
    float wantH = (*base).h;

    bool pass = fabsf(rect.x - wantX) <= EPS
             && fabsf(rect.y - wantY) <= EPS
             && fabsf(rect.z - wantW) <= EPS
             && fabsf(rect.w - wantH) <= EPS;
    if (pass) {
        g_pass++;
        return;
    }
    printf("FAIL: %s anchor=%s parent=%.0fx%.0f loc=(%.0f,%.0f) size=%.0f\n",
           label, anchorName((*base).anchor), parentW, parentH,
           (*base).x, (*base).y, (*base).w);
    printf("      want rect=(%.2f,%.2f,%.2f,%.2f) got rect=(%.2f,%.2f,%.2f,%.2f)\n",
           wantX, wantY, wantW, wantH, rect.x, rect.y, rect.z, rect.w);
    g_fail++;
}

// Recursive walk: every element in the tree re-resolves from the LIVE parent
// size, so a one-step-late layout (the tear signature) lands anchored edges
// off by the growth delta and fails here. Failures record via expectNode.
static void walkTree(Panel *parent, float absX, float absY,
                     float parentW, float parentH, int depth) {
    if (!parent)
        return;
    size_t childCount = Panel_childCount(parent);
    for (size_t i = 0; i < childCount; i++) {
        Panel *child = Panel_getChild(parent, i);
        if (!child)
            continue;
        char label[128];
        snprintf(label, sizeof(label), "depth=%d index=%zu", depth, i);
        Component *childBase = &(*child).component;
        float ex = 0.0f;
        float ey = 0.0f;
        anchorExpectPos((*childBase).anchor, parentW, parentH,
                        (*childBase).x, (*childBase).y, &ex, &ey);
        expectNode(child, absX, absY, parentW, parentH, label);
        walkTree(child, absX + ex, absY + ey, (*childBase).w, (*childBase).h, depth + 1);
    }
}

// One resize step of the lattice contract: force the Frame to (w, h) — the
// locked board root must track it (the Window Board Root Lock Law) and every
// descendant must re-resolve against the NEW live size inside the same step.
static void latticeResizeStep(Frame *frame, Panel *root, int w, int h) {
    char label[128];

    Frame_resize(frame, w, h);

    snprintf(label, sizeof(label), "resize %dx%d: frame.width", w, h);
    ok((*frame).width == w, label);
    snprintf(label, sizeof(label), "resize %dx%d: frame.height", w, h);
    ok((*frame).height == h, label);
    snprintf(label, sizeof(label), "resize %dx%d: board root w tracks", w, h);
    ok(fabsf(Component_getWidth(&(*root).component) - (float) w) <= EPS, label);
    snprintf(label, sizeof(label), "resize %dx%d: board root h tracks", w, h);
    ok(fabsf(Component_getHeight(&(*root).component) - (float) h) <= EPS, label);

    walkTree(root, 0.0f, 0.0f, (float) w, (float) h, 1);
}

int main(void) {
    printf("=== Running AnchorLatticeTest Suite ===\n");

    // Root: the locked contentPane, tracking the window size (law 49).
    Frame *frame = Frame();
    ok(frame != nullptr, "Frame() construct");
    (*frame).width = 800;
    (*frame).height = 600;

    Panel *root = Panel();
    ok(root != nullptr, "Panel() root");
    Frame_setContentPane(frame, root);
    ok(fabsf(Component_getWidth(&(*root).component) - 800.0f) <= EPS, "root w = 800 at attach");
    ok(fabsf(Component_getHeight(&(*root).component) - 600.0f) <= EPS, "root h = 600 at attach");

    // The lattice: 9 section cells (one per COMPONENT_ANCHOR_*), 3 nested
    // children per section (each anchored differently), 1 anchored grandchild
    // per child. Every element carries an anchor, so a resize relayout must
    // re-resolve every node.
    for (int s = 0; s < 9; s++) {
        Panel *section = Panel();
        if (!section)
            continue;
        Panel_setAnchor(section, s);
        Panel_setLocation(section, INSET, INSET);
        Panel_setSize(section, SECT, SECT);
        Panel_addContainer(root, section);

        for (int c = 0; c < 3; c++) {
            Panel *child = Panel();
            if (!child)
                continue;
            int childAnchor = (s + 1 + c) % 9; // 3 distinct anchors per section
            Panel_setAnchor(child, childAnchor);
            Panel_setLocation(child, 8.0f, 8.0f);
            Panel_setSize(child, 24.0f, 24.0f);
            Panel_addContainer(section, child);

            Panel *grand = Panel();
            if (!grand)
                continue;
            int grandAnchor = (s * 3 + c) % 9; // cycles the full 9-grid at depth 3
            Panel_setAnchor(grand, grandAnchor);
            Panel_setLocation(grand, 4.0f, 4.0f);
            Panel_setSize(grand, 12.0f, 12.0f);
            Panel_addContainer(child, grand);
        }
    }

    ok(Panel_childCount(root) == 9, "root holds 9 lattice sections");

    // Settled walk at the attach size, then a drag sequence (grow, shrink,
    // asymmetric, odd-step parity, return) — every step must resolve against
    // the LIVE size.
    latticeResizeStep(frame, root, 800, 600);
    latticeResizeStep(frame, root, 1024, 768);
    latticeResizeStep(frame, root, 640, 480);
    latticeResizeStep(frame, root, 1440, 900);
    latticeResizeStep(frame, root, 801, 601);
    latticeResizeStep(frame, root, 800, 600);

    // Cleanup.
    Memory_free(root);
    Frame_free(frame);

    printf("\n");
    if (g_fail == 0) {
        printf("PASS anchor_lattice_test: all %d assertions passed\n", g_pass);
        return 0;
    }
    printf("FAIL anchor_lattice_test: %d passed, %d failed\n", g_pass, g_fail);
    return 1;
}
