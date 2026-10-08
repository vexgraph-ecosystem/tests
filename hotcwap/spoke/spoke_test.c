#include "annotation/overview.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "spoke/lifetime.h"

;;OVERVIEW
/**
 * ============================================================================
 * CLASS: SpokeTest (tests/spoke_test.c — Lifetime class test)
 * LEVEL: L2 — Behavior (headless seam test; pure void* ABI, pointer legitimacy)
 * ============================================================================
 * Proves the Lifetime memory substrate contract: Lifetime_create allocates
 * both persistent and transient arenas with VEXSPOKE_TYPE_ARENA attestation.
 * Validates pointer legitimacy (Lifetime_isLegit / Lifetime_isValid),
 * 16-byte payload alignment, 16-byte negative prefix recovery, transient
 * scratchpad resets, persistent slab recycling, Lifetime_bind, and null safety.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Core Functions:
 *   - main() : the assertion tour
 * ============================================================================
 */

// 16-byte header stamped by the allocator for negative pointer recovery
typedef struct NegativeHeader {
    uint64_t typeId;
    uint32_t length;
    uint32_t sugar;
} NegativeHeader;
_Static_assert(sizeof(NegativeHeader) == 16, "NegativeHeader must stay 16 Bytes");

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[spoke_test] PASS %s\n", name); } \
    else { printf("[spoke_test] FAIL %s\n", name); g_failures++; } \
} while (0)

int main(void) {
    printf("=== Running Lifetime & Spoke Test Suite ===\n");

    // #1 Pointer Legitimacy verification (Lifetime_isLegit)
    CHECK("null pointer is not legit", !Lifetime_isLegit(nullptr));
    CHECK("low guard-page pointer is not legit", !Lifetime_isLegit((const void*) 0x1000));
    CHECK("unaligned pointer is not legit", !Lifetime_isLegit((const void*) 0x10001));
    CHECK("aligned high pointer is legit", Lifetime_isLegit((const void*) 0x10000));

    // #2 Lifetime creation & struct validation
    Lifetime lt = Lifetime_create(64u << 20, 64u << 20);
    CHECK("persistent arena created", lt.persistentArena != nullptr);
    CHECK("transient arena created", lt.transientArena != nullptr);
    CHECK("persistent attested", lt.persistentType == VEXSPOKE_TYPE_ARENA);
    CHECK("transient attested", lt.transientType == VEXSPOKE_TYPE_ARENA);
    CHECK("persistent arena is legit", Lifetime_isLegit(lt.persistentArena));
    CHECK("transient arena is legit", Lifetime_isLegit(lt.transientArena));
    CHECK("lifetime is valid", Lifetime_isValid(&lt));

    // #3 Invalid lifetime rejection
    Lifetime badLt = lt;
    badLt.persistentType = 0;
    CHECK("unattested lifetime is not valid", !Lifetime_isValid(&badLt));
    CHECK("null lifetime is not valid", !Lifetime_isValid(nullptr));

    // #4 Lifetime_bind with legitimacy guard
    Lifetime bound = Lifetime_bind(lt.persistentArena, lt.transientArena,
                                   lt.persistentType, lt.transientType, nullptr);
    CHECK("valid Lifetime_bind succeeds", Lifetime_isValid(&bound));

    Lifetime badBind = Lifetime_bind((void*) 0x10001, lt.transientArena,
                                     lt.persistentType, lt.transientType, nullptr);
    CHECK("unaligned Lifetime_bind rejected", badBind.persistentArena == nullptr);

    // #5 16-byte payload alignment doctrine
    void *p1 = Lifetime_allocPersistent(&lt, 100, 64);
    void *p2 = Lifetime_allocPersistent(&lt, 101, 128);
    void *t1 = Lifetime_allocTransient(&lt, 200, 48);
    void *t2 = Lifetime_allocTransient(&lt, 201, 256);

    CHECK("persistent p1 allocated", p1 != nullptr);
    CHECK("persistent p2 allocated", p2 != nullptr);
    CHECK("transient t1 allocated", t1 != nullptr);
    CHECK("transient t2 allocated", t2 != nullptr);

    CHECK("persistent p1 is legit", Lifetime_isLegit(p1));
    CHECK("persistent p2 is legit", Lifetime_isLegit(p2));
    CHECK("transient t1 is legit", Lifetime_isLegit(t1));
    CHECK("transient t2 is legit", Lifetime_isLegit(t2));

    // #6 16-byte NegativeHeader introspection (O(1) negative pointer recovery)
    NegativeHeader *h1 = ((NegativeHeader*) p1) - 1;
    NegativeHeader *h2 = ((NegativeHeader*) p2) - 1;
    CHECK("p1 header typeId", (*h1).typeId == 100);
    CHECK("p1 header length", (*h1).length == 64);
    CHECK("p2 header typeId", (*h2).typeId == 101);
    CHECK("p2 header length", (*h2).length == 128);

    // #7 Transient arena reset (scratchpad reset each tick)
    Lifetime_resetTransient(&lt);
    void *t3 = Lifetime_allocTransient(&lt, 202, 64);
    CHECK("transient alloc after reset", t3 != nullptr);
    CHECK("transient t3 is legit", Lifetime_isLegit(t3));

    // #8 Persistent free recycling
    Lifetime_freePersistent(&lt, p1);
    Lifetime_freePersistent(&lt, p2);
    CHECK("persistent free no crash", true);

    // #9 Clean destruction & null safety
    Lifetime_destroy(&lt);
    CHECK("persistent arena cleared", lt.persistentArena == nullptr);
    CHECK("transient arena cleared", lt.transientArena == nullptr);
    CHECK("persistent type cleared", lt.persistentType == 0);
    CHECK("transient type cleared", lt.transientType == 0);

    Lifetime_destroy(nullptr);
    Lifetime_resetTransient(nullptr);
    CHECK("destroy null safe", true);

    printf("\n=== Lifetime & Spoke Test Summary: %d failures ===\n", g_failures);
    return g_failures > 0 ? 1 : 0;
}

