// Owner: page/page.c — the page: layout, seals, capacity, and the corruption matrix.
//
// The point of this file is the last third: a page whose bytes changed in ANY of the
// sealed fields must fail verification, including the page's own ID and the store's
// page size. That is what makes a misplaced or mis-sized page detectable instead of
// silently plausible. Storage is a stack buffer; the page owns nothing.
#include "page/page.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define SIZE PAGE_SIZE_MIN          /* 512: plenty, and fast */
static uint8_t storage[SIZE];

/* The raw header offsets the format defines; asserted as bytes, not through getters. */
static uint32_t rawU32(size_t offset) {
    return (uint32_t) storage[offset] | ((uint32_t) storage[offset + 1] << 8) |
           ((uint32_t) storage[offset + 2] << 16) | ((uint32_t) storage[offset + 3] << 24);
}

int main(void) {
    /* Legal page sizes are powers of two inside the advertised range. */
    assert(Page_sizeValid(PAGE_SIZE_MIN) && Page_sizeValid(PAGE_SIZE_DEFAULT));
    assert(Page_sizeValid(1024u) && Page_sizeValid(PAGE_SIZE_MAX));
    assert(!Page_sizeValid(0u));
    assert(!Page_sizeValid(256u));          // below the floor
    assert(!Page_sizeValid(PAGE_SIZE_MAX + 1u));
    assert(!Page_sizeValid(300u));          // not a power of two
    assert(!Page_sizeValid(1000u));

    /* A page with no storage refuses every operation. */
    Page *bare = Page_0();
    assert(bare != nullptr);
    assert(Page_size(bare) == 0u && Page_payload(bare) == nullptr);
    assert(Page_format(bare, 1u, PAGE_KIND_LEAF) == PAGE_INVALID);
    assert(Page_append(bare, (const uint8_t*) "x", 1u) == PAGE_INVALID);
    assert(Page_seal(bare) == PAGE_INVALID);
    assert(Page_verify(bare) == PAGE_INVALID);
    Page_free(bare);

    /* Binding refuses a null buffer and an illegal size. */
    assert(Page_2(nullptr, SIZE) == nullptr);
    assert(Page_2(storage, 300u) == nullptr);

    Page *page = Page_2(storage, SIZE);
    assert(page != nullptr);
    assert(Page_size(page) == SIZE && Page_capacity(page) == SIZE - PAGE_HEADER_BYTES);

    /* Format writes the documented layout, in bytes. */
    assert(Page_format(page, 7u, PAGE_KIND_LEAF) == PAGE_OK);
    assert(Page_code(page) == PAGE_OK);
    assert(Page_id(page) == 7u && Page_kind(page) == PAGE_KIND_LEAF);
    assert(Page_used(page) == 0u && Page_freeSpace(page) == Page_capacity(page));
    assert(!Page_isSealed(page) && Page_isDirty(page) && Page_sealValue(page) == 0u);
    assert(rawU32(0u) == PAGE_MAGIC);
    assert(rawU32(4u) == 7u);
    assert(rawU32(8u) == (uint32_t) PAGE_KIND_LEAF);
    assert(rawU32(12u) == 0u);
    assert(rawU32(16u) == SIZE);
    assert(rawU32(PAGE_SEAL_OFFSET) == 0u);          // unsealed until sealed

    /* Appending fills the payload; the header's `used` follows it. */
    assert(Page_append(page, (const uint8_t*) "alpha", 5u) == PAGE_OK);
    assert(Page_used(page) == 5u && Page_freeSpace(page) == Page_capacity(page) - 5u);
    assert(rawU32(12u) == 5u);
    assert(memcmp(Page_payloadOf(page), "alpha", 5u) == 0);
    assert(Page_append(page, nullptr, 0u) == PAGE_OK);      // an empty append is legal
    assert(Page_used(page) == 5u);
    assert(Page_append(page, (const uint8_t*) "x", 0u) == PAGE_OK);

    /* Fill to exactly full, then refuse the next byte WITHOUT a partial append. */
    uint8_t filler[SIZE];
    memset(filler, 0x5A, sizeof filler);
    uint32_t remaining = Page_freeSpace(page);
    assert(Page_append(page, filler, remaining) == PAGE_OK);
    assert(Page_freeSpace(page) == 0u && Page_used(page) == Page_capacity(page));
    uint8_t before[8];
    memcpy(before, Page_payloadOf(page), sizeof before);
    assert(Page_append(page, (const uint8_t*) "!", 1u) == PAGE_FULL);
    assert(Page_code(page) == PAGE_FULL);
    assert(Page_used(page) == Page_capacity(page));         // nothing was appended
    assert(memcmp(Page_payloadOf(page), before, sizeof before) == 0);
    assert(Page_append(page, nullptr, 1u) == PAGE_INVALID); // null source, real length

    /* Sealing stores the checksum; a sealed page is immutable. */
    assert(Page_seal(page) == PAGE_OK);
    assert(Page_isSealed(page) && !Page_isDirty(page));
    uint32_t seal = Page_sealValue(page);
    assert(seal != 0u && rawU32(PAGE_SEAL_OFFSET) == seal);
    assert(Page_verify(page) == PAGE_OK);
    assert(Page_append(page, (const uint8_t*) "x", 1u) == PAGE_SEALED);
    assert(Page_reset(page, PAGE_KIND_BRANCH) == PAGE_OK);
    assert(Page_id(page) == 7u);                            // reset keeps the identity
    assert(Page_kind(page) == PAGE_KIND_BRANCH);
    assert(Page_used(page) == 0u && !Page_isSealed(page));
    /* An unsealed page must NOT verify: that is what makes "seal before durable" real. */
    assert(Page_verify(page) == PAGE_BAD_SEAL);

    /* ------------------------------------------------------------------ *
     * The corruption matrix. Every sealed field is covered, so a misplaced
     * page (wrong id), a mis-sized read (wrong size), a truncated payload
     * (wrong used) and a flipped byte each fail instead of passing.         */
    assert(Page_format(page, 42u, PAGE_KIND_LEAF) == PAGE_OK);
    assert(Page_append(page, (const uint8_t*) "payload-bytes", 13u) == PAGE_OK);
    assert(Page_seal(page) == PAGE_OK);
    assert(Page_verify(page) == PAGE_OK);

    struct { const char *what; size_t offset; } cases[] = {
        { "a flipped payload byte", PAGE_HEADER_BYTES + 3u },
        { "the page id",            4u },
        { "the page kind",          8u },
        { "the used length",        12u },
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
        uint8_t saved = storage[cases[i].offset];
        storage[cases[i].offset] ^= 0x5Au;
        assert(Page_verify(page) == PAGE_BAD_SEAL);          // always BAD_SEAL, never OK
        storage[cases[i].offset] = saved;
        assert(Page_verify(page) == PAGE_OK);                // restored, good again
    }

    /* A wrong magic or page size is not a damaged page, it is not OUR page. */
    uint8_t savedMagic = storage[0];
    storage[0] ^= 0xFFu;
    assert(Page_verify(page) == PAGE_INVALID);
    storage[0] = savedMagic;
    uint8_t savedSize = storage[16];
    storage[16] ^= 0x01u;
    assert(Page_verify(page) == PAGE_INVALID);
    storage[16] = savedSize;
    assert(Page_verify(page) == PAGE_OK);

    /* A used length that cannot fit the page is refused rather than read through. */
    uint8_t savedUsedLow = storage[12];
    uint8_t savedUsedHigh = storage[13];
    storage[12] = 0xFFu;
    storage[13] = 0xFFu;
    assert(Page_verify(page) == PAGE_INVALID);
    storage[12] = savedUsedLow;
    storage[13] = savedUsedHigh;
    assert(Page_verify(page) == PAGE_OK);

    /* Verify never crashes on arbitrary bytes: an all-zero buffer is simply refused. */
    uint8_t zeros[64];
    memset(zeros, 0, sizeof zeros);
    Page *foreign = Page_2(zeros, SIZE);
    assert(foreign != nullptr);
    assert(Page_verify(foreign) == PAGE_INVALID);
    Page_free(foreign);

    /* Naming covers every code and kind, and unknown values do not crash. */
    assert(strcmp(Page_codeName(PAGE_OK), "ok") == 0);
    assert(strcmp(Page_codeName(PAGE_INVALID), "invalid") == 0);
    assert(strcmp(Page_codeName(PAGE_BAD_SEAL), "bad-seal") == 0);
    assert(strcmp(Page_codeName(PAGE_FULL), "full") == 0);
    assert(strcmp(Page_codeName(PAGE_NOT_SEALED), "not-sealed") == 0);
    assert(strcmp(Page_codeName(PAGE_SEALED), "sealed") == 0);
    assert(strcmp(Page_codeName((PageCode) 99), "unknown") == 0);
    assert(strcmp(Page_kindName(PAGE_KIND_FREE), "free") == 0);
    assert(strcmp(Page_kindName(PAGE_KIND_LEAF), "leaf") == 0);
    assert(strcmp(Page_kindName(PAGE_KIND_BRANCH), "branch") == 0);
    assert(strcmp(Page_kindName(PAGE_KIND_OVERFLOW), "overflow") == 0);
    assert(strcmp(Page_kindName(PAGE_KIND_CATALOG), "catalog") == 0);
    assert(strcmp(Page_kindName(77u), "unknown") == 0);
    assert(Page_format(page, 1u, 99u) == PAGE_INVALID);      // unknown kind refused
    assert(Page_codeName(Page_code(page)) != nullptr);

    /* Projections are bounded and report truncation rather than cutting silently. */
    char text[128];
    bool truncated = true;
    assert(Page_toString(page, text, sizeof text, &truncated));
    assert(!truncated && strstr(text, "Page(id=") != nullptr);
    assert(Page_toStringStruct(page, text, sizeof text, &truncated));
    assert(strstr(text, "seal=") != nullptr && strstr(text, "kind=") != nullptr);
    char tiny[4];
    truncated = false;
    assert(!Page_toString(page, tiny, sizeof tiny, &truncated));
    assert(truncated);
    assert(!Page_toStringStruct(page, nullptr, 0u, nullptr));
    assert(Page_toString(nullptr, text, sizeof text, nullptr));   // the toString Law
    assert(strcmp(text, "nullptr") == 0);

    /* Null-owner accessors are safe defaults, not crashes. */
    assert(Page_id(nullptr) == 0u && Page_kind(nullptr) == 0u && Page_size(nullptr) == 0u);
    assert(Page_payload(nullptr) == nullptr && Page_payloadOf(nullptr) == nullptr);
    assert(!Page_isSealed(nullptr) && !Page_isDirty(nullptr));
    assert(Page_code(nullptr) == PAGE_INVALID);
    Page_markClean(nullptr);
    Page_free(nullptr);

    /* markClean is the caller telling us it flushed. */
    assert(Page_format(page, 3u, PAGE_KIND_OVERFLOW) == PAGE_OK);
    assert(Page_isDirty(page));
    Page_markClean(page);
    assert(!Page_isDirty(page));

    Page_free(page);
    printf("page_test: PASS\n");
    return 0;
}
