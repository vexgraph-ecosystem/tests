/* Owner test for space/support.h (header-only unit). Per the Public Surface
 * Proof Law every inline helper and macro is exercised: the 0..8 arity counter,
 * the SPACE_CONSTRUCT chooser, ASCII handle validation across the 23+1 extent,
 * bounded formatting/truncation, and byte-span projection escaping. The four
 * rejection paths deliberately emit their [vex] THROW diagnostic.
 * Caller serializes access; these helpers touch no shared state.
 */
#include "test_support.h"

#include <stdio.h>
#include <string.h>

#include "space/support.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[support] PASS %s\n", name); } \
    else { printf("[support] FAIL %s\n", name); g_failures++; } \
} while (0)

/* Arity counter: 0..8 select the exact index (compile-time, no runtime cost). */
enum { A0 = SPACE_COUNT(), A1 = SPACE_COUNT(1), A2 = SPACE_COUNT(1, 2),
       A3 = SPACE_COUNT(1, 2, 3), A4 = SPACE_COUNT(1, 2, 3, 4),
       A5 = SPACE_COUNT(1, 2, 3, 4, 5), A6 = SPACE_COUNT(1, 2, 3, 4, 5, 6),
       A7 = SPACE_COUNT(1, 2, 3, 4, 5, 6, 7), A8 = SPACE_COUNT(1, 2, 3, 4, 5, 6, 7, 8) };
_Static_assert(A0 == 0 && A1 == 1 && A2 == 2 && A3 == 3 && A4 == 4 &&
               A5 == 5 && A6 == 6 && A7 == 7 && A8 == 8, "arity counter must rank 0..8");

/* A local class proves SPACE_CONSTRUCT dispatches to Class_N by argument count. */
typedef struct Probe { int sum; } Probe;
static Probe Probe_0(void) { return (Probe) { 0 }; }
static Probe Probe_2(int a, int b) { return (Probe) { a + b }; }
static Probe Probe_3(int a, int b, int c) { return (Probe) { a + b + c }; }
#define Probe(...) SPACE_CONSTRUCT(Probe, __VA_ARGS__)

int main(void) {
    printf("=== Space support owner suite ===\n");

    CHECK("chooser 0 args -> Probe_0", Probe().sum == 0);
    CHECK("chooser 2 args -> Probe_2", Probe(2, 3).sum == 5);
    CHECK("chooser 3 args -> Probe_3", Probe(1, 2, 3).sum == 6);

    // --- Handle validation across the 23+1 extent and the character set.
    CHECK("name nullptr rejected", !Space_nameValid(nullptr));
    CHECK("name empty rejected", !Space_nameValid(""));
    CHECK("name single char ok", Space_nameValid("a"));
    CHECK("name digits ok", Space_nameValid("0"));
    CHECK("name dash underscore ok", Space_nameValid("a-b_c"));
    CHECK("name 22 chars ok", Space_nameValid("aaaaaaaaaaaaaaaaaaaaaa"));
    CHECK("name 23 chars ok", Space_nameValid("aaaaaaaaaaaaaaaaaaaaaaa"));
    CHECK("name 24 chars rejected", !Space_nameValid("aaaaaaaaaaaaaaaaaaaaaaaa"));
    CHECK("name uppercase rejected", !Space_nameValid("Ab"));
    CHECK("name dot rejected", !Space_nameValid("a.b"));
    CHECK("name at rejected", !Space_nameValid("a@b"));
    CHECK("name space rejected", !Space_nameValid("a b"));
    CHECK("name DEL rejected", !Space_nameValid("\x7f"));
    CHECK("name high byte rejected", !Space_nameValid("\x80"));
    CHECK("name stops at embedded NUL", Space_nameValid("abc\0def"));

    // --- Bounded formatting: fits, truncates, and null/zero destinations.
    char buf[64];
    bool cut = true;
    CHECK("format fits", Space_format(buf, sizeof(buf), &cut, "%d", 42) && !cut && strcmp(buf, "42") == 0);
    CHECK("format exact fit", Space_format(buf, 3, &cut, "ab") && !cut && strcmp(buf, "ab") == 0);
    CHECK("format one short truncates", !Space_format(buf, 2, &cut, "ab") && cut && strcmp(buf, "a") == 0);
    CHECK("format null dest rejects", !Space_format(nullptr, 8, &cut, "x") && cut);
    CHECK("format zero cap rejects", !Space_format(buf, 0, &cut, "x") && cut);
    CHECK("format null flag tolerated", Space_format(buf, sizeof(buf), nullptr, "ok") && strcmp(buf, "ok") == 0);

    // --- Byte-span projection: escaping then the closing `"}`. Space_quote
    //     appends to whatever prefix dest already holds, so reset each time.
    buf[0] = '\0';
    bool ok = Space_quote("hi", 2, buf, sizeof(buf), &cut);
    CHECK("quote plain", ok && !cut && strcmp(buf, "hi\"}") == 0);
    buf[0] = '\0';
    const char *esc = "a\nb\tc\"d\\";
    ok = Space_quote(esc, strlen(esc), buf, sizeof(buf), &cut);
    CHECK("quote escapes", ok && strcmp(buf, "a\\nb\\tc\\\"d\\\\\"}") == 0);
    buf[0] = '\0';
    ok = Space_quote("\x01\x7f\x80", 3, buf, sizeof(buf), &cut);
    CHECK("quote control/high bytes", ok && strcmp(buf, "\\x01\\x7f\\x80\"}") == 0);
    buf[0] = '\0';
    ok = Space_quote("", 0, buf, sizeof(buf), &cut);
    CHECK("quote empty span", ok && strcmp(buf, "\"}") == 0);
    buf[0] = '\0';
    ok = Space_quote("abcdef", 6, buf, 6, &cut);
    CHECK("quote truncates", !ok && cut && buf[0] == 'a');
    buf[0] = '\0';
    ok = Space_quote("", 0, buf, 2, &cut);
    CHECK("quote no room for closing", !ok && cut);

    if (g_failures != 0) {
        printf("=== %d SUPPORT FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL SUPPORT PASS ===\n");
    return 0;
}
