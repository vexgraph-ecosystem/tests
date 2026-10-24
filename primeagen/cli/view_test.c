/* MODULE: workspace tree + persistent chat owner. A user expands IDE-like folders,
 * selects a conversation, and keeps the chat pane in place; native cells prove
 * folding, selection, divider and live-history isolation, not visual acceptance. */
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
    assert(start_color() == OK);
    assert(use_default_colors() == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_BACKGROUND, COLOR_WHITE, COLOR_BLACK) == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_SELECTED, COLOR_WHITE, COLOR_BLUE) == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_FOLDER, COLOR_YELLOW, COLOR_BLACK) == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_CHAT, COLOR_CYAN, COLOR_BLACK) == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_USER, COLOR_MAGENTA, COLOR_BLACK) == OK);
    assert(init_pair(PRIMEAGEN_PAIR_TREE_SERVER, COLOR_GREEN, COLOR_BLACK) == OK);
    Tui state = Tui();
    TerminalTreeView *tree = TerminalTreeView();
    assert(tree != nullptr);
    TerminalTreeNodeId chats = TerminalTreeView_addChild(tree, TERMINAL_TREE_NO_NODE, "chats", true);
    TerminalTreeNodeId recent = TerminalTreeView_addChild(tree, chats, "recent", true);
    TerminalTreeNodeId saved = TerminalTreeView_addChild(tree, recent, "Welcome", false);
    TerminalTreeNodeId notes = TerminalTreeView_addChild(tree, recent, "Project notes", false);
    TerminalTreeNodeId users = TerminalTreeView_addChild(tree, TERMINAL_TREE_NO_NODE, "users", true);
    TerminalTreeNodeId opencode = TerminalTreeView_addChild(tree, users, "OpenCode", false);
    TerminalTreeNodeId userExample = TerminalTreeView_addChild(tree, users, "example-user", false);
    TerminalTreeNodeId servers = TerminalTreeView_addChild(tree, TERMINAL_TREE_NO_NODE, "servers", true);
    TerminalTreeNodeId server = TerminalTreeView_addChild(tree, servers, "serverNumber1", true);
    TerminalTreeNodeId general = TerminalTreeView_addChild(tree, server, "#general", false);
    TerminalTreeNodeId privateFolder = TerminalTreeView_addChild(tree, TERMINAL_TREE_NO_NODE, "private chat", true);
    TerminalTreeNodeId privateConversation = TerminalTreeView_addChild(tree, privateFolder, "unsaved conversation", false);
    assert(chats && recent && saved && notes && users && opencode && userExample && servers && server && general && privateFolder && privateConversation);
    assert(TerminalTreeView_nodeCount(tree) == 12);
    assert(TerminalTreeView_childCountOf(tree, server) == 1);
    assert(TerminalTreeView_setExpanded(tree, chats, true));
    assert(TerminalTreeView_setExpanded(tree, recent, true));
    assert(TerminalTreeView_setExpanded(tree, users, true));
    assert(TerminalTreeView_setExpanded(tree, servers, true));
    assert(TerminalTreeView_setExpanded(tree, server, true));
    assert(TerminalTreeView_setExpanded(tree, privateFolder, true));
    assert(TerminalTreeView_setSelected(tree, opencode));
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
    // The header rule is the toolkit CONNECTOR run (striped), not a bare ACS_HLINE.
    // (mvinch moves the caret, so it must follow the getyx above.)
    assert((mvinch(1, left + 0) & A_CHARTEXT) == '-');
    assert((mvinch(1, left + 3) & A_CHARTEXT) == '.');
    cchar_t cell;
    wchar_t text[CCHARW_MAX];
    attr_t attr;
    short pair;
    assert((mvinch(0, 1) & A_CHARTEXT) == 'v');
    assert((mvinch(2, 1) & A_CHARTEXT) == '>');
    assert((mvinch(7, 1) & A_CHARTEXT) == '>');
    assert((mvinch(11, 1) & A_CHARTEXT) == '>');
    assert((mvinch(15, 1) & A_CHARTEXT) == '>');
    assert((mvinch(10, left - 1) & A_CHARTEXT) != ' '); // tree/chat divider
    assert((mvinch(13, 5) & A_CHARTEXT) == '#'); // one server channel
    assert(Primeagen_drawTreePanels(&state, tree, false, true, false, false, false,
                                    PRIMEAGEN_PANEL_PEOPLE, 0, 32, row, styles, sizeof row));
    assert(TerminalTreeView_visibleCount(tree) == 12);
    assert(mvin_wch(1, 1, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK && text[0] == L'⌄');
    assert(mvin_wch(1, 3, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK && text[0] == (wchar_t) 0xf07c);
    assert(mvin_wch(6, 5, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK && text[0] == (wchar_t) 0xf544);
    assert(PAIR_NUMBER(mvinch(6, 0)) == PRIMEAGEN_PAIR_TREE_SELECTED); // full-width selected-row rectangle
    assert(mvin_wch(10, left - 1, &cell) == OK);
    assert(getcchar(&cell, text, &attr, &pair, nullptr) == OK && text[0] == L' '); // no divider
    assert((mvinch(1, 5) & A_CHARTEXT) == 'c');
    assert((mvinch(3, 9) & A_CHARTEXT) == 'W');
    assert((mvinch(6, 7) & A_CHARTEXT) == 'O');
    assert((mvinch(8, 5) & A_CHARTEXT) == 's');
    assert((mvinch(10, 9) & A_CHARTEXT) == '#');
    assert(TerminalTreeView_nodeAtOffset(tree, 0) == chats);
    assert(TerminalTreeView_setViewportRows(tree, 3));
    assert(TerminalTreeView_setScrollRow(tree, 2) == 2);
    assert(TerminalTreeView_nodeAtOffset(tree, 0) == saved);
    assert(TerminalTreeView_setScrollRow(tree, 0) == 0);
    assert(TerminalTreeView_setViewportRows(tree, 19));
    assert(TerminalTreeView_toggle(tree, chats) == false);
    assert(TerminalTreeView_visibleCount(tree) == 9);
    assert(TerminalTreeView_toggle(tree, chats));
    assert(TerminalTreeView_visibleCount(tree) == 12);
    assert(TerminalTreeView_rowOf(tree, general) != TERMINAL_TREE_NO_ROW);
    assert(TerminalTreeView_setSelected(tree, general));
    assert(TerminalTreeView_getSelected(tree) == general);
    assert(Primeagen_chatLeft(100, 0) == 0);
    assert(Tui_setSize(&state, 24, 100));
    assert(Primeagen_drawTreePanels(&state, tree, false, true, false, false, false,
                                    PRIMEAGEN_PANEL_PEOPLE, 0, 0, row, styles, sizeof row));
    assert((mvinch(2, 1) & A_CHARTEXT) == 'l'); // hidden tree leaves the chat full-width
    assert(Tui_setSize(&state, 24, 100 - (left + 1)));
    assert(Primeagen_drawTreePanels(&state, tree, false, true, false, false, false,
                                    PRIMEAGEN_PANEL_PEOPLE, 0, 32, row, styles, sizeof row));
    assert((mvinch(2, left + 2) & A_CHARTEXT) == 'l');
    size_t scroll = Tui_getScroll(&state);
    assert(Primeagen_drawPanels(&state, false, true, false, false, false, PRIMEAGEN_PANEL_PEOPLE, 1, 32, row, styles, sizeof row));
    assert(Tui_getScroll(&state) == scroll);
    assert((mvinch(2, left + 1) & A_CHARTEXT) == 'u');
    assert((mvinch(22, left + 2) & A_CHARTEXT) == 'm'); // preview prompt, never the live draft
    assert(strcmp(Tui_getInput(&state), "draft") == 0);
    int panel = PRIMEAGEN_PANEL_PRIVATE, selected = 0;
    assert(Primeagen_drawPanels(&state, false, true, false, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(0, left + 1) & A_CHARTEXT) == '#');
    assert((mvinch(4, left + 1) & A_CHARTEXT) == 'T');
    assert(Primeagen_panelsClick(8, 3, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHATS && selected == 0);
    assert(Primeagen_panelsClick(8, 13, 24, 100, 32, PRIMEAGEN_PANEL_SERVERS, 0, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHANNELS && selected == 3);
    assert(!Primeagen_panelsClick(8, 14, 24, 100, 32, PRIMEAGEN_PANEL_SERVERS, 0, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHANNELS && selected == 3);
    assert(!Primeagen_panelsClick(-1, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(0, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 100, 32, 9, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 100, 32, panel, 7, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHANNELS && selected == 3); // rejection preserves both outputs
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
        assert(left == 0 || (left >= 12 && columns - left >= TUI_MIN_COLUMNS));
        assert(Tui_setSize(&state, 10, columns - left));
        assert(Primeagen_drawPanels(&state, false, false, false, false, false, PRIMEAGEN_PANEL_CHANNELS, 3, 32, row, styles, sizeof row));
        assert(Primeagen_drawPanels(&state, false, true, false, false, false, 0, 0, 32, row, styles, sizeof row));
    }
    assert(!Primeagen_drawPanels(nullptr, false, false, false, false, false, 0, 0, 32, row, styles, sizeof row));
    assert(!Primeagen_drawPanels(&state, false, false, false, false, false, 0, 7, 32, row, styles, sizeof row));
    assert(resizeterm(24, 100) != ERR);
    assert(Tui_setSize(&state, 24, 63));
    assert(setlocale(LC_CTYPE, "C") != nullptr);
    assert(Primeagen_drawPanels(&state, false, false, false, false, false, 0, 0, 32, row, styles, sizeof row));
    assert((mvinch(2, 1) & A_CHARTEXT) == '>'); // directory tree remains ASCII
    assert((mvinch(2, 44) & A_CHARTEXT) == '?'); // no byte escapes or invalid terminal encoding
    endwin();
    delscreen(screen);
    fclose(in);
    fclose(out);
    Tui_free(&state);
    TerminalTreeView_free(tree);
}
