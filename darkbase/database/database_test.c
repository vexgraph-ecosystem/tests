// tests/darkbase/database/database_test.c — owner test for database/database.c.

#include "annotation/overview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

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

/** Verifies database construction, type identity, null-safe access, and destruction. */
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

/** Verifies entity registration, case-folded lookup, duplicate rejection, and cleanup. */
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

/** Verifies borrowed row insertion, stable indexed reads, growth, and rejected inserts. */
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

/** Verifies recorded database errors and bounded value/structure projections. */
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

/** Verifies save/load round trips, loaded-row ownership, and corrupt-file rejection. */
static void testPersistence(void) {
    printf("[5] .vexdb save/load round-trip, ownership, checksum, atomicity\n");

    const char *tmp = getenv("TMPDIR");
    if (tmp == nullptr)
        tmp = "/tmp";
    char path[256];
    snprintf(path, sizeof(path), "%sdarkbase_test_XXXXXX", tmp);
    int fd = mkstemp(path);
    CHECK(fd >= 0);
    if (fd < 0)
        return;
    close(fd);

    Database *db = Database();
    Struct *player = Struct("player");
    CHECK(db && player);
    if (!db || !player) {
        remove(path);
        Database_free(db);
        Struct_free(player);
        return;
    }
    Struct_setSize(player, (uint32_t) sizeof(Row));
    CHECK(Database_define(db, player) == 0);

    Row rows[4];
    for (uint32_t i = 0u; i < 4u; i++) {
        rows[i].id = 7u + i;
        rows[i].health = (int32_t) (i + 1u);
        CHECK(Database_insert(db, "player", &rows[i]) == (int32_t) i);
    }
    CHECK(Database_save(db, path) == DATABASE_OK);

    // A fresh database with the same schema loads the rows back.
    Database *db2 = Database();
    Struct *player2 = Struct("player");
    CHECK(db2 && player2);
    if (!db2 || !player2) {
        remove(path);
        Database_free(db2);
        Struct_free(player2);
        Database_free(db);
        Struct_free(player);
        return;
    }
    Struct_setSize(player2, (uint32_t) sizeof(Row));
    CHECK(Database_define(db2, player2) == 0);
    CHECK(Database_load(db2, path) == DATABASE_OK);
    CHECK(Database_count(db2, "player") == 4u);
    for (uint32_t i = 0u; i < 4u; i++) {
        Row *got = (Row*) Database_row(db2, "player", i);
        CHECK(got != nullptr && got->id == 7u + i && got->health == (int32_t) (i + 1u));
    }

    // Loaded rows are owned: binding a live row into them rejects.
    CHECK(Database_insert(db2, "player", &rows[0]) == -1);
    CHECK(Database_errorCode(db2) == DATABASE_INVALID);

    // Re-loading into a non-empty entity rejects without mutation.
    CHECK(Database_load(db2, path) == DATABASE_DUPLICATE);
    CHECK(Database_count(db2, "player") == 4u);

    // Corrupt the trailing checksum: reject and load nothing (atomic).
    FILE *fp = fopen(path, "r+b");
    CHECK(fp != nullptr);
    if (fp) {
        unsigned char last = 0u;
        fseek(fp, -1L, SEEK_END);
        if (fread(&last, 1u, 1u, fp) == 1u) {
            last ^= 0xFFu;
            fseek(fp, -1L, SEEK_END);
            fwrite(&last, 1u, 1u, fp);
        }
        fclose(fp);
    }
    Database *db3 = Database();
    Struct *player3 = Struct("player");
    CHECK(db3 && player3);
    if (db3 && player3) {
        Struct_setSize(player3, (uint32_t) sizeof(Row));
        CHECK(Database_define(db3, player3) == 0);
        CHECK(Database_load(db3, path) == DATABASE_INVALID);
        CHECK(Database_count(db3, "player") == 0u); // nothing loaded
        Database_free(db3);
    }
    Struct_free(player3);

    remove(path);
    Database_free(db2);
    Struct_free(player2);
    Database_free(db);
    Struct_free(player);
}

/** Verifies atomic save replacement and preservation of the previous file on rejection. */
static void testSafeSave(void) {
    printf("[6] atomic save publication (temp write + fsync + rename)\n");

    const char *tmp = getenv("TMPDIR");
    if (tmp == nullptr)
        tmp = "/tmp";
    char path[256];
    snprintf(path, sizeof(path), "%sdarkbase_save_XXXXXX", tmp);
    int fd = mkstemp(path);
    CHECK(fd >= 0);
    if (fd < 0)
        return;
    close(fd);

    char tpath[300];
    snprintf(tpath, sizeof(tpath), "%s.%ld.tmp", path, (long) getpid());

    // First snapshot: 4 rows.
    Database *a = Database();
    Struct *sa = Struct("player");
    Struct_setSize(sa, (uint32_t) sizeof(Row));
    CHECK(Database_define(a, sa) == 0);
    Row ra[4];
    for (uint32_t i = 0u; i < 4u; i++) {
        ra[i].id = 1u;
        ra[i].health = (int32_t) i;
        Database_insert(a, "player", &ra[i]);
    }
    CHECK(Database_save(a, path) == DATABASE_OK);
    CHECK(access(tpath, F_OK) != 0); // temp is renamed away, never left behind

    // Replace with a bigger snapshot: 6 rows.
    Database *b = Database();
    Struct *sb = Struct("player");
    Struct_setSize(sb, (uint32_t) sizeof(Row));
    CHECK(Database_define(b, sb) == 0);
    Row rb[6];
    for (uint32_t i = 0u; i < 6u; i++) {
        rb[i].id = 2u;
        rb[i].health = (int32_t) (10 + i);
        Database_insert(b, "player", &rb[i]);
    }
    CHECK(Database_save(b, path) == DATABASE_OK);
    CHECK(access(tpath, F_OK) != 0);

    // The destination now holds the new snapshot (atomic replace, not truncate).
    Database *c = Database();
    Struct *sc = Struct("player");
    Struct_setSize(sc, (uint32_t) sizeof(Row));
    Database_define(c, sc);
    CHECK(Database_load(c, path) == DATABASE_OK);
    CHECK(Database_count(c, "player") == 6u);
    CHECK(((Row*) Database_row(c, "player", 0u))->id == 2u);

    // A rejected save must not touch the destination.
    Database *d = Database();
    Struct *sd = Struct("empty"); // stride stays 0 -> save rejects
    Database_define(d, sd);
    CHECK(Database_save(d, path) == DATABASE_INVALID);
    CHECK(access(tpath, F_OK) != 0);

    // The prior snapshot survives the failed save.
    Database *e = Database();
    Struct *se = Struct("player");
    Struct_setSize(se, (uint32_t) sizeof(Row));
    Database_define(e, se);
    CHECK(Database_load(e, path) == DATABASE_OK);
    CHECK(Database_count(e, "player") == 6u);

    remove(path);
    Database_free(a);
    Struct_free(sa);
    Database_free(b);
    Struct_free(sb);
    Database_free(c);
    Struct_free(sc);
    Database_free(d);
    Struct_free(sd);
    Database_free(e);
    Struct_free(se);
}

int main(void) {
    printf("=== Database Test Suite ===\n\n");

    testConstructorAndNull();
    testDefineAndLookup();
    testInsertAndRead();
    testDiagnostics();
    testPersistence();
    testSafeSave();

    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    if (g_fail == 0)
        printf("=== Database Test Suite: ALL PASS ===\n");
    return g_fail == 0 ? 0 : 1;
}
