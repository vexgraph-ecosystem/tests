// tests/vexspoke/time/datetime_test.c — owner test for time/datetime.
//
// Proves the UTC date/time breakdown at pinned epochs (an exact oracle):
//   - the epoch 0 boundary (1970-01-01 Thursday), a leap day, a modern date,
//     a negative epoch (-1 ms), and a date with nonzero h/m/s/ms;
//   - ISO-8601 day-of-week (1=Monday..7=Sunday) agrees with the calendar;
//   - every getter returns the field and epochMillis round-trips;
//   - DateTime_set snapshots a plausible current wall-clock year.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "time/datetime.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void expect(int64_t ms, int32_t y, int32_t mo, int32_t d,
                   int32_t h, int32_t mi, int32_t s, int32_t milli, int32_t dow) {
    DateTime dt;
    setEpochMillis(&dt, ms);
    CHECK(DateTime_epochMillis(&dt) == ms);
    CHECK(DateTime_year(&dt) == y);
    CHECK(DateTime_month(&dt) == mo);
    CHECK(DateTime_day(&dt) == d);
    CHECK(DateTime_hour(&dt) == h);
    CHECK(DateTime_minute(&dt) == mi);
    CHECK(DateTime_second(&dt) == s);
    CHECK(DateTime_millisecond(&dt) == milli);
    CHECK(DateTime_dayOfWeek(&dt) == dow);
}

static void test_epoch_zero(void) {
    expect(0, 1970, 1, 1, 0, 0, 0, 0, 4);                 // Thursday
}

static void test_known_epochs(void) {
    expect(946684800000LL, 2000, 1, 1, 0, 0, 0, 0, 6);    // Saturday
    expect(1582934400000LL, 2020, 2, 29, 0, 0, 0, 0, 6);  // leap day
    expect(1709210096789LL, 2024, 2, 29, 12, 34, 56, 789, 4);
    expect(1767225600000LL, 2026, 1, 1, 0, 0, 0, 0, 4);
    expect(1790726400000LL, 2026, 9, 30, 0, 0, 0, 0, 3);  // Wednesday
}

static void test_negative_epoch(void) {
    // -1 ms is 1969-12-31 23:59:59.999 (Wednesday).
    expect(-1, 1969, 12, 31, 23, 59, 59, 999, 3);
    expect(-1000, 1969, 12, 31, 23, 59, 59, 0, 3);
    expect(-86400000, 1969, 12, 31, 0, 0, 0, 0, 3);
    expect(-86400001, 1969, 12, 30, 23, 59, 59, 999, 2);
}

static void test_set_snapshot(void) {
    DateTime dt;
    setEpochMillis(&dt, 0);
    DateTime_set(&dt);
    CHECK(DateTime_year(&dt) >= 2024);
    CHECK(DateTime_month(&dt) >= 1 && DateTime_month(&dt) <= 12);
    CHECK(DateTime_day(&dt) >= 1 && DateTime_day(&dt) <= 31);
    CHECK(DateTime_hour(&dt) >= 0 && DateTime_hour(&dt) <= 23);
    CHECK(DateTime_dayOfWeek(&dt) >= 1 && DateTime_dayOfWeek(&dt) <= 7);
}

int main(void) {
    test_epoch_zero();
    test_known_epochs();
    test_negative_epoch();
    test_set_snapshot();

    if (g_failures == 0) {
        printf("datetime_test: all assertions held\n");
        return 0;
    }
    printf("datetime_test: %d FAILURES\n", g_failures);
    return 1;
}
