/* Owner test for space/message.c. Exhausts the routed event value: borrowed
 * (zero-copy) payload identity, length-based spans including embedded NUL,
 * boundary length, every constructor form, null-safe getters and both
 * projections with escaping. Rejection returns the empty event and never
 * touches the payload; retention stays the caller's contract.
 */
#include "test_support.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "space/message.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[message] PASS %s\n", name); } \
    else { printf("[message] FAIL %s\n", name); g_failures++; } \
} while (0)

int main(void) {
    printf("=== Message owner suite ===\n");

    Message zero = Message_0();
    CHECK("zero is all-zero", zero.id == 0 && zero.channelId == 0 && zero.text == nullptr && zero.length == 0);
    CHECK("chooser() == _0", Message().id == 0 && Message().text == nullptr);
    CHECK("zero() == _0", Message_zero().id == 0);

    const char *payload = "hello";
    Message m = Message(5, 6, 0, 0, payload, 5);
    CHECK("chooser 6-arg id", m.id == 5);
    CHECK("chooser 6-arg channel", m.channelId == 6);
    CHECK("author 0 is allowed (host)", m.authorId == 0);
    CHECK("task 0 is allowed (plain)", m.taskId == 0);
    CHECK("text is borrowed, not copied", m.text == payload);
    CHECK("length recorded", m.length == 5);
    CHECK("same 6-arg via _6", Message_6(5, 6, 1, 2, payload, 5).authorId == 1);

    Message bounded = Message(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, "x", 1);
    CHECK("boundary max ids accepted", bounded.id == UINT64_MAX && bounded.taskId == UINT64_MAX);

    // --- Invalid admission returns the empty event.
    CHECK("zero id rejected", Message(0, 6, 0, 0, payload, 5).id == 0 && Message(0, 6, 0, 0, payload, 5).text == nullptr);
    CHECK("zero channel rejected", Message(5, 0, 0, 0, payload, 5).id == 0);
    CHECK("null text rejected", Message(5, 6, 0, 0, nullptr, 5).id == 0);
    CHECK("zero length rejected", Message(5, 6, 0, 0, payload, 0).id == 0);
    CHECK("oversized length rejected", Message(5, 6, 0, 0, payload, (size_t) PTRDIFF_MAX + 1u).id == 0);

    // --- Length-based spans: embedded NUL and non-terminated bytes survive.
    const char raw[4] = { 'a', '\0', 'b', 'c' };
    Message nul = Message(1, 2, 0, 0, raw, 4);
    CHECK("embedded NUL kept", Message_getLength(&nul) == 4 && Message_getText(&nul)[1] == '\0' &&
                               Message_getText(&nul)[3] == 'c');

    CHECK("getters null self", Message_getId(nullptr) == 0 && Message_getChannelId(nullptr) == 0 &&
                               Message_getAuthorId(nullptr) == 0 && Message_getTaskId(nullptr) == 0 &&
                               Message_getText(nullptr) == nullptr && Message_getLength(nullptr) == 0);
    CHECK("getters live", Message_getId(&m) == 5 && Message_getChannelId(&m) == 6 &&
                          Message_getText(&m) == payload && Message_getLength(&m) == 5);

    char buf[256];
    bool cut = false;
    CHECK("toString null self", Message_toString(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toString value", Message_toString(&m, buf, sizeof(buf), &cut) && strcmp(buf, "message 5 (5 bytes)") == 0);
    CHECK("toStringStruct null self", Message_toStringStruct(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toStringStruct fields",
          Message_toStringStruct(&m, buf, sizeof(buf), &cut) &&
          strcmp(buf, "Message{id=5,channelId=6,authorId=0,taskId=0,text[length=5]=\"hello\"}") == 0 && !cut);

    const char *esc = "a\nb\tc\"d\\";
    Message e = Message(1, 2, 0, 0, esc, strlen(esc));
    CHECK("toStringStruct escapes payload",
          Message_toStringStruct(&e, buf, sizeof(buf), &cut) &&
          strcmp(buf, "Message{id=1,channelId=2,authorId=0,taskId=0,text[length=8]=\"a\\nb\\tc\\\"d\\\\\"}") == 0);
    CHECK("toStringStruct escapes NUL", Message_toStringStruct(&nul, buf, sizeof(buf), &cut) &&
                                        strstr(buf, "\\x00") != nullptr);
    CHECK("toString truncates", !Message_toString(&m, buf, 6, &cut) && cut);
    CHECK("toStringStruct truncates", !Message_toStringStruct(&m, buf, 12, &cut) && cut);
    CHECK("toStringStruct null flag tolerated", Message_toStringStruct(&m, buf, sizeof(buf), nullptr));

    if (g_failures != 0) {
        printf("=== %d MESSAGE FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL MESSAGE PASS ===\n");
    return 0;
}
