// tests/vexspoke/math/fast_math_test.c — owner test for math/fast_math.
//
// FastMath deliberately relaxes IEEE 754, so the oracle is a bounded error
// against libm rather than bit equality:
//   - invSqrt: positive relative error vs 1/sqrt for several magnitudes, with
//     the non-positive guard returning 0;
//   - inv: reciprocal on normal values, 0 at 0;
//   - sin/cos: Bhaskara error within a bounded tolerance on [-pi, pi], odd/even
//     symmetry, and 2*pi periodicity (wrap_pi);
//   - tan, atan, atan2 quadrant correctness;
//   - abs clears the sign bit (including -0.0);
//   - clamp/lerp/approxEqual and the angle converters;
//   - FastMath_round is RECORDED as a defect (magic constant 16384 rounds to
//     the float ulp at 2^14, not to integers); asserted as a known gap.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "math/fast_math.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

// A known, documented production gap: printed, never counted as a pass.
#define GAP(cond, msg)                                                     \
    do {                                                                   \
        if (!(cond))                                                       \
            printf("KNOWN GAP (unproved): %s\n", msg);                     \
    } while (0)

#define REL(a, b, tol) (fabsf((a) - (b)) <= (tol) * fabsf(b) + 1e-6f)

static void test_inv_sqrt(void) {
    float xs[] = { 0.25f, 1.0f, 2.0f, 4.0f, 9.0f, 100.0f, 0.001f };
    for (size_t i = 0; i < sizeof(xs) / sizeof(xs[0]); i++)
        CHECK(REL(FastMath_invSqrt(xs[i]), 1.0f / sqrtf(xs[i]), 0.01f));
    CHECK(FastMath_invSqrt(0.0f) == 0.0f);
    CHECK(FastMath_invSqrt(-9.0f) == 0.0f);
}

static void test_inv(void) {
    float xs[] = { 1.0f, 2.0f, 4.0f, 0.5f, 100.0f, -3.0f };
    for (size_t i = 0; i < sizeof(xs) / sizeof(xs[0]); i++)
        CHECK(REL(FastMath_inv(xs[i]), 1.0f / xs[i], 0.02f));
    CHECK(FastMath_inv(0.0f) == 0.0f);
}

static void test_trig(void) {
    CHECK(fabsf(FastMath_sin(0.0f)) < 1e-6f);
    CHECK(fabsf(FastMath_cos(0.0f) - 1.0f) < 0.01f);
    for (int i = -10; i <= 10; i++) {
        float x = (float) i * 0.3f;
        CHECK(fabsf(FastMath_sin(x) - sinf(x)) <= 0.01f);
        CHECK(fabsf(FastMath_cos(x) - cosf(x)) <= 0.01f);
    }
    // Odd/even symmetry.
    for (int i = 1; i <= 5; i++) {
        float x = (float) i * 0.5f;
        CHECK(fabsf(FastMath_sin(-x) + FastMath_sin(x)) < 1e-5f);
        CHECK(fabsf(FastMath_cos(-x) - FastMath_cos(x)) < 1e-5f);
    }
    // 2*pi periodicity (wrap_pi handles a periodic domain).
    for (int i = -5; i <= 5; i++) {
        float x = (float) i * 0.4f;
        CHECK(fabsf(FastMath_sin(x + FAST_MATH_TWO_PI * 3.0f) - FastMath_sin(x)) < 0.01f);
    }
    // tangent away from the poles.
    for (int i = -4; i <= 4; i++) {
        float x = (float) i * 0.3f;
        CHECK(fabsf(FastMath_tan(x) - tanf(x)) <= 0.05f);
    }
}

static void test_atan(void) {
    for (int i = -20; i <= 20; i++) {
        float x = (float) i * 0.25f;
        CHECK(fabsf(FastMath_atan(x) - atanf(x)) <= 0.03f);  // ~0.021 worst-case
    }
    CHECK(fabsf(FastMath_atan2(1.0f, 1.0f) - (float) atan2(1.0, 1.0)) <= 0.03f);
    CHECK(fabsf(FastMath_atan2(-1.0f, 1.0f) - (float) atan2(-1.0, 1.0)) <= 0.03f);
    CHECK(fabsf(FastMath_atan2(1.0f, -1.0f) - (float) atan2(1.0, -1.0)) <= 0.03f);
    CHECK(fabsf(FastMath_atan2(-1.0f, -1.0f) - (float) atan2(-1.0, -1.0)) <= 0.03f);
    CHECK(FastMath_atan2(0.0f, 0.0f) == 0.0f);
    CHECK(FastMath_atan2(1.0f, 0.0f) == FAST_MATH_HALF_PI);
    CHECK(FastMath_atan2(-1.0f, 0.0f) == -FAST_MATH_HALF_PI);
}

static void test_abs_round_clamp(void) {
    CHECK(FastMath_abs(-3.5f) == 3.5f);
    CHECK(FastMath_abs(3.5f) == 3.5f);
    CHECK(FastMath_abs(0.0f) == 0.0f);
    CHECK(FastMath_abs(-0.0f) == 0.0f);
    CHECK(signbit(FastMath_abs(-0.0f)) == 0);          // sign bit cleared

    // The 2^23 magic constant rounds to the nearest integer (IEEE
    // round-half-to-even: ties go to the even neighbour).
    CHECK(FastMath_round(3.7f) == 4.0f);
    CHECK(FastMath_round(2.4f) == 2.0f);
    CHECK(FastMath_round(2.5f) == 2.0f);    // tie -> even
    CHECK(FastMath_round(3.5f) == 4.0f);    // tie -> even
    CHECK(FastMath_round(-2.5f) == -2.0f);  // tie -> even
    CHECK(FastMath_round(-3.7f) == -4.0f);
    CHECK(FastMath_round(0.0f) == 0.0f);

    CHECK(FastMath_clamp(5.0f, 0.0f, 3.0f) == 3.0f);
    CHECK(FastMath_clamp(-5.0f, 0.0f, 3.0f) == 0.0f);
    CHECK(FastMath_clamp(1.5f, 0.0f, 3.0f) == 1.5f);
    CHECK(FastMath_lerp(0.0f, 10.0f, 0.5f) == 5.0f);
    CHECK(FastMath_lerp(1.0f, 3.0f, 0.0f) == 1.0f);
    CHECK(FastMath_lerp(1.0f, 3.0f, 1.0f) == 3.0f);

    CHECK(FastMath_approxEqual(1.0f, 1.00005f, 0.001f));
    CHECK(!FastMath_approxEqual(1.0f, 1.5f, 0.001f));
    CHECK(FastMath_approxEqual(0.0f, 0.0f, 0.0f));
    CHECK(FastMath_approxEqual(-1.0f, -1.0000001f, 0.001f));
}

static void test_angles(void) {
    CHECK(fabsf(FastMath_toDegrees(FastMath_toRadians(90.0f)) - 90.0f) < 0.01f);
    CHECK(fabsf(FastMath_toRadians(180.0f) - FAST_MATH_PI) < 0.001f);
}

int main(void) {
    test_inv_sqrt();
    test_inv();
    test_trig();
    test_atan();
    test_abs_round_clamp();
    test_angles();

    if (g_failures == 0) {
        printf("fast_math_test: all assertions held\n");
        return 0;
    }
    printf("fast_math_test: %d FAILURES\n", g_failures);
    return 1;
}
