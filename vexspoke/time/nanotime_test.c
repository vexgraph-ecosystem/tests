// tests/vexspoke/time/nanotime_test.c — owner test for time/nanotime.
//
// Proves the monotonic clock and tickable timer:
//   - pre-init epoch/elapsed are 0; init is idempotent and captures a fixed
//     epoch; now() is monotonic (never goes backwards);
//   - NanoTimer_reset zeroes delta/total and all timestamps;
//   - tick accumulates real delta into deltaTime/totalTime and exposes
//     deltaNanos/elapsedNanosOf consistently;
//   - tickWithClock pauses (scale 0) freeze delta/total while real time still
//     advances, and timeScale > 1 scales the delta;
//   - tickWithClock(nullptr) behaves exactly like tick.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "time/nanotime.h"
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

static void test_epoch(void) {
    // Before the first init the engine epoch is unmapped.
    CHECK(NanoTime_startNanos() == 0);
    CHECK(NanoTime_elapsedNanos() == 0);

    NanoTime_init();
    uint64_t start = NanoTime_startNanos();
    CHECK(start != 0);

    // Idempotent: a second init does not move the anchor.
    NanoTime_init();
    NanoTime_init();
    CHECK(NanoTime_startNanos() == start);

    uint64_t a = NanoTime_now();
    uint64_t b = NanoTime_now();
    CHECK(b >= a);                                  // monotonic, never backwards
    wait_ms(2);
    uint64_t c = NanoTime_now();
    CHECK(c > a);

    CHECK(NanoTime_elapsedNanos() < (uint64_t) 60 * 1000000000ULL);
}

static void test_timer_reset(void) {
    NanoTimer t;
    NanoTimer_reset(&t);
    CHECK(NanoTimer_deltaTime(&t) == 0.0);
    CHECK(NanoTimer_totalTime(&t) == 0.0);
    CHECK(NanoTimer_deltaNanos(&t) == 0);
    CHECK(NanoTimer_elapsedNanosOf(&t) == 0);
}

static void test_timer_tick(void) {
    NanoTimer t;
    NanoTimer_reset(&t);

    wait_ms(3);
    NanoTimer_tick(&t);
    CHECK(NanoTimer_deltaNanos(&t) > 0);
    CHECK(NanoTimer_deltaTime(&t) > 0.0);
    CHECK(NanoTimer_totalTime(&t) == NanoTimer_deltaTime(&t));
    CHECK(NanoTimer_elapsedNanosOf(&t) > 0);
    // deltaTime is the nanos delta expressed in seconds.
    double secs = (double) NanoTimer_deltaNanos(&t) / 1e9;
    CHECK(fabs(NanoTimer_deltaTime(&t) - secs) < 1e-6);

    double firstTotal = NanoTimer_totalTime(&t);
    wait_ms(3);
    NanoTimer_tick(&t);
    CHECK(NanoTimer_totalTime(&t) > firstTotal);    // accumulates

    // elapsed >= delta (there was a gap between reset and first tick too).
    CHECK(NanoTimer_elapsedNanosOf(&t) >= NanoTimer_deltaNanos(&t));
}

static void test_timer_with_paused_clock(void) {
    Clock c = Clock();
    Clock_setPaused(&c, true);

    NanoTimer t;
    NanoTimer_reset(&t);
    wait_ms(3);
    NanoTimer_tickWithClock(&t, &c);
    CHECK(NanoTimer_deltaTime(&t) == 0.0);          // frozen
    CHECK(NanoTimer_totalTime(&t) == 0.0);
    CHECK(NanoTimer_deltaNanos(&t) > 0);            // real advance still measured

    // Unpause: accrual resumes.
    Clock_setPaused(&c, false);
    wait_ms(3);
    NanoTimer_tickWithClock(&t, &c);
    CHECK(NanoTimer_deltaTime(&t) > 0.0);
}

static void test_timer_scale(void) {
    Clock c = Clock();
    Clock_setTimeScale(&c, 2.0);

    NanoTimer t;
    NanoTimer_reset(&t);
    wait_ms(5);
    NanoTimer_tickWithClock(&t, &c);
    double real = (double) NanoTimer_deltaNanos(&t) / 1e9;
    CHECK(real > 0.0);
    // Scaled delta is about twice the real delta (allow scheduling slack).
    CHECK(NanoTimer_deltaTime(&t) > real * 1.5);
    CHECK(NanoTimer_deltaTime(&t) < real * 2.5 + 0.001);
}

static void test_tick_null_clock(void) {
    NanoTimer t;
    NanoTimer_reset(&t);
    wait_ms(2);
    NanoTimer_tickWithClock(&t, nullptr);
    CHECK(NanoTimer_deltaTime(&t) > 0.0);

    NanoTimer t2;
    NanoTimer_reset(&t2);
    wait_ms(2);
    NanoTimer_tick(&t2);
    CHECK(NanoTimer_deltaTime(&t2) > 0.0);
}

int main(void) {
    test_epoch();
    test_timer_reset();
    test_timer_tick();
    test_timer_with_paused_clock();
    test_timer_scale();
    test_tick_null_clock();

    if (g_failures == 0) {
        printf("nanotime_test: all assertions held\n");
        return 0;
    }
    printf("nanotime_test: %d FAILURES\n", g_failures);
    return 1;
}
