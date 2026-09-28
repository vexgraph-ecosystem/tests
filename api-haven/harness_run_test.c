#include "annotation/overview.h"
#include "harness/engine_provider.h"
#include "harness/harness.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: HarnessRunTest (_test/harness_run_test.c — local-only scratch, never committed)
 * LEVEL: L2 — Behavior verification (headless; no spawn, no network)
 * ============================================================================
 * Executable proof of the src/harness/ seam: the EngineProvider table
 * (count, at, get, resolveCli, null-safety), the unbound-driver degrade
 * (run false, no slot consumed), 16 bounded slots + BUSY_FULL on the
 * 17th run, and the TIMEOUT drop-degrade path through a stub poll fn.
 * All drivers are in-process stubs — nothing spawns, nothing blocks.
 *
 * Lives in umbrella _test/ (gitignored) so api-haven stays GitHub-clean.
 * Build: cc -std=gnu23 -Wall -Wextra -Werror -I projects/api-haven/src
 *   -I projects/vexspoke/src _test/harness_run_test.c
 *   projects/api-haven/src/harness/engine_provider.c
 *   projects/api-haven/src/harness/harness.c -o /tmp/harness_run_test
 * Exit code 0 = all checks green; 1 = at least one check failed.
 * ============================================================================
 */

static int sFailures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);     \
            sFailures++;                                               \
        }                                                              \
    } while (0)

// --- stub driver (in-process; records calls, never spawns) ------------------
static int sSpawnCalls = 0;
static int sCancelCalls = 0;
static HarnessStatus sPollVerdict = HARNESS_STATUS_RUNNING;
static uint64_t sLastTimeoutMs = 0;

static bool stubSpawn(void *driverCtx, const EngineProviderSlot *engine,
                      const char *prompt, size_t promptLen,
                      uint64_t timeoutMs, void **outHandle) {
    (void)driverCtx;
    (void)engine;
    (void)prompt;
    (void)promptLen;
    sSpawnCalls++;
    sLastTimeoutMs = timeoutMs;
    if (outHandle)
        (*outHandle) = (void*) (uintptr_t) (0x1000 + (uintptr_t) sSpawnCalls);
    return true;
}

static HarnessStatus stubPoll(void *driverCtx, void *handle, uint64_t timeoutMs) {
    (void)driverCtx;
    (void)handle;
    (void)timeoutMs;
    return sPollVerdict;
}

static void stubCancel(void *driverCtx, void *handle) {
    (void)driverCtx;
    (void)handle;
    sCancelCalls++;
}

static HarnessDriverTable stubTable(void) {
    HarnessDriverTable table;
    table.spawnFn = stubSpawn;
    table.pollFn = stubPoll;
    table.cancelFn = stubCancel;
    return table;
}

int main(int argc, const char **argv) {
    (void)argc;
    (void)argv;

    // --- EngineProvider directory shape ------------------------------------
    EngineProvider *dir = EngineProvider_shared();
    CHECK(dir != NULL);
    CHECK(EngineProvider_count(dir) == 20);
    CHECK(EngineProvider_at(dir, 0) != NULL);
    CHECK(EngineProvider_at(dir, 20) == NULL);
    CHECK(EngineProvider_get(dir, "claude-code") != NULL);
    CHECK(EngineProvider_get(dir, "opencode") != NULL);
    CHECK(EngineProvider_get(dir, "kilocode") != NULL);
    CHECK(EngineProvider_get(dir, "t3-code") != NULL);
    CHECK(EngineProvider_get(dir, "agy") != NULL);
    CHECK(EngineProvider_get(dir, "codex") != NULL);
    CHECK(EngineProvider_get(dir, "gemini-cli") != NULL);
    CHECK(EngineProvider_get(dir, "aider") != NULL);
    CHECK(EngineProvider_get(dir, "pi") != NULL);
    CHECK(EngineProvider_get(dir, "goose") != NULL);
    CHECK(EngineProvider_get(dir, "cursor-cli") != NULL);
    CHECK(EngineProvider_get(dir, "kiro-cli") != NULL);
    CHECK(EngineProvider_get(dir, "windsurf") != NULL);
    CHECK(EngineProvider_get(dir, "qwen-code") != NULL);
    CHECK(EngineProvider_get(dir, "crush") != NULL);
    CHECK(EngineProvider_get(dir, "hermes") != NULL);
    CHECK(EngineProvider_get(dir, "amp") != NULL);
    CHECK(EngineProvider_get(dir, "gptme") != NULL);
    CHECK(EngineProvider_get(dir, "openhands") != NULL);
    CHECK(EngineProvider_get(dir, "copilot-cli") != NULL);
    CHECK(EngineProvider_get(dir, "definitely-not-an-engine") == NULL);

    const EngineProviderSlot *opencode = EngineProvider_get(dir, "opencode");
    CHECK(opencode && strcmp(EngineProvider_getCliName(dir, opencode), "opencode") == 0);
    CHECK(opencode && EngineProvider_getFamily(dir, opencode) == ENGINE_PROVIDER_FAMILY_CLI);
    CHECK(opencode && EngineProvider_getAuth(dir, opencode) == ENGINE_PROVIDER_AUTH_API_KEY);

    char cli[32];
    CHECK(EngineProvider_resolveCli(dir, opencode, cli, sizeof(cli)));
    CHECK(strcmp(cli, "opencode") == 0);
    char tiny[4];
    CHECK(!EngineProvider_resolveCli(dir, opencode, tiny, sizeof(tiny)));
    CHECK(!EngineProvider_resolveCli(dir, opencode, NULL, 32));
    CHECK(!EngineProvider_resolveCli(dir, opencode, cli, 0));
    CHECK(!EngineProvider_resolveCli(NULL, opencode, cli, sizeof(cli)));
    CHECK(!EngineProvider_resolveCli(dir, NULL, cli, sizeof(cli)));

    // --- null-safety (Rule 24) ----------------------------------------------
    CHECK(EngineProvider_count(NULL) == 0);
    CHECK(EngineProvider_at(NULL, 0) == NULL);
    CHECK(EngineProvider_get(NULL, NULL) == NULL);
    CHECK(EngineProvider_get(dir, NULL) == NULL);
    CHECK(EngineProvider_getSlug(NULL, NULL) == NULL);
    CHECK(EngineProvider_getDisplayName(NULL, NULL) == NULL);
    CHECK(EngineProvider_getCliName(NULL, NULL) == NULL);
    CHECK(EngineProvider_getNote(NULL, NULL) == NULL);
    CHECK(EngineProvider_getFamily(NULL, NULL) == ENGINE_PROVIDER_FAMILY_CLI);
    CHECK(EngineProvider_getAuth(NULL, NULL) == ENGINE_PROVIDER_AUTH_NONE);

    // --- unbound-driver degrade (no engine, no driver: no slot consumed) ----
    Harness bare = Harness_0();
    CHECK(Harness_getEngine(&bare) == NULL);
    CHECK(Harness_getDriver(&bare) == NULL);
    CHECK(Harness_getTimeout(&bare) == 100);
    CHECK(!Harness_isRunning(&bare));
    uint32_t jobId = 0;
    CHECK(!Harness_run(&bare, "hello", 5, 100, &jobId));
    CHECK(Harness_getLastStatus(&bare) == HARNESS_STATUS_IDLE);
    CHECK(Harness_getJobCount(&bare) == 0);
    CHECK(!Harness_run(NULL, "hello", 5, 100, &jobId));
    CHECK(!Harness_run(&bare, NULL, 5, 100, &jobId));
    CHECK(!Harness_run(&bare, "hello", 0, 100, &jobId));

    // --- bound run + poll + cancel through the stub --------------------------
    Harness h = Harness_1(250);
    Harness_setEngine(&h, opencode);
    int driverCtx = 7;
    Harness_setDriver(&h, &driverCtx, stubTable());
    CHECK(Harness_getEngine(&h) == opencode);
    CHECK(Harness_getDriver(&h) == &driverCtx);
    CHECK(Harness_getTimeout(&h) == 250);
    sSpawnCalls = 0;
    sCancelCalls = 0;
    sPollVerdict = HARNESS_STATUS_RUNNING;
    CHECK(Harness_run(&h, "hello", 5, 0, &jobId));
    CHECK(jobId == 1);
    CHECK(sSpawnCalls == 1);
    CHECK(sLastTimeoutMs == 250); // zero arg falls back to the default
    CHECK(Harness_isRunning(&h));
    CHECK(Harness_getJobStatus(&h, jobId) == HARNESS_STATUS_RUNNING);
    CHECK(Harness_poll(&h, jobId) == HARNESS_STATUS_RUNNING);
    CHECK(Harness_getJobAt(&h, 0) != NULL);
    CHECK(Harness_getJobAt(&h, 1) == NULL);
    CHECK(Harness_cancel(&h, jobId));
    CHECK(sCancelCalls == 1);
    CHECK(Harness_getJobStatus(&h, jobId) == HARNESS_STATUS_IDLE);
    CHECK(!Harness_isRunning(&h));
    CHECK(!Harness_cancel(&h, jobId)); // idle slot: false
    CHECK(!Harness_cancel(&h, 999));   // unknown job: false
    CHECK(Harness_poll(&h, 999) == HARNESS_STATUS_IDLE);
    CHECK(Harness_poll(NULL, 1) == HARNESS_STATUS_IDLE);
    CHECK(Harness_getJobStatus(NULL, 1) == HARNESS_STATUS_IDLE);

    // --- BUSY_FULL: 16 bounded slots, 17th run refused -----------------------
    Harness full = Harness_0();
    Harness_setEngine(&full, EngineProvider_get(dir, "claude-code"));
    Harness_setDriver(&full, &driverCtx, stubTable());
    sPollVerdict = HARNESS_STATUS_RUNNING;
    uint32_t ids[HARNESS_MAX_JOBS];
    for (uint32_t i = 0; i < (uint32_t) HARNESS_MAX_JOBS; i++) {
        CHECK(Harness_run(&full, "p", 1, 50, &ids[i]));
        CHECK(ids[i] == i + 1);
    }
    CHECK(Harness_getJobCount(&full) == (uint32_t) HARNESS_MAX_JOBS);
    uint32_t extra = 0;
    CHECK(!Harness_run(&full, "one-too-many", 12, 50, &extra));
    CHECK(Harness_getLastStatus(&full) == HARNESS_STATUS_BUSY_FULL);
    CHECK(Harness_getJobCount(&full) == (uint32_t) HARNESS_MAX_JOBS);

    // --- TIMEOUT drop-degrade path via the stub poll -------------------------
    sPollVerdict = HARNESS_STATUS_TIMEOUT;
    CHECK(Harness_poll(&full, ids[0]) == HARNESS_STATUS_TIMEOUT);
    CHECK(Harness_getJobStatus(&full, ids[0]) == HARNESS_STATUS_TIMEOUT);
    CHECK(Harness_getJobStatus(&full, ids[1]) == HARNESS_STATUS_RUNNING);

    // --- NULL-driver poll degrades to the stored status ----------------------
    HarnessDriverTable noPoll = stubTable();
    noPoll.pollFn = NULL;
    Harness_setDriver(&h, &driverCtx, noPoll);
    CHECK(Harness_poll(&h, 1) == HARNESS_STATUS_IDLE);

    if (sFailures == 0) {
        printf("harness_run_test: ALL CHECKS PASSED\n");
        return 0;
    }
    printf("harness_run_test: %d FAILURES\n", sFailures);
    return 1;
}
