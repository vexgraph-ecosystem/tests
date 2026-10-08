// tests/darkbase/database/database_test.c — owner test for database/database.c.

#include "annotation/overview.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "darkbase/type.h"
#include "database/database.h"
#include "nio/mem.h"
#include "oop/type.h"
#include "reflection/field.h"
#include "reflection/struct.h"
#include "test_support.h"

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: DatabaseTest (tests/darkbase/database/database_test.c)
 * LEVEL: L3 — Module Code (headless verification harness)
 * ============================================================================
 * Owner test for the L2 Database switchboard: register a reflection Struct as
 * an entity, bind caller-owned live rows, look them up, and report cold
 * rejections (duplicate/absent/invalid) through the recorded code. Rows are
 * borrowed pointers; the test proves they are returned unchanged and that their
 * slots stay stable across growth.
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

typedef struct Row {
    uintptr_t id;
    int32_t health;
} Row;

static void testConstructorAndNull(void) {
    printf("[1] construction, identity, null-safety\n");

    Database *db = Database();
    CHECK(db != nullptr);
    if (!db)
        return;
    CHECK(Database_check(db, TYPE_DB_DATABASE_SINGLETON));
    CHECK(Database_kind(db) == TYPE_DB_DATABASE_SINGLETON);
    CHECK(!Database_check(db, TYPE_DB_DATABASE_RESULT_SINGLETON));
    CHECK(Database_entityCount(db) == 0u);
    CHECK(Database_errorCode(db) == DATABASE_OK);

    CHECK(Database_entityCount(nullptr) == 0u);
    CHECK(Database_errorCode(nullptr) == DATABASE_INVALID);
    CHECK(Database_entityIndex(nullptr, "x") == -1);
    CHECK(Database_schema(nullptr, "x") == nullptr);
    CHECK(Database_row(nullptr, "x", 0u) == nullptr);
    CHECK(Database_count(nullptr, "x") == 0u);
    CHECK(Database_lastError(nullptr, nullptr, 0u) == -1);

    Database_free(nullptr); // no crash
    Database_free(db);

    char buf[64];
    bool truncated = true;
    Database_toString(nullptr, buf, sizeof(buf), &truncated);
    CHECK(truncated == false && strcmp(buf, "nullptr") == 0);
}

static void testDefineAndLookup(void) {
    printf("[2] define entities, index/count/schema lookups\n");

    Database *db = Database();
    Struct *player = Struct("player");
    Struct *item = Struct("item");
    CHECK(db && player && item);
    if (!db || !player || !item) {
        Database_free(db);
        Struct_free(player);
        Struct_free(item);
        return;
    }

    CHECK(Database_define(db, player) == 0);
    CHECK(Database_define(db, item) == 1);
    CHECK(Database_entityCount(db) == 2u);
    CHECK(Database_entityIndex(db, "player") == 0);
    CHECK(Database_entityIndex(db, "item") == 1);
    CHECK(Database_entityIndex(db, "Player") == 0); // folded lookup
    CHECK(Database_entityIndex(db, "absent") == -1);
    CHECK(Database_schema(db, "player") == player);
    CHECK(Database_schema(db, "absent") == nullptr);

    // Duplicate entity names reject and are recorded.
    Struct *player2 = Struct("player");
    CHECK(player2 != nullptr);
    CHECK(Database_define(db, player2) == -1);
    CHECK(Database_errorCode(db) == DATABASE_DUPLICATE);
    Struct_free(player2);

    Struct_free(player);
    Struct_free(item);
    Database_free(db);
}

static void testInsertAndRead(void) {
    printf("[3] bind live rows, read them back, stable across growth\n");

    Database *db = Database();
    Struct *player = Struct("player");
    CHECK(db && player);
    if (!db || !player) {
        Database_free(db);
        Struct_free(player);
        return;
    }
    CHECK(Database_define(db, player) == 0);

    Row rows[64];
    for (uint32_t i = 0u; i < 64u; i++) {
        rows[i].id = 100u + i;
        rows[i].health = (int32_t) i;
    }

    CHECK(Database_count(db, "player") == 0u);
    for (uint32_t i = 0u; i < 64u; i++)
        CHECK(Database_insert(db, "player", &rows[i]) == (int32_t) i);
    CHECK(Database_count(db, "player") == 64u);
    CHECK(Database_errorCode(db) == DATABASE_OK);

    // The exact bound pointer comes back, at its index.
    void *got = Database_row(db, "player", 37u);
    CHECK(got == (void*) &rows[37]);
    CHECK(((Row*) got)->id == 137u);
    CHECK(Database_row(db, "player", 64u) == nullptr); // out of range
    CHECK(Database_row(db, "player", 0u) == (void*) &rows[0]);

    // Absent entity + null argument rejection.
    CHECK(Database_insert(db, "ghost", &rows[0]) == -1);
    CHECK(Database_errorCode(db) == DATABASE_ABSENT);
    CHECK(Database_insert(db, "player", nullptr) == -1);
    CHECK(Database_errorCode(db) == DATABASE_INVALID);

    Database_free(db);
    Struct_free(player);
}

static void testDiagnostics(void) {
    printf("[4] recorded rejection is legible and bounded\n");

    Database *db = Database();
    CHECK(db != nullptr);
    if (!db)
        return;

    CHECK(Database_define(db, nullptr) == -1);
    CHECK(Database_errorCode(db) == DATABASE_INVALID);

    char buf[128];
    int len = Database_lastError(db, buf, sizeof(buf));
    CHECK(len > 0 && (size_t) len == strlen(buf));
    CHECK(strstr(buf, "define") != nullptr);

    // A too-small destination truncates silently at the byte level (bounded).
    char tiny[4];
    int tinyLen = Database_lastError(db, tiny, sizeof(tiny));
    CHECK(tinyLen == 3 && tiny[3] == '\0');

    CHECK(strcmp(Database_errorText(DATABASE_INVALID), "invalid argument") == 0);
    CHECK(strcmp(Database_errorText(DATABASE_DUPLICATE), "duplicate entity") == 0);
    CHECK(strcmp(Database_errorText(999), "unknown") == 0);

    char proj[96];
    bool truncated = true;
    Database_toStringStruct(db, proj, sizeof(proj), &truncated);
    CHECK(truncated == false && strstr(proj, "Database {") != nullptr);

    Database_free(db);
}

int main(void) {
    printf("=== Database Test Suite ===\n\n");

    testConstructorAndNull();
    testDefineAndLookup();
    testInsertAndRead();
    testDiagnostics();

    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    if (g_fail == 0)
        printf("=== Database Test Suite: ALL PASS ===\n");
    return g_fail == 0 ? 0 : 1;
}
