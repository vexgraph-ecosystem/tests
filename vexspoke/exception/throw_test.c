// tests/vexspoke/exception/throw_test.c — the THROW macro _test.
//
// THROW is a header-only macro with no state: its proof is that it composes and
// runs — a bare message and a formatted message both emit one "[vex]" line to
// stderr and return to the caller (it never unwinds). The output below is the
// evidence.

#include <stdio.h>

#include "exception/throw.h"

int main(void) {
    THROW("throw_test: a bare message");
    THROW("throw_test: formatted %d and %s", 42, "text");
    THROW("throw_test: no trailing format argument needed");
    printf("throw_test: THROW composed and returned cleanly\n");
    return 0;
}
