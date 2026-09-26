#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "hot/throwable.h"

static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if (cond) { \
        printf("PASS: %s\n", msg); \
    } else { \
        printf("FAIL: %s (line %d)\n", msg, __LINE__); \
        g_failures++; \
    } \
} while (0)

static int g_teardownCallCount = 0;
static void testTeardownCallback(void) {
    g_teardownCallCount++;
}

#define BULK_TEARDOWN_COUNT 64
static int g_bulkCount = 0;
#define DEFINE_DUMMY_TEARDOWN(N) \
    static void dummyTeardown_##N(void) { g_bulkCount++; }

DEFINE_DUMMY_TEARDOWN(0) DEFINE_DUMMY_TEARDOWN(1) DEFINE_DUMMY_TEARDOWN(2) DEFINE_DUMMY_TEARDOWN(3)
DEFINE_DUMMY_TEARDOWN(4) DEFINE_DUMMY_TEARDOWN(5) DEFINE_DUMMY_TEARDOWN(6) DEFINE_DUMMY_TEARDOWN(7)
DEFINE_DUMMY_TEARDOWN(8) DEFINE_DUMMY_TEARDOWN(9) DEFINE_DUMMY_TEARDOWN(10) DEFINE_DUMMY_TEARDOWN(11)
DEFINE_DUMMY_TEARDOWN(12) DEFINE_DUMMY_TEARDOWN(13) DEFINE_DUMMY_TEARDOWN(14) DEFINE_DUMMY_TEARDOWN(15)
DEFINE_DUMMY_TEARDOWN(16) DEFINE_DUMMY_TEARDOWN(17) DEFINE_DUMMY_TEARDOWN(18) DEFINE_DUMMY_TEARDOWN(19)
DEFINE_DUMMY_TEARDOWN(20) DEFINE_DUMMY_TEARDOWN(21) DEFINE_DUMMY_TEARDOWN(22) DEFINE_DUMMY_TEARDOWN(23)
DEFINE_DUMMY_TEARDOWN(24) DEFINE_DUMMY_TEARDOWN(25) DEFINE_DUMMY_TEARDOWN(26) DEFINE_DUMMY_TEARDOWN(27)
DEFINE_DUMMY_TEARDOWN(28) DEFINE_DUMMY_TEARDOWN(29) DEFINE_DUMMY_TEARDOWN(30) DEFINE_DUMMY_TEARDOWN(31)

int main(void) {
    printf("=== Running Throwable & Exception Handling Test Suite ===\n");

    // Test 1: TRY with true condition succeeds without error
    bool successResult = TRY(1 + 1 == 2, "Math broken", "ThrowableTest::testMath");
    CHECK(successResult == true, "TRY returns true when condition is met");

    // Test 2: TRY with false condition gives grace, prints warning, and returns false
    printf("Expect graceful warning below:\n");
    bool failResult = TRY(1 + 1 == 3, "Arithmetic mismatch", "ThrowableTest::testMath", "expected=%d, got=%d", 2, 3);
    CHECK(failResult == false, "TRY returns false when condition is not met");

    // Test 3: TRY_EX with custom category
    bool catResult = TRY_EX(2 > 5, EXCEPTION_WINDOW, "Window size violation", "Window::setSize", "requestedWidth=%d", -100);
    CHECK(catResult == false, "TRY_EX returns false on condition failure");

    // Test 4: Dynamic scalability of teardowns (Preference 47: No Hardcode Law)
    // Register 32 separate unique teardown functions, far exceeding any static 16 ceiling
    ThrowableTeardownFn bulkFns[] = {
        dummyTeardown_0, dummyTeardown_1, dummyTeardown_2, dummyTeardown_3,
        dummyTeardown_4, dummyTeardown_5, dummyTeardown_6, dummyTeardown_7,
        dummyTeardown_8, dummyTeardown_9, dummyTeardown_10, dummyTeardown_11,
        dummyTeardown_12, dummyTeardown_13, dummyTeardown_14, dummyTeardown_15,
        dummyTeardown_16, dummyTeardown_17, dummyTeardown_18, dummyTeardown_19,
        dummyTeardown_20, dummyTeardown_21, dummyTeardown_22, dummyTeardown_23,
        dummyTeardown_24, dummyTeardown_25, dummyTeardown_26, dummyTeardown_27,
        dummyTeardown_28, dummyTeardown_29, dummyTeardown_30, dummyTeardown_31,
    };
    for (size_t i = 0; i < sizeof(bulkFns)/sizeof(bulkFns[0]); i++) {
        Throwable_registerTeardown(bulkFns[i]);
    }
    // Test unregistering one
    Throwable_unregisterTeardown(dummyTeardown_0);
    Throwable_runTeardown();
    CHECK(g_bulkCount == 31, "Dynamic teardown scaled beyond 16 and executed all registered hooks (31 executed, 1 unregistered)");

    // Test 5: Re-register single test teardown callback
    Throwable_registerTeardown(testTeardownCallback);
    Throwable_runTeardown();
    CHECK(g_teardownCallCount == 1, "Teardown callback executed exactly once");

    // Test 6: Dynamic size string allocation (no artificial 1024 limit)
    char largeMsg[5000];
    memset(largeMsg, 'A', sizeof(largeMsg) - 1);
    largeMsg[sizeof(largeMsg) - 1] = '\0';
    Exception bigEx;
    Exception_init(&bigEx, EXCEPTION_RUNTIME, "BigTest", __FILE__, __LINE__, "%s", largeMsg);
    CHECK(bigEx.message != NULL && strlen(bigEx.message) == sizeof(largeMsg) - 1, "Dynamic Exception message scaled to 5000 bytes without truncation");
    Exception_free(&bigEx);

    // Test 7: Graphics-category Exception — the category contract lives in R2
    // (exception/exception.h, EXCEPTION_GRAPHICS). The retired graphvex
    // graphics_exception module died with the old API; the language returns
    // bools (the Cold-Strict, Hot-Minimal Validation Law) and the category
    // records the failure family.
    Exception gfxEx;
    Exception_init(&gfxEx, EXCEPTION_GRAPHICS, "Pipeline::create", __FILE__, __LINE__,
                   "Failed to compile pipeline shader");
    CHECK(gfxEx.category == EXCEPTION_GRAPHICS, "Graphics-category Exception records the failure family");
    CHECK(gfxEx.message != NULL && strstr(gfxEx.message, "pipeline") != NULL, "Exception message formatted");
    Exception_free(&gfxEx);

    // Test 8: THROW in isolated child process terminates with code 1 after running teardowns
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("[Child] Executing THROW to test runtime termination banner & emergency teardown...\n");
        THROW("Intentional fatal exception for test", "ThrowableTest::childTest", "dummyVar=%d", 12345);
        exit(0); // Should never be reached
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        CHECK(WIFEXITED(status), "Child process exited normally (not via unhandled signal or segfault)");
        CHECK(WEXITSTATUS(status) == 1, "Child process exited with status code 1 as specified by THROW()");
    } else {
        CHECK(false, "Failed to fork process for THROW test");
    }

    if (g_failures == 0) {
        printf("\nALL THROWABLE TESTS PASSED (0 failures)\n");
        return 0;
    } else {
        printf("\nTHROWABLE TESTS COMPLETED WITH %d FAILURES\n", g_failures);
        return 1;
    }
}
