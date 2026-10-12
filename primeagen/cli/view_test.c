/* MODULE: terminal panel view owner. Real offscreen ncurses cells verify text
 * symbols, shared borders, adaptive geometry and preview isolation; no user
 * appearance acceptance, real mouse device or provider routing is claimed. */
#define _XOPEN_SOURCE_EXTENDED 1
#include "cli/view.h"
#include <assert.h>
#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void) {
    assert(setlocale(LC_CTYPE, "en_US.UTF-8") != nullptr);
    FILE *in = tmpfile(), *out = tmpfile();
    assert(in && out);
    SCREEN *screen = newterm("xterm-256color", out, in);
    assert(screen && resizeterm(24, 100) != ERR);
    Tui state = Tui();
    int left = Primeagen_chatLeft(100, 32);
    assert(Tui_setSize(&state, 24, 100 - left));
    assert(Tui_add(&state, "live chat", 9));
    assert(Tui_setInput(&state, "draft", 5));
    char row[256];
    uint8_t styles[256];
    assert(Primeagen_drawPanels(&state, false, true, false, false, false, 0, 0, 32, row, styles, sizeof row));
    int caretY, caretX;
    getyx(stdscr, caretY, caretX);
    assert(caretY == 22 && caretX == left + 7);
    cchar_t cell;
    wchar_t text[CCHARW_MAX];
    attr_t attr;
    short pair;
    assert(mvin_wch(1, 2, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK);
    assert(text[0] == L'⌘' && (attr & A_REVERSE));
    assert(mvin_wch(3, 2, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK);
    assert(text[0] == L'▦' && !(attr & A_REVERSE));
    assert((mvinch(2, 2) & A_ALTCHARSET) != 0); // shared separator row
    assert((mvinch(2, left + 1) & A_CHARTEXT) == 'l');
    size_t scroll = Tui_getScroll(&state);
    assert(Primeagen_drawPanels(&state, false, true, false, false, false, 0, 1, 32, row, styles, sizeof row));
    assert(Tui_getScroll(&state) == scroll);
    assert((mvinch(2, left + 1) & A_CHARTEXT) == 'S');
    assert((mvinch(22, left + 2) & A_CHARTEXT) == ' ');
    assert(strcmp(Tui_getInput(&state), "draft") == 0);
    int panel = 2, selected = 5;
    assert(Primeagen_panelsClick(8, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == 1 && selected == 5);
    assert(!Primeagen_panelsClick(-1, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(4, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 100, 32, 9, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 100, 32, panel, 6, &panel, &selected));
    assert(panel == 1 && selected == 5); // rejection preserves both outputs
    Tui_clear(&state);
    assert(Tui_addStyled(&state, "**Hello 👋 café 中文 é**", strlen("**Hello 👋 café 中文 é**"), 0, true));
    assert(Primeagen_drawPanels(&state, false, true, false, false, false, 0, 0, 32, row, styles, sizeof row));
    assert(mvin_wch(2, left + 7, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK);
    assert(text[0] == (wchar_t) 0x1f44b && (attr & A_BOLD));
    assert(mvin_wch(2, left + 13, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK);
    assert(text[0] == L'é');
    assert(mvin_wch(2, left + 20, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK);
    assert(text[0] == L'e' && text[1] == (wchar_t) 0x301);
    const int widths[] = {20, 40, 63, 64, 80, 100, 200};
    for (size_t i = 0; i < sizeof widths / sizeof widths[0]; ++i) {
        int columns = widths[i];
        assert(resizeterm(10, columns) != ERR);
        left = Primeagen_chatLeft(columns, 32);
        assert(left == 0 || (left > PRIMEAGEN_RAIL_WIDTH && columns - left >= TUI_MIN_COLUMNS));
        assert(Tui_setSize(&state, 10, columns - left));
        assert(Primeagen_drawPanels(&state, false, false, false, false, false, 2, 5, 32, row, styles, sizeof row));
        assert(Primeagen_drawPanels(&state, false, true, false, false, false, 0, 0, 32, row, styles, sizeof row));
    }
    assert(!Primeagen_drawPanels(nullptr, false, false, false, false, false, 0, 0, 32, row, styles, sizeof row));
    assert(!Primeagen_drawPanels(&state, false, false, false, false, false, 0, 6, 32, row, styles, sizeof row));
    assert(resizeterm(24, 100) != ERR);
    assert(Tui_setSize(&state, 24, 63));
    assert(setlocale(LC_CTYPE, "C") != nullptr);
    assert(Primeagen_drawPanels(&state, false, false, false, false, false, 0, 0, 32, row, styles, sizeof row));
    assert((mvinch(1, 2) & A_CHARTEXT) == '@'); // usable non-UTF locale fallback
    assert((mvinch(2, 44) & A_CHARTEXT) == '?'); // no byte escapes or invalid terminal encoding
    endwin();
    delscreen(screen);
    fclose(in);
    fclose(out);
    Tui_free(&state);
}
