#include "annotation/overview.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "nio/mem.h"
#include "oop/type.h"
#include "reactive/reactive_primitive.h"
#include "reactive/dispatch.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: ReactiveTypedTest (tests/reactive_typed_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Headless suite for the typed reactives (bool / int / double / string): the
 * embed-first contract, per-type set/get, bit-exact double round-trip, pointer
 * rebind as change, observer fan-out, batch coalescing, out-of-band raw writes,
 * and null-safety.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main(void)
 * ============================================================================
 */

static int g_fail = 0;
static int g_checks = 0;

#define CHECK(cond)                                                       \
    do {                                                                  \
        g_checks++;                                                       \
        if (!(cond)) {                                                    \
            g_fail++;                                                     \
            printf("  FAIL line %d: %s\n", __LINE__, #cond);              \
        }                                                                 \
    } while (0)

static int g_changed = 0;
static int g_sets = 0;
static int32_t g_old = 0;
static int32_t g_new = 0;

// Records typed integer change notifications and their old/new payloads.
static void onChangedInt(Reactive *r, uintptr_t o, uintptr_t n, void *ud) {
    (void) r;
    (void) ud;
    g_changed++;
    g_old = (int32_t) (uint32_t) o;
    g_new = (int32_t) (uint32_t) n;
}

// Counts typed integer onSet notifications.
static void onSetInt(Reactive *r, uintptr_t v, void *ud) {
    (void) r;
    (void) v;
    (void) ud;
    g_sets++;
}

// Verifies a typed reactive begins at the same address as its raw Reactive base.
static void testEmbedFirst(void) {
    printf("[1] embed-first: a typed reactive* is a Reactive*\n");
    ReactiveInt *h = ReactiveInt_1(7);
    CHECK(h != nullptr);
    if (h) {
        CHECK((uintptr_t) h == (uintptr_t) &(*h).base);
        CHECK(Reactive_isActive(&(*h).base) == true);
        ReactiveInt_free(h);
    }
}

// Checks integer access, notifications/coalescing, raw writes, and negative values.
static void testInt(void) {
    printf("[2] int: set/get, exact old/new, coalescing, out-of-band write\n");
    ReactiveInt *h = ReactiveInt_1(10);
    CHECK(h != nullptr);
    if (!h)
        return;
    CHECK(ReactiveInt_get(h) == 10);

    Reactive_addOnChanged(&(*h).base, onChangedInt, nullptr);
    Reactive_addOnSet(&(*h).base, onSetInt, nullptr);

    ReactiveInt_set(h, 20);
    CHECK(Reactive_drain(&(*h).base) == true);
    CHECK(g_changed == 1 && g_old == 10 && g_new == 20);
    CHECK(g_sets == 1);
    CHECK(ReactiveInt_get(h) == 20);

    // Coalesce: three writes -> ONE set, ONE change (last wins).
    ReactiveInt_set(h, 21);
    ReactiveInt_set(h, 22);
    ReactiveInt_set(h, 23);
    CHECK(Reactive_drain(&(*h).base) == true);
    CHECK(g_changed == 2 && g_old == 20 && g_new == 23);
    CHECK(g_sets == 2);

    // No write -> no fire.
    CHECK(Reactive_drain(&(*h).base) == false);

    // Out-of-band: a raw engine write to the word IS the value -> detected.
    Reactive_set(&(*h).base, (uintptr_t) (uint32_t) 99);
    CHECK(Reactive_drain(&(*h).base) == true);
    CHECK(g_changed == 3 && g_new == 99);
    CHECK(ReactiveInt_get(h) == 99);

    // Negative values survive the sign-extension.
    ReactiveInt_set(h, -5);
    CHECK(ReactiveInt_get(h) == -5);

    ReactiveInt_free(h);
}

// Checks boolean reactive round trips and the null getter default.
static void testBool(void) {
    printf("[3] bool\n");
    ReactiveBool *b = ReactiveBool_1(false);
    CHECK(b != nullptr);
    if (!b)
        return;
    CHECK(ReactiveBool_get(b) == false);
    ReactiveBool_set(b, true);
    CHECK(ReactiveBool_get(b) == true);
    ReactiveBool_set(b, false);
    CHECK(ReactiveBool_get(b) == false);
    ReactiveBool_free(b);
    CHECK(ReactiveBool_get(nullptr) == false);
}

// Checks double round trips preserve signed zero and NaN payload meaning.
static void testDouble(void) {
    printf("[4] double: bit-exact round trip, incl. -0.0 and NaN\n");
    ReactiveDouble *d = ReactiveDouble_1(1.5);
    CHECK(d != nullptr);
    if (!d)
        return;
    CHECK(ReactiveDouble_get(d) == 1.5);

    // -0.0 must keep its sign bit (== 0.0 numerically, different bits).
    double negZero = -0.0;
    uint64_t negBits = 0u;
    memcpy(&negBits, &negZero, sizeof(negBits));
    ReactiveDouble_set(d, negZero);
    CHECK((uint64_t) Reactive_get(&(*d).base) == negBits);
    CHECK(ReactiveDouble_get(d) == 0.0);

    // NaN round-trips (a numerically unequal value must still survive).
    double nan = NAN;
    ReactiveDouble_set(d, nan);
    CHECK(isnan(ReactiveDouble_get(d)));

    ReactiveDouble_set(d, -42.25);
    CHECK(ReactiveDouble_get(d) == -42.25);
    ReactiveDouble_free(d);
}

// Checks borrowed string-pointer storage, rebinding notifications, and same-pointer writes.
static void testString(void) {
    printf("[5] string: pointer word, rebind is a change\n");
    const uint8_t *a = (const uint8_t*) "hello";
    const uint8_t *b = (const uint8_t*) "world";
    ReactiveString *s = ReactiveString_1(a);
    CHECK(s != nullptr);
    if (!s)
        return;
    CHECK(ReactiveString_get(s) == a);

    g_changed = 0;
    Reactive_addOnChanged(&(*s).base, onChangedInt, nullptr); // counts only
    ReactiveString_set(s, b);
    CHECK(Reactive_drain(&(*s).base) == true);
    CHECK(g_changed == 1);
    CHECK(ReactiveString_get(s) == b);

    // Same pointer again -> onSet fires (a batch happened) but NOT onChanged.
    ReactiveString_set(s, b);
    CHECK(Reactive_drain(&(*s).base) == true);
    CHECK(g_changed == 1); // unchanged: no onChanged on a same-value write

    ReactiveString_free(s);
}

// Exercises null-safe setters, getters, and destructors for each typed reactive.
static void testNull(void) {
    printf("[6] null-safety\n");
    ReactiveInt_set(nullptr, 1);
    CHECK(ReactiveInt_get(nullptr) == 0);
    ReactiveBool_set(nullptr, true);
    ReactiveDouble_set(nullptr, 1.0);
    ReactiveString_set(nullptr, nullptr);
    CHECK(ReactiveDouble_get(nullptr) == 0.0);
    CHECK(ReactiveString_get(nullptr) == nullptr);
    ReactiveInt_free(nullptr);
    ReactiveBool_free(nullptr);
    ReactiveDouble_free(nullptr);
    ReactiveString_free(nullptr);
    CHECK(true);
}

// Runs typed reactive embedding, scalar behavior, notifications, and null-safety cases.
int main(void) {
    printf("=== Reactive (typed) Test Suite ===\n\n");

    testEmbedFirst();
    testInt();
    testBool();
    testDouble();
    testString();
    testNull();

    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    if (g_fail == 0)
        printf("=== Reactive Typed Test Suite: ALL PASS ===\n");
    return g_fail == 0 ? 0 : 1;
}
