// Owner: PrimeagenScreen — the toolkit-composed frame. Home layout (rail + menu
// + banner panel), chat layout (three detached columns), focus, the frame's
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
// A bright-paper banner (the other polarity).
static uint8_t brightPixels[2 * 4] = { 255, 255, 255, 255,   255, 255, 255, 255 };

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

    // Terminal EVENTS: a zoom (font size), a fullscreen toggle and a window drag
    // all arrive as a resize and are their own event — revalidated at once, never
    // deferred to a later "settle".
    assert(PrimeagenScreen_classify(KEY_RESIZE) == PRIMEAGEN_SCREEN_EVENT_RESIZE);
    assert(PrimeagenScreen_classify('q') == PRIMEAGEN_SCREEN_EVENT_QUIT);
    assert(PrimeagenScreen_classify('Q') == PRIMEAGEN_SCREEN_EVENT_QUIT);
    assert(PrimeagenScreen_classify('1') == PRIMEAGEN_SCREEN_EVENT_HOME);
    assert(PrimeagenScreen_classify('2') == PRIMEAGEN_SCREEN_EVENT_CHAT);
    assert(PrimeagenScreen_classify('f') == PRIMEAGEN_SCREEN_EVENT_FOCUS);
    assert(PrimeagenScreen_classify('z') == PRIMEAGEN_SCREEN_EVENT_NONE);
    {
        PrimeagenScreen zoom;
        PrimeagenScreen_init(&zoom);
        // Zoomed below the floor: re-bound (so the notice has a real grid) and
        // reported as not renderable.
        assert(!PrimeagenScreen_revalidate(&zoom, 24, 70));
        TerminalCanvas zoomed = PrimeagenScreen_canvas(&zoom);
        assert(TerminalCanvas_getWidth(&zoomed) == 70);
        assert(!PrimeagenScreen_revalidate(&zoom, 0, 10));       // unusable size refused
        assert(PrimeagenScreen_revalidate(&zoom, 40, 120));      // zoom back in
        zoomed = PrimeagenScreen_canvas(&zoom);
        assert(TerminalCanvas_getWidth(&zoomed) == 120);
        assert(TerminalCanvas_getHeight(&zoomed) == 40);
        // Still renderable at exactly the advertised minimum.
        assert(PrimeagenScreen_revalidate(&zoom, PRIMEAGEN_SCREEN_MIN_ROWS, PRIMEAGEN_SCREEN_MIN_COLUMNS));
        PrimeagenScreen_free(&zoom);
    }

    // --- HOME: stacked menu boxes, the app's banner box beside them ---
    TerminalImage banner = TerminalImage(pixels, 2, 1);
    TerminalImage_setInvert(&banner, true);          // white paper -> blank
    TerminalImage_setColorRamp(&banner, TerminalImage_rampPurple(nullptr), 8);
    static const char *menu[] = { "People", "Servers", "Settings" };
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, &banner);
    // The frame draws its OWN box with real corners (never '+').
    assert(AT(0, 0) == 0x250C);                     // ┌ top-left
    assert(AT(0, COLS - 1) == 0x2510);              // ┐ top-right
    assert(AT(ROWS - 1, 0) == 0x2514);              // └ bottom-left
    assert(AT(ROWS - 1, COLS - 1) == 0x2518);       // ┘ bottom-right
    assert(AT(0, 1) == '-');                        // the run between the corners
    // The left column is DETACHED boxes: a title box, then one per menu entry,
    // each with its own corners and a blank row between them.
    assert(AT(1, 1) == 0x250C);                     // the title box's corner
    assert(AT(1, 22) == 0x2510);
    assert(AT(3, 1) == 0x2514);                     // ...and its bottom
    assert(AT(4, 1) == ' ');                        // blank row: nothing welded
    assert(AT(5, 1) == 0x250C);                     // the People box
    assert(AT(9, 1) == 0x250C);                     // the Servers box
    assert(AT(13, 1) == 0x250C);                    // the Settings box
    int titleSeen = 0, peopleSeen = 0;
    for (size_t x = 2; x < 22; ++x) {
        if (AT(2, x) == 'P') { titleSeen = 1; }     // "Primeagen"
        if (AT(6, x) == 'P') { peopleSeen = 1; }    // "People"
    }
    assert(titleSeen && peopleSeen);
    // The selected entry is HIGHLIGHTED only while it owns focus.
    assert(TerminalCanvas_styleAt(&canvas, 9, 6) & TERMINAL_STYLE_REVERSE);
    // The banner is its own detached box on the right, decor filled edge to edge.
    assert(AT(1, 25) == 0x250C);                     // its own top-left corner
    assert(AT(2, 25) == '|');
    assert(AT(2, COLS - 2) == '|');
    int bannerInk = 0;
    for (size_t y = 2; y < ROWS - 2; ++y)
        for (size_t x = 26; x < COLS - 2; ++x)
            if (AT(y, x) != ' ')
                bannerInk = 1;
    assert(bannerInk);
    // A null banner and an empty menu must not fault.
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
        assert(edge.cells[0 * PRIMEAGEN_SCREEN_MIN_COLUMNS + 0] == 0x250C);   // the real frame
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
    assert(AT(0, 0) == 0x250C);                      // the frame's own corner
    assert(AT(1, 1) == 0x250C);                      // the left panel is detached too
    assert(AT(1, 25) == 0x250C);                     // ...and so is the centre panel
    // FOCUS: the rail HIGHLIGHTS its selected row only while it owns the keyboard;
    // when another panel has focus the same row is a COLOUR change, not a highlight.
    state.focusPanel = 0;
    assert(PrimeagenScreen_renderChat(&screen, &state));
    assert(TerminalCanvas_styleAt(&canvas, 4, 6) & TERMINAL_STYLE_REVERSE);
    // Internal dividers are CUT: they stop short with an endpoint glyph.
    assert(AT(3, 3) == 0x2576);                      // ╶ the divider's cut end
    state.focusPanel = 2;
    assert(PrimeagenScreen_renderChat(&screen, &state));
    assert(!(TerminalCanvas_styleAt(&canvas, 4, 6) & TERMINAL_STYLE_REVERSE));
    assert(TerminalCanvas_styleAt(&canvas, 4, 6) & TERMINAL_STYLE_ACCENT);
    assert(TerminalCanvas_fgAt(&canvas, 4, 6) != TERMINAL_COLOR_DEFAULT);
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
    PrimeagenScreen_setBanner(pixels, 2, 1);
    assert(PrimeagenScreen_hasBanner());
    // The POLARITY is derived from the picture, not assumed: a bright paper is
    // inverted (paper -> blank) while a dark paper is left alone. That is why one
    // banner can be white-on-black and another black-on-white with no setting —
    // 68% of resources/banner.png is near-black, so it must NOT be inverted.
    PrimeagenScreen_setBanner(brightPixels, 2, 1);
    assert(TerminalImage_inverted(PrimeagenScreen_banner()));
    PrimeagenScreen_setBanner(pixels, 2, 1);
    const TerminalImage *stored = PrimeagenScreen_banner();
    assert(stored != nullptr && !TerminalImage_inverted(stored));
    size_t rampCount = 0;
    assert(TerminalImage_getColorRamp(stored, &rampCount) != nullptr && rampCount > 0);
    assert(TerminalImage_getNoise(stored) != nullptr);
    assert(TerminalImage_getNoiseDensity(stored) > 0);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, stored);
    // Edge to edge: the banner box's interior carries decor at both far corners,
    // so the field is sized by the panel rather than by the picture's fit rect.
    assert(AT(2, 26) != ' ');
    assert(AT(ROWS - 3, COLS - 3) != ' ');
    uint32_t paper[COLS];
    for (size_t x = 26; x < COLS - 2; ++x)
        paper[x] = AT(9, x);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 0, stored);
    int rescattered = 0;
    for (size_t x = 26; x < COLS - 2; ++x)
        if (AT(9, x) != paper[x])
            rescattered = 1;
    assert(rescattered);
    PrimeagenScreen_setBanner(nullptr, 0, 0);
    assert(!PrimeagenScreen_hasBanner() && PrimeagenScreen_banner() == nullptr);

    // --- present: one pass into a headless screen ---
    FILE *in = fopen("/dev/null", "r");
    FILE *out = fopen("/dev/null", "w");
    SCREEN *term = newterm(nullptr, out, in);
    assert(term && resizeterm(ROWS, COLS) != ERR);
    PrimeagenScreen_renderHome(&screen, "Primeagen", menu, 3, 1, &banner);
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
