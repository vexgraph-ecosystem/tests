#include "annotation/overview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "darling/panel/markdown_panel.h"
#include "darling/label/label.h"
#include "event/keyevent.h"
#include "input/key.h"
#include "nio/mem.h"
#include "text/text_core.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: MarkdownPanelTest (darling/panel/markdown_panel_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for MarkdownPanel document selection over its stacked Label
 * rows (no Font, so every row is a Label). Proves the pointer seam end to
 * end: document index mapping in rendered-text space, fixed-anchor forward
 * and backward drags, cross-row span aggregation, click-to-clear, outside
 * click, hard unhighlightable lock, programmatic selection, highlight color
 * roundtrip, and getter null-safety.
 *
 * Source: "# Title\nsome paragraph\n- bullet one\n- **bullet two**\n```\ncode line\n```"
 * Rows (visible bytes, cell base, height at 400pt wide):
 *   row0 Label "Title"           cells [0,5)   h 37.80
 *   row1 Label "some paragraph"  cells [5,19)  h 18.90
 *   row2 Label "• bullet one"    cells [19,33) h 18.90
 *   row3 Label "• bullet two"    cells [33,47) h 18.90
 *   row4 Label "code line"       cells [47,56) h 16.20
 * Y bands add rowSpacing 4 (row0 0..37.8, row1 41.8..60.7, row2 64.7..83.6,
 * row3 87.6..106.5, row4 110.5..126.7). LocalX maps by ratio x/400 * textLen.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[markdown_panel_test] PASS %s\n", name); } \
    else { printf("[markdown_panel_test] FAIL %s\n", name); g_failures++; } \
} while (0)

static void rowSel(MarkdownPanel *mp, size_t i, int32_t *outStart, int32_t *outEnd) {
    Panel *rows = MarkdownPanel_getRows(mp);
    Panel *row = Panel_getChild(rows, (int32_t) i);
    Label_getSelection((const Label*) row, outStart, outEnd);
}

int main(void) {
    MarkdownPanel *mp = MarkdownPanel_0();
    CHECK("constructor", mp != NULL);

    MarkdownPanel_setSize(mp, 400.0f, 400.0f);
    MarkdownPanel_setText(mp, "# Title\nsome paragraph\n- bullet one\n- **bullet two**\n```\ncode line\n```");

    // 5 rows, cell starts [0,5,19,33,47], text lens [5,14,14,14,9].
    CHECK("row count", MarkdownPanel_getRowCount(mp) == 5);
    CHECK("not highlightable by default", !MarkdownPanel_isHighlightable(mp));

    int32_t s0 = 0, s1 = 0;
    char *copy = nullptr;
    MarkdownPanel_getSelection(mp, &s0, &s1);
    CHECK("selection empty initially", s0 == -1 && s1 == -1);

    MarkdownPanel_setHighlightable(mp, true);
    CHECK("highlightable now", MarkdownPanel_isHighlightable(mp));

    int32_t r0 = 0, r1 = 0;
    int32_t sel0 = 0, sel1 = 0;

    // Forward drag across two rows: down mid-row0, active edge lands in row1.
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 20.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_DRAG, 200.0f, 50.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("forward drag doc range", sel0 == 1 && sel1 == 12);
    rowSel(mp, 0, &r0, &r1);
    CHECK("forward drag row0 span", r0 == 1 && r1 == 5);
    rowSel(mp, 1, &r0, &r1);
    CHECK("forward drag row1 span", r0 == 0 && r1 == 7);
    MarkdownPanel_handlePointer(mp, PTR_UP, 200.0f, 50.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("release keeps forward selection", sel0 == 1 && sel1 == 12);

    // Backward drag: anchor row3, active edge pulled back to row0.
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 300.0f, 100.0f, nullptr);
    rowSel(mp, 3, &r0, &r1);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("backward anchor mid-row3", sel0 == sel1 && sel0 == 44);
    MarkdownPanel_handlePointer(mp, PTR_DRAG, 50.0f, 10.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("backward drag ordered doc range", sel0 == 1 && sel1 == 44);
    rowSel(mp, 0, &r0, &r1);
    CHECK("backward drag row0 span", r0 == 1 && r1 == 5);
    rowSel(mp, 1, &r0, &r1);
    CHECK("backward drag row1 full", r0 == 0 && r1 == 14);
    rowSel(mp, 2, &r0, &r1);
    CHECK("backward drag row2 full", r0 == 0 && r1 == 14);
    rowSel(mp, 3, &r0, &r1);
    CHECK("backward drag row3 up to anchor", r0 == 0 && r1 == 11);
    rowSel(mp, 4, &r0, &r1);
    CHECK("backward drag row4 untouched", r0 == -1 && r1 == -1);

    // Fixed anchor survives a forward pass past it.
    MarkdownPanel_handlePointer(mp, PTR_DRAG, 200.0f, 120.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("forward past anchor keeps anchor", sel0 == 44 && sel1 == 52);
    rowSel(mp, 4, &r0, &r1);
    CHECK("pulled row4 tail selected", r0 == 0 && r1 == 5);
    MarkdownPanel_handlePointer(mp, PTR_UP, 200.0f, 120.0f, nullptr);

    // A plain click collapses to no selection.
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 200.0f, 200.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_UP, 200.0f, 200.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("click clears selection", sel0 == -1 && sel1 == -1);
    rowSel(mp, 1, &r0, &r1);
    CHECK("click clears rows", r0 == -1 && r1 == -1);

    // Outside click (left of panel) clears.
    MarkdownPanel_handlePointer(mp, PTR_DOWN, -20.0f, 40.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_UP, -20.0f, 40.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 20.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("down anchors after outside reset", sel0 == sel1 && sel0 == 1);
    MarkdownPanel_handlePointer(mp, PTR_UP, 100.0f, 20.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("outside click cleared last session", sel0 == -1 && sel1 == -1);

    // Hard lock: unhighlightable ignores pointer events.
    MarkdownPanel_setHighlightable(mp, false);
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 20.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_DRAG, 200.0f, 50.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("unhighlightable swallows pointer", sel0 == -1 && sel1 == -1);
    MarkdownPanel_setHighlightable(mp, true);

    // Programmatic document-wide selection mirrors every row.
    MarkdownPanel_setSelection(mp, 5, 47);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("programmatic range stored", sel0 == 5 && sel1 == 47);
    rowSel(mp, 1, &r0, &r1);
    CHECK("programmatic row1 full", r0 == 0 && r1 == 14);
    rowSel(mp, 3, &r0, &r1);
    CHECK("programmatic row3 full", r0 == 0 && r1 == 14);
    rowSel(mp, 4, &r0, &r1);
    CHECK("programmatic row4 outside range", r0 == -1 && r1 == -1);

    MarkdownPanel_setSelection(mp, 30, 10);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("getSelection orders reversed input", sel0 == 10 && sel1 == 30);

    // Highlight color roundtrip (packs AARRGGBB).
    MarkdownPanel_setHighlightColorRGBA(mp, 255, 0, 0, 255);
    CHECK("packed color", MarkdownPanel_getHighlightColor(mp) == 0xFFFF0000u);
    uint8_t r = 0, g = 0, b = 0, a = 0;
    MarkdownPanel_getHighlightColorRGBA(mp, &r, &g, &b, &a);
    CHECK("color channel rd", r == 255 && g == 0 && b == 0 && a == 255);

    // Display-accurate copy: every row here is a Label (no Font headless), so
    // getSelectedText slices the stored visible strings — the "• " bullet
    // literal participates only when the selection touches it.
    MarkdownPanel_setSelection(mp, 6, 11);
    copy = MarkdownPanel_getSelectedText(mp);
    CHECK("mid-row copy", copy && strcmp(copy, "ome p") == 0);
    if (copy)
        Memory_free(copy);

    // [18,24): row1's last byte 'h' plus row2's first 5 bytes "• b" separated by newline.
    MarkdownPanel_setSelection(mp, 18, 24);
    copy = MarkdownPanel_getSelectedText(mp);
    CHECK("bullet-touch copy with newline", copy && strcmp(copy, "h\n\xE2\x80\xA2 b") == 0);
    if (copy)
        Memory_free(copy);

    // Whole document [0,56): rows concatenate with '\n' inter-row separators
    // (visible strings already include bullets).
    MarkdownPanel_setSelection(mp, 0, 56);
    copy = MarkdownPanel_getSelectedText(mp);
    CHECK("whole-doc copy with newlines", copy && strcmp(copy, "Title\nsome paragraph\n\xE2\x80\xA2 bullet one\n\xE2\x80\xA2 bullet two\ncode line") == 0);
    if (copy)
        Memory_free(copy);

    // A committed selection's copy re-reads the span: clear via click → null.
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 20.0f, nullptr);
    MarkdownPanel_handlePointer(mp, PTR_UP, 100.0f, 20.0f, nullptr);
    CHECK("collapsed click nulls copy", MarkdownPanel_getSelectedText(mp) == nullptr);

    // Key seam: Cmd+C copies the committed span through the in-memory board.
    TextCore_setTestClipboard(true);
    MarkdownPanel_setSelection(mp, 6, 18);
    UIKeyEvent *kev = UIKeyEvent_3(KEY_C, true, false);
    UIKeyEvent_setMods(kev, 8u);
    MarkdownPanel_handleKey(mp, kev);
    CHECK("copy consumed", UIKeyEvent_isConsumed(kev));
    char *pasted = TextCore_pasteFromClipboard();
    CHECK("copy lands on board", pasted && strcmp(pasted, "ome paragrap") == 0);
    if (pasted)
        free(pasted);

    // Cmd+V replaces the document text with the board (cold rebuild).
    kev = UIKeyEvent_3(KEY_V, true, false);
    UIKeyEvent_setMods(kev, 8u);
    MarkdownPanel_handleKey(mp, kev);
    CHECK("paste consumed", UIKeyEvent_isConsumed(kev));
    CHECK("paste replaces text", MarkdownPanel_getRowCount(mp) == 1);

    // Restore the original document + selection so the null-safety block below
    // still acts on the pristine 5-row fixture (row1 holds [5,14)).
    MarkdownPanel_setText(mp, "# Title\nsome paragraph\n- bullet one\n- **bullet two**\n```\ncode line\n```");
    MarkdownPanel_setSelection(mp, 30, 10);


    // Repeat and plain presses never copy; no mods → not consumed.
    kev = UIKeyEvent_3(KEY_C, true, true);
    UIKeyEvent_setMods(kev, 8u);
    MarkdownPanel_handleKey(mp, kev);
    CHECK("repeat ignored", !UIKeyEvent_isConsumed(kev));

    kev = UIKeyEvent_3(KEY_C, false, false);
    UIKeyEvent_setMods(kev, 8u);
    MarkdownPanel_handleKey(mp, kev);
    CHECK("release ignored", !UIKeyEvent_isConsumed(kev));

    kev = UIKeyEvent_3(KEY_C, true, false);
    UIKeyEvent_setMods(kev, 0u);
    MarkdownPanel_handleKey(mp, kev);
    CHECK("no-mods ignored", !UIKeyEvent_isConsumed(kev));

    TextCore_setTestClipboard(false);

    // Null-safety (Rule 35).
    MarkdownPanel_getSelection(NULL, &s0, &s1);
    CHECK("null getSelection safe", s0 == -1 && s1 == -1);
    CHECK("null isHighlightable", !MarkdownPanel_isHighlightable(NULL));
    CHECK("null getHighlightColor", MarkdownPanel_getHighlightColor(NULL) == 0u);
    rowSel(mp, 1, &r0, &r1);
    CHECK("row1 span before null call", r0 == 5 && r1 == 14);
    // Out-of-bounds continuous drag clamping (GPUI model).
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 50.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    int32_t anchor50 = sel0;
    // Drag far above-left outside bounds -> clamps to doc index 0
    MarkdownPanel_handlePointer(mp, PTR_DRAG, -50.0f, -50.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("drag outside top-left clamps to 0", sel0 == 0 && sel1 == anchor50);
    // Drag far below-right outside bounds -> clamps to doc end (56)
    MarkdownPanel_handlePointer(mp, PTR_DRAG, 1000.0f, 1000.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("drag outside bottom-right clamps to end", sel0 == anchor50 && sel1 == 56);
    MarkdownPanel_handlePointer(mp, PTR_UP, 1000.0f, 1000.0f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("release outside commits clamped selection", sel0 == anchor50 && sel1 == 56);

    // Gap hit-testing (midpoint rule):
    // Row 0 height = 28 * 1.35 = 37.8, spacing = 4.0. Midpoint between row0 and row1 is 39.8.
    // Row 0 cell span is [0, 5]. Row 1 cell span is [5, 19].
    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 38.5f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("gap click before midpoint hits row 0", sel0 >= 0 && sel0 <= 5);
    MarkdownPanel_handlePointer(mp, PTR_UP, 100.0f, 38.5f, nullptr);

    MarkdownPanel_handlePointer(mp, PTR_DOWN, 100.0f, 40.5f, nullptr);
    MarkdownPanel_getSelection(mp, &sel0, &sel1);
    CHECK("gap click after midpoint hits row 1", sel0 >= 5 && sel0 <= 19);
    MarkdownPanel_handlePointer(mp, PTR_UP, 100.0f, 40.5f, nullptr);

    // H4 heading support (####).
    MarkdownPanel *mp4 = MarkdownPanel_0();
    MarkdownPanel_setText(mp4, "#### Sub Heading Level 4\nbody");
    CHECK("H4 row count", MarkdownPanel_getRowCount(mp4) == 2);
    Panel *h4Row = MarkdownPanel_getRow(mp4, 0);
    float h4H = h4Row ? Component_getHeight(&(*h4Row).component) : 0.0f;
    float expH4 = 15.0f * 1.35f;
    CHECK("H4 row height matches 15pt", h4H > expH4 - 1.0f && h4H < expH4 + 1.0f);

    // Typography properties
    CHECK("MarkdownPanel default textAlign", MarkdownPanel_getTextAlign(mp4) == TEXT_ALIGN_LEFT);
    MarkdownPanel_setTextAlign(mp4, TEXT_ALIGN_CENTER);
    CHECK("MarkdownPanel textAlign center", MarkdownPanel_getTextAlign(mp4) == TEXT_ALIGN_CENTER);
    MarkdownPanel_setTextAlign(mp4, TEXT_ALIGN_RIGHT);
    CHECK("MarkdownPanel textAlign right", MarkdownPanel_getTextAlign(mp4) == TEXT_ALIGN_RIGHT);

    CHECK("MarkdownPanel default spacingWidth", MarkdownPanel_getSpacingWidth(mp4) == 0.0f);
    MarkdownPanel_setSpacingWidth(mp4, 1.5f);
    CHECK("MarkdownPanel spacingWidth", fabsf(MarkdownPanel_getSpacingWidth(mp4) - 1.5f) < 0.001f);

    CHECK("MarkdownPanel default spacingHeight", MarkdownPanel_getSpacingHeight(mp4) == 0.0f);
    MarkdownPanel_setSpacingHeight(mp4, 3.0f);
    CHECK("MarkdownPanel spacingHeight", fabsf(MarkdownPanel_getSpacingHeight(mp4) - 3.0f) < 0.001f);

    CHECK("MarkdownPanel default ligatures", MarkdownPanel_hasLigatures(mp4) == true);
    MarkdownPanel_setLigatures(mp4, false);
    CHECK("MarkdownPanel ligatures false", MarkdownPanel_hasLigatures(mp4) == false);

    MarkdownPanel_free(mp4);

    MarkdownPanel_free(mp);

    if (g_failures == 0)
        printf("\n=== MarkdownPanel Test Summary: 0 failures ===\n");
    else
        printf("\n=== MarkdownPanel Test Summary: %d failures ===\n", g_failures);
    return g_failures == 0 ? 0 : 1;
}