// Headless ownership proof for ScrollCapture. No window, GPU, thread, fixed
// candidate array, or allocation occurs in the capture/event hot path.

#include "darling/panel/scroll_panel.h"
#include "event/scroll_capture.h"

#include <stdbool.h>
#include <stdio.h>

static int failures;

static void check(bool condition, const char *name) {
    if (!condition) {
        failures++;
        printf("FAIL %s\n", name);
    } else {
        printf("ok %s\n", name);
    }
}

static ScrollPanel *panel(float contentW, float contentH, int mode) {
    ScrollPanel *result = ScrollPanel_2(100.0f, 100.0f);
    Panel *content = Panel_0();
    Panel_setSize(content, contentW, contentH);
    ScrollPanel_setContent(result, content);
    ScrollPanel_horizontalScroll_setScrollMode(result, mode);
    ScrollPanel_verticalScroll_setScrollMode(result, mode);
    return result;
}

int main(void) {
    ScrollCapture *capture = ScrollCapture_0();
    check(capture != nullptr, "construct");
    check(ScrollCapture_getWheelIdleMs(capture) == SCROLL_CAPTURE_WHEEL_IDLE_MS_DEFAULT,
          "wheel-timeout-default");
    ScrollCapture_setWheelIdleMs(capture, 37u);
    ScrollCapture_setMomentumGraceMs(capture, 19u);
    check(ScrollCapture_getWheelIdleMs(capture) == 37u, "wheel-timeout-runtime");
    check(ScrollCapture_getMomentumGraceMs(capture) == 19u, "grace-runtime");

    ScrollPanel *child = panel(100.0f, 300.0f, SCROLL_BAR_STEP);
    ScrollPanel *parent = panel(100.0f, 500.0f, SCROLL_BAR_STEP);
    ScrollPanel *ordered[2] = { child, parent };
    ScrollCapture_directBegin(capture, ordered, 2u, 0.0f, 20.0f, 100u);
    check(ScrollCapture_getOwner(capture) == child, "interior-child-captures");
    ScrollCapture_directChange(capture, 0.0f, 250.0f, 110u);
    float x = 0.0f, y = 0.0f;
    ScrollPanel_getOffset(child, &x, &y);
    check(y == 200.0f, "reaching-edge-stays-child");
    ScrollCapture_directChange(capture, 0.0f, -20.0f, 120u);
    check(ScrollCapture_getOwner(capture) == child, "reversing-stays-child");
    ScrollCapture_cancel(capture);

    ScrollPanel_setOffset(child, 0.0f, 200.0f);
    ScrollCapture_directBegin(capture, ordered, 2u, 0.0f, 10.0f, 200u);
    check(ScrollCapture_getOwner(capture) == parent, "hard-stop-selects-parent");
    ScrollCapture_cancel(capture);

    ScrollPanel_verticalScroll_setScrollMode(child, SCROLL_BAR_ELASTIC);
    ScrollCapture_directBegin(capture, ordered, 2u, 0.0f, 10.0f, 300u);
    check(ScrollCapture_getOwner(capture) == child, "elastic-edge-captures");
    ScrollCapture_cancel(capture);

    ScrollPanel *fit = panel(100.0f, 100.0f, SCROLL_BAR_ELASTIC);
    ScrollPanel *fitOrdered[2] = { fit, parent };
    ScrollCapture_directBegin(capture, fitOrdered, 2u, 0.0f, 10.0f, 400u);
    check(ScrollCapture_getOwner(capture) == parent, "non-scrollable-elastic-skipped");
    ScrollCapture_cancel(capture);

    ScrollPanel *diagonal = panel(300.0f, 100.0f, SCROLL_BAR_STEP);
    ScrollPanel *diagonalOrdered[2] = { diagonal, parent };
    ScrollCapture_directBegin(capture, diagonalOrdered, 2u, 10.0f, 10.0f, 500u);
    check(ScrollCapture_getOwner(capture) == diagonal, "diagonal-any-axis-captures-whole");
    ScrollCapture_directChange(capture, 10.0f, 10.0f, 510u);
    ScrollPanel_getOffset(diagonal, &x, &y);
    check(x == 10.0f && y == 0.0f, "unsupported-component-dropped");
    ScrollCapture_directEnd(capture, 520u);
    check(ScrollCapture_getOwner(capture) == diagonal, "direct-end-retains-for-handoff");
    ScrollCapture_nativeMomentumBegin(capture, 525u);
    check(ScrollCapture_getOwner(capture) == diagonal, "native-momentum-same-owner");
    ScrollCapture_nativeMomentumChange(capture, 10.0f, 0.0f, 530u);
    check(ScrollPanel_horizontalScroll_getMotionWriter(diagonal) == SCROLLPANEL_MOTION_NATIVE,
          "native-is-motion-writer");
    ScrollCapture_nativeMomentumEnd(capture, 540u);
    check(!ScrollCapture_isActive(capture), "native-end-releases");

    ScrollPanel *wheelA = panel(100.0f, 300.0f, SCROLL_BAR_STEP);
    ScrollPanel *wheelB = panel(100.0f, 300.0f, SCROLL_BAR_STEP);
    ScrollPanel *wheelFirst[1] = { wheelA };
    ScrollPanel *wheelChanged[1] = { wheelB };
    ScrollCapture_wheel(capture, wheelFirst, 1u, 0.0f, 10.0f, 600u);
    ScrollCapture_wheel(capture, wheelChanged, 1u, 0.0f, 10.0f, 620u);
    check(ScrollCapture_getOwner(capture) == wheelA, "wheel-burst-does-not-retarget");
    ScrollCapture_tick(capture, 656u);
    check(ScrollCapture_isActive(capture), "wheel-before-timeout-retained");
    ScrollCapture_tick(capture, 657u);
    check(!ScrollCapture_isActive(capture), "wheel-timeout-releases");
    ScrollCapture_wheel(capture, wheelChanged, 1u, 0.0f, 10.0f, 700u);
    check(ScrollCapture_getOwner(capture) == wheelB, "wheel-after-timeout-reacquires");
    ScrollCapture_wheel(capture, wheelFirst, 1u, 0.0f, 10.0f, 738u);
    check(ScrollCapture_getOwner(capture) == wheelA,
          "wheel-new-packet-expires-burst-without-display-tick");
    ScrollCapture_cancel(capture);
    check(ScrollCapture_getState(capture) == SCROLL_CAPTURE_IDLE, "cancel-idle");

    // No eligible ancestor is still a captured decision for this gesture.
    ScrollPanel_setOffset(child, 0.0f, 200.0f);
    ScrollPanel_setOffset(parent, 0.0f, 400.0f);
    ScrollPanel_verticalScroll_setScrollMode(child, SCROLL_BAR_STEP);
    ScrollCapture_directBegin(capture, ordered, 2u, 0.0f, 10.0f, 800u);
    check(ScrollCapture_getState(capture) == SCROLL_CAPTURE_DIRECT
          && ScrollCapture_getOwner(capture) == nullptr, "blocked-gesture-remains-pending");
    ScrollCapture_directChange(capture, 0.0f, -10.0f, 810u);
    check(ScrollCapture_getOwner(capture) == nullptr, "blocked-gesture-does-not-reacquire");
    ScrollCapture_directEnd(capture, 820u);
    check(ScrollCapture_getState(capture) == SCROLL_CAPTURE_IDLE, "blocked-end-idle");
    ScrollCapture_wheel(capture, ordered, 2u, 0.0f, 10.0f, 900u);
    check(ScrollCapture_getState(capture) == SCROLL_CAPTURE_WHEEL_BURST
          && ScrollCapture_getOwner(capture) == nullptr, "blocked-wheel-burst-retained");
    ScrollCapture_wheel(capture, ordered, 2u, 0.0f, -10.0f, 910u);
    check(ScrollCapture_getOwner(capture) == nullptr, "blocked-wheel-does-not-reacquire");
    ScrollCapture_wheel(capture, ordered, 2u, 0.0f, -10.0f, 950u);
    check(ScrollCapture_getOwner(capture) == child, "blocked-wheel-new-burst-reacquires");
    ScrollCapture_cancel(capture);

    // Releasing an elastic overscroll starts the rebound at fingers-up; it must
    // not wait out the momentum grace as a dead beat.
    ScrollCapture_setMomentumGraceMs(capture, 500u);
    ScrollPanel *bound = panel(100.0f, 300.0f, SCROLL_BAR_ELASTIC);
    ScrollPanel *boundOrdered[1] = { bound };
    ScrollCapture_directBegin(capture, boundOrdered, 1u, 0.0f, 20.0f, 1000u);
    ScrollCapture_directChange(capture, 0.0f, 500.0f, 1020u);
    ScrollPanel_getOffset(bound, &x, &y);
    float parked = y;
    check(parked > 200.0f, "release-starts-overscrolled");
    ScrollCapture_directEnd(capture, 1040u);
    for (uint64_t t = 1060u; t <= 1080u; t += 20u) {
        ScrollCapture_tick(capture, t);
        ScrollPanel_tick(bound, t);
    }
    ScrollPanel_getOffset(bound, &x, &y);
    check(y < parked, "release-rebounds-before-momentum");
    ScrollCapture_cancel(capture);

    // AppKit may put contact-ended and momentum-began on one packet. The
    // capture must release the finger hold even without a separate END event.
    ScrollPanel *combined = panel(100.0f, 300.0f, SCROLL_BAR_ELASTIC);
    ScrollPanel *combinedOrdered[1] = { combined };
    ScrollCapture_directBegin(capture, combinedOrdered, 1u, 0.0f, 20.0f, 2000u);
    ScrollCapture_directChange(capture, 0.0f, 500.0f, 2020u);
    ScrollPanel_getOffset(combined, &x, &y);
    parked = y;
    ScrollCapture_nativeMomentumBegin(capture, 2040u);
    check(!ScrollPanel_isGestureHeld(combined), "combined-phase-releases-contact");
    ScrollCapture_nativeMomentumChange(capture, 0.0f, 20.0f, 2050u);
    ScrollPanel_tick(combined, 2060u);
    ScrollPanel_getOffset(combined, &x, &y);
    check(y < parked, "combined-phase-rebounds-immediately");
    ScrollCapture_nativeMomentumEnd(capture, 2080u);

    if (failures == 0)
        printf("scroll_capture_test: all green\n");
    return failures == 0 ? 0 : 1;
}
