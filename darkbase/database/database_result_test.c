// tests/darkbase/database/database_result_test.c — owner test for database/database_result.c.

#include "annotation/overview.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "darkbase/type.h"
#include "database/database.h"
#include "database/database_result.h"
#include "nio/mem.h"
#include "oop/type.h"
#include "reflection/struct.h"
#include "struct/chunked_list.h"
#include "test_support.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: DatabaseResultTest (tests/darkbase/database/database_result_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Owner test for the dest-last row cursor: a DatabaseResult borrows one
 * entity's row-pointer list and walks it without copying; next() writes into a
 * caller destination, rewind restarts, and the count is the construction
 * snapshot. Also proves the empty cursor and the null-source rejection.
 *
 * STRUCT FIELDS: none — procedural test harness.
 *
 * FUNCTION REGISTRY:
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

typedef struct Row2 {
    uint32_t v;
} Row2;

static void testEmptyCursor(void) {
    printf("[1] empty cursor + null-safety\n");

    DatabaseResult *r = DatabaseResult();
    CHECK(r != nullptr);
    if (!r)
        return;
    CHECK(DatabaseResult_check(r, TYPE_DB_DATABASE_RESULT_SINGLETON));
    CHECK(DatabaseResult_count(r) == 0u);
    CHECK(DatabaseResult_position(r) == 0u);

    void *row = (void*) (uintptr_t) 1u;
    CHECK(DatabaseResult_next(r, &row) == false);
    CHECK(row == (void*) (uintptr_t) 1u); // untouched on false
    CHECK(DatabaseResult_next(r, nullptr) == false);
    CHECK(DatabaseResult_next(nullptr, &row) == false);

    DatabaseResult_rewind(nullptr); // no crash
    CHECK(DatabaseResult_count(nullptr) == 0u);
    DatabaseResult_free(nullptr);

    // A null row source is a cold rejection.
    CHECK(DatabaseResult_1(nullptr) == nullptr);

    DatabaseResult_free(r);
}

static void testWalk(void) {
    printf("[2] walk an entity's rows dest-last, rewind, no copies\n");

    Database *db = Database();
    Struct *player = Struct("player");
    CHECK(db && player);
    if (!db || !player) {
        Database_free(db);
        Struct_free(player);
        return;
    }
    CHECK(Database_define(db, player) == 0);

    Row2 rows[8];
    for (uint32_t i = 0u; i < 8u; i++) {
        rows[i].v = i * 10u;
        CHECK(Database_insert(db, "player", &rows[i]) == (int32_t) i);
    }

    DatabaseResult *r = Database_select(db, "player");
    CHECK(r != nullptr);
    if (!r) {
        Database_free(db);
        Struct_free(player);
        return;
    }
    CHECK(DatabaseResult_count(r) == 8u);

    // Each next() yields the exact bound pointer, in insertion order.
    for (uint32_t i = 0u; i < 8u; i++) {
        void *got = nullptr;
        CHECK(DatabaseResult_next(r, &got));
        CHECK(got == (void*) &rows[i]);
        CHECK(((Row2*) got)->v == i * 10u);
    }
    CHECK(DatabaseResult_position(r) == 8u);
    void *past = (void*) (uintptr_t) 2u;
    CHECK(DatabaseResult_next(r, &past) == false);
    CHECK(past == (void*) (uintptr_t) 2u);

    // Rewind replays the same rows.
    DatabaseResult_rewind(r);
    CHECK(DatabaseResult_position(r) == 0u);
    void *first = nullptr;
    CHECK(DatabaseResult_next(r, &first) && first == (void*) &rows[0]);

    // Selecting an absent entity rejects.
    CHECK(Database_select(db, "ghost") == nullptr);
    CHECK(Database_errorCode(db) == DATABASE_ABSENT);

    DatabaseResult_free(r);
    Database_free(db);
    Struct_free(player);
}

static void testStrings(void) {
    printf("[3] string projections\n");

    DatabaseResult *r = DatabaseResult();
    CHECK(r != nullptr);
    if (!r)
        return;
    char buf[96];
    bool truncated = true;
    DatabaseResult_toString(r, buf, sizeof(buf), &truncated);
    CHECK(truncated == false && strcmp(buf, "DatabaseResult(0/0)") == 0);

    char tiny[4];
    DatabaseResult_toString(r, tiny, sizeof(tiny), &truncated);
    CHECK(truncated == true);

    DatabaseResult_toString(nullptr, buf, sizeof(buf), &truncated);
    CHECK(truncated == false && strcmp(buf, "nullptr") == 0);

    DatabaseResult_free(r);
}

int main(void) {
    printf("=== DatabaseResult Test Suite ===\n\n");

    testEmptyCursor();
    testWalk();
    testStrings();

    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    if (g_fail == 0)
        printf("=== DatabaseResult Test Suite: ALL PASS ===\n");
    return g_fail == 0 ? 0 : 1;
}
