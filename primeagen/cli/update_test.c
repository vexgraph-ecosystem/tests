/* MODULE: real headless view-update owner. Hotcwap replaces executing view code
 * while the same terminal screen/Tui draft/history remain resident. No provider,
 * interactive appearance, general migration or permission-enforcement claim. */
#include "cli/update.h"
#include "cli/view.h"
#include "hot/manifest.h"
#include <assert.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void render(HotModule *hot, Tui *state, char expected) {
    PrimeagenDrawFn fn = (PrimeagenDrawFn) Hot_get_symbol(hot, "Primeagen_draw");
    assert(fn);
    char row[128];
    uint8_t styles[128];
    assert(fn(state, false, true, false, false, row, styles, sizeof(row)));
    assert((mvinch(0, 11) & A_CHARTEXT) == (chtype) expected);
    assert((mvinch(18, 2) & A_CHARTEXT) == 'd');
    assert(strcmp(Tui_getInput(state), "draft") == 0);
    assert(Tui_getCount(state) == 1);
    PrimeagenCommandFn command = (PrimeagenCommandFn) Hot_get_symbol(hot, "Primeagen_command");
    assert(command);
    assert(command(state, true) == 0); // draft is not a slash command
    assert(command(nullptr, true) == -1);
}

int main(int argc, char **argv) {
    assert(argc == 6);
    assert(setenv("VEX_MANIFEST", argv[1], 1) == 0);
    assert(setenv("HOME", argv[1], 1) == 0);
    assert(MANIFEST(MANIFEST_APP_DATA, "vexgraph", "primeagen"));
    assert(MANIFEST_LIBRARY("primeagen", (const char*) nullptr));
    assert(MANIFEST_ENSURE());
    assert(MANIFEST_REFLECT("primeagen", argv[2]));
    HotModule *hot = Hot_init("primeagen");
    assert(hot);
    uint32_t loaded;
    assert(Primeagen_update(hot, argv[2], argv[3], &loaded) && loaded == 1);
    FILE *input = tmpfile(), *output = tmpfile();
    assert(input && output);
    SCREEN *screen = newterm("xterm-256color", output, input);
    assert(screen && resizeterm(20, 60) != ERR);
    Tui state = Tui();
    assert(Tui_setSize(&state, 20, 60));
    assert(Tui_setInput(&state, "draft", 5));
    assert(Tui_add(&state, "history", 7));
    render(hot, &state, 'A');
    uint64_t generation = Hot_get_generation(hot);
    assert(Primeagen_update(hot, argv[4], argv[5], &loaded));
    assert(Hot_get_generation(hot) == generation + 1);
    render(hot, &state, 'B');
    // Wrong checksum and malformed descriptor both preserve current code/state.
    assert(!Primeagen_update(hot, argv[4], argv[3], &loaded) && loaded == 0);
    render(hot, &state, 'B');
    assert(!Primeagen_update(hot, argv[4], "/not-present", &loaded) && loaded == 0);
    assert(!Primeagen_update(nullptr, argv[4], argv[5], &loaded));
    assert(Hot_get_generation(hot) == generation + 1);
    render(hot, &state, 'B');
    assert(Primeagen_update(hot, argv[2], argv[3], &loaded));
    render(hot, &state, 'A');
    endwin();
    delscreen(screen);
    fclose(input);
    fclose(output);
    Tui_free(&state);
    HotShutdown(hot);
    puts("update_test: real view code changed; resident draft/history preserved; rejection/recovery passed");
}
