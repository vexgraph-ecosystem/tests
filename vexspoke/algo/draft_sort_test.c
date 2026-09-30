// tests/vexspoke/algo/draft_sort_test.c — owner test for algo/draft_sort.
//
//   - DraftSort_partitionInt32 is a three-way-partition building block: it
//     returns the boundary i where [0,i) < pivot and [i,count) >= pivot, is
//     stable in the partition sense (each side keeps relative order of the
//     values it gathers), and handles nullptr / count 0 / all-equal /
//     all-less / all-greater / negative values;
//   - DraftSort_quicksortUint64 sorts in place and matches a qsort oracle,
//     including duplicates, 0 and UINT64_MAX, with nullptr/0/1 no-ops;
//   - DraftSort_morton3D interleaves 21-bit axes into a 63-bit Z-order code:
//     zero fixed point, single bits land in distinct positions, the all-ones
//     bound is 2^63-1, and bits above bit 20 are masked away.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "algo/draft_sort.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void check_partition(const int32_t *a, size_t count, int32_t pivot, size_t boundary) {
    CHECK(boundary <= count);
    for (size_t i = 0; i < boundary; i++)
        CHECK(a[i] < pivot);
    for (size_t i = boundary; i < count; i++)
        CHECK(a[i] >= pivot);
}

static void test_partition(void) {
    CHECK(DraftSort_partitionInt32(nullptr, 5, 0) == 0);
    int32_t empty[1] = { 1 };
    CHECK(DraftSort_partitionInt32(empty, 0, 0) == 0);

    int32_t a[] = { 3, 1, 2, 5, 4, -1, 3 };
    size_t n = sizeof(a) / sizeof(a[0]);
    size_t b = DraftSort_partitionInt32(a, n, 3);
    check_partition(a, n, 3, b);
    CHECK(b == 3);                                       // -1,1,2 are below 3

    // All equal to the pivot: nothing is below.
    int32_t eq[] = { 7, 7, 7, 7 };
    CHECK(DraftSort_partitionInt32(eq, 4, 7) == 0);
    check_partition(eq, 4, 7, 0);

    // Everything below.
    int32_t lo[] = { 0, 1, 2, 3 };
    CHECK(DraftSort_partitionInt32(lo, 4, 10) == 4);

    // Everything at/above.
    int32_t hi[] = { 10, 11, 12 };
    CHECK(DraftSort_partitionInt32(hi, 3, 5) == 0);

    // Negative pivot.
    int32_t neg[] = { -5, -1, -10, 0, 2 };
    size_t nb = DraftSort_partitionInt32(neg, 5, -1);
    check_partition(neg, 5, -1, nb);
    CHECK(nb == 2);
}

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t*) a, y = *(const uint64_t*) b;
    return (x > y) - (x < y);
}

static void test_quicksort(void) {
    DraftSort_quicksortUint64(nullptr, 10);
    uint64_t one[1] = { 42 };
    DraftSort_quicksortUint64(one, 1);
    CHECK(one[0] == 42);
    DraftSort_quicksortUint64(one, 0);
    CHECK(one[0] == 42);

    uint64_t a[] = { 5, 5, 0, UINT64_MAX, 1, 0x8000000000000000ull, 1 };
    size_t n = sizeof(a) / sizeof(a[0]);
    DraftSort_quicksortUint64(a, n);
    for (size_t i = 1; i < n; i++)
        CHECK(a[i - 1] <= a[i]);
    CHECK(a[0] == 0 && a[1] == 1 && a[2] == 1 && a[3] == 5 && a[4] == 5);
    CHECK(a[n - 1] == UINT64_MAX);
    CHECK(a[n - 2] == 0x8000000000000000ull);

    enum { N = 1024 };
    uint64_t r[N], ref[N];
    uint64_t seed = 0xabcdef01u;
    for (size_t i = 0; i < N; i++) {
        seed = seed * 2862933555777941757ull + 3037000493ull;
        r[i] = seed;
        ref[i] = seed;
    }
    DraftSort_quicksortUint64(r, N);
    qsort(ref, N, sizeof(uint64_t), cmp_u64);
    for (size_t i = 0; i < N; i++)
        CHECK(r[i] == ref[i]);
}

static unsigned popcount64(uint64_t v) {
    unsigned c = 0;
    while (v) {
        c += (unsigned) (v & 1u);
        v >>= 1;
    }
    return c;
}

static void test_morton(void) {
    CHECK(DraftSort_morton3D(0, 0, 0) == 0);

    uint64_t mx = DraftSort_morton3D(1, 0, 0);
    uint64_t my = DraftSort_morton3D(0, 1, 0);
    uint64_t mz = DraftSort_morton3D(0, 0, 1);
    CHECK(popcount64(mx) == 1);
    CHECK(popcount64(my) == 1);
    CHECK(popcount64(mz) == 1);
    CHECK(mx != my && my != mz && mx != mz);

    // The full 21-bit cube maps to 2^63 - 1 (all 63 interleaved bits set).
    CHECK(DraftSort_morton3D(0x1FFFFFu, 0x1FFFFFu, 0x1FFFFFu) == 0x7FFFFFFFFFFFFFFFull);

    // Bits above bit 20 are outside the 21-bit contract and masked away.
    CHECK(DraftSort_morton3D(1u << 21, 0, 0) == 0);
    CHECK(DraftSort_morton3D(0, 1u << 31, 0) == 0);

    // Distinct adjacent coordinates produce distinct codes, and the mapping
    // is deterministic.
    CHECK(DraftSort_morton3D(1, 1, 1) != DraftSort_morton3D(2, 0, 0));
    for (int i = 0; i < 4; i++)
        CHECK(DraftSort_morton3D(7, 9, 11) == DraftSort_morton3D(7, 9, 11));
}

int main(void) {
    test_partition();
    test_quicksort();
    test_morton();

    if (g_failures == 0) {
        printf("draft_sort_test: all assertions held\n");
        return 0;
    }
    printf("draft_sort_test: %d FAILURES\n", g_failures);
    return 1;
}
