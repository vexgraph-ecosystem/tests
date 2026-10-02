// tests/test_support.h — the one way a test declares itself skipped.
//
// The Executable Evidence and Readiness Law (tests/test-preferences.md): "A
// skip is neither a pass nor a failure. State which claim remains unproved."
// A test that cannot exercise its contract on this host must exit with
// B_TEST_SKIP (77) — never 0 — so `b test` reports SKIP separately from PASS
// and a script cannot mistake "did not run" for "proved".
//
//   #include "test_support.h"
//   ...
//   if (!window_server_available()) {
//       fprintf(stderr, "window_test: SKIP (no window server)\\n");
//       return B_TEST_SKIP;
//   }
//
// The reason is required and printed to stderr; it is the recorded statement of
// what remains unproved.

#ifndef VEXGRAPH_TEST_SUPPORT_H
#define VEXGRAPH_TEST_SUPPORT_H

#define B_TEST_SKIP 77

#endif  // VEXGRAPH_TEST_SUPPORT_H
