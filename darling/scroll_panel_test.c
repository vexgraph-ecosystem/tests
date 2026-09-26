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

    // section 9 nesting + chaining: inner at end bubbles to the parent
    ScrollPanel *outer = ScrollPanel_2(200.0f, 200.0f);
    ScrollPanel_verticalScroll_setScrollMode(outer, SCROLL_BAR_STEP);
    ScrollPanel_horizontalScroll_setScrollMode(outer, SCROLL_BAR_STEP);
    Panel *outerContent = Panel_0();
    Panel_setSize(outerContent, 200.0f, 800.0f);
    ScrollPanel_setContent(outer, outerContent);
    ScrollPanel *inner = ScrollPanel_2(200.0f, 200.0f);
    ScrollPanel_verticalScroll_setScrollMode(inner, SCROLL_BAR_STEP);
    ScrollPanel_horizontalScroll_setScrollMode(inner, SCROLL_BAR_STEP);
    Panel *innerContent = Panel_0();
    Panel_setSize(innerContent, 200.0f, 600.0f);
    ScrollPanel_setContent(inner, innerContent);
    Panel_addContainer(outerContent, &(*inner).base);
    float leftX = -1.0f, leftY = -1.0f;
    ScrollPanel_scrollByChained(inner, 0.0f, 1000.0f, 9000u, &leftX, &leftY);
    ScrollPanel_getOffset(inner, &ox, &oy);
    check(near(oy, 400.0f) && near(leftY, 600.0f), "chain-inner-consumes");
    ScrollPanel_scrollByAt(outer, leftX, leftY, 9000u);
    ScrollPanel_getOffset(outer, &ox, &oy);
    check(near(oy, 600.0f) && near(leftY, 600.0f), "chain-parent-continues");
    ScrollPanel_scrollByChained(inner, 0.0f, 100.0f, 9100u, &leftX, &leftY);
    check(near(leftY, 100.0f), "chain-inner-at-end");
    ScrollPanel_scrollByAt(outer, leftX, leftY, 9100u);
    ScrollPanel_getOffset(outer, &ox, &oy);
    check(near(oy, 600.0f), "chain-parent-clamped");
    ScrollPanel_scrollByChained(inner, 0.0f, -100.0f, 9200u, &leftX, &leftY);
    ScrollPanel_getOffset(inner, &ox, &oy);
    check(near(oy, 300.0f) && near(leftY, 0.0f), "chain-reverse-consumes");
    ScrollPanel_scrollByChained(nullptr, 1.0f, 2.0f, 0u, &leftX, &leftY);
    check(near(leftX, 1.0f) && near(leftY, 2.0f), "chain-null-passthrough");
    float cx = GraphicsComponent_getX(&(*innerContent).component);
    float cy = GraphicsComponent_getY(&(*innerContent).component);
    check(near(cx, 0.0f) && near(cy, -300.0f), "content-rides-offset");

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

    // section 13 elastic (slinky): overscroll past an end, spring home
    ScrollPanel *e = ScrollPanel_2(200.0f, 200.0f);
    Panel *ec = Panel_0();
    Panel_setSize(ec, 200.0f, 400.0f);
    ScrollPanel_setContent(e, ec);
    ScrollPanel_layoutBars(e);
    check(ScrollPanel_verticalScroll_getScrollMode(e) == SCROLL_BAR_ELASTIC, "elastic-default");
    ScrollPanel_setOverscrollLimit(e, 80.0f);
    check(ScrollPanel_getOverscrollLimit(e) == 80.0f, "elastic-limit-set");
    // Isolate stretch+spring: friction 0 disables glide so only the spring
    // moves the offset (the input lands, clamped to the stretched bound).
    ScrollPanel_verticalScroll_setScrollFriction(e, 0.0f);
    // overscroll past the bottom (hard hi = 200) stretches to 280
    ScrollPanel_scrollInputAt(e, 0.0f, 2000.0f, 1000u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 280.0f), "elastic-overscroll");
    // the thumb value stays pinned at the end (no overshoot in the bar)
    check(near(ScrollPanel_verticalScroll_getValue(e), 1.0f), "elastic-bar-pinned");
    // within the release window the stretch is HELD (slinky rubber)
    ScrollPanel_tick(e, 1050u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 280.0f), "elastic-held");
    // after release it springs home, monotonically (no vibration/oscillation)
    float prevOff = oy;
    bool monotonic = true;
    for (uint64_t t = 1100u; t < 2000u; t += 50u) {
        ScrollPanel_tick(e, t);
        ScrollPanel_getOffset(e, &ox, &oy);
        if (oy > prevOff + 0.001f)
            monotonic = false;
        prevOff = oy;
    }
    check(monotonic, "elastic-no-vibration");
    check(oy <= 200.0f && oy >= 199.0f, "elastic-springs-home");
    // overscroll past the top
    ScrollPanel_scrollInputAt(e, 0.0f, -5000.0f, 3000u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, -80.0f), "elastic-overscroll-top");
    for (uint64_t t = 3100u; t < 4000u; t += 50u)
        ScrollPanel_tick(e, t);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(oy >= 0.0f && oy <= 1.0f, "elastic-springs-top");
    // a non-elastic axis hard-clamps (no stretch)
    ScrollPanel_verticalScroll_setScrollMode(e, SCROLL_BAR_STEP);
    ScrollPanel_scrollInputAt(e, 0.0f, 2000.0f, 5000u);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 200.0f), "step-hard-clamp");

    // section 13b elastic HOLD: gravity waits for release, not for a timer
    ScrollPanel_verticalScroll_setScrollMode(e, SCROLL_BAR_ELASTIC);
    ScrollPanel_setGestureHeld(e, true);
    check(ScrollPanel_isGestureHeld(e), "held-flag");
    ScrollPanel_setOffsetAt(e, 0.0f, 400.0f, 6000u);   // stretch to the limit
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 280.0f), "held-stretch");
    for (uint64_t t = 6100u; t < 7000u; t += 50u)
        ScrollPanel_tick(e, t);                        // far past any release window
    ScrollPanel_getOffset(e, &ox, &oy);
    check(near(oy, 280.0f), "held-no-gravity");
    ScrollPanel_setGestureHeld(e, false);
    for (uint64_t t = 7100u; t < 8000u; t += 50u)
        ScrollPanel_tick(e, t);
    ScrollPanel_getOffset(e, &ox, &oy);
    check(oy >= 199.0f && oy <= 200.0f, "released-springs");

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
