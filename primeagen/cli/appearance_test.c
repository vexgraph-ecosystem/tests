// Owner: PrimeagenAppearance — the chosen look and its connector rule drawing.
// Headless ncurses; no interactive terminal.
#define _XOPEN_SOURCE_EXTENDED 1
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "cli/appearance.h"

int main(void) {
    assert(setlocale(LC_CTYPE, "en_US.UTF-8") != nullptr);
    FILE *in = fopen("/dev/null", "r");
    FILE *out = fopen("/dev/null", "w");
    assert(in && out);
    SCREEN *screen = newterm(nullptr, out, in);
    assert(screen && resizeterm(6, 40) != ERR);

    // Default: the toolkit's striped connector.
    assert(strcmp(PrimeagenAppearance_name(), "ascii-striped") == 0);
    const TerminalAppearance *look = PrimeagenAppearance_current();
    assert(look != nullptr);
    TerminalRule rule = TerminalAppearance_getRule(look);
    assert(rule.horizontal == '-' && rule.vertical == '|' && rule.stripe == '.');

    // Adopt a named preset, and an unknown name falls back to the default.
    PrimeagenAppearance_use("unicode-emdash");
    assert(strcmp(PrimeagenAppearance_name(), "unicode-emdash") == 0);
    rule = TerminalAppearance_getRule(PrimeagenAppearance_current());
    assert(rule.horizontal == 0x2014u /* — */);
    PrimeagenAppearance_use("no-such-look");
    rule = TerminalAppearance_getRule(PrimeagenAppearance_current());
    assert(rule.stripe == '.');            // default
    PrimeagenAppearance_use(nullptr);
    PrimeagenAppearance_use("ascii-solid");

    // The rule blit: a solid run of dashes.
    assert(PrimeagenAppearance_drawRule(stdscr, 0, 0, 10));
    assert((mvinch(0, 0) & A_CHARTEXT) == '-');
    assert((mvinch(0, 9) & A_CHARTEXT) == '-');

    // A striped run lands the stripe glyph on the period.
    PrimeagenAppearance_use("ascii-striped");
    assert(PrimeagenAppearance_drawRule(stdscr, 1, 4, 12));
    assert((mvinch(1, 4) & A_CHARTEXT) == '-');
    assert((mvinch(1, 4 + 3) & A_CHARTEXT) == '.');
    assert((mvinch(1, 4 + 7) & A_CHARTEXT) == '.');

    // A Unicode preset blits a real codepoint.
    PrimeagenAppearance_use("unicode-light");
    assert(PrimeagenAppearance_drawRule(stdscr, 2, 0, 4));
    cchar_t cell;
    wchar_t text[8];
    attr_t gotAttr;
    short gotPair;
    assert(mvin_wch(2, 0, &cell) == OK);
    assert(getcchar(&cell, text, &gotAttr, &gotPair, nullptr) == OK);
    assert(text[0] == L'\u2500');

    // Rejections and clipping.
    assert(!PrimeagenAppearance_drawRule(nullptr, 0, 0, 4));
    assert(!PrimeagenAppearance_drawRule(stdscr, 0, 0, 0));
    assert(!PrimeagenAppearance_drawRule(stdscr, -1, 0, 4));
    assert(!PrimeagenAppearance_drawRule(stdscr, 0, -1, 4));
    assert(PrimeagenAppearance_drawRule(stdscr, 3, 30, 40));   // clips at the screen edge

    endwin();
    delscreen(screen);
    fclose(in);
    fclose(out);
    printf("appearance_test: PASS\n");
    return 0;
}
