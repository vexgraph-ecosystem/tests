// Owner: PrimeagenScreen — the toolkit-composed frame. Home layout (rail + menu
// + badge panel), chat layout (three columns), focus-aware selection, the
// frame's own border, one-pass present. Headless.
#define _XOPEN_SOURCE_EXTENDED 1
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "cli/screen.h"

#define COLS 110
#define ROWS 20
#define AT(y, x) (screen.cells[(y) * COLS + (x)])

// A tiny "white paper, dark ink" image: one dark pixel, one blank.
static uint8_t pixels[2 * 4] = { 0, 0, 0, 255,   255, 255, 255, 255 };

int main(void) {
    assert(setlocale(LC_CTYPE, "en_US.UTF-8") != nullptr);
    PrimeagenScreen screen;
    PrimeagenScreen_init(&screen);
    assert(!PrimeagenScreen_isBound(&screen));
    assert(!PrimeagenScreen_present(&screen, nullptr));

    assert(PrimeagenScreen_bind(&screen, ROWS, COLS));
    assert(PrimeagenScreen_isBound(&screen));
    assert(!PrimeagenScreen_bind(&screen, 0, 10));
    TerminalCanvas canvas = PrimeagenScreen_canvas(&screen);
    assert(TerminalCanvas_isValid(&canvas));
    assert(TerminalCanvas_getWidth(&canvas) == COLS && TerminalCanvas_getHeight(&canvas) == ROWS);

    // --- HOME: rail with the menu, badge (and its decor field) in the panel ---
    TerminalImage badge = TerminalImage(pixels, 2, 1);
    TerminalImage_setInvert(&badge, true);          // white paper -> blank
    TerminalImage_setColorRamp(&badge, TerminalImage_rampPurple(nullptr), 8);
    static const char *menu[] = { "People", "Servers", "Settings" };
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, &badge);
    // The frame draws its OWN border with corners, so it reads as a TUI inside
    // the window instead of leaning on the window's edges.
    assert(AT(0, 0) == '+');                        // top-left corner
    assert(AT(0, 1) == '-');                        // top border run
    assert(AT(ROWS - 1, 0) == '+');                 // bottom-left corner
    assert(AT(ROWS - 1, COLS - 1) == '+');          // bottom-right corner
    assert(AT(1, 0) == '|');                        // left border
    // Inset rail (1..22) | separator (23) | panel (24..COLS-2); connector rule.
    assert(AT(1, 7) != ' ');                        // rail row 0 carries the title
    assert(AT(2, 1) == '-');                        // striped connector rule
    assert(AT(2, 4) == '.');
    assert(AT(1, 23) == '|');                       // column separator
    // The selected menu row is HIGHLIGHTED (reverse video) while it owns focus.
    assert(TerminalCanvas_styleAt(&canvas, 9, 4) & TERMINAL_STYLE_REVERSE);
    // The badge is ZOOM-FIT: letterboxed (blank margin) and centred, never squashed.
    assert(AT(1, 24) == ' ');
    int badgeInk = 0;
    for (size_t y = 0; y < ROWS; ++y)
        for (size_t x = 24; x < COLS - 1; ++x)
            if (AT(y, x) != ' ')
                badgeInk = 1;
    assert(badgeInk);
    // A null badge and an empty menu must not fault.
    PrimeagenScreen_renderHome(&screen, "Primeagen", nullptr, 0, 0, nullptr);
    PrimeagenScreen_renderHome(nullptr, "x", menu, 3, 0, nullptr);

    // Too small: one centred notice instead of a half-drawn frame (it re-centres
    // every render, so it stays anchored while the window is dragged).
    {
        PrimeagenScreen small;
        PrimeagenScreen_init(&small);
        assert(PrimeagenScreen_bind(&small, 8, 30));
        PrimeagenScreen_renderHome(&small, "Primeagen", menu, 3, 0, nullptr);
        assert(small.cells[2 * 30 + 1] == 'R');          // word-wrapped, centred
        PrimeagenScreen_free(&small);
    }
    // The threshold is the advertised minimum: 89 columns is too small, 90 is not.
    {
        PrimeagenScreen edge;
        PrimeagenScreen_init(&edge);
        assert(PrimeagenScreen_bind(&edge, PRIMEAGEN_SCREEN_MIN_ROWS, PRIMEAGEN_SCREEN_MIN_COLUMNS - 1));
        PrimeagenScreen_renderHome(&edge, "Primeagen", menu, 3, 0, nullptr);
        // One row below the minimum: the centred notice, not a half-drawn frame.
        int notice = 0;
        for (size_t i = 0; i < (size_t) PRIMEAGEN_SCREEN_MIN_ROWS * (PRIMEAGEN_SCREEN_MIN_COLUMNS - 1); ++i)
            if (edge.cells[i] == 'R')
                notice = 1;
        assert(notice);
        assert(PrimeagenScreen_bind(&edge, PRIMEAGEN_SCREEN_MIN_ROWS, PRIMEAGEN_SCREEN_MIN_COLUMNS));
        PrimeagenScreen_renderHome(&edge, "Primeagen", menu, 3, 0, nullptr);
        assert(edge.cells[0 * PRIMEAGEN_SCREEN_MIN_COLUMNS + 0] == '+');   // the real frame
        PrimeagenScreen_free(&edge);
    }

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
    assert(AT(1, 0) == '|');                         // left border
    assert(AT(2, 24) == '-');                        // centre rule (inset by the border)
    // FOCUS: the rail HIGHLIGHTS its selected row only while it owns the keyboard;
    // when another panel has focus the same row is a COLOUR change, not a highlight.
    state.focusPanel = 0;
    assert(PrimeagenScreen_renderChat(&screen, &state));
    assert(TerminalCanvas_styleAt(&canvas, 3, 5) & TERMINAL_STYLE_REVERSE);
    state.focusPanel = 2;
    assert(PrimeagenScreen_renderChat(&screen, &state));
    assert(!(TerminalCanvas_styleAt(&canvas, 3, 5) & TERMINAL_STYLE_REVERSE));
    assert(TerminalCanvas_styleAt(&canvas, 3, 5) & TERMINAL_STYLE_ACCENT);
    assert(TerminalCanvas_fgAt(&canvas, 3, 5) != TERMINAL_COLOR_DEFAULT);
    // The input prompt sits on the input row; the transcript carries the text.
    int seen = 0;
    for (size_t i = 0; i < ROWS * COLS; ++i)
        if (screen.cells[i] == 'h')
            seen = 1;
    assert(seen);
    assert(!PrimeagenScreen_renderChat(&screen, nullptr));
    assert(!PrimeagenScreen_renderChat(nullptr, &state));

    // Badge: the app's white-paper image; the screen inverts it and the caller's
    // palette treats the ink. The paper is NOT blank — a gray glyph field fills
    // the WHOLE panel (edge to edge, not just the picture) and re-scatters on
    // every render, so a revalidate shows a fresh decor.
    PrimeagenScreen_setBadge(pixels, 2, 1);
    assert(PrimeagenScreen_hasBadge());
    const TerminalImage *stored = PrimeagenScreen_badge();
    assert(stored != nullptr && TerminalImage_inverted(stored));
    size_t rampCount = 0;
    assert(TerminalImage_getColorRamp(stored, &rampCount) != nullptr && rampCount > 0);
    assert(TerminalImage_getNoise(stored) != nullptr);
    assert(TerminalImage_getNoiseDensity(stored) > 0);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, stored);
    // Edge to edge: the top-left panel cell carries decor, so there is no gap.
    assert(AT(1, 24) != ' ');
    assert(AT(ROWS - 2, COLS - 2) != ' ');
    uint32_t paper[COLS];
    for (size_t x = 24; x < COLS - 1; ++x)
        paper[x] = AT(8, x);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, stored);
    int rescattered = 0;
    for (size_t x = 24; x < COLS - 1; ++x)
        if (AT(8, x) != paper[x])
            rescattered = 1;
    assert(rescattered);
    PrimeagenScreen_setBadge(nullptr, 0, 0);
    assert(!PrimeagenScreen_hasBadge() && PrimeagenScreen_badge() == nullptr);

    // --- present: one pass into a headless screen ---
    FILE *in = fopen("/dev/null", "r");
    FILE *out = fopen("/dev/null", "w");
    SCREEN *term = newterm(nullptr, out, in);
    assert(term && resizeterm(ROWS, COLS) != ERR);
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
