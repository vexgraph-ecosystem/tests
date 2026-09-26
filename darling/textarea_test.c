#include "annotation/overview.h"

#include <stdio.h>
#include <string.h>

#include "darling/field/textarea.h"
#include "event/keyevent.h"
#include "event/pointer.h"
#include "input/key.h"
#include "nio/mem.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: TextareaTest (darling/field/textarea_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for Textarea live editing (Pkg 4): multiline insert,
 * enter-newline, backspace across newline join, up/down/left/right nav,
 * caret-follow scroll, onChange notification, focus on DOWN, null-safety.
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
    if (cond) { printf("[textarea_test] PASS %s\n", name); } \
    else { printf("[textarea_test] FAIL %s\n", name); g_failures++; } \
} while (0)

static void onFire(void *ctx) {
    (*(int*) ctx)++;
}

static UIKeyEvent *keyPress(int32_t code, int32_t ch) {
    UIKeyEvent *ev = UIKeyEvent_0();
    if (ev) {
        UIKeyEvent_setKeyCode(ev, code);
        UIKeyEvent_setCh(ev, ch);
        UIKeyEvent_setPressed(ev, true);
        UIKeyEvent_setRepeat(ev, false);
    }
    return ev;
}

static void sendKey(Textarea *ta, int32_t code, int32_t ch) {
    UIKeyEvent *ev = keyPress(code, ch);
    Textarea_handleKey(ta, ev);
    Memory_free(ev);
}

int main(void) {
    printf("=== Running Textarea Test Suite ===\n");

    // section 1 Null-safety: handlers and getters are null-safe no-ops.
    {
        Textarea_handlePointer(nullptr, PTR_DOWN, 0.0f, 0.0f);
        Textarea_handleKey(nullptr, nullptr);
        Textarea_goTo(nullptr, 3);
        Textarea_setCursor(nullptr, 3);
        Textarea_setOnChange(nullptr, onFire);
        Textarea_setCtx(nullptr, nullptr);
        CHECK("null handlePointer no crash", true);
        Textarea *ta = Textarea_0();
        CHECK("Textarea_0 created", ta != nullptr);
        Textarea_handleKey(ta, nullptr);
        CHECK("null ev no crash", Textarea_getText(ta) == nullptr);
        UIKeyEvent *rel = UIKeyEvent_0();
        UIKeyEvent_setKeyCode(rel, KEY_A);
        UIKeyEvent_setCh(rel, 'Z');
        UIKeyEvent_setPressed(rel, false);
        Textarea_handleKey(ta, rel);
        Memory_free(rel);
        CHECK("release ignored", Textarea_getText(ta) == nullptr && Textarea_getCursor(ta) == 0);
        CHECK("null getters", Textarea_getCursor(nullptr) == 0 && Textarea_isFocused(nullptr) == false);
        CHECK("null callback getters", Textarea_getOnChange(nullptr) == nullptr && Textarea_getCtx(nullptr) == nullptr);
        Textarea_free(ta);
    }

    // section 2 Multiline insert + enter-newline + onChange notification.
    {
        Textarea *ta = Textarea_0();
        int fires = 0;
        Textarea_setOnChange(ta, onFire);
        Textarea_setCtx(ta, &fires);
        CHECK("callback getter round-trip", Textarea_getOnChange(ta) == onFire && Textarea_getCtx(ta) == &fires);
        sendKey(ta, KEY_H, 'H');
        sendKey(ta, KEY_I, 'i');
        CHECK("printable insert", strcmp(Textarea_getText(ta), "Hi") == 0 && Textarea_getCursor(ta) == 2);
        sendKey(ta, KEY_ENTER, -1);
        CHECK("enter inserts newline", strcmp(Textarea_getText(ta), "Hi\n") == 0 && Textarea_getCursor(ta) == 3);
        sendKey(ta, KEY_Y, 'y');
        sendKey(ta, KEY_O, 'o');
        CHECK("type on second line", strcmp(Textarea_getText(ta), "Hi\nyo") == 0 && Textarea_getCursor(ta) == 5);
        CHECK("onChange fired per edit", fires == 5);
        Textarea_free(ta);
    }

    // section 3 Backspace across newline join.
    {
        Textarea *ta = Textarea_0();
        int fires = 0;
        Textarea_setOnChange(ta, onFire);
        Textarea_setCtx(ta, &fires);
        Textarea_setText(ta, "Hi\nyo");
        Textarea_goTo(ta, 3);
        sendKey(ta, KEY_BACKSPACE, -1);
        CHECK("backspace joins lines", strcmp(Textarea_getText(ta), "Hiyo") == 0 && Textarea_getCursor(ta) == 2);
        CHECK("backspace fired", fires == 1);
        Textarea_goTo(ta, 0);
        sendKey(ta, KEY_BACKSPACE, -1);
        CHECK("backspace at 0 no-op", strcmp(Textarea_getText(ta), "Hiyo") == 0 && fires == 1);
        Textarea_free(ta);
    }

    // section 4 Left/right by one char, up/down across line boundaries.
    {
        Textarea *ta = Textarea_0();
        int fires = 0;
        Textarea_setOnChange(ta, onFire);
        Textarea_setCtx(ta, &fires);
        Textarea_setText(ta, "ab\ncdef\ngh");
        Textarea_goTo(ta, 0);
        sendKey(ta, KEY_RIGHT, -1);
        CHECK("right by one", Textarea_getCursor(ta) == 1);
        sendKey(ta, KEY_LEFT, -1);
        CHECK("left by one", Textarea_getCursor(ta) == 0);
        sendKey(ta, KEY_LEFT, -1);
        CHECK("left at 0 clamps", Textarea_getCursor(ta) == 0);
        sendKey(ta, KEY_DOWN, -1);
        CHECK("down to line1 col0", Textarea_getCursor(ta) == 3);
        sendKey(ta, KEY_DOWN, -1);
        CHECK("down to line2 col0", Textarea_getCursor(ta) == 8);
        sendKey(ta, KEY_DOWN, -1);
        CHECK("down past end clamps", Textarea_getCursor(ta) == 8);
        sendKey(ta, KEY_UP, -1);
        CHECK("up to line1 col0", Textarea_getCursor(ta) == 3);
        Textarea_goTo(ta, 6);
        sendKey(ta, KEY_DOWN, -1);
        CHECK("down clamps column", Textarea_getCursor(ta) == 10);
        sendKey(ta, KEY_UP, -1);
        CHECK("up keeps column", Textarea_getCursor(ta) == 5);
        CHECK("nav never fires", fires == 0);
        Textarea_setCursor(ta, 999);
        CHECK("setCursor clamps", Textarea_getCursor(ta) == 10);
        Textarea_free(ta);
    }

    // section 5 Caret-follow scroll keeps the caret line visible.
    {
        Textarea *ta = Textarea_0();
        Textarea_setVisibleLines(ta, 2);
        Textarea_setText(ta, "l0\nl1\nl2\nl3\nl4");
        Textarea_setScrollY(ta, 0.0f);
        Textarea_goTo(ta, 0);
        sendKey(ta, KEY_DOWN, -1);
        sendKey(ta, KEY_DOWN, -1);
        sendKey(ta, KEY_DOWN, -1);
        sendKey(ta, KEY_DOWN, -1);
        CHECK("scroll follows down", Textarea_getScrollY(ta) == 3.0f);
        sendKey(ta, KEY_UP, -1);
        sendKey(ta, KEY_UP, -1);
        sendKey(ta, KEY_UP, -1);
        sendKey(ta, KEY_UP, -1);
        CHECK("scroll follows up", Textarea_getScrollY(ta) == 0.0f);
        Textarea_free(ta);
    }

    // section 6 DOWN focuses; other kinds leave focus alone.
    {
        Textarea *ta = Textarea_0();
        CHECK("unfocused initially", Textarea_isFocused(ta) == false);
        Textarea_handlePointer(ta, PTR_HOVER, 1.0f, 1.0f);
        CHECK("hover does not focus", Textarea_isFocused(ta) == false);
        Textarea_handlePointer(ta, PTR_DOWN, 1.0f, 1.0f);
        CHECK("DOWN focuses", Textarea_isFocused(ta) == true);
        Textarea_free(ta);
        Textarea_free(nullptr);
        CHECK("free null no crash", true);
    }

    // Pkg 6: Typography, Alignment & Selection
    {
        Textarea *ta = Textarea_0();
        CHECK("default fontSize 13", Textarea_getFontSize(ta) == 13.0f);
        CHECK("default align LEFT", Textarea_getTextAlign(ta) == TEXT_ALIGN_LEFT);
        CHECK("default ligatures true", Textarea_hasLigatures(ta) == true);

        Textarea_setFontSize(ta, 16.0f);
        Textarea_setTextColor(ta, 0xFF00FF00u);
        Textarea_setTextAlign(ta, TEXT_ALIGN_CENTER);
        Textarea_setSpacingWidth(ta, 1.5f);
        Textarea_setSpacingHeight(ta, 4.0f);
        Textarea_setLigatures(ta, false);
        Textarea_setSelectionColor(ta, 0x88336699u);

        CHECK("fontSize 16", Textarea_getFontSize(ta) == 16.0f);
        CHECK("textColor green", Textarea_getTextColor(ta) == 0xFF00FF00u);
        CHECK("align CENTER", Textarea_getTextAlign(ta) == TEXT_ALIGN_CENTER);
        CHECK("spacingWidth 1.5", Textarea_getSpacingWidth(ta) == 1.5f);
        CHECK("spacingHeight 4.0", Textarea_getSpacingHeight(ta) == 4.0f);
        CHECK("ligatures false", Textarea_hasLigatures(ta) == false);
        CHECK("selectionColor", Textarea_getSelectionColor(ta) == 0x88336699u);

        Textarea_setText(ta, "Hello World\nLine Two");
        Textarea_setSelection(ta, 6, 11);
        int32_t s0 = -1, s1 = -1;
        Textarea_getSelection(ta, &s0, &s1);
        CHECK("selection span start 6", s0 == 6);
        CHECK("selection span end 11", s1 == 11);

        char *sel = Textarea_getSelectedText(ta);
        CHECK("getSelectedText World", sel != nullptr && strcmp(sel, "World") == 0);
        if (sel) Memory_free(sel);

        Textarea_setSelectedText(ta, "Darling");
        CHECK("setSelectedText replaces span", strcmp(Textarea_getText(ta), "Hello Darling\nLine Two") == 0);

        Textarea_free(ta);
    }

    printf("\n=== Textarea Test Summary: %d failures ===\n", g_failures);
    return g_failures > 0 ? 1 : 0;
}

