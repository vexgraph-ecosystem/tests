// tests/scroll_panel_test.c — headless proof for ScrollPanel + ScrollBar
// overlay behaviors.
//
// MODULE harness (procedural entry, no owned struct): all layers attached
// (content + h/v bars, bars front), scroll offsets clamp to bounds,
// thumb short-length floor, per-bar hide-when-unused + opacity + idle
// timeout independence, tick-driven auto-hide, and string forms. Pure
// Component math with an explicit caller clock — no window, no GPU,
// no threads.

#include "darling/component.h"
#include "darling/field/scrollbar.h"
#include "darling/panel/panel.h"
#include "darling/panel/scroll_panel.h"
#include "lang/graphics_component.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static int failures;

static void check(bool cond, const char *name) {
    if (!cond) {
        failures++;
        printf("FAIL %s\n", name);
    } else {
        printf("ok %s\n", name);
    }
}

static bool near(float a, float b) {
    float d = a - b;
    if (d < 0.0f)
        d = -d;
    return d < 0.05f;
}

int main(void) {
    // section 1 nullptr guards (cold-strict: never crash, fail closed)
    ScrollPanel_setContent(nullptr, nullptr);
    ScrollPanel_setOffset(nullptr, 0.0f, 0.0f);
    ScrollPanel_setOffsetAt(nullptr, 0.0f, 0.0f, 0u);
    ScrollPanel_scrollBy(nullptr, 1.0f, 1.0f);
    ScrollPanel_tick(nullptr, 0u);
    ScrollPanel_layoutBars(nullptr);
    ScrollPanel_syncToBars(nullptr);
    ScrollPanel_syncFromBars(nullptr);
    float ox = -1.0f, oy = -1.0f;
    ScrollPanel_getOffset(nullptr, &ox, &oy);
    check(ox == 0.0f && oy == 0.0f, "null-offset");
    check(ScrollPanel_getContentPanel(nullptr) == nullptr, "null-content");
    check(!ScrollPanel_verticalScroll_isEffectiveVisible(nullptr), "null-v-visible");
    check(!ScrollPanel_horizontalScroll_isEffectiveVisible(nullptr), "null-h-visible");
    check(ScrollPanel_verticalScroll_getShortLengthLimit(nullptr) == 0.0f, "null-short");
    check(ScrollPanel_verticalScroll_getOpacity(nullptr) == 0.0f, "null-opacity");

    // section 2 all layers: viewport + content + h/v bars, bars front
    ScrollPanel *sp = ScrollPanel_2(200.0f, 200.0f);
    check(sp != nullptr, "construct");
    // Hard-clamp sections below assert the STEP law; elastic is the default
    // and gets its own section further down.
    ScrollPanel_verticalScroll_setScrollMode(sp, SCROLL_BAR_STEP);
    ScrollPanel_horizontalScroll_setScrollMode(sp, SCROLL_BAR_STEP);
    Panel *content = Panel_0();
    check(content != nullptr, "content-panel");
    Panel_setSize(content, 200.0f, 600.0f);
    ScrollPanel_setContent(sp, content);
    check(ScrollPanel_getContentPanel(sp) == content, "content-attached");
    Panel *self = &(*sp).base;
    size_t n = Panel_childCount(self);
    check(n == 3u, "all-layers");
    Panel *last = Panel_getChild(self, n - 1u);
    Panel *prev = Panel_getChild(self, n - 2u);
    check(last != content && prev != content, "bars-front");

    // section 3 scrolling clamps to content/viewport bounds
    ScrollPanel_setOffsetAt(sp, 0.0f, 100.0f, 100u);
    ScrollPanel_getOffset(sp, &ox, &oy);
    check(near(ox, 0.0f) && near(oy, 100.0f), "offset-set");
    ScrollPanel_setOffsetAt(sp, 0.0f, 9000.0f, 200u);
    ScrollPanel_getOffset(sp, &ox, &oy);
    check(near(oy, 400.0f), "offset-clamp-hi");
    ScrollPanel_scrollByAt(sp, 0.0f, -1000.0f, 300u);
    ScrollPanel_getOffset(sp, &ox, &oy);
    check(near(oy, 0.0f), "offset-clamp-lo");
    ScrollPanel_scrollByAt(sp, 0.0f, 150.0f, 400u);
    ScrollPanel_getOffset(sp, &ox, &oy);
    check(near(oy, 150.0f), "scroll-by");
    check(near(ScrollPanel_verticalScroll_getValue(sp), 0.375f), "bar-sync");

    // section 4 short-length floor keeps the thumb grippable
    ScrollPanel_verticalScroll_setShortLengthLimit(sp, 0.0f);
    check(ScrollPanel_verticalScroll_getShortLengthLimit(sp) == 0.0f, "short-zero");
    float tx = 0.0f, ty = 0.0f, tw = 0.0f, th = 0.0f;
    ScrollPanel_verticalScroll_getThumbRect(sp, &tx, &ty, &tw, &th);
    float smallThumb = th;
    ScrollBar_setThumbMin((*sp).vBar, 0.0f);
    ScrollPanel_verticalScroll_getThumbRect(sp, &tx, &ty, &tw, &th);
    smallThumb = th;
    ScrollPanel_verticalScroll_setShortLengthLimit(sp, 0.25f);
    check(ScrollPanel_verticalScroll_getShortLengthLimit(sp) == 0.25f, "short-set");
    ScrollPanel_verticalScroll_getThumbRect(sp, &tx, &ty, &tw, &th);
    float trackH = GraphicsComponent_getAbsH(&(*(*sp).vBar).track);
    if (trackH <= 0.0f)
        trackH = 200.0f;
    check(th >= 0.25f * trackH - 1.0f && th >= smallThumb, "short-floor");
    ScrollPanel_verticalScroll_setShortLengthLimit(sp, 5.0f);
    check(ScrollPanel_verticalScroll_getShortLengthLimit(sp) == 1.0f, "short-clamp-hi");
    ScrollPanel_verticalScroll_setShortLengthLimit(sp, -1.0f);
    check(ScrollPanel_verticalScroll_getShortLengthLimit(sp) == 0.0f, "short-clamp-lo");
    ScrollPanel_verticalScroll_setShortLengthLimit(sp, 0.0f);

    // section 5 hide-when-unused: per-bar independence
    ScrollPanel_verticalScroll_setHideWhenUnused(sp, true);
    ScrollPanel_horizontalScroll_setHideWhenUnused(sp, false);
    check(ScrollPanel_verticalScroll_isHideWhenUnused(sp), "v-autohide-on");
    check(!ScrollPanel_horizontalScroll_isHideWhenUnused(sp), "h-autohide-off");
    check(!ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-starts-hidden");
    check(ScrollPanel_horizontalScroll_isEffectiveVisible(sp), "h-stays-shown");
    ScrollPanel_setOffsetAt(sp, 0.0f, 50.0f, 1000u);
    check(ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-shows-on-scroll");
    ScrollPanel_tick(sp, 1000u + 500u);
    check(ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-visible-before-timeout");
    ScrollPanel_tick(sp, 1000u + 2000u);   // idle hold + full fade
    check(!ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-hides-on-idle");
    check(ScrollPanel_horizontalScroll_isEffectiveVisible(sp), "h-unaffected-by-idle");
    ScrollPanel_verticalScroll_setHideWhenUnused(sp, false);
    check(ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-restored");

    // section 6 hide-when-unused with nothing to scroll stays hidden
    Panel_setSize(content, 200.0f, 200.0f);
    ScrollPanel_layoutBars(sp);
    ScrollPanel_horizontalScroll_setHideWhenUnused(sp, true);
    ScrollPanel_tick(sp, 5000u);
    check(!ScrollPanel_horizontalScroll_isEffectiveVisible(sp), "h-hidden-when-unused");
    check(ScrollPanel_verticalScroll_isEffectiveVisible(sp), "v-manual-kept");
    Panel_setSize(content, 200.0f, 600.0f);
    ScrollPanel_layoutBars(sp);
    ScrollPanel_horizontalScroll_setHideWhenUnused(sp, false);
    ScrollPanel_tick(sp, 5000u);

    // section 7 opacity per bar
    ScrollPanel_verticalScroll_setOpacity(sp, 0.5f);
    ScrollPanel_horizontalScroll_setOpacity(sp, 0.25f);
    check(near(ScrollPanel_verticalScroll_getOpacity(sp), 0.5f), "v-opacity");
    check(near(ScrollPanel_horizontalScroll_getOpacity(sp), 0.25f), "h-opacity");
    ScrollPanel_verticalScroll_setOpacity(sp, 2.0f);
    check(near(ScrollPanel_verticalScroll_getOpacity(sp), 1.0f), "opacity-clamp-hi");
    ScrollPanel_verticalScroll_setOpacity(sp, -1.0f);
    check(near(ScrollPanel_verticalScroll_getOpacity(sp), 0.0f), "opacity-clamp-lo");
    ScrollPanel_verticalScroll_setOpacity(sp, 1.0f);
    ScrollPanel_horizontalScroll_setOpacity(sp, 1.0f);

    // section 9 public acquisition ability: hard stops do not claim, elastic
    // overflowing axes do, and fitting elastic axes do not.
    ScrollPanel *ability = ScrollPanel_2(200.0f, 200.0f);
    Panel *abilityContent = Panel_0();
    Panel_setSize(abilityContent, 200.0f, 600.0f);
    ScrollPanel_setContent(ability, abilityContent);
    ScrollPanel_verticalScroll_setScrollMode(ability, SCROLL_BAR_STEP);
    check(ScrollPanel_canAcquire(ability, 0.0f, 10.0f), "ability-interior");
    ScrollPanel_setOffset(ability, 0.0f, 400.0f);
    check(!ScrollPanel_canAcquire(ability, 0.0f, 10.0f), "ability-hard-stop");
    ScrollPanel_verticalScroll_setScrollMode(ability, SCROLL_BAR_ELASTIC);
    check(ScrollPanel_canAcquire(ability, 0.0f, 10.0f), "ability-elastic-edge");
    Panel_setSize(abilityContent, 200.0f, 200.0f);
    ScrollPanel_layoutBars(ability);
    check(!ScrollPanel_canAcquire(ability, 0.0f, 10.0f), "ability-fit-elastic-skipped");

    // section 10 scroll behavior: sensitivity, mode, friction, delay
    ScrollPanel *b = ScrollPanel_2(200.0f, 200.0f);
    Panel *bc = Panel_0();
    Panel_setSize(bc, 200.0f, 1000.0f);
    ScrollPanel_setContent(b, bc);
    ScrollPanel_layoutBars(b);
    check(ScrollPanel_verticalScroll_getScrollSensitivity(b) == 1.0f, "sens-default");
    check(ScrollPanel_verticalScroll_getScrollMode(b) == SCROLL_BAR_ELASTIC, "mode-default-elastic");
    ScrollPanel_verticalScroll_setScrollSensitivity(b, 2.0f);
    check(ScrollPanel_verticalScroll_getScrollSensitivity(b) == 2.0f, "sens-set");
    ScrollPanel_scrollInputAt(b, 0.0f, 100.0f, 1000u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 200.0f), "sens-scaled");
    ScrollPanel_verticalScroll_setScrollSensitivity(b, 0.5f);
    ScrollPanel_scrollInputAt(b, 0.0f, 100.0f, 1100u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 250.0f), "sens-half");
    ScrollPanel_verticalScroll_setScrollSensitivity(b, 1.0f);

    // step mode: input lands, tick glides nothing
    ScrollPanel_setOffsetAt(b, 0.0f, 0.0f, 2000u);
    ScrollPanel_verticalScroll_setScrollMode(b, SCROLL_BAR_STEP);
    ScrollPanel_verticalScroll_setScrollFriction(b, 1.0f);
    ScrollPanel_scrollInputAt(b, 0.0f, 120.0f, 2100u);
    ScrollPanel_tick(b, 2200u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 120.0f), "step-no-glide");

    // smooth mode with friction: tick glides past the input, then settles
    ScrollPanel_setOffsetAt(b, 0.0f, 0.0f, 3000u);
    ScrollPanel_verticalScroll_setScrollMode(b, SCROLL_BAR_SMOOTH);
    ScrollPanel_verticalScroll_setScrollFriction(b, 1.0f);
    ScrollPanel_verticalScroll_setScrollDelay(b, 0u);
    ScrollPanel_scrollInputAt(b, 0.0f, 120.0f, 3100u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 120.0f), "smooth-input-lands");
    ScrollPanel_tick(b, 3160u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(oy > 120.0f && oy < 240.0f, "smooth-glides");
    float afterGlide = oy;
    for (uint64_t t = 3200u; t < 8000u; t += 100u)
        ScrollPanel_tick(b, t);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(oy >= afterGlide && oy <= 240.0f, "smooth-settles");

    // friction 0: smooth mode glides nothing (no sliding action)
    ScrollPanel_setOffsetAt(b, 0.0f, 0.0f, 9000u);
    ScrollPanel_verticalScroll_setScrollFriction(b, 0.0f);
    ScrollPanel_scrollInputAt(b, 0.0f, 120.0f, 9100u);
    ScrollPanel_tick(b, 9160u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 120.0f), "friction-zero-stops");

    // delay: glide holds until the settle delay elapses
    ScrollPanel_setOffsetAt(b, 0.0f, 0.0f, 10000u);
    ScrollPanel_verticalScroll_setScrollFriction(b, 1.0f);
    ScrollPanel_verticalScroll_setScrollDelay(b, 500u);
    ScrollPanel_scrollInputAt(b, 0.0f, 120.0f, 10100u);
    ScrollPanel_tick(b, 10300u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(near(oy, 120.0f), "delay-holds");
    ScrollPanel_tick(b, 10600u);
    ScrollPanel_getOffset(b, &ox, &oy);
    check(oy > 120.0f, "delay-then-glides");

    // section 11 bar drag: thumb grab + track click (viewport-local coords)
    ScrollPanel *d = ScrollPanel_2(200.0f, 200.0f);
    Panel *dc = Panel_0();
    Panel_setSize(dc, 200.0f, 1000.0f);
    ScrollPanel_setContent(d, dc);
    ScrollPanel_layoutBars(d);
    check(!ScrollPanel_isBarDragging(d), "drag-idle");
    check(!ScrollPanel_barDragBegin(d, 100.0f, 100.0f), "drag-offbar");
    check(!ScrollPanel_isBarDragging(d), "drag-offbar-idle");
    check(ScrollPanel_barDragBegin(d, 192.0f, 5.0f), "drag-grab");
    check(ScrollPanel_isBarDragging(d), "drag-active");
    ScrollPanel_barDragTo(d, 192.0f, 83.4f);
    ScrollPanel_getOffset(d, &ox, &oy);
    check(near(oy, 400.0f), "drag-thumb-half");
    ScrollPanel_barDragEnd(d);
    check(!ScrollPanel_isBarDragging(d), "drag-ended");
    ScrollPanel_setOffsetAt(d, 0.0f, 0.0f, 1000u);
    check(ScrollPanel_barDragBegin(d, 192.0f, 190.0f), "drag-track-click");
    ScrollPanel_getOffset(d, &ox, &oy);
    check(oy > 700.0f, "drag-track-jump-end");
    ScrollPanel_barDragEnd(d);
    ScrollPanel_barDragTo(d, 192.0f, 100.0f);   // no grab: ignored
    check(!ScrollPanel_isBarDragging(d), "drag-after-end");

    // section 12 needed flags + horizontal scrolling
    ScrollPanel *nf = ScrollPanel_2(200.0f, 200.0f);
    Panel *nfc = Panel_0();
    Panel_setSize(nfc, 200.0f, 100.0f);           // fits: neither axis needed
    ScrollPanel_setContent(nf, nfc);
    ScrollPanel_layoutBars(nf);
    check(!ScrollPanel_verticalScroll_isNeeded(nf), "needed-v-fits");
    check(!ScrollPanel_horizontalScroll_isNeeded(nf), "needed-h-fits");
    Panel_setSize(nfc, 400.0f, 600.0f);           // overflows both: both needed
    ScrollPanel_layoutBars(nf);
    check(ScrollPanel_verticalScroll_isNeeded(nf), "needed-v-overflow");
    check(ScrollPanel_horizontalScroll_isNeeded(nf), "needed-h-overflow");
    ScrollPanel_scrollInputAt(nf, 100.0f, 0.0f, 1000u);
    ScrollPanel_getOffset(nf, &ox, &oy);
    check(near(ox, 100.0f), "h-scroll-x");
    check(near(oy, 0.0f), "h-scroll-y-untouched");

    // section 13 elastic: exact nonlinear resistance, raw reversal, layout
    // preservation, and an exact critically damped return.
    ScrollPanel *e = ScrollPanel_2(200.0f, 200.0f);
    Panel *ec = Panel_0();
    Panel_setSize(ec, 200.0f, 400.0f);
    ScrollPanel_setContent(e, ec);
    ScrollPanel_layoutBars(e);
    check(ScrollPanel_verticalScroll_getScrollMode(e) == SCROLL_BAR_ELASTIC, "elastic-default");
    ScrollPanel_setOverscrollLimit(e, 80.0f);
    check(ScrollPanel_getOverscrollLimit(e) == 80.0f, "elastic-limit-set");
    ScrollPanel_verticalScroll_setScrollFriction(e, 0.0f);
    ScrollPanel_setOffset(e, 0.0f, 200.0f);
    ScrollPanel_directBegin(e, 1000u);
    ScrollPanel_directChange(e, 0.0f, 100.0f, 1100u);
    ScrollPanel_getOffset(e, &ox, &oy);
    float expectedResistance = 100.0f * 80.0f * 0.55f / (80.0f + 0.55f * 100.0f);
    check(near(oy, 200.0f + expectedResistance), "elastic-resistance-exact");
    ScrollPanel_directChange(e, 0.0f, 1000000.0f, 1150u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(oy < 280.0f && oy > 279.0f, "elastic-asymptote");
    ScrollPanel_setOffset(e, 0.0f, 200.0f);
    ScrollPanel_directBegin(e, 1150u);
    ScrollPanel_directChange(e, 0.0f, 100.0f, 1160u);
    check(near(ScrollPanel_verticalScroll_getValue(e), 1.0f), "elastic-bar-pinned");
    ScrollPanel_layoutBars(e);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 200.0f + expectedResistance), "layout-preserves-overscroll");
    ScrollPanel_directChange(e, 0.0f, -50.0f, 1200u);
    ScrollPanel_getOffset(e, &ox, &oy);
    float halfResistance = 50.0f * 80.0f * 0.55f / (80.0f + 0.55f * 50.0f);
    check(near(oy, 200.0f + halfResistance), "elastic-reversal-unwinds-raw");
    ScrollPanel_directChange(e, 0.0f, -60.0f, 1300u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 190.0f), "elastic-reversal-reenters-bounds");
    ScrollPanel_directChange(e, 0.0f, -290.0f, 1400u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, -expectedResistance), "elastic-symmetric");
    ScrollPanel_directEnd(e, 1400u);
    float prevOff = oy;
    bool monotonic = true;
    for (uint64_t t = 1450u; t < 3000u; t += 50u) {
        ScrollPanel_tick(e, t);
        ScrollPanel_getOffset(e, &ox, &oy);
        if (oy < prevOff - 0.001f || oy > 0.001f)
            monotonic = false;
        prevOff = oy;
    }
    check(monotonic, "critical-return-no-cross-no-vibration");
    check(oy >= 0.0f && oy <= 0.5f, "critical-return-snaps-home");
    // a non-elastic axis hard-clamps (no stretch)
    ScrollPanel_verticalScroll_setScrollMode(e, SCROLL_BAR_STEP);
    ScrollPanel_scrollInputAt(e, 0.0f, 2000.0f, 5000u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 200.0f), "step-hard-clamp");

    // Every motion constant is runtime-backed and independently axis-tunable.
    ScrollPanel_verticalScroll_setRubberCoefficient(e, 0.7f);
    ScrollPanel_verticalScroll_setOverscrollExtent(e, 70.0f);
    ScrollPanel_verticalScroll_setVelocitySampleTauMs(e, 30.0f);
    ScrollPanel_verticalScroll_setDecelerationTauMs(e, 250.0f);
    ScrollPanel_verticalScroll_setStopVelocity(e, 3.0f);
    ScrollPanel_verticalScroll_setSpringOmega(e, 20.0f);
    ScrollPanel_verticalScroll_setSpringSnapDistance(e, 0.25f);
    ScrollPanel_verticalScroll_setSpringSnapVelocity(e, 2.0f);
    check(near(ScrollPanel_verticalScroll_getRubberCoefficient(e), 0.7f), "tune-rubber");
    check(near(ScrollPanel_verticalScroll_getOverscrollExtent(e), 70.0f), "tune-extent");
    check(near(ScrollPanel_verticalScroll_getVelocitySampleTauMs(e), 30.0f), "tune-sample-tau");
    check(near(ScrollPanel_verticalScroll_getDecelerationTauMs(e), 250.0f), "tune-decel-tau");
    check(near(ScrollPanel_verticalScroll_getStopVelocity(e), 3.0f), "tune-stop");
    check(near(ScrollPanel_verticalScroll_getSpringOmega(e), 20.0f), "tune-spring");
    check(near(ScrollPanel_verticalScroll_getSpringSnapDistance(e), 0.25f), "tune-snap-distance");
    check(near(ScrollPanel_verticalScroll_getSpringSnapVelocity(e), 2.0f), "tune-snap-velocity");

    // Writer exclusivity is per axis: Y springs while X independently
    // decelerates. The first Y spring tick is the exact critical solution,
    // with no synthetic displacement integrated before it.
    ScrollPanel *writers = ScrollPanel_2(200.0f, 200.0f);
    Panel *writersContent = Panel_0();
    Panel_setSize(writersContent, 1000.0f, 400.0f);
    ScrollPanel_setContent(writers, writersContent);
    ScrollPanel_horizontalScroll_setScrollMode(writers, SCROLL_BAR_SMOOTH);
    ScrollPanel_verticalScroll_setScrollMode(writers, SCROLL_BAR_ELASTIC);
    ScrollPanel_setOffset(writers, 0.0f, 200.0f);
    ScrollPanel_directBegin(writers, 100u);
    ScrollPanel_directChange(writers, 50.0f, 100.0f, 200u);
    ScrollPanel_directEnd(writers, 200u);
    check(ScrollPanel_horizontalScroll_getMotionWriter(writers) == SCROLLPANEL_MOTION_SYNTHETIC,
          "writer-x-synthetic");
    check(ScrollPanel_verticalScroll_getMotionWriter(writers) == SCROLLPANEL_MOTION_SPRING,
          "writer-y-spring-after-release");
    float springSeconds = 0.05f;
    float springDecay = expf(-18.0f * springSeconds);
    float springRaw = (100.0f + 18.0f * 100.0f * springSeconds)
        * springDecay;
    float springDistance = springRaw * 80.0f * 0.55f
        / (80.0f + 0.55f * springRaw);
    ScrollPanel_tick(writers, 250u);
    ScrollPanel_getOffset(writers, &ox, &oy);
    check(ox > 50.0f, "writer-x-keeps-decelerating");
    check(near(oy, 200.0f + springDistance), "first-spring-tick-has-no-synthetic-step");
    check(ScrollPanel_horizontalScroll_getMotionWriter(writers) == SCROLLPANEL_MOTION_SYNTHETIC,
          "spring-does-not-stop-other-axis");

    ScrollPanel *zeroResistance = ScrollPanel_2(200.0f, 200.0f);
    Panel *zeroContent = Panel_0();
    Panel_setSize(zeroContent, 200.0f, 400.0f);
    ScrollPanel_setContent(zeroResistance, zeroContent);
    ScrollPanel_setOffset(zeroResistance, 0.0f, 200.0f);
    ScrollPanel_verticalScroll_setRubberCoefficient(zeroResistance, 0.0f);
    ScrollPanel_directBegin(zeroResistance, 100u);
    ScrollPanel_directChange(zeroResistance, 0.0f, 100.0f, 200u);
    ScrollPanel_directEnd(zeroResistance, 200u);
    ScrollPanel_getOffset(zeroResistance, &ox, &oy);
    check(near(oy, 200.0f), "zero-coefficient-hard-stops");
    check(ScrollPanel_verticalScroll_getMotionWriter(zeroResistance) != SCROLLPANEL_MOTION_SPRING,
          "zero-coefficient-no-invisible-pull");
    ScrollPanel_verticalScroll_setRubberCoefficient(zeroResistance, 0.55f);
    ScrollPanel_verticalScroll_setOverscrollExtent(zeroResistance, 0.0f);
    ScrollPanel_directBegin(zeroResistance, 300u);
    ScrollPanel_directChange(zeroResistance, 0.0f, 100.0f, 400u);
    ScrollPanel_directEnd(zeroResistance, 400u);
    ScrollPanel_getOffset(zeroResistance, &ox, &oy);
    check(near(oy, 200.0f), "zero-extent-hard-stops");
    check(ScrollPanel_verticalScroll_getMotionWriter(zeroResistance) != SCROLLPANEL_MOTION_SPRING,
          "zero-extent-no-invisible-pull");

    // Exact elapsed-time integration is cadence stable.
    ScrollPanel *cadenceA = ScrollPanel_2(200.0f, 200.0f);
    ScrollPanel *cadenceB = ScrollPanel_2(200.0f, 200.0f);
    Panel *cadenceContentA = Panel_0();
    Panel *cadenceContentB = Panel_0();
    Panel_setSize(cadenceContentA, 200.0f, 2000.0f);
    Panel_setSize(cadenceContentB, 200.0f, 2000.0f);
    ScrollPanel_setContent(cadenceA, cadenceContentA);
    ScrollPanel_setContent(cadenceB, cadenceContentB);
    ScrollPanel_verticalScroll_setScrollMode(cadenceA, SCROLL_BAR_SMOOTH);
    ScrollPanel_verticalScroll_setScrollMode(cadenceB, SCROLL_BAR_SMOOTH);
    ScrollPanel_directBegin(cadenceA, 100u);
    ScrollPanel_directBegin(cadenceB, 100u);
    ScrollPanel_directChange(cadenceA, 0.0f, 100.0f, 200u);
    ScrollPanel_directChange(cadenceB, 0.0f, 100.0f, 200u);
    ScrollPanel_directEnd(cadenceA, 200u);
    ScrollPanel_directEnd(cadenceB, 200u);
    ScrollPanel_tick(cadenceA, 1200u);
    for (uint64_t t = 250u; t <= 1200u; t += 50u)
        ScrollPanel_tick(cadenceB, t);
    float cadenceY = 0.0f;
    ScrollPanel_getOffset(cadenceA, &ox, &oy);
    ScrollPanel_getOffset(cadenceB, &ox, &cadenceY);
    check(near(oy, cadenceY), "deceleration-cadence-stable");

    // Native momentum is authoritative and cannot arm synthetic continuation.
    ScrollPanel *native = ScrollPanel_2(200.0f, 200.0f);
    Panel *nativeContent = Panel_0();
    Panel_setSize(nativeContent, 200.0f, 2000.0f);
    ScrollPanel_setContent(native, nativeContent);
    ScrollPanel_directBegin(native, 100u);
    ScrollPanel_directChange(native, 0.0f, 20.0f, 200u);
    ScrollPanel_nativeMomentumBegin(native, 200u);
    ScrollPanel_nativeMomentumChange(native, 0.0f, 40.0f, 250u);
    ScrollPanel_nativeMomentumEnd(native, 300u);
    ScrollPanel_getOffset(native, &ox, &oy);
    float nativeEnd = oy;
    ScrollPanel_tick(native, 1300u);
    ScrollPanel_getOffset(native, &ox, &oy);
    check(near(oy, nativeEnd), "native-no-synthetic-continuation");
    check(ScrollPanel_verticalScroll_getMotionWriter(native) == SCROLLPANEL_MOTION_IDLE,
          "writer-synchronized-idle");

    // A still-running momentum tail must not pin an overscrolled axis in
    // mid-air: the rubber band rebounds at fingers-up, and momentum aimed at a
    // spring-owned axis is ignored rather than deepening the pull.
    ScrollPanel *tail = ScrollPanel_2(200.0f, 200.0f);
    Panel *tailContent = Panel_0();
    Panel_setSize(tailContent, 200.0f, 400.0f);
    ScrollPanel_setContent(tail, tailContent);
    ScrollPanel_setOverscrollLimit(tail, 80.0f);
    ScrollPanel_verticalScroll_setScrollFriction(tail, 0.0f);   // isolate the spring
    ScrollPanel_directBegin(tail, 100u);
    ScrollPanel_directChange(tail, 0.0f, 300.0f, 150u);
    ScrollPanel_getOffset(tail, &ox, &oy);
    float tailParked = oy;
    check(tailParked > 200.0f, "tail-overscroll-parked");
    ScrollPanel_directEnd(tail, 200u);
    ScrollPanel_nativeMomentumBegin(tail, 210u);
    bool tailRebounds = true;
    float tailPrev = tailParked;
    uint64_t tailClock = 220u;
    for (int i = 0; i < 6; i++) {
        ScrollPanel_nativeMomentumChange(tail, 0.0f, 20.0f, tailClock);
        ScrollPanel_tick(tail, tailClock);
        ScrollPanel_getOffset(tail, &ox, &oy);
        if (oy > tailPrev + 0.001f)
            tailRebounds = false;
        tailPrev = oy;
        tailClock += 50u;
    }
    check(tailRebounds, "momentum-tail-does-not-pin-overscroll");
    // The spring can reach its edge BEFORE AppKit has finished sending native
    // momentum. Later packets belong to that same gesture and must not start
    // a second, smaller excursion after the spring writer becomes IDLE.
    ScrollPanel_tick(tail, 2000u);
    ScrollPanel_getOffset(tail, &ox, &oy);
    check(near(oy, 200.0f), "momentum-tail-spring-settled");
    ScrollPanel_nativeMomentumChange(tail, 0.0f, 20.0f, 2016u);
    ScrollPanel_getOffset(tail, &ox, &oy);
    check(near(oy, 200.0f), "momentum-tail-no-second-bounce");
    ScrollPanel_nativeMomentumEnd(tail, 2032u);

    ScrollPanel *lateMomentum = ScrollPanel_2(200.0f, 200.0f);
    Panel *lateContent = Panel_0();
    Panel_setSize(lateContent, 200.0f, 400.0f);
    ScrollPanel_setContent(lateMomentum, lateContent);
    ScrollPanel_setOffset(lateMomentum, 0.0f, 200.0f);
    ScrollPanel_directBegin(lateMomentum, 100u);
    ScrollPanel_directChange(lateMomentum, 0.0f, 100.0f, 150u);
    ScrollPanel_directEnd(lateMomentum, 200u);
    ScrollPanel_tick(lateMomentum, 2000u);
    ScrollPanel_nativeMomentumBegin(lateMomentum, 2010u);
    ScrollPanel_nativeMomentumChange(lateMomentum, 0.0f, 20.0f, 2020u);
    ScrollPanel_getOffset(lateMomentum, &ox, &oy);
    check(near(oy, 200.0f), "post-spring-momentum-no-second-bounce");
    ScrollPanel_nativeMomentumEnd(lateMomentum, 2030u);

    ScrollPanel *nativeSpring = ScrollPanel_2(200.0f, 200.0f);
    Panel *nativeSpringContent = Panel_0();
    Panel_setSize(nativeSpringContent, 200.0f, 400.0f);
    ScrollPanel_setContent(nativeSpring, nativeSpringContent);
    ScrollPanel_setOffset(nativeSpring, 0.0f, 200.0f);
    ScrollPanel_nativeMomentumBegin(nativeSpring, 100u);
    ScrollPanel_nativeMomentumChange(nativeSpring, 0.0f, 100.0f, 200u);
    ScrollPanel_nativeMomentumEnd(nativeSpring, 200u);
    check(ScrollPanel_verticalScroll_getMotionWriter(nativeSpring) == SCROLLPANEL_MOTION_SPRING,
          "native-end-overscroll-enters-spring");

    // A successful scrollbar grab is an authoritative interruption: neither
    // fallback glide nor spring motion may resume after release.
    ScrollPanel *dragGlide = ScrollPanel_2(200.0f, 200.0f);
    Panel *dragGlideContent = Panel_0();
    Panel_setSize(dragGlideContent, 200.0f, 1000.0f);
    ScrollPanel_setContent(dragGlide, dragGlideContent);
    ScrollPanel_verticalScroll_setScrollMode(dragGlide, SCROLL_BAR_SMOOTH);
    ScrollPanel_directBegin(dragGlide, 100u);
    ScrollPanel_directChange(dragGlide, 0.0f, 100.0f, 200u);
    ScrollPanel_directEnd(dragGlide, 200u);
    check(ScrollPanel_verticalScroll_getMotionWriter(dragGlide) == SCROLLPANEL_MOTION_SYNTHETIC,
          "drag-interrupt-glide-armed");
    check(ScrollPanel_barDragBegin(dragGlide, 192.0f, 190.0f), "drag-interrupt-glide-grab");
    ScrollPanel_barDragEnd(dragGlide);
    ScrollPanel_getOffset(dragGlide, &ox, &oy);
    float draggedGlideOffset = oy;
    ScrollPanel_tick(dragGlide, 2000u);
    ScrollPanel_getOffset(dragGlide, &ox, &oy);
    check(near(oy, draggedGlideOffset), "drag-interrupt-glide-stays");
    check(ScrollPanel_verticalScroll_getMotionWriter(dragGlide) == SCROLLPANEL_MOTION_IDLE,
          "drag-interrupt-glide-idle");

    ScrollPanel *dragSpring = ScrollPanel_2(200.0f, 200.0f);
    Panel *dragSpringContent = Panel_0();
    Panel_setSize(dragSpringContent, 200.0f, 400.0f);
    ScrollPanel_setContent(dragSpring, dragSpringContent);
    ScrollPanel_setOffset(dragSpring, 0.0f, 200.0f);
    ScrollPanel_directBegin(dragSpring, 100u);
    ScrollPanel_directChange(dragSpring, 0.0f, 100.0f, 200u);
    ScrollPanel_directEnd(dragSpring, 200u);
    check(ScrollPanel_verticalScroll_getMotionWriter(dragSpring) == SCROLLPANEL_MOTION_SPRING,
          "drag-interrupt-spring-armed");
    check(ScrollPanel_barDragBegin(dragSpring, 192.0f, 5.0f), "drag-interrupt-spring-grab");
    ScrollPanel_barDragEnd(dragSpring);
    ScrollPanel_getOffset(dragSpring, &ox, &oy);
    float draggedSpringOffset = oy;
    ScrollPanel_tick(dragSpring, 2000u);
    ScrollPanel_getOffset(dragSpring, &ox, &oy);
    check(near(oy, draggedSpringOffset), "drag-interrupt-spring-stays");
    check(ScrollPanel_verticalScroll_getMotionWriter(dragSpring) == SCROLLPANEL_MOTION_IDLE,
          "drag-interrupt-spring-idle");

    // Cold non-finite writes reject atomically; hot packets drop without
    // poisoning offsets, velocity, resistance, or bar values.
    float oldCoefficient = ScrollPanel_verticalScroll_getRubberCoefficient(e);
    float oldExtent = ScrollPanel_verticalScroll_getOverscrollExtent(e);
    ScrollPanel_verticalScroll_setRubberCoefficient(e, NAN);
    ScrollPanel_verticalScroll_setOverscrollExtent(e, INFINITY);
    ScrollPanel_setOverscrollLimit(e, NAN);
    check(near(ScrollPanel_verticalScroll_getRubberCoefficient(e), oldCoefficient),
          "nonfinite-rubber-rejected");
    check(near(ScrollPanel_verticalScroll_getOverscrollExtent(e), oldExtent),
          "nonfinite-extent-rejected");
    ScrollPanel_setOffset(e, 0.0f, 50.0f);
    ScrollPanel_getOffset(e, &ox, &oy);
    float finiteOffset = oy;
    ScrollPanel_setOffsetAt(e, 0.0f, NAN, 100u);
    ScrollPanel_directChange(e, 0.0f, INFINITY, 200u);
    ScrollPanel_nativeMomentumBegin(e, 200u);
    ScrollPanel_nativeMomentumChange(e, 0.0f, NAN, 300u);
    ScrollPanel_nativeMomentumEnd(e, 300u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, finiteOffset) && isfinite(oy), "nonfinite-motion-packets-dropped");
    check(isfinite(ScrollPanel_verticalScroll_getValue(e)), "bar-value-remains-finite");

    ScrollPanel *hugeDt = ScrollPanel_2(200.0f, 200.0f);
    Panel *hugeDtContent = Panel_0();
    Panel_setSize(hugeDtContent, 200.0f, 400.0f);
    ScrollPanel_setContent(hugeDt, hugeDtContent);
    ScrollPanel_setOffset(hugeDt, 0.0f, 200.0f);
    ScrollPanel_directBegin(hugeDt, 1u);
    ScrollPanel_directChange(hugeDt, 0.0f, 100.0f, 2u);
    ScrollPanel_directEnd(hugeDt, 2u);
    ScrollPanel_tick(hugeDt, UINT64_MAX);
    ScrollPanel_getOffset(hugeDt, &ox, &oy);
    check(near(oy, 200.0f) && isfinite(oy), "huge-dt-spring-snaps-finite");

    // Layout derives current hard bounds: resize clamps a former edge, and
    // loss of overflow discards both active and canceled raw pull.
    ScrollPanel *resize = ScrollPanel_2(200.0f, 200.0f);
    Panel *resizeContent = Panel_0();
    Panel_setSize(resizeContent, 200.0f, 400.0f);
    ScrollPanel_setContent(resize, resizeContent);
    ScrollPanel_verticalScroll_setScrollMode(resize, SCROLL_BAR_STEP);
    ScrollPanel_setOffset(resize, 0.0f, 200.0f);
    ScrollPanel_setViewportSize(resize, 200.0f, 300.0f);
    ScrollPanel_getOffset(resize, &ox, &oy);
    check(near(oy, 100.0f), "resize-edge-clamps-to-current-bound");
    check(!ScrollPanel_canAcquire(resize, 0.0f, 10.0f)
          && ScrollPanel_canAcquire(resize, 0.0f, -10.0f), "resize-edge-acquisition-current");

    ScrollPanel_verticalScroll_setScrollMode(resize, SCROLL_BAR_ELASTIC);
    ScrollPanel_setViewportSize(resize, 200.0f, 200.0f);
    ScrollPanel_setOffset(resize, 0.0f, 200.0f);
    ScrollPanel_directBegin(resize, 100u);
    ScrollPanel_directChange(resize, 0.0f, 100.0f, 200u);
    ScrollPanel_cancelMotion(resize);
    Panel_setSize(resizeContent, 200.0f, 100.0f);
    ScrollPanel_layoutBars(resize);
    ScrollPanel_getOffset(resize, &ox, &oy);
    check(near(oy, 0.0f), "no-overflow-discards-canceled-pull");
    check(ScrollPanel_verticalScroll_getMotionWriter(resize) == SCROLLPANEL_MOTION_IDLE,
          "no-overflow-cancels-spring-writer");
    check(!ScrollPanel_canAcquire(resize, 0.0f, 10.0f), "no-overflow-not-acquired");

    // section 14 fade-out (not a toggle) + grappable
    ScrollPanel *fd = ScrollPanel_2(200.0f, 200.0f);
    Panel *fdc = Panel_0();
    Panel_setSize(fdc, 200.0f, 600.0f);
    ScrollPanel_setContent(fd, fdc);
    ScrollPanel_layoutBars(fd);
    ScrollPanel_verticalScroll_setHideWhenUnused(fd, true);
    ScrollPanel_verticalScroll_setIdleTimeoutMs(fd, 1000u);
    ScrollPanel_verticalScroll_setFadeOutMs(fd, 1000u);
    check(ScrollPanel_verticalScroll_getFadeOutMs(fd) == 1000u, "fade-ms-set");
    ScrollPanel_setOffsetAt(fd, 0.0f, 50.0f, 1000u);
    ScrollPanel_tick(fd, 1000u);
    check(near(ScrollBar_getFadeAlpha((*fd).vBar), 1.0f), "fade-hold-full");
    ScrollPanel_tick(fd, 2500u);              // 500 into the fade (1000 hold + 500)
    float mid = ScrollBar_getFadeAlpha((*fd).vBar);
    check(mid < 1.0f && mid > 0.0f, "fade-mid");
    ScrollPanel_tick(fd, 3500u);              // past hold + fade
    check(ScrollBar_getFadeAlpha((*fd).vBar) == 0.0f, "fade-done");
    ScrollPanel_verticalScroll_setGrappable(fd, false);
    check(!ScrollPanel_verticalScroll_isGrappable(fd), "grappable-off");
    check(!ScrollPanel_barDragBegin(fd, 192.0f, 5.0f), "grappable-refuses");
    ScrollPanel_verticalScroll_setGrappable(fd, true);
    check(ScrollPanel_verticalScroll_isGrappable(fd), "grappable-on");
    check(ScrollPanel_barDragBegin(fd, 192.0f, 5.0f), "grappable-accepts");
    ScrollPanel_barDragEnd(fd);

    // section 15 strings
    char buf[512];
    bool trunc = false;
    ScrollPanel_toString(sp, buf, sizeof(buf), &trunc);
    check(!trunc && buf[0] != '\0', "to-string");
    ScrollPanel_toStringStruct(sp, buf, sizeof(buf), &trunc);
    check(!trunc && buf[0] != '\0', "to-struct");
    char tiny[4];
    bool t2 = false;
    ScrollPanel_toString(sp, tiny, sizeof(tiny), &t2);
    check(t2, "string-trunc");

    if (failures == 0)
        printf("scroll_panel_test: all green\n");
    return failures == 0 ? 0 : 1;
}
