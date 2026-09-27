// tests/vexspoke/objects/probable_objects_test.c — the ProbableObjects class _test.
//
// The weighted pool: append choices, cumulative weights, a draw lands in the
// pool, capacity overflow is refused, and addProbable carries a Probable across.

#include <stddef.h>
#include <stdio.h>

#include "objects/probable_objects.h"
#include "objects/probable.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

int main(void) {
    ProbableObjects *po = ProbableObjects_1(4);
    CHECK(po != nullptr);
    CHECK(ProbableObjects_size(po) == 0);
    CHECK(ProbableObjects_capacity(po) >= 4);
    CHECK(ProbableObjects_totalWeight(po) == 0);

    CHECK(ProbableObjects_add(po, 100, 1) == 1);
    CHECK(ProbableObjects_add(po, 200, 2) == 1);
    CHECK(ProbableObjects_size(po) == 2);
    CHECK(ProbableObjects_totalWeight(po) == 3);

    // Cumulative column: 1, then 3.
    CHECK(ProbableObjects_objectAt(po, 0) == 100);
    CHECK(ProbableObjects_weightAt(po, 0) == 1);
    CHECK(ProbableObjects_cumulativeAt(po, 0) == 1);
    CHECK(ProbableObjects_objectAt(po, 1) == 200);
    CHECK(ProbableObjects_cumulativeAt(po, 1) == 3);

    // A draw lands on one of the pool's objects.
    uintptr_t drawn = ProbableObjects_get(po);
    CHECK(drawn == 100 || drawn == 200);

    // capacity overflow is refused, not corrupting.
    ProbableObjects *small = ProbableObjects_1(2);
    CHECK(ProbableObjects_add(small, 1, 1) == 1);
    CHECK(ProbableObjects_add(small, 2, 1) == 1);
    CHECK(ProbableObjects_add(small, 3, 1) == 0); // full
    CHECK(ProbableObjects_size(small) == 2);
    ProbableObjects_free(small);

    // addProbable copies object + weight.
    Probable *pr = Probable_3((uintptr_t) 500, 5, 5);
    CHECK(ProbableObjects_addProbable(po, pr) == 1);
    CHECK(ProbableObjects_size(po) == 3);
    CHECK(ProbableObjects_totalWeight(po) == 8);
    CHECK(ProbableObjects_objectAt(po, 2) == 500);
    Probable_free(pr);

    // Null-safety.
    CHECK(ProbableObjects_size(nullptr) == 0);
    CHECK(ProbableObjects_capacity(nullptr) == 0);
    CHECK(ProbableObjects_totalWeight(nullptr) == 0);
    CHECK(ProbableObjects_add(nullptr, 1, 1) == 0);
    CHECK(ProbableObjects_objectAt(nullptr, 0) == 0);
    CHECK(ProbableObjects_cumulativeAt(nullptr, 0) == 0);
    CHECK(ProbableObjects_get(nullptr) == 0);
    ProbableObjects_free(nullptr);

    ProbableObjects_free(po);

    if (g_failures == 0) {
        printf("probable_objects_test: all assertions held\n");
        return 0;
    }
    printf("probable_objects_test: %d FAILURES\n", g_failures);
    return 1;
}
