#include "annotation/overview.h"
#include "system/capture_tool.h"
#include "system/process_probe.h"

#include <stdio.h>
#include <string.h>

;;OVERVIEW
/**
 * ============================================================================
 * MODULE: CaptureToolTest (tests/capture_tool_test.c)
 * LEVEL: L2 — Behavior verification (headless; libproc probes, no
 * recording, no permission prompts)
 * ============================================================================
 * Executable proof of the capture directory: registry shape (count, slug
 * lookups, kind coverage, driver-vs-process row contract), live
 * liveness probes (self-truthy via ProcessProbe, false for fake names,
 * false for driver rows), driver dir scanning (absent dir = false), and
 * every null-safety guard.
 *
 * Environment-tolerant: capture tools' running state is REPORTED, never
 * asserted — the suite must pass whether or not OBS is open.
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

int main(int argc, const char **argv) {
    CaptureTool *tools = CaptureTool_shared();
    CHECK(tools != NULL);
    const uint32_t total = CaptureTool_count(tools);
    printf("CaptureTool_count = %u\n", total);
    CHECK(total == 15);
    CHECK(CaptureTool_at(tools, 0) != NULL);
    CHECK(CaptureTool_at(tools, total) == NULL);

    // --- registry shape --------------------------------------------------------
    const CaptureSlot *obs = CaptureTool_get(tools, "obs");
    CHECK(obs != NULL);
    CHECK(obs && strcmp(CaptureTool_getDisplayName(tools, obs), "OBS Studio") == 0);
    CHECK(obs && strcmp(CaptureTool_getProcKey(tools, obs), "obs") == 0);
    CHECK(obs && CaptureTool_getKind(tools, obs) == CAPTURE_KIND_SCREEN);

    const CaptureSlot *quicktime = CaptureTool_get(tools, "quicktime-player");
    CHECK(quicktime && strcmp(CaptureTool_getProcKey(tools, quicktime),
                              "QuickTime Player") == 0);

    const CaptureSlot *screenshot = CaptureTool_get(tools, "screenshot");
    CHECK(screenshot && strcmp(CaptureTool_getProcKey(tools, screenshot),
                               "screencaptureui") == 0);

    const CaptureSlot *voice = CaptureTool_get(tools, "voice-memos");
    CHECK(voice && CaptureTool_getKind(tools, voice) == CAPTURE_KIND_AUDIO);

    const CaptureSlot *zoom = CaptureTool_get(tools, "zoom");
    CHECK(zoom && CaptureTool_getKind(tools, zoom) == CAPTURE_KIND_STREAM);

    CHECK(CaptureTool_get(tools, "definitely-not-a-capture-tool") == NULL);

    // --- driver-vs-process contract -------------------------------------------
    const CaptureSlot *blackhole = CaptureTool_get(tools, "blackhole");
    CHECK(blackhole && CaptureTool_getProcKey(tools, blackhole) == NULL);
    CHECK(blackhole && CaptureTool_getDriverDir(tools, blackhole) != NULL);
    const CaptureSlot *soundflower = CaptureTool_get(tools, "soundflower");
    CHECK(soundflower && CaptureTool_getProcKey(tools, soundflower) == NULL);
    CHECK(soundflower && CaptureTool_getDriverDir(tools, soundflower) != NULL);

    for (uint32_t i = 0; i < total; i++) {
        const CaptureSlot *row = CaptureTool_at(tools, i);
        CHECK(row != NULL);
        CHECK(row && CaptureTool_getSlug(tools, row) != NULL);
        CHECK(row && CaptureTool_getDisplayName(tools, row) != NULL);
        CHECK(row && CaptureTool_getKind(tools, row) >= CAPTURE_KIND_SCREEN);
        CHECK(row && CaptureTool_getKind(tools, row) <= CAPTURE_KIND_AUDIO);
        // exactly one probe path per row: process XOR driver
        if (row) {
            const char *proc = CaptureTool_getProcKey(tools, row);
            const char *dir = CaptureTool_getDriverDir(tools, row);
            CHECK((proc && !dir) || (!proc && dir));
        }
    }

    // --- live liveness (reported, never asserted for real tools) ---------------
    printf("isRunning(obs) = %s\n",
           CaptureTool_isRunning(tools, obs) ? "yes" : "no");
    printf("isRunning(quicktime) = %s\n",
           CaptureTool_isRunning(tools, quicktime) ? "yes" : "no");
    printf("countRunningAll = %u\n", CaptureTool_countRunningAll(tools));
    printf("countRunning(screen) = %u\n",
           CaptureTool_countRunning(tools, CAPTURE_KIND_SCREEN));
    // Contract truths: driver rows never "run"; counts never exceed rows.
    CHECK(!CaptureTool_isRunning(tools, blackhole));
    CHECK(CaptureTool_countRunningAll(tools) <= total);
    CHECK(CaptureTool_countRunning(tools, CAPTURE_KIND_SCREEN) <= total);
    // Self-truthy liveness via ProcessProbe on our own executable.
    const char *selfName = argc > 0 && argv[0] ? strrchr(argv[0], '/') : NULL;
    selfName = selfName ? selfName + 1 : (argc > 0 ? argv[0] : NULL);
    ProcessProbe *probe = ProcessProbe_shared();
    CHECK(selfName && ProcessProbe_isRunning(probe, selfName));
    CHECK(!ProcessProbe_isRunning(probe, "definitely-not-a-proc-xyzzy"));
    CHECK(!ProcessProbe_isDriverLoaded(probe,
                                       "/definitely/not/a/dir-xyzzy", "obs"));

    // --- null-safety (Rule 24) -------------------------------------------------
    CHECK(CaptureTool_count(NULL) == 0);
    CHECK(CaptureTool_at(NULL, 0) == NULL);
    CHECK(CaptureTool_get(NULL, NULL) == NULL);
    CHECK(CaptureTool_get(tools, NULL) == NULL);
    CHECK(CaptureTool_isRunning(NULL, NULL) == false);
    CHECK(CaptureTool_isInstalled(NULL, NULL) == false);
    CHECK(CaptureTool_countRunning(NULL, CAPTURE_KIND_SCREEN) == 0);
    CHECK(CaptureTool_countRunningAll(NULL) == 0);
    CHECK(CaptureTool_getSlug(NULL, NULL) == NULL);
    CHECK(CaptureTool_getDisplayName(NULL, NULL) == NULL);
    CHECK(CaptureTool_getProcKey(NULL, NULL) == NULL);
    CHECK(CaptureTool_getDriverDir(NULL, NULL) == NULL);
    CHECK(CaptureTool_getNote(NULL, NULL) == NULL);
    CHECK(CaptureTool_getKind(NULL, NULL) == CAPTURE_KIND_SCREEN);

    if (sFailures == 0) {
        printf("capture_tool_test: ALL CHECKS PASSED (%u capture tools)\n", total);
        return 0;
    }
    printf("capture_tool_test: %d FAILURES\n", sFailures);
    return 1;
}