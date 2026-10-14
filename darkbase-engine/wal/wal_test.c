// Owner: wal/wal.c — the write-ahead log: ordering, checksums, torn tails, recovery.
//
// The test writes a private log under TMPDIR and never touches a real store file.
// It proves the three things the log exists for: a record is recoverable after a
// clean sync, an incomplete TRAILING record is dropped rather than obeyed, and a
// complete record with a bad checksum is reported as corruption rather than skipped.
#include "wal/wal.h"
#include "util/crc32.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RECORDS 8u

static char path[512];
static uint32_t seenOps[RECORDS];
static uint64_t seenSeq[RECORDS];
static uint8_t seenPayload[RECORDS][64];
static uint32_t seenLength[RECORDS];
static uint32_t seenCount = 0u;

static bool collect(void *context, const WalRecord *record) {
    (void) context;
    assert(seenCount < RECORDS);
    seenOps[seenCount] = (*record).op;
    seenSeq[seenCount] = (*record).sequence;
    seenLength[seenCount] = (*record).length;
    if ((*record).length > 0u && (*record).length <= sizeof seenPayload[0])
        memcpy(seenPayload[seenCount], (*record).payload, (*record).length);
    ++seenCount;
    return true;
}

static uint32_t stopCalls = 0u;
static bool stopAfterOne(void *context, const WalRecord *record) {
    (void) context;
    assert((*record).sequence == 1u);   // it was handed the FIRST record, then stopped
    ++stopCalls;
    return false;   // the visitor may stop the walk; the log itself is still intact
}

static void resetSeen(void) { seenCount = 0u; memset(seenPayload, 0, sizeof seenPayload); }

static void makePath(void) {
    const char *dir = getenv("TMPDIR");
    (void) snprintf(path, sizeof path, "%s/re-wal-%d.log", dir != nullptr ? dir : "/tmp", (int) getpid());
    (void) unlink(path);
}

/* Overwrite bytes at an absolute offset without going through the log. */
static void poke(size_t offset, const uint8_t *bytes, size_t length) {
    FILE *f = fopen(path, "r+b");
    assert(f != nullptr);
    assert(fseek(f, (long) offset, SEEK_SET) == 0);
    assert(fwrite(bytes, 1, length, f) == length);
    assert(fclose(f) == 0);
}

/* Append raw bytes at the end, as a crash mid-append would leave them. */
static void appendRaw(const uint8_t *bytes, size_t length) {
    FILE *f = fopen(path, "ab");
    assert(f != nullptr);
    assert(fwrite(bytes, 1, length, f) == length);
    assert(fclose(f) == 0);
}

static long fileSize(void) {
    FILE *f = fopen(path, "rb");
    assert(f != nullptr);
    assert(fseek(f, 0, SEEK_END) == 0);
    long size = ftell(f);
    (void) fclose(f);
    return size;
}

int main(void) {
    /* An unopened owner is CLOSED on every operation; arguments are a different code. */
    Wal *closed = Wal_0();
    assert(closed != nullptr && !Wal_isOpen(closed));
    assert(Wal_append(closed, WAL_OP_INSERT, (const uint8_t*) "x", 1u, nullptr) == WAL_CLOSED);
    assert(Wal_replay(closed, nullptr, 0u, nullptr, nullptr) == WAL_CLOSED);
    assert(Wal_sync(closed) == WAL_CLOSED);
    assert(Wal_checkpoint(closed) == WAL_CLOSED);
    assert(Wal_sequence(closed) == 0u && Wal_count(closed) == 0u);
    Wal_free(closed);
    assert(Wal_1(nullptr) == nullptr);
    assert(!Wal_isOpen(nullptr) && Wal_sequence(nullptr) == 0u);
    Wal_free(nullptr);

    assert(strcmp(Wal_codeName(WAL_OK), "ok") == 0);
    assert(strcmp(Wal_codeName(WAL_INVALID), "invalid") == 0);
    assert(strcmp(Wal_codeName(WAL_IO), "io") == 0);
    assert(strcmp(Wal_codeName(WAL_CORRUPT), "corrupt") == 0);
    assert(strcmp(Wal_codeName(WAL_TRUNCATED), "truncated") == 0);
    assert(strcmp(Wal_codeName(WAL_CLOSED), "closed") == 0);
    assert(strcmp(Wal_codeName((WalCode) 77), "unknown") == 0);

    makePath();
    Wal *log = Wal_1(path);
    assert(log != nullptr && Wal_isOpen(log));
    assert(Wal_code(log) == WAL_OK);
    assert(strcmp(Wal_path(log), path) == 0);
    assert(Wal_sequence(log) == 0u && Wal_count(log) == 0u);
    assert(fileSize() == (long) WAL_HEADER_BYTES);   // a fresh log is header-only

    assert(Wal_append(log, WAL_OP_INSERT, (const uint8_t*) "alpha", 5u, nullptr) == WAL_OK);
    assert(Wal_append(log, WAL_OP_UPDATE, (const uint8_t*) "beta!", 5u, nullptr) == WAL_OK);
    uint64_t third = 0u;
    assert(Wal_append(log, WAL_OP_DELETE, (const uint8_t*) "gamma", 5u, &third) == WAL_OK);
    assert(third == 3u);
    assert(Wal_sequence(log) == 3u && Wal_count(log) == 3u);

    /* The durability boundary is also the point at which ANOTHER handle can see the
     * bytes: stdio buffers writes, so an external size measurement is only honest
     * after a flush. Sync first, then measure. */
    assert(Wal_sync(log) == WAL_OK);
    long afterThree = fileSize();
    assert(afterThree == (long) (WAL_HEADER_BYTES + 3u * (WAL_RECORD_FIXED + 5u + 4u)));

    /* Argument refusals never write; the file grows only for a legal record. */
    assert(Wal_append(log, 0u, (const uint8_t*) "x", 1u, nullptr) == WAL_INVALID);
    assert(Wal_append(log, 99u, (const uint8_t*) "x", 1u, nullptr) == WAL_INVALID);
    assert(Wal_append(log, WAL_OP_INSERT, nullptr, 4u, nullptr) == WAL_INVALID);
    assert(Wal_append(log, WAL_OP_INSERT, (const uint8_t*) "x", WAL_PAYLOAD_MAX + 1u, nullptr) == WAL_INVALID);
    assert(fileSize() == afterThree);         // every refusal left the log untouched
    assert(Wal_code(log) == WAL_INVALID);     // the last refusal is recorded
    assert(Wal_append(log, WAL_OP_INSERT, nullptr, 0u, nullptr) == WAL_OK);   // empty payload is legal

    assert(Wal_sync(log) == WAL_OK);
    assert(Wal_code(log) == WAL_OK);
    Wal_free(log);

    Wal *reopened = Wal_1(path);
    assert(reopened != nullptr && Wal_isOpen(reopened));
    assert(Wal_sequence(reopened) == 4u && Wal_count(reopened) == 4u);
    assert(!Wal_tailTorn(reopened));

    uint8_t scratch[64];
    resetSeen();
    assert(Wal_replay(reopened, scratch, sizeof scratch, collect, nullptr) == WAL_OK);
    assert(seenCount == 4u && Wal_recovered(reopened) == 4u);
    assert(seenOps[0] == WAL_OP_INSERT && seenSeq[0] == 1u && seenLength[0] == 5u);
    assert(memcmp(seenPayload[0], "alpha", 5u) == 0);
    assert(seenOps[1] == WAL_OP_UPDATE && seenSeq[1] == 2u && memcmp(seenPayload[1], "beta!", 5u) == 0);
    assert(seenOps[2] == WAL_OP_DELETE && seenSeq[2] == 3u && memcmp(seenPayload[2], "gamma", 5u) == 0);
    assert(seenLength[3] == 0u);

    stopCalls = 0u;
    assert(Wal_replay(reopened, scratch, sizeof scratch, stopAfterOne, nullptr) == WAL_OK);
    assert(stopCalls == 1u);              // stopped after the first, and that is not a failure

    assert(Wal_replay(reopened, nullptr, 0u, collect, nullptr) == WAL_INVALID);
    /* A record larger than the caller's buffer is refused, never truncated. */
    assert(Wal_append(reopened, WAL_OP_INSERT, (const uint8_t*) "0123456789", 10u, nullptr) == WAL_OK);
    uint8_t tiny[4];
    assert(Wal_replay(reopened, tiny, sizeof tiny, collect, nullptr) == WAL_INVALID);
    Wal_free(reopened);

    /* A TORN TAIL is dropped, not obeyed: a crash mid-append looks exactly like this. */
    static const uint8_t torn[7] = { 'V', 'E', 'X', 'W', 1u, 0u, 0u };   // a partial prefix
    appendRaw(torn, sizeof torn);
    Wal *tornLog = Wal_1(path);
    assert(tornLog != nullptr);
    assert(Wal_tailTorn(tornLog));
    assert(Wal_sequence(tornLog) == 5u && Wal_count(tornLog) == 5u);   // the real records only
    assert(Wal_code(tornLog) == WAL_TRUNCATED);
    resetSeen();
    assert(Wal_replay(tornLog, scratch, sizeof scratch, collect, nullptr) == WAL_TRUNCATED);
    assert(seenCount == 5u);
    Wal_free(tornLog);

    /* Cut the torn bytes away, then prove CHECKPOINT retires the log without ever
     * restarting the sequence. */
    assert(truncate(path, afterThree) == 0);
    Wal *clean = Wal_1(path);
    assert(clean != nullptr && !Wal_tailTorn(clean));
    assert(Wal_sequence(clean) == 3u && Wal_count(clean) == 3u);
    assert(Wal_sync(clean) == WAL_OK);
    assert(Wal_checkpoint(clean) == WAL_OK);
    assert(Wal_count(clean) == 0u && Wal_sequence(clean) == 3u && Wal_baseSequence(clean) == 3u);
    assert(fileSize() == (long) WAL_HEADER_BYTES);              // header-only again
    assert(Wal_append(clean, WAL_OP_INSERT, (const uint8_t*) "delta", 5u, &third) == WAL_OK);
    assert(third == 4u);                                        // the sequence NEVER restarts
    assert(Wal_sync(clean) == WAL_OK);
    Wal_free(clean);

    Wal *afterCheckpoint = Wal_1(path);
    assert(afterCheckpoint != nullptr);
    assert(Wal_baseSequence(afterCheckpoint) == 3u && Wal_sequence(afterCheckpoint) == 4u);
    assert(Wal_count(afterCheckpoint) == 1u);
    resetSeen();
    assert(Wal_replay(afterCheckpoint, scratch, sizeof scratch, collect, nullptr) == WAL_OK);
    assert(seenCount == 1u && seenSeq[0] == 4u && memcmp(seenPayload[0], "delta", 5u) == 0);
    Wal_free(afterCheckpoint);

    /* A COMPLETE record with a bad checksum is CORRUPTION, never a skip: a hole in a
     * log destroys the order the log exists to guarantee. */
    assert(fileSize() > (long) (WAL_HEADER_BYTES + WAL_RECORD_FIXED));
    uint8_t flipped[1];
    {
        FILE *f = fopen(path, "rb");
        assert(f != nullptr);
        assert(fseek(f, (long) (WAL_HEADER_BYTES + WAL_RECORD_FIXED), SEEK_SET) == 0);
        assert(fread(flipped, 1, 1, f) == 1u);
        (void) fclose(f);
    }
    flipped[0] ^= 0x5Au;
    poke(WAL_HEADER_BYTES + WAL_RECORD_FIXED, flipped, 1u);

    Wal *corrupt = Wal_1(path);
    assert(corrupt != nullptr);
    assert(Wal_code(corrupt) == WAL_CORRUPT);
    assert(Wal_replay(corrupt, scratch, sizeof scratch, collect, nullptr) == WAL_CORRUPT);
    Wal_free(corrupt);

    /* A corrupt HEADER cannot be used either. */
    static const uint8_t badMagic[4] = { 0u, 0u, 0u, 0u };
    poke(0u, badMagic, sizeof badMagic);
    Wal *badHeader = Wal_1(path);
    assert(badHeader != nullptr);
    assert(Wal_code(badHeader) == WAL_CORRUPT);
    Wal_free(badHeader);

    /* Projections are bounded and report truncation rather than cutting silently. */
    makePath();
    Wal *final = Wal_1(path);
    assert(final != nullptr);
    char text[96];
    bool truncated = true;
    assert(Wal_toString(final, text, sizeof text, &truncated));
    assert(!truncated && strstr(text, "Wal(") != nullptr);
    char structText[256];   // the struct dump includes the (long) temp path
    assert(Wal_toStringStruct(final, structText, sizeof structText, &truncated));
    assert(!truncated && strstr(structText, "path=") != nullptr);
    assert(strstr(structText, "baseSequence=") != nullptr && strstr(structText, "tailTorn=") != nullptr);
    char tinyText[6];
    truncated = false;
    assert(!Wal_toString(final, tinyText, sizeof tinyText, &truncated));
    assert(truncated);
    assert(!Wal_toStringStruct(final, nullptr, 0u, nullptr));
    Wal_free(final);
    assert(unlink(path) == 0);

    printf("wal_test: PASS\n");
    return 0;
}
