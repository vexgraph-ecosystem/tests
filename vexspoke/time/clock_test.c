// tests/vexspoke/time/clock_test.c — owner test for time/clock.
//
// Proves the virtual clock:
//   - create: scale 1.0, unpaused, virtual time zeroed;
//   - set/get time scale and paused round trips;
//   - tick accrues monotonic real elapsed * scale into the virtual timeline;
//   - while paused the real reading advances but virtual time does NOT (no
//     resume backlog), and unpausing resumes accrual;
//   - a zero time scale freezes virtual time without pausing;
//   - reset zeroes the virtual timeline and re-anchors.
//
// Timing is bounded by a small sleep and asserted with inequalities, never an
// exact duration, so the test is deterministic in outcome.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "time/clock.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void wait_ms(long ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, nullptr);
}

// Spin (bounded) until the virtual clock has accrued past `target` ms.
static bool accrue_at_least(Clock *c, uint64_t target) {
    for (int i = 0; i < 2000; i++) {          // bounded: <= ~2s
        Clock_tick(c);
        if (Clock_virtualTimeMillis(c) >= target)
            return true;
        wait_ms(1);
    }
    return false;
}

static void test_create(void) {
    Clock c = Clock_create();
    CHECK(Clock_timeScale(&c) == 1.0);
    CHECK(!Clock_isPaused(&c));
    CHECK(Clock_virtualTimeMillis(&c) == 0);
}

static void test_settings(void) {
    Clock c = Clock_create();
    Clock_setTimeScale(&c, 2.5);
    CHECK(Clock_timeScale(&c) == 2.5);
    Clock_setPaused(&c, true);
    CHECK(Clock_isPaused(&c));
    Clock_setPaused(&c, false);
    CHECK(!Clock_isPaused(&c));
}

static void test_accrual(void) {
    Clock c = Clock_create();
    CHECK(accrue_at_least(&c, 1));
    uint64_t afterFirst = Clock_virtualTimeMillis(&c);
    CHECK(afterFirst >= 1);

    // Accrual is cumulative and monotonic.
    CHECK(accrue_at_least(&c, afterFirst + 1));
    CHECK(Clock_virtualTimeMillis(&c) >= afterFirst + 1);
}

static void test_pause_freezes_virtual(void) {
    Clock c = Clock_create();
    CHECK(accrue_at_least(&c, 1));
    uint64_t v = Clock_virtualTimeMillis(&c);

    Clock_setPaused(&c, true);
    wait_ms(5);
    for (int i = 0; i < 5; i++) {
        Clock_tick(&c);
        wait_ms(2);
    }
    CHECK(Clock_virtualTimeMillis(&c) == v);    // frozen while paused

    // Unpause and confirm accrual resumes (the real reading was kept current,
    // so there is no phantom backlog — the first tick after resume is small).
    Clock_setPaused(&c, false);
    CHECK(accrue_at_least(&c, v + 1));
    CHECK(Clock_virtualTimeMillis(&c) > v);
}

static void test_zero_scale(void) {
    Clock c = Clock_create();
    Clock_setTimeScale(&c, 0.0);
    for (int i = 0; i < 10; i++) {
        Clock_tick(&c);
        wait_ms(1);
    }
    CHECK(Clock_virtualTimeMillis(&c) == 0);
}

static void test_reset(void) {
    Clock c = Clock_create();
    CHECK(accrue_at_least(&c, 1));
    CHECK(Clock_virtualTimeMillis(&c) >= 1);
    Clock_reset(&c);
    CHECK(Clock_virtualTimeMillis(&c) == 0);
}

int main(void) {
    test_create();
    test_settings();
    test_accrual();
    test_pause_freezes_virtual();
    test_zero_scale();
    test_reset();

    if (g_failures == 0) {
        printf("clock_test: all assertions held\n");
        return 0;
    }
    printf("clock_test: %d FAILURES\n", g_failures);
    return 1;
}
