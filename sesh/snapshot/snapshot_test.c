/* SeshSnapshot owner: a user explicitly saves bytes, survives transient failure,
 * and retrieves identical content through the API Haven provider vocabulary.
 * All public forms/getters, cold rejection preservation, empty/exact/overflow,
 * retry timing/exhaustion, pending/cancel and string boundaries are exercised.
 * External runner supplies watchdog + ASan/UBSan. Provider is an in-memory fake,
 * not Google or durable disk. Thread safety, stale pointer admission, schema
 * migration and concurrent merge are not offered. Storage is caller-owned.
 */
#include "snapshot/snapshot.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fake {
    uint8_t bytes[256];
    size_t length;
    uint64_t key;
    unsigned calls;
    unsigned cancels;
    int result;
} Fake;

/* Fake provider stores completed snapshots and records cancellation requests. */
static int put(void *context, uint64_t key, const uint8_t *bytes, size_t length, int cancel) {
    Fake *fake = context;
    if (cancel) {
        (*fake).cancels++;
        return HAVEN_SNAPSHOT_REJECT;
    }
    (*fake).calls++;
    assert(length <= sizeof((*fake).bytes));
    if ((*fake).result == HAVEN_SNAPSHOT_DONE) {
        memcpy((*fake).bytes, bytes, length);
        (*fake).length = length;
        (*fake).key = key;
    }
    return (*fake).result;
}

/* Copies a stored snapshot into the caller's buffer when its key and capacity are valid. */
static int get(void *context, uint64_t key, uint8_t *dest, size_t capacity, size_t *outLength) {
    Fake *fake = context;
    if (key != (*fake).key || (*fake).length > capacity)
        return HAVEN_SNAPSHOT_REJECT;
    memcpy(dest, (*fake).bytes, (*fake).length);
    *outLength = (*fake).length;
    return HAVEN_SNAPSHOT_DONE;
}

/* Covers public configuration, fixture round-trip, retry timing, cancellation, boundaries, and projections. */
static void normal(const char *fixture) {
    SeshSnapshot job = SeshSnapshot();
    SeshSnapshot direct = SeshSnapshot_0();
    SeshSnapshot zero = SeshSnapshot_zero();
    assert(direct.state == zero.state && zero.state == SESH_SNAPSHOT_IDLE);
    uint8_t staging[256];
    Fake fake = {0};
    assert(SeshSnapshot_setBuffer(&job, staging, sizeof(staging)));
    assert(SeshSnapshot_setProvider(&job, put, &fake));
    assert(SeshSnapshot_setRetryPolicy(&job, 10, 2));
    assert(SeshSnapshot_getBuffer(&job) == staging);
    assert(SeshSnapshot_getCapacity(&job) == sizeof(staging));
    assert(SeshSnapshot_getProvider(&job) == put);
    assert(SeshSnapshot_getContext(&job) == &fake);
    assert(SeshSnapshot_getRetryDelayMs(&job) == 10);
    assert(SeshSnapshot_getMaxAttempts(&job) == 2);
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_IDLE);

    FILE *file = fopen(fixture, "rb");
    assert(file != nullptr);
    uint8_t input[256];
    size_t length = fread(input, 1, sizeof(input), file);
    assert(!ferror(file) && length != 0 && length < sizeof(input));
    assert(fgetc(file) == EOF);
    assert(fclose(file) == 0);
    assert(SeshSnapshot_queue(&job, 1, input, length));
    memset(input, 0, length); /* source no longer borrowed */
    fake.result = HAVEN_SNAPSHOT_PENDING;
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_QUEUED);
    fake.result = HAVEN_SNAPSHOT_RETRY;
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_QUEUED);
    assert(SeshSnapshot_getAttempts(&job) == 1);
    assert(SeshSnapshot_getNextAttemptMs(&job) == 10);
    unsigned calls = fake.calls;
    assert(SeshSnapshot_step(&job, 9) == SESH_SNAPSHOT_QUEUED && fake.calls == calls);
    fake.result = HAVEN_SNAPSHOT_DONE;
    assert(SeshSnapshot_step(&job, 10) == SESH_SNAPSHOT_SAVED);
    assert(SeshSnapshot_getKey(&job) == 1 && SeshSnapshot_getLength(&job) == length);
    assert(SeshSnapshot_getState(&job) == SESH_SNAPSHOT_SAVED);
    HavenSnapshotGetFn download = get;
    size_t restored = 0;
    assert(download(&fake, 1, input, sizeof(input), &restored) == HAVEN_SNAPSHOT_DONE);
    assert(restored == length && memcmp(input, staging, length) == 0);
    assert(download(&fake, 1, input, length - 1, &restored) == HAVEN_SNAPSHOT_REJECT);
    assert(restored == length && memcmp(input, staging, length) == 0);
    calls = fake.calls;
    SeshSnapshot_cancel(&job);
    assert(SeshSnapshot_step(&job, 11) == SESH_SNAPSHOT_SAVED && fake.calls == calls);
    assert(SeshSnapshot_queue(&job, 2, nullptr, 0));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_SAVED && fake.length == 0);
    uint8_t one = 0;
    assert(SeshSnapshot_queue(&job, UINT64_MAX, &one, 1));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_SAVED);
    assert(fake.key == UINT64_MAX && fake.length == 1 && fake.bytes[0] == 0);
    memset(input, 0xA5, sizeof(input));
    assert(SeshSnapshot_queue(&job, 3, input, sizeof(input)));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_SAVED);
    assert(SeshSnapshot_queue(&job, 4, staging + 1, sizeof(staging) - 1));
    fake.result = HAVEN_SNAPSHOT_PENDING;
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_QUEUED);
    SeshSnapshot_cancel(&job);
    SeshSnapshot_cancel(&job);
    SeshSnapshot_cancel(nullptr);
    assert(fake.cancels == 1 && SeshSnapshot_getState(&job) == SESH_SNAPSHOT_CANCELLED);

    assert(SeshSnapshot_getBuffer(nullptr) == nullptr);
    assert(SeshSnapshot_getCapacity(nullptr) == 0 && SeshSnapshot_getLength(nullptr) == 0);
    assert(SeshSnapshot_getProvider(nullptr) == nullptr && SeshSnapshot_getContext(nullptr) == nullptr);
    assert(SeshSnapshot_getKey(nullptr) == 0 && SeshSnapshot_getNextAttemptMs(nullptr) == 0);
    assert(SeshSnapshot_getRetryDelayMs(nullptr) == 0 && SeshSnapshot_getAttempts(nullptr) == 0);
    assert(SeshSnapshot_getMaxAttempts(nullptr) == 0 && SeshSnapshot_getState(nullptr) == SESH_SNAPSHOT_IDLE);
    char text[512];
    bool truncated = true;
    assert(SeshSnapshot_toString(&job, text, sizeof(text), &truncated) && !truncated);
    assert(strstr(text, "snapshot 4") != nullptr);
    assert(SeshSnapshot_toStringStruct(&job, text, sizeof(text), nullptr));
    const char *fields[] = {"buffer=", "capacity=", "length=", "put=", "context=", "key=", "nextAttemptMs=", "retryDelayMs=", "attempts=", "maxAttempts=", "state="};
    const char *cursor = text;
    for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
        cursor = strstr(cursor, fields[i]);
        assert(cursor != nullptr);
        cursor += strlen(fields[i]);
    }
    assert(SeshSnapshot_toString(nullptr, text, 8, &truncated) && strcmp(text, "nullptr") == 0);
    assert(SeshSnapshot_toStringStruct(nullptr, text, 8, nullptr));
    puts("PASS: fixture round trip, pending/retry/cancel, boundaries and public projections");
}

/* Checks rejected admissions preserve state, report failures, and permit later valid recovery. */
static void invalid(void) {
    SeshSnapshot job = SeshSnapshot();
    uint8_t bytes[2] = {1, 2};
    Fake fake = { .result = HAVEN_SNAPSHOT_RETRY };
    assert(!SeshSnapshot_setBuffer(nullptr, bytes, 2));
    assert(!SeshSnapshot_setBuffer(&job, nullptr, 2));
    assert(!SeshSnapshot_setBuffer(&job, bytes, 0));
    assert(!SeshSnapshot_setProvider(nullptr, put, &fake));
    assert(!SeshSnapshot_setProvider(&job, nullptr, &fake));
    assert(!SeshSnapshot_setRetryPolicy(nullptr, 1, 1));
    assert(!SeshSnapshot_setRetryPolicy(&job, 0, 1));
    assert(!SeshSnapshot_setRetryPolicy(&job, 1, 0));
    assert(!SeshSnapshot_queue(nullptr, 1, bytes, 2));
    assert(!SeshSnapshot_queue(&job, 0, bytes, 2));
    assert(!SeshSnapshot_queue(&job, 1, nullptr, 2));
    assert(!SeshSnapshot_queue(&job, 1, bytes, 2));
    assert(SeshSnapshot_setBuffer(&job, bytes, 2));
    assert(SeshSnapshot_setProvider(&job, put, &fake));
    SeshSnapshot before = job;
    assert(!SeshSnapshot_queue(&job, 1, bytes, SIZE_MAX));
    assert(memcmp(&before, &job, sizeof(job)) == 0);
    assert(!SeshSnapshot_queue(&job, 1, bytes, 3));
    assert(memcmp(&before, &job, sizeof(job)) == 0 && bytes[0] == 1);
    assert(SeshSnapshot_queue(&job, 1, bytes, 2));
    before = job;
    assert(!SeshSnapshot_queue(&job, 2, bytes, 2));
    assert(!SeshSnapshot_setBuffer(&job, bytes, 2));
    assert(!SeshSnapshot_setProvider(&job, put, &fake));
    assert(!SeshSnapshot_setRetryPolicy(&job, 1, 1));
    assert(memcmp(&before, &job, sizeof(job)) == 0);
    assert(SeshSnapshot_step(nullptr, 0) == SESH_SNAPSHOT_FAILED);
    assert(SeshSnapshot_step(&job, UINT64_MAX) == SESH_SNAPSHOT_FAILED);
    assert(fake.cancels == 1);
    assert(SeshSnapshot_setRetryPolicy(&job, 1, 1));
    assert(SeshSnapshot_queue(&job, 2, bytes, 2));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_FAILED && fake.cancels == 2);
    fake.result = HAVEN_SNAPSHOT_REJECT;
    assert(SeshSnapshot_queue(&job, 3, bytes, 2));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_FAILED);
    fake.result = 99;
    assert(SeshSnapshot_queue(&job, 4, bytes, 2));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_FAILED);
    fake.result = HAVEN_SNAPSHOT_DONE;
    assert(SeshSnapshot_queue(&job, 5, bytes, 2));
    assert(SeshSnapshot_step(&job, 0) == SESH_SNAPSHOT_SAVED);
    char text[8];
    bool truncated = false;
    assert(!SeshSnapshot_toString(nullptr, nullptr, 8, &truncated) && truncated);
    assert(!SeshSnapshot_toString(nullptr, text, 0, &truncated) && truncated);
    assert(!SeshSnapshot_toString(nullptr, text, 7, &truncated) && truncated);
    assert(!SeshSnapshot_toStringStruct(&job, text, 1, nullptr) && text[0] == '\0');
    puts("PASS: 27 loud cold rejections; preserved admission and recovery");
}

/* Selects the normal snapshot contract suite or its cold-rejection suite. */
int main(int argc, char **argv) {
    assert(argc == 2);
    if (strcmp(argv[1], "invalid") == 0)
        invalid();
    else
        normal(argv[1]);
    return 0;
}
