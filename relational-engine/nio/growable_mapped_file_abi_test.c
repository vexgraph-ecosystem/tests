/* C client of the Rust reservation-backed growing mapping ABI, through the
 * canonical header nio/growable_mapped_file.h. Proves the whole handshake from C,
 * and above all the property the type exists for: a borrowed pointer taken before
 * a growth step is the SAME pointer after it, because the mapping is never remade.
 * Status 8 (no reservation backend on this host) is documented but cannot be
 * exercised here: this host has a backend, so create succeeds.
 * Not a Hotcwap loader test and not a second mapping implementation. */
#define _POSIX_C_SOURCE 200809L
#include "nio/growable_mapped_file.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char path[512];

/* A path that does NOT exist yet: mkstemp reserves a unique name, then we remove it. */
static void reservePath(void) {
    const char *dir = getenv("TMPDIR");
    (void) snprintf(path, sizeof path, "%s/re-gmap-XXXXXX", dir != nullptr ? dir : "/tmp");
    int fd = mkstemp(path);
    assert(fd >= 0);
    assert(close(fd) == 0);
    assert(unlink(path) == 0);
}

int main(void) {
    ReGrowableMappedFile *owner = nullptr;
    size_t len = 0u, reserved = 0u;
    uint64_t allocated = 0u;
    bool flag = false;
    uint8_t dest[64];
    bool truncated = true;

    /* Argument admission happens before anything reaches the OS. */
    assert(re_gmap_create(nullptr, 4096u, 0u, &owner) == 1u);
    assert(re_gmap_create("/tmp/x", 4096u, 0u, nullptr) == 1u);
    assert(re_gmap_len(nullptr, &len) == 1u);
    assert(re_gmap_reserved(nullptr, &reserved) == 1u);
    assert(re_gmap_is_open(nullptr, &flag) == 1u);
    assert(re_gmap_allocated_bytes(nullptr, &allocated) == 1u);
    assert(re_gmap_read(nullptr, 0u, 1u, dest, sizeof dest, &truncated) == 1u);
    assert(re_gmap_write(nullptr, 0u, dest, 1u) == 1u);
    assert(re_gmap_borrow(nullptr, nullptr, &len) == 1u);
    assert(re_gmap_grow_to(nullptr, 1u) == 1u);
    assert(re_gmap_flush(nullptr) == 1u);
    assert(re_gmap_sync(nullptr) == 1u);
    assert(re_gmap_close(nullptr) == 1u);
    assert(re_gmap_to_string(nullptr, (char*) dest, sizeof dest, &truncated) == 1u);
    re_gmap_drop(nullptr);   /* quiet no-op */

    /* An existing destination is refused, never clobbered. */
    reservePath();
    int fd = open(path, O_CREAT | O_WRONLY, 0600);
    assert(fd >= 0 && close(fd) == 0);
    assert(re_gmap_create(path, 4096u, 0u, &owner) == 2u);
    uint8_t untouched[8];
    int check = open(path, O_RDONLY);
    assert(check >= 0);
    assert(read(check, untouched, sizeof untouched) == 0);   /* still empty, not truncated */
    assert(close(check) == 0);
    assert(unlink(path) == 0);

    /* Geometry admission: committed past the reservation, and a zero reservation. */
    reservePath();
    assert(re_gmap_create(path, 64u, 65u, &owner) == 3u);
    assert(re_gmap_create(path, 0u, 0u, &owner) == 3u);

    /* Create the reservation and grow inside it. */
    assert(re_gmap_create(path, 4096u, 512u, &owner) == 0u);
    assert(re_gmap_is_open(owner, &flag) == 0u && flag);
    assert(re_gmap_len(owner, &len) == 0u && len == 512u);
    assert(re_gmap_reserved(owner, &reserved) == 0u && reserved == 4096u);
    assert(re_gmap_allocated_bytes(owner, &allocated) == 0u && allocated >= 512u);

    uint8_t payload[64];
    for (size_t i = 0; i < sizeof payload; ++i)
        payload[i] = (uint8_t) i;
    assert(re_gmap_write(owner, 0u, payload, sizeof payload) == 0u);
    assert(re_gmap_read(owner, 0u, sizeof payload, dest, sizeof dest, &truncated) == 0u);
    assert(memcmp(dest, payload, sizeof payload) == 0);

    /* Past the COMMITTED length is refused, reservation or not. */
    assert(re_gmap_write(owner, 512u, payload, 1u) == 3u);
    assert(re_gmap_read(owner, 511u, 2u, dest, sizeof dest, &truncated) == 3u);
    assert(re_gmap_read(owner, (size_t) -1, 2u, dest, sizeof dest, &truncated) == 3u);

    /* A destination too small refuses with 7 and leaves it untouched. */
    uint8_t small[4];
    memset(small, 0x5a, sizeof small);
    assert(re_gmap_read(owner, 0u, sizeof payload, small, sizeof small, &truncated) == 7u);
    assert(truncated);
    for (size_t i = 0; i < sizeof small; ++i)
        assert(small[i] == 0x5a);

    /* THE INVARIANT, from C: growth allocates and publishes, and never remaps. */
    const uint8_t *before = nullptr;
    size_t beforeLen = 0u;
    assert(re_gmap_borrow(owner, &before, &beforeLen) == 0u);
    assert(before != nullptr && beforeLen == 512u);

    assert(re_gmap_grow_to(owner, 2048u) == 0u);
    assert(re_gmap_len(owner, &len) == 0u && len == 2048u);
    assert(re_gmap_allocated_bytes(owner, &allocated) == 0u && allocated >= 2048u);

    const uint8_t *after = nullptr;
    size_t afterLen = 0u;
    assert(re_gmap_borrow(owner, &after, &afterLen) == 0u);
    assert(after == before);                      /* the mapping did not move */
    assert(afterLen == 2048u);
    assert(memcmp(after, payload, sizeof payload) == 0);   /* bytes survived growth */

    /* The new region is usable, and growth is bounded and never backwards. */
    assert(re_gmap_write(owner, 2000u, (const uint8_t*) "xyz", 3u) == 0u);
    assert(re_gmap_read(owner, 2000u, 3u, dest, sizeof dest, &truncated) == 0u);
    assert(memcmp(dest, "xyz", 3u) == 0);
    assert(re_gmap_grow_to(owner, 4096u) == 0u);
    assert(re_gmap_grow_to(owner, 4097u) == 3u);   /* past the reservation */
    assert(re_gmap_grow_to(owner, 100u) == 3u);    /* backwards */
    assert(re_gmap_grow_to(owner, 4096u) == 0u);   /* same length is a no-op */

    /* The bounded projection, and its truncation refusal. */
    char text[96];
    assert(re_gmap_to_string(owner, text, sizeof text, &truncated) == 0u && !truncated);
    assert(strstr(text, "GrowableMappedFile") != nullptr);
    char tiny[6];
    assert(re_gmap_to_string(owner, tiny, sizeof tiny, &truncated) == 7u && truncated);

    assert(re_gmap_flush(owner) == 0u);
    assert(re_gmap_sync(owner) == 0u);

    /* Close is not drop: the owner lives on and reports itself closed. */
    assert(re_gmap_close(owner) == 0u);
    assert(re_gmap_is_open(owner, &flag) == 0u && !flag);
    assert(re_gmap_len(owner, &len) == 0u && len == 0u);
    assert(re_gmap_read(owner, 0u, 1u, dest, sizeof dest, &truncated) == 5u);
    assert(re_gmap_borrow(owner, &after, &afterLen) == 5u);
    assert(re_gmap_grow_to(owner, 1u) == 5u);
    assert(re_gmap_flush(owner) == 5u);
    assert(re_gmap_allocated_bytes(owner, &allocated) == 5u);
    assert(re_gmap_close(owner) == 0u);   /* repeat safely */
    re_gmap_drop(owner);

    /* The file really grew, and holds the bytes we wrote through the mapping. */
    fd = open(path, O_RDONLY);
    assert(fd >= 0);
    uint8_t disk[64];
    assert((size_t) read(fd, disk, sizeof disk) == sizeof disk);
    assert(memcmp(disk, payload, sizeof payload) == 0);
    assert(close(fd) == 0);
    assert(unlink(path) == 0);

    /* An empty committed region is a valid OPEN owner, and grows on demand. */
    reservePath();
    assert(re_gmap_create(path, 256u, 0u, &owner) == 0u);
    assert(re_gmap_is_open(owner, &flag) == 0u && flag);
    assert(re_gmap_len(owner, &len) == 0u && len == 0u);
    assert(re_gmap_read(owner, 0u, 1u, dest, sizeof dest, &truncated) == 3u);
    assert(re_gmap_grow_to(owner, 64u) == 0u);
    assert(re_gmap_write(owner, 63u, (const uint8_t*) "!", 1u) == 0u);
    assert(re_gmap_read(owner, 63u, 1u, dest, sizeof dest, &truncated) == 0u);
    assert(dest[0] == '!');
    re_gmap_drop(owner);
    assert(unlink(path) == 0);

    printf("growable_mapped_file_abi_test: PASS\n");
    return 0;
}
