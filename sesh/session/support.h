#ifndef SESH_OWNER_SUPPORT_H
#define SESH_OWNER_SUPPORT_H
#include "lang/sesh.h"
#include <assert.h>
#include <string.h>

static inline bool verify(const ApiAuth *auth, void *context, uint64_t *dest) {
    assert(auth != nullptr && context != nullptr && dest != nullptr);
    *dest = *(uint64_t*) context;
    return *dest != 0;
}
#define PROJECTIONS(Class, object, fieldText) do { \
    char text[512]; bool truncated = true; \
    assert(Class##_toString(&(object), text, sizeof(text), &truncated) && !truncated); \
    assert(Class##_toStringStruct(&(object), text, sizeof(text), nullptr)); \
    assert(strstr(text, fieldText) != nullptr); \
    assert(Class##_toString(nullptr, text, 8, &truncated) && !truncated); \
    assert(strcmp(text, "nullptr") == 0); \
    assert(Class##_toStringStruct(nullptr, text, 8, nullptr)); \
} while (0)
#define BAD_PROJECTIONS(Class, object) do { \
    char text[1]; bool truncated = false; \
    assert(!Class##_toString(&(object), nullptr, 1, &truncated) && truncated); \
    assert(!Class##_toStringStruct(&(object), text, 0, &truncated) && truncated); \
    assert(!Class##_toString(&(object), text, 1, &truncated) && truncated && text[0] == '\0'); \
} while (0)
#define INVALID_MODE (argc == 2 && strcmp(argv[1], "invalid") == 0)
#endif
