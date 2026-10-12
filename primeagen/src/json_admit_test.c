#include <stdio.h>

#include "src/json_admit.h"

// Owner: bounded JSON admission parity with the Python oracle
// (tests/primeagen/src/json_input_test.py): depth ceiling, duplicate-key
// rejection, malformed/encoding rejection, and recovery. Rejections emit one
// [vex] THROW line; acceptance is silent.

#define NODES 512u
#define SCRATCH 4096u

static int failures = 0;

static void check(bool ok, const char *message) {
    if (!ok) {
        fprintf(stderr, "[test] FAIL: %s\n", message);
        ++failures;
    }
}

static bool admit(const char *text) {
    static JsonNode nodes[NODES];
    static char scratch[SCRATCH];
    JsonDoc doc;
    return JsonAdmit_parse(&doc, nodes, NODES, scratch, SCRATCH, text);
}

static void makeNested(char *out, int depth) {
    int at = 0;
    for (int i = 0; i < depth; ++i)
        out[at++] = '[';
    out[at++] = '0';
    for (int i = 0; i < depth; ++i)
        out[at++] = ']';
    out[at] = '\0';
}

int main(void) {
    char buffer[512];

    makeNested(buffer, JSON_MAX_DEPTH);
    check(admit(buffer), "exact depth accepted");
    makeNested(buffer, JSON_MAX_DEPTH + 1);
    check(!admit(buffer), "depth + 1 rejected");
    makeNested(buffer, 200);
    check(!admit(buffer), "deep nesting rejected without overflow");

    check(!admit("{\"a\":1,\"a\":2}"), "duplicate top-level key rejected");
    check(!admit("{\"args\":{\"path\":\"a\",\"path\":\"b\"}}"), "duplicate nested key rejected");
    check(!admit("]"), "unbalanced close rejected");
    check(!admit("{"), "unterminated object rejected");
    check(!admit(nullptr), "null rejected");

    check(admit("{\"answer\":\"ok\",\"n\":1}"), "ordinary object accepted");
    check(admit("{\"text\":\"[]{}[{}]\"}"), "brackets inside a string are not depth");
    check(admit("{\"answer\":\"recovered\"}"), "recovery after rejection");

    if (failures == 0)
        printf("json_admit owner: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
