// tests/hotcwap/hot/hot_trampoline_test.c — the HotTrampolineTable _test.
//
// Proves the per-instance atomic dispatch table (the mid-swap "never jump to
// NULL" contract the loader stands on):
//   - register allocates rows in order; NULL self/name is rejected (-1).
//   - find resolves by name; absent or NULL name is -1.
//   - set stores the current target and stashes the prior as fallback.
//   - get returns the current target, and falls back rather than returning
//     NULL when only the prior generation is set (the mid-swap cover);
//     out-of-range indices are NULL.
//   - an over-long name is clamped with a NUL terminator.
//
// Headless, deterministic, no dylibs.

#include <stdio.h>
#include <string.h>

#include "hot/hot_trampoline.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static int g_targetA = 1;
static int g_targetB = 2;

int main(void) {
    static HotTrampolineTable table; // zero-initialized (count 0)

    // register: in-order indices; NULL self / NULL name rejected.
    int a = HotTrampolineTable_register(&table, "alpha");
    int b = HotTrampolineTable_register(&table, "beta");
    CHECK(a == 0);
    CHECK(b == 1);
    CHECK(HotTrampolineTable_register(&table, nullptr) == -1);
    CHECK(HotTrampolineTable_register(nullptr, "x") == -1);

    // find: by-name resolution, absent and NULL rejected.
    CHECK(HotTrampolineTable_find(&table, "alpha") == 0);
    CHECK(HotTrampolineTable_find(&table, "beta") == 1);
    CHECK(HotTrampolineTable_find(&table, "gamma") == -1);
    CHECK(HotTrampolineTable_find(&table, nullptr) == -1);
    CHECK(HotTrampolineTable_find(nullptr, "alpha") == -1);

    // get before set: no target, no fallback -> NULL; bad indices -> NULL.
    CHECK(HotTrampolineTable_get(&table, 0) == nullptr);
    CHECK(HotTrampolineTable_get(&table, -1) == nullptr);
    CHECK(HotTrampolineTable_get(&table, 999) == nullptr);
    CHECK(HotTrampolineTable_get(nullptr, 0) == nullptr);

    // set + get: the current target wins.
    void *pa = (void*) &g_targetA;
    void *pb = (void*) &g_targetB;
    HotTrampolineTable_set(&table, a, pa);
    CHECK(HotTrampolineTable_get(&table, a) == pa);
    HotTrampolineTable_set(&table, a, pb);
    CHECK(HotTrampolineTable_get(&table, a) == pb);

    // out-of-range set is a no-op.
    HotTrampolineTable_set(&table, 999, pa);
    HotTrampolineTable_set(nullptr, 0, pa);
    CHECK(HotTrampolineTable_get(&table, 999) == nullptr);

    // mid-swap cover: clear the current target; get must fall back to the
    // prior generation (pb) rather than returning NULL.
    HotTrampolineTable_set(&table, a, nullptr);
    CHECK(HotTrampolineTable_get(&table, a) == pb);

    // over-long name is clamped to HOT_MANIFEST_MAX_NAME-1 + NUL.
    char big[HOT_MANIFEST_MAX_NAME + 40];
    memset(big, 'z', sizeof big - 1);
    big[sizeof big - 1] = '\0';
    int c = HotTrampolineTable_register(&table, big);
    CHECK(c == 2);
    char clamped[HOT_MANIFEST_MAX_NAME];
    memset(clamped, 'z', HOT_MANIFEST_MAX_NAME - 1);
    clamped[HOT_MANIFEST_MAX_NAME - 1] = '\0';
    CHECK(HotTrampolineTable_find(&table, clamped) == 2);
    HotTrampoline *row = &table.rows[2];
    CHECK(strlen((*row).name) == HOT_MANIFEST_MAX_NAME - 1);

    if (g_failures == 0) {
        printf("hot_trampoline_test: all assertions held\n");
        return 0;
    }
    printf("hot_trampoline_test: %d FAILURES\n", g_failures);
    return 1;
}
