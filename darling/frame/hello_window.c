// _main/hello_window.c — a MINIMAL sample app proving that `b run` discovers it,
// builds it, wraps it in a macOS .app, ad-hoc codesigns it, and launches it.
//
// Drop any _main/<name>.c with a main() and it becomes an app target the same
// way. This file is just a demo — delete it freely.
//
//   ./tools/b run hello_window

#include <stdio.h>

#include "window/window.h"

int main(void) {
    Window *w = Window_create("hello from b", 900, 600);
    Window_show(w);

    while (!Window_shouldClose(w)) {
        Window_pollEvents();
        Window_dispatchEvents(w);
    }

    Window_destroy(w);
    printf("hello_window: closed cleanly\n");
    return 0;
}
