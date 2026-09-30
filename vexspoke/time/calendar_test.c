// tests/vexspoke/time/calendar_test.c — owner test for time/calendar.
//
// Proves the stateless date arithmetic:
//   - leap-year rule (divisible by 4, not 100, unless 400) at century edges;
//   - daysInMonth for every month, February in leap/common years, and the
//     out-of-range month guard (0);
//   - ISO-8601 day-of-week via Zeller against pinned dates;
//   - addDays crosses month/year/leap boundaries, including negative counts;
//   - addMonths rolls the year and CLIPS the day into the target month;
//   - addYears clips Feb 29 in a common year.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "time/calendar.h"
#include "time/datetime.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void expect_date(int64_t ms, int32_t y, int32_t mo, int32_t d, int32_t dow) {
    DateTime dt;
    setEpochMillis(&dt, ms);
    CHECK(DateTime_year(&dt) == y);
    CHECK(DateTime_month(&dt) == mo);
    CHECK(DateTime_day(&dt) == d);
    if (dow != 0)
        CHECK(DateTime_dayOfWeek(&dt) == dow);
}

static void test_leap_year(void) {
    CHECK(Calendar_isLeapYear(2000));
    CHECK(Calendar_isLeapYear(2400));
    CHECK(Calendar_isLeapYear(2024));
    CHECK(Calendar_isLeapYear(1996));
    CHECK(!Calendar_isLeapYear(1900));
    CHECK(!Calendar_isLeapYear(2100));
    CHECK(!Calendar_isLeapYear(2023));
    CHECK(!Calendar_isLeapYear(2025));
    CHECK(!Calendar_isLeapYear(1));
}

static void test_days_in_month(void) {
    CHECK(Calendar_daysInMonth(2024, 1) == 31);
    CHECK(Calendar_daysInMonth(2024, 2) == 29);
    CHECK(Calendar_daysInMonth(2023, 2) == 28);
    CHECK(Calendar_daysInMonth(2024, 3) == 31);
    CHECK(Calendar_daysInMonth(2024, 4) == 30);
    CHECK(Calendar_daysInMonth(2024, 5) == 31);
    CHECK(Calendar_daysInMonth(2024, 6) == 30);
    CHECK(Calendar_daysInMonth(2024, 7) == 31);
    CHECK(Calendar_daysInMonth(2024, 8) == 31);
    CHECK(Calendar_daysInMonth(2024, 9) == 30);
    CHECK(Calendar_daysInMonth(2024, 10) == 31);
    CHECK(Calendar_daysInMonth(2024, 11) == 30);
    CHECK(Calendar_daysInMonth(2024, 12) == 31);
    // Out-of-range month.
    CHECK(Calendar_daysInMonth(2024, 0) == 0);
    CHECK(Calendar_daysInMonth(2024, 13) == 0);
    CHECK(Calendar_daysInMonth(2024, -1) == 0);
}

static void test_day_of_week(void) {
    CHECK(Calendar_dayOfWeek(1970, 1, 1) == 4);      // Thursday
    CHECK(Calendar_dayOfWeek(2000, 1, 1) == 6);      // Saturday
    CHECK(Calendar_dayOfWeek(2020, 2, 29) == 6);     // Saturday
    CHECK(Calendar_dayOfWeek(2024, 2, 29) == 4);     // Thursday
    CHECK(Calendar_dayOfWeek(2026, 9, 30) == 3);     // Wednesday
    CHECK(Calendar_dayOfWeek(2026, 3, 1) == 7);      // Sunday
}

static void test_add_days(void) {
    DateTime dt;
    setEpochMillis(&dt, 1582848000000LL);            // 2020-02-28
    Calendar_addDays(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2020, 2, 29, 6);
    Calendar_addDays(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2020, 3, 1, 7);
    Calendar_addDays(&dt, -1);
    expect_date(DateTime_epochMillis(&dt), 2020, 2, 29, 6);
    Calendar_addDays(&dt, -29);
    expect_date(DateTime_epochMillis(&dt), 2020, 1, 31, 5);
    // Cross a year boundary.
    setEpochMillis(&dt, 1767225600000LL);            // 2026-01-01
    Calendar_addDays(&dt, -1);
    expect_date(DateTime_epochMillis(&dt), 2025, 12, 31, 3);
}

static void test_add_months(void) {
    DateTime dt;

    // Jan 31 + 1 month clips to Feb 29 in a leap year.
    setEpochMillis(&dt, 1580428800000LL);            // 2020-01-31
    Calendar_addMonths(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2020, 2, 29, 0);

    // Jan 31 + 1 month clips to Feb 28 in a common year.
    setEpochMillis(&dt, 1675123200000LL);            // 2023-01-31
    Calendar_addMonths(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2023, 2, 28, 0);

    // Year rollover forward and backward.
    setEpochMillis(&dt, 1606780800000LL);            // 2020-12-01
    Calendar_addMonths(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2021, 1, 1, 0);
    setEpochMillis(&dt, 1580428800000LL);            // 2020-01-31
    Calendar_addMonths(&dt, -1);
    expect_date(DateTime_epochMillis(&dt), 2019, 12, 31, 0);

    // Large jump: +12 months moves a full year.
    setEpochMillis(&dt, 1582848000000LL);            // 2020-02-28
    Calendar_addMonths(&dt, 12);
    expect_date(DateTime_epochMillis(&dt), 2021, 2, 28, 0);
}

static void test_add_years(void) {
    DateTime dt;
    setEpochMillis(&dt, 1582934400000LL);            // 2020-02-29
    Calendar_addYears(&dt, 1);
    expect_date(DateTime_epochMillis(&dt), 2021, 2, 28, 0);   // clipped
    setEpochMillis(&dt, 1582934400000LL);
    Calendar_addYears(&dt, 4);
    expect_date(DateTime_epochMillis(&dt), 2024, 2, 29, 0);   // leap preserved
    setEpochMillis(&dt, 1582934400000LL);
    Calendar_addYears(&dt, -4);
    expect_date(DateTime_epochMillis(&dt), 2016, 2, 29, 0);
}

int main(void) {
    test_leap_year();
    test_days_in_month();
    test_day_of_week();
    test_add_days();
    test_add_months();
    test_add_years();

    if (g_failures == 0) {
        printf("calendar_test: all assertions held\n");
        return 0;
    }
    printf("calendar_test: %d FAILURES\n", g_failures);
    return 1;
}
