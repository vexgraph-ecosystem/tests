#include "annotation/overview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "darling/label/label.h"
#include "darling/cursor/cursor.h"
#include "text/text_core.h"
#include "nio/mem.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: LabelHighlightTest (label_highlight_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for the Label selection-highlight split: the raster is
 * text-only stable (selection never baked, glyphX still installed) while
 * the live TextSelect span drives a per-frame Vk_fillRect overlay.
 *
 * Selection edits must leave rasterTex identity + rasterDirty untouched;
 * the suite pokes rasterDirty false (test seam — the flag has no public
 * reset) and asserts span tracking through null/empty/reversed/cleared,
 * glyphX-NULL uniform fallback, mnemonic-folded tables, and a full
 * pointer drag sequence.
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
    if (cond) { printf("[label_highlight_test] PASS %s\n", name); } \
    else { printf("[label_highlight_test] FAIL %s\n", name); g_failures++; } \
} while (0)

int main(void) {
    printf("=== Running Label Highlight Split Test Suite ===\n");

    // section 1 Null safety (Rule 35 cold seam): no crash, safe defaults.
    {
        Label_setSelection(nullptr, 2, 7);
        int32_t s0 = -2, s1 = -2;
        Label_getSelection(nullptr, &s0, &s1);
        CHECK("null getSelection defaults", s0 == -1 && s1 == -1);
        CHECK("null charIndexAt", Label_charIndexAt(nullptr, 10.0f) == 0);
        CHECK("null isRasterDirty", Label_isRasterDirty(nullptr) == false);
        CHECK("null rasterTexture", Label_getRasterTexture(nullptr) == -1);
        Label_handlePointer(nullptr, PTR_DRAG, 10.0f, 10.0f, nullptr);
        CHECK("null handlePointer survives", 1);
    }

    // section 2 Empty text: selection ops are no-ops on the raster, never crash.
    {
        Label *lbl = Label_1("");
        Label_setHighlightable(lbl, true);
        (*lbl).rasterDirty = false;
        int32_t tex0 = Label_getRasterTexture(lbl);
        Label_setSelection(lbl, 0, 5);
        CHECK("empty charIndexAt", Label_charIndexAt(lbl, 10.0f) == 0);
        CHECK("empty rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("empty rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        Label_handlePointer(lbl, PTR_DOWN, 5.0f, 5.0f, nullptr);
        Label_handlePointer(lbl, PTR_DRAG, 40.0f, 5.0f, nullptr);
        Label_handlePointer(lbl, PTR_UP, 40.0f, 5.0f, nullptr);
        CHECK("empty drag rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("empty drag rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        Label_free(lbl);
    }

    // section 3 Reversed span orders; cleared span resets; raster untouched.
    {
        Label *lbl = Label_1("Hello World");
        Label_setHighlightable(lbl, true);
        Label_setSize(lbl, 150.0f, 24.0f);
        (*lbl).rasterDirty = false;
        int32_t tex0 = Label_getRasterTexture(lbl);
        Label_setSelection(lbl, 7, 2);
        int32_t s0 = -1, s1 = -1;
        Label_getSelection(lbl, &s0, &s1);
        CHECK("reversed orders to (2,7)", s0 == 2 && s1 == 7);
        CHECK("reversed rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("reversed rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        Label_setSelection(lbl, 2, 5);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("forward span (2,5)", s0 == 2 && s1 == 5);
        Label_setSelection(lbl, -1, 5);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("negative start clears", s0 == -1 && s1 == -1);
        Label_setSelection(lbl, 2, 5);
        Label_setSelection(lbl, 2, -1);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("negative end clears", s0 == -1 && s1 == -1);
        CHECK("cleared rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("cleared rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        // Control: text edits still dirty the raster (flag mechanism alive).
        Label_setText(lbl, "Hello Worlds");
        CHECK("text edit dirties raster", Label_isRasterDirty(lbl) == true);
        Label_free(lbl);
    }

    // section 4 glyphX-NULL uniform fallback: span tracks, raster stable.
    {
        Label *lbl = Label_1("Hello World");
        CHECK("glyph table absent headless", Label_getGlyphOffsets(lbl) == nullptr);
        Label_setHighlightable(lbl, true);
        (*lbl).rasterDirty = false;
        int32_t tex0 = Label_getRasterTexture(lbl);
        Label_setSelection(lbl, 1, 4);
        int32_t s0 = -1, s1 = -1;
        Label_getSelection(lbl, &s0, &s1);
        CHECK("fallback span (1,4)", s0 == 1 && s1 == 4);
        int32_t mid = Label_charIndexAt(lbl, 30.0f);
        CHECK("fallback hit in range", mid > 0 && mid < 11);
        CHECK("fallback rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("fallback rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        Label_free(lbl);
    }

    // section 5 Mnemonic-folded table: the '&' marker byte folds onto the previous
    // glyph (zero-width phantom) while the label-space span covers the full
    // range the overlay paints.
    {
        Label *lbl = Label_1("a&b");
        Label_setMnemonic(lbl, true);
        Label_setHighlightable(lbl, true);
        float *gx = (float*) Memory_alloc(TYPE_ARRAY, 4 * sizeof(float));
        gx[0] = 0.0f;
        gx[1] = 0.0f;
        gx[2] = 10.0f;
        gx[3] = 20.0f;
        (*lbl).glyphX = gx;
        (*lbl).glyphN = 4;
        (*lbl).rasterDirty = false;
        int32_t tex0 = Label_getRasterTexture(lbl);
        Label_setSelection(lbl, 0, 3);
        int32_t s0 = -1, s1 = -1;
        Label_getSelection(lbl, &s0, &s1);
        CHECK("mnemonic span covers label range", s0 == 0 && s1 == 3);
        const float *tab = Label_getGlyphOffsets(lbl);
        CHECK("marker folds onto previous glyph", tab[1] == tab[0]);
        CHECK("overlay width spans stripped advance", tab[3] - tab[0] == 20.0f);
        CHECK("mnemonic rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("mnemonic rasterTex identity", Label_getRasterTexture(lbl) == tex0);
        Label_free(lbl);
    }

    // section 6 Drag sequence: rasterTex identity + rasterDirty==false throughout
    // while the overlay span tracks anchor/active. Uniform fallback:
    // len 15, fontSize 12 -> qw = 90; DOWN x=30 anchors 5, DRAG x=120 hits 15.
    {
        Label *lbl = Label_1("Selectable Text");
        Label_setHighlightable(lbl, true);
        Label_setSize(lbl, 150.0f, 24.0f);
        (*lbl).rasterDirty = false;
        int32_t tex0 = Label_getRasterTexture(lbl);
        int32_t s0 = -1, s1 = -1;

        Label_handlePointer(lbl, PTR_DOWN, 30.0f, 10.0f, nullptr);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("drag anchor collapses at 5", s0 == 5 && s1 == 5);
        CHECK("down rasterDirty stays false", Label_isRasterDirty(lbl) == false);

        Label_handlePointer(lbl, PTR_DRAG, 120.0f, 10.0f, nullptr);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("drag overlay tracks to (5,15)", s0 == 5 && s1 == 15);
        CHECK("drag rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("drag rasterTex identity", Label_getRasterTexture(lbl) == tex0);

        Label_handlePointer(lbl, PTR_UP, 120.0f, 10.0f, nullptr);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("release commits (5,15)", s0 == 5 && s1 == 15);
        CHECK("up rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("up rasterTex identity", Label_getRasterTexture(lbl) == tex0);

        // Backward drag from the far edge re-anchors and orders mid-drag.
        Label_handlePointer(lbl, PTR_DOWN, 120.0f, 10.0f, nullptr);
        Label_handlePointer(lbl, PTR_DRAG, 30.0f, 10.0f, nullptr);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("backward drag orders (5,15)", s0 == 5 && s1 == 15);
        CHECK("backward rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("backward rasterTex identity", Label_getRasterTexture(lbl) == tex0);

        // Plain click collapses and clears without touching the raster.
        Label_handlePointer(lbl, PTR_DOWN, 60.0f, 10.0f, nullptr);
        Label_handlePointer(lbl, PTR_UP, 60.0f, 10.0f, nullptr);
        Label_getSelection(lbl, &s0, &s1);
        CHECK("plain click clears", s0 == -1 && s1 == -1);
        CHECK("click rasterDirty stays false", Label_isRasterDirty(lbl) == false);
        CHECK("click rasterTex identity", Label_getRasterTexture(lbl) == tex0);

        Label_free(lbl);
    }

    printf("\n=== Label Highlight Split Test Summary: %d failures ===\n", g_failures);
    return g_failures > 0 ? 1 : 0;
}
