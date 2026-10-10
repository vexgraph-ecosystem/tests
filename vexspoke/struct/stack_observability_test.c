/* Owner test for the Stack's self-describing observability contract.
 *
 * Stack_push returns void, so it cannot report a growth failure. Two facts make
 * the outcome checkable with the API as it stands (the Self-Describing memory
 * Block Law):
 *   1. the data buffer IS a self-describing arena block — Memory_type() is the
 *      array type id and Memory_length() is exactly capacity * stride;
 *   2. a dropped push is observable — a successful push always advances
 *      Stack_size(), so an unchanged size means the element was not admitted.
 *
 * The test deliberately exhausts a small master arena (the Deliberate Exhaustion
 * and Backend Trust Law) so the failure path is actually reached, then proves the
 * stack stays coherent. Exhaustion is process-local: this is its own binary.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "nio/mem.h"
#include "oop/type.h"
#include "struct/stack.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[stackobs] PASS %s\n", name); } \
    else { printf("[stackobs] FAIL %s\n", name); g_failures++; } \
} while (0)

#define ARENA_BYTES (32u * 1024u * 1024u)
#define PUSH_LIMIT  6000000u   // exceeds the 64MB default arena's reachable capacity

int main(void) {
    printf("=== Stack observability owner suite ===\n");
    // Must run before any other allocation so the small arena wins.
    CHECK("small arena initialized", Memory_init(ARENA_BYTES));

    Stack *s = Stack_1(ID_INT);
    CHECK("stack constructed", s != nullptr && Stack_stride(s) == 4);

    // --- 1. The buffer is self-describing: identity and byte size are in-band.
    CHECK("buffer carries the array type", Memory_type(Stack_dataBuffer(s)) == TYPE_INT_ARRAY);
    CHECK("buffer length == capacity * stride",
          Memory_length(Stack_dataBuffer(s)) == (size_t) Stack_capacity(s) * Stack_stride(s));

    // The header tracks every growth.
    for (uint64_t i = 0; i < 4000u; ++i)
        Stack_push(s, i);
    CHECK("size advanced after pushes", Stack_size(s) == 4000);
    CHECK("buffer descriptor tracked growth",
          Memory_length(Stack_dataBuffer(s)) == (size_t) Stack_capacity(s) * Stack_stride(s));

    // --- 2. Exhaust the arena, then prove the drop is observable.
    size_t stalledAt = 0;
    uint64_t attempted = 0;
    for (uint64_t i = 4000u; i < PUSH_LIMIT; ++i) {
        size_t before = Stack_size(s);
        Stack_push(s, i);
        attempted++;
        if (Stack_size(s) == before) { stalledAt = before; break; }
    }
    CHECK("growth failure was reached", stalledAt != 0);
    CHECK("dropped push left size unchanged", Stack_size(s) == stalledAt);
    CHECK("capacity did not change on the dropped push",
          Memory_length(Stack_dataBuffer(s)) == (size_t) Stack_capacity(s) * Stack_stride(s));

    // --- 3. The stack is still coherent after the observable drop.
    CHECK("top element intact", Stack_peek(s) == (uint64_t) (stalledAt - 1));
    CHECK("pop returns the top", Stack_pop(s) == (uint64_t) (stalledAt - 1));
    CHECK("size decrements on pop", Stack_size(s) == stalledAt - 1);
    CHECK("no phantom capacity from a rejected push", Stack_capacity(s) * Stack_stride(s) >= Stack_size(s) * Stack_stride(s));

    Stack_free(s);
    printf("[stackobs] attempted %llu pushes before the drop\n", (unsigned long long) attempted);
    if (g_failures != 0) {
        printf("=== %d FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL STACKOBS PASS ===\n");
    return 0;
}
