#include <stdio.h>
#include <string.h>

#include "darling/field/inputotp.h"
#include "event/keyevent.h"
#include "event/pointer.h"
#include "input/key.h"
#include "nio/mem.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[inputotp_test] PASS %s\n", name); } \
    else { printf("[inputotp_test] FAIL %s\n", name); g_failures++; } \
} while (0)

static void onComplete(void *ctx) {
    (*(int*) ctx)++;
}

static UIKeyEvent *keyPress(int32_t code, int32_t ch, uint32_t mods) {
    UIKeyEvent *ev = UIKeyEvent_0();
    if (ev) {
        UIKeyEvent_setKeyCode(ev, code);
        UIKeyEvent_setCh(ev, ch);
        UIKeyEvent_setMods(ev, mods);
        UIKeyEvent_setPressed(ev, true);
        UIKeyEvent_setRepeat(ev, false);
    }
    return ev;
}

static void sendKey(InputOTP *otp, int32_t code, int32_t ch, uint32_t mods) {
    UIKeyEvent *ev = keyPress(code, ch, mods);
    InputOTP_handleKey(otp, ev);
    Memory_free(ev);
}

int main(void) {
    printf("=== Running InputOTP Test Suite ===\n");

    // 1. Lifecycle & Defaults
    {
        InputOTP *otp = InputOTP_1(6);
        CHECK("created 6 boxes", InputOTP_getLength(otp) == 6);
        CHECK("default boxSize 40", InputOTP_getBoxSize(otp) == 40.0f);
        CHECK("default gap 8", InputOTP_getGap(otp) == 8.0f);
        CHECK("default cursor 0", InputOTP_getCursor(otp) == 0);
        CHECK("unfocused initially", InputOTP_isFocused(otp) == false);
        CHECK("password false", InputOTP_isPassword(otp) == false);
        CHECK("digits empty", strcmp(InputOTP_getDigits(otp), "") == 0);

        InputOTP_setBoxSize(otp, 48.0f);
        InputOTP_setGap(otp, 12.0f);
        InputOTP_setPassword(otp, true);
        InputOTP_setFontSize(otp, 20.0f);
        InputOTP_setTextColor(otp, 0xFFEEDDCCu);
        InputOTP_setBoxBackground(otp, 0xFF223344u);
        InputOTP_setBoxBorderColor(otp, 0xFF556677u);
        InputOTP_setBoxActiveBorder(otp, 0xFF8899AAu);

        CHECK("boxSize 48", InputOTP_getBoxSize(otp) == 48.0f);
        CHECK("gap 12", InputOTP_getGap(otp) == 12.0f);
        CHECK("password true", InputOTP_isPassword(otp) == true);
        CHECK("fontSize 20", InputOTP_getFontSize(otp) == 20.0f);
        CHECK("textColor", InputOTP_getTextColor(otp) == 0xFFEEDDCCu);
        CHECK("boxBackground", InputOTP_getBoxBackground(otp) == 0xFF223344u);
        CHECK("boxBorderColor", InputOTP_getBoxBorderColor(otp) == 0xFF556677u);
        CHECK("boxActiveBorder", InputOTP_getBoxActiveBorder(otp) == 0xFF8899AAu);

        InputOTP_free(otp);
        InputOTP_free(nullptr);
        CHECK("free null no crash", true);
    }

    // 2. Typing & Auto-Advance & Completion
    {
        int completedCount = 0;
        InputOTP *otp = InputOTP_1(4);
        InputOTP_setOnComplete(otp, onComplete);
        InputOTP_setCtx(otp, &completedCount);

        // PTR_DOWN focuses
        InputOTP_handlePointer(otp, PTR_DOWN, 10.0f, 10.0f);
        CHECK("focused after DOWN", InputOTP_isFocused(otp) == true);

        // Type '1'
        sendKey(otp, KEY_NUM_1, '1', 0);
        CHECK("digit 1 typed", strcmp(InputOTP_getDigits(otp), "1") == 0);
        CHECK("cursor advanced to 1", InputOTP_getCursor(otp) == 1);
        CHECK("not completed yet", completedCount == 0);

        // Type '2'
        sendKey(otp, KEY_NUM_2, '2', 0);
        CHECK("digit 2 typed", strcmp(InputOTP_getDigits(otp), "12") == 0);
        CHECK("cursor advanced to 2", InputOTP_getCursor(otp) == 2);

        // Type '3'
        sendKey(otp, KEY_NUM_3, '3', 0);
        CHECK("digit 3 typed", strcmp(InputOTP_getDigits(otp), "123") == 0);
        CHECK("cursor advanced to 3", InputOTP_getCursor(otp) == 3);

        // Type '4' - full!
        sendKey(otp, KEY_NUM_4, '4', 0);
        CHECK("digit 4 typed", strcmp(InputOTP_getDigits(otp), "1234") == 0);
        CHECK("onComplete fired", completedCount == 1);

        // Backspace
        sendKey(otp, KEY_BACKSPACE, 8, 0);
        CHECK("backspace cleared last digit", strcmp(InputOTP_getDigits(otp), "123") == 0);

        // Navigation
        sendKey(otp, KEY_LEFT, 0, 0);
        CHECK("left moves cursor", InputOTP_getCursor(otp) == 2);
        sendKey(otp, KEY_RIGHT, 0, 0);
        CHECK("right moves cursor", InputOTP_getCursor(otp) == 3);

        InputOTP_free(otp);
    }

    // 3. Direct Digits Setter & Clamping
    {
        InputOTP *otp = InputOTP_1(6);
        InputOTP_setDigits(otp, "123456");
        CHECK("setDigits 123456", strcmp(InputOTP_getDigits(otp), "123456") == 0);
        CHECK("cursor clamped to 5", InputOTP_getCursor(otp) == 5);

        InputOTP_setDigits(otp, "123456789");
        CHECK("setDigits truncated to length 6", strcmp(InputOTP_getDigits(otp), "123456") == 0);

        InputOTP_free(otp);
    }

    printf("\n=== InputOTP Test Summary: %d failures ===\n", g_failures);
    return g_failures > 0 ? 1 : 0;
}
