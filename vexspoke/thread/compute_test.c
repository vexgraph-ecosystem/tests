// tests/vexspoke/thread/compute_test.c — owner test for thread/compute.
//
// Proves the heavy-work job pool:
//   - ComputeJob_isJob recognizes the tag on a real job and rejects nullptr and
//     a foreign packet; ComputeJob_run executes run then onDone (onDone
//     optional) and refuses non-jobs;
//   - the pool is lazily started, count-clamped (0 -> 1, >8 -> 8), and
//     idempotent on restart;
//   - round-robin submit runs every submitted job exactly once (bounded wait);
//   - submitTo a specific established worker runs the job;
//   - null submits are refused; after stop/free the pool reports zero and
//     refuses submissions.

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "thread/compute.h"
#include "thread/thread.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void wait_ms(long ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, nullptr);
}

static atomic_int g_run_count;
static atomic_int g_done_count;

static void on_run(void *ctx) {
    (void) ctx;
    atomic_fetch_add(&g_run_count, 1);
}

static void on_done(void *ctx) {
    (void) ctx;
    atomic_fetch_add(&g_done_count, 1);
}

static ComputeJob make_job(void) {
    ComputeJob j;
    j.tag = COMPUTE_JOB_TAG;
    j.run = on_run;
    j.ctx = nullptr;
    j.onDone = on_done;
    return j;
}

static void test_recognize(void) {
    ComputeJob job = make_job();
    CHECK(ComputeJob_isJob(&job));
    CHECK(!ComputeJob_isJob(nullptr));

    struct Foreign { uint64_t tag; } foreign = { 0xdeadbeef };
    CHECK(!ComputeJob_isJob(&foreign));

    CHECK(!ComputeJob_run(&foreign));
    CHECK(!ComputeJob_run(nullptr));

    g_run_count = 0;
    g_done_count = 0;
    CHECK(ComputeJob_run(&job));
    CHECK(atomic_load(&g_run_count) == 1);
    CHECK(atomic_load(&g_done_count) == 1);

    // onDone is optional.
    ComputeJob only = make_job();
    only.onDone = nullptr;
    CHECK(ComputeJob_run(&only));
    CHECK(atomic_load(&g_run_count) == 2);
    CHECK(atomic_load(&g_done_count) == 1);
}

static bool wait_for(atomic_int *count, int target) {
    for (int i = 0; i < 3000; i++) {          // bounded <= ~3s
        if (atomic_load(count) >= target)
            return true;
        wait_ms(1);
    }
    return false;
}

static void test_pool(void) {
    ComputeThread_free();                       // reset any prior pool state
    CHECK(ComputeThread_count() == 0);
    CHECK(!ComputeThread_submit(nullptr));      // no pool -> refused

    CHECK(ComputeThread_start(0));              // clamped to 1
    CHECK(ComputeThread_count() == 1);
    CHECK(ComputeThread_start(3));              // grow to 3
    CHECK(ComputeThread_count() == 3);
    CHECK(ComputeThread_start(3));              // idempotent
    CHECK(ComputeThread_count() == 3);
    CHECK(ComputeThread_start(100));            // clamped to 8
    CHECK(ComputeThread_count() == 8);

    enum { N = 16 };
    ComputeJob jobs[N];
    for (int i = 0; i < N; i++)
        jobs[i] = make_job();

    g_run_count = 0;
    g_done_count = 0;
    int submitted = 0;
    for (int i = 0; i < N; i++) {
        for (int tries = 0; tries < 1000; tries++) {
            if (ComputeThread_submit(&jobs[i])) {
                submitted++;
                break;
            }
            wait_ms(1);
        }
    }
    CHECK(submitted == N);
    CHECK(wait_for(&g_done_count, N));
    CHECK(atomic_load(&g_run_count) == N);      // each job ran exactly once

    CHECK(!ComputeThread_submit(nullptr));
    CHECK(ComputeThread_submitTo(nullptr, &jobs[0]) == false);

    // Submit to a specific private worker.
    Thread *worker = ComputeThread_invoke();
    CHECK(worker != nullptr);
    CHECK(ComputeThread_submitTo(worker, nullptr) == false);   // null job refused
    g_run_count = 0;
    g_done_count = 0;
    CHECK(ComputeThread_submitTo(worker, &jobs[0]));
    CHECK(wait_for(&g_done_count, 1));
    CHECK(atomic_load(&g_run_count) == 1);
    Thread_stop(worker);
    Thread_free(worker);

    ComputeThread_stop();
    ComputeThread_free();
    CHECK(ComputeThread_count() == 0);
    CHECK(!ComputeThread_submit(&jobs[1]));     // pool gone
}

int main(void) {
    test_recognize();
    test_pool();

    if (g_failures == 0) {
        printf("compute_test: all assertions held\n");
        return 0;
    }
    printf("compute_test: %d FAILURES\n", g_failures);
    return 1;
}
