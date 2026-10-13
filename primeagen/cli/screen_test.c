// Owner: PrimeagenScreen — the toolkit-composed frame. Home layout (rail + menu
// + badge panel), chat layout (three columns), one-pass present. Headless.
#define _XOPEN_SOURCE_EXTENDED 1
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "cli/screen.h"

// A tiny "white paper, dark ink" image: one dark pixel, one blank.
static uint8_t pixels[2 * 4] = { 0, 0, 0, 255,   255, 255, 255, 255 };

int main(void) {
    assert(setlocale(LC_CTYPE, "en_US.UTF-8") != nullptr);
    PrimeagenScreen screen;
    PrimeagenScreen_init(&screen);
    assert(!PrimeagenScreen_isBound(&screen));
    assert(!PrimeagenScreen_present(&screen, nullptr));

    assert(PrimeagenScreen_bind(&screen, 20, 70));
    assert(PrimeagenScreen_isBound(&screen));
    assert(!PrimeagenScreen_bind(&screen, 0, 10));
    TerminalCanvas canvas = PrimeagenScreen_canvas(&screen);
    assert(TerminalCanvas_isValid(&canvas));
    assert(TerminalCanvas_getWidth(&canvas) == 70 && TerminalCanvas_getHeight(&canvas) == 20);

    // --- HOME: rail with the menu, badge filling the panel beside it ---
    TerminalImage badge = TerminalImage(pixels, 2, 1);
    TerminalImage_setInvert(&badge, true);          // white paper -> blank
    TerminalImage_setColorRamp(&badge, TerminalImage_rampPurple(nullptr), 8);
    static const char *menu[] = { "People", "Servers", "Settings" };
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, &badge);
    // Rail (0..21) | separator (22) | panel (23..69); the rule is the connector.
    assert(screen.cells[0 * 70 + 0] != 0);          // rail row 0 has the title
    assert(screen.cells[1 * 70 + 0] == '-');        // striped connector rule
    assert(screen.cells[1 * 70 + 3] == '.');
    assert(screen.cells[0 * 70 + 22] == '|');       // column separator
    // The badge fills the panel: dark ink -> a glyph, white paper -> blank.
    assert(screen.cells[0 * 70 + 23] != ' ');
    assert(screen.cells[0 * 70 + 69] == ' ');
    // A null badge and an empty menu must not fault.
    PrimeagenScreen_renderHome(&screen, "Primeagen", nullptr, 0, 0, nullptr);
    PrimeagenScreen_renderHome(nullptr, "x", menu, 3, 0, nullptr);

    // --- CHAT: the three-column layout over resident Tui state ---
    Tui tui = Tui();
    assert(Tui_isReady(&tui));
    assert(Tui_setSize(&tui, 18, 48));
    assert(Tui_add(&tui, "hello there", 11));
    assert(Tui_setInput(&tui, "draft", 5));
    PrimeagenScreenState state = {0};
    state.tui = &tui;
    state.focused = true;
    assert(PrimeagenScreen_renderChat(&screen, &state));
    assert(screen.cells[1 * 70 + 0] == '-');        // centre rule
    // The input prompt sits on the input row; the transcript carries the text.
    int seen = 0;
    for (size_t i = 0; i < 20 * 70; ++i)
        if (screen.cells[i] == 'h') seen = 1;
    assert(seen);
    assert(!PrimeagenScreen_renderChat(&screen, nullptr));
    assert(!PrimeagenScreen_renderChat(nullptr, &state));

    // Badge: the app's white-paper image; the screen inverts it and the caller's
    // palette treats the ink. NULL/zero clears it.
    PrimeagenScreen_setBadge(pixels, 2, 1);
    assert(PrimeagenScreen_hasBadge());
    const TerminalImage *stored = PrimeagenScreen_badge();
    assert(stored != nullptr && TerminalImage_inverted(stored));
    size_t rampCount = 0;
    assert(TerminalImage_getColorRamp(stored, &rampCount) != nullptr && rampCount > 0);
    PrimeagenScreen_setBadge(nullptr, 0, 0);
    assert(!PrimeagenScreen_hasBadge() && PrimeagenScreen_badge() == nullptr);

    // --- present: one pass into a headless screen ---
    FILE *in = fopen("/dev/null", "r");
    FILE *out = fopen("/dev/null", "w");
    SCREEN *term = newterm(nullptr, out, in);
    assert(term && resizeterm(20, 70) != ERR);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 1, &badge);
    assert(PrimeagenScreen_present(&screen, stdscr));
    assert(!PrimeagenScreen_present(nullptr, stdscr));
    assert(!PrimeagenScreen_present(&screen, nullptr));
    endwin();
    delscreen(term);
    fclose(in);
    fclose(out);

    PrimeagenScreen_free(&screen);
    assert(!PrimeagenScreen_isBound(&screen));
    printf("screen_test: PASS\n");
    return 0;
}
