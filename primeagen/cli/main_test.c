/* Headless ncurses owner: real virtual-screen cells and injected mouse/key
 * decoding, not an interactive gallery or physical terminal acceptance. */
#define main previewMain
#include "cli/main.c"
#undef main
#include <assert.h>

static bool mouseSuffix(int key, const char *suffix, int *x, int *y, TuiMouseKind *kind) {
    size_t length = strlen(suffix);
    while (length > 0)
        assert(ungetch((unsigned char) suffix[--length]) == OK);
    return readMouse(key, x, y, kind);
}

int main(int argc, char **argv) {
    if (argc == 3 && strcmp(argv[1], "--connect-probe") == 0)
        return connectHost(argv[2], true);
    FILE *input = tmpfile(); FILE *output = tmpfile();
    assert(input != nullptr && output != nullptr);
    SCREEN *screen = newterm("xterm-256color", output, input);
    assert(screen != nullptr);
    assert(resizeterm(20, 60) != ERR);
    Tui state = Tui();
    assert(Tui_setSize(&state, 20, 60));
    assert(Tui_add(&state, "visible", 7));
    assert(Tui_setInput(&state, "fixed", 5));
    char row[128];
    uint8_t styles[128];
    assert(draw(&state, true, true, false, false, false, row, styles, sizeof(row)));
    assert((mvinch(2, 1) & A_CHARTEXT) == 'v');
    assert((mvinch(18, 2) & A_CHARTEXT) == 'f');
    assert((mvinch(0, 45) & A_CHARTEXT) == '[');
    assert(Tui_setInput(&state, "/help", 5));
    assert(command(&state, false) == TUI_COMMAND_MESSAGE);
    assert(Tui_getInputLength(&state) == 0);
    assert(Tui_setInput(&state, "/connect", 8));
    assert(command(&state, false) == TUI_COMMAND_CONNECT);
    assert(command(&state, true) == TUI_COMMAND_MESSAGE);
    assert(Tui_setInput(&state, "/server add", 11));
    size_t before = Tui_getCount(&state);
    assert(command(&state, true) == TUI_COMMAND_MESSAGE);
    assert(Tui_getCount(&state) == before + 1);
    assert(Tui_setInput(&state, "/quit", 5));
    assert(command(&state, true) == TUI_COMMAND_QUIT);
    assert(Tui_setInput(&state, "/update", 7));
    assert(command(&state, true) == TUI_COMMAND_UPDATE);
    assert(strcmp(approvalReply('o'), "once") == 0 && strcmp(approvalReply('y'), "once") == 0);
    assert(strcmp(approvalReply('a'), "always") == 0);
    assert(strcmp(approvalReply('r'), "reject") == 0 && strcmp(approvalReply('n'), "reject") == 0);
    assert(approvalReply('x') == nullptr && approvalReply('\n') == nullptr && approvalReply(TUI_CTRL_Q) == nullptr);
    // Real protocol suffixes, not fabricated fifth-button/modifier bits.
    int mouseX = -1, mouseY = -1;
    TuiMouseKind mouseKind;
    assert(mouseSuffix(TUI_MOUSE_SGR, "64;50;4M", &mouseX, &mouseY, &mouseKind));
    assert(mouseKind == TUI_MOUSE_WHEEL_UP && mouseX == 49 && mouseY == 3);
    assert(mouseSuffix(TUI_MOUSE_SGR, "65;50;4M", &mouseX, &mouseY, &mouseKind));
    assert(mouseKind == TUI_MOUSE_WHEEL_DOWN);
    assert(mouseSuffix(TUI_MOUSE_SGR, "4;50;4M", &mouseX, &mouseY, &mouseKind));
    assert(mouseKind == TUI_MOUSE_CLICK); // shift-click is never wheel-down
    assert(mouseSuffix(TUI_MOUSE_SGR, "81;50;4M", &mouseX, &mouseY, &mouseKind));
    assert(mouseKind == TUI_MOUSE_WHEEL_DOWN); // modifier plus wheel
    assert(mouseSuffix(TUI_MOUSE_X10, "aR$", &mouseX, &mouseY, &mouseKind));
    assert(mouseKind == TUI_MOUSE_WHEEL_DOWN && mouseX == 49 && mouseY == 3);
    for (size_t i = 0; i < 4; ++i) {
        const char *bad[] = {"65;0;4M", "65;99999;4M", "65;50;4m", "32;50;4M"};
        assert(!mouseSuffix(TUI_MOUSE_SGR, bad[i], &mouseX, &mouseY, &mouseKind));
        assert(mouseX == 49 && mouseY == 3);
    }
    drawApproval(20, 0, 60);
    assert((mvinch(18, 0) & A_CHARTEXT) == 'A');
    // The speaker label is opened once per turn, then reused for notes/answers.
    bool open = false;
    size_t mark = Tui_getCount(&state);
    assert(openTurn(&state, &open) && open && Tui_getCount(&state) == mark + 1);
    assert(openTurn(&state, &open) && Tui_getCount(&state) == mark + 1);
    assert(Tui_setInput(&state, "ordinary text", 13));
    assert(command(&state, true) == 0);
    assert(Tui_getInputLength(&state) == 13);
    assert(Tui_setInput(&state, "/clear", 6));
    assert(command(&state, true) == TUI_COMMAND_MESSAGE);
    assert(Tui_getCount(&state) == 0 && Tui_getInputLength(&state) == 0);
    assert(Tui_setInput(&state, "fixed", 5));
    for (int i = 0; i < 30; ++i)
        assert(Tui_add(&state, "extra", 5));
    Tui_scrollBy(&state, 10);
    size_t scrollBefore = Tui_getScroll(&state);
    assert(mouseSuffix(TUI_MOUSE_SGR, "65;2;4M", &mouseX, &mouseY, &mouseKind));
    TuiMouse_dispatch(&state, mouseX, mouseY, mouseKind);
    assert(Tui_getScroll(&state) < scrollBefore);
    assert(draw(&state, false, false, true, true, false, row, styles, sizeof(row)));
    assert((mvinch(19, 0) & A_CHARTEXT) == 'O');
    assert((mvinch(18, 2) & A_CHARTEXT) == 'f');
    MEVENT event = {0};
    mousemask(BUTTON1_PRESSED, nullptr);
    event.x = 46; event.y = 0; event.bstate = BUTTON1_PRESSED;
    assert(ungetmouse(&event) == OK);
    assert(getch() == KEY_MOUSE);
    MEVENT received;
    assert(getmouse(&received) == OK);
    assert(TuiMouse_dispatch(&state, received.x, received.y, TUI_MOUSE_CLICK) == TUI_MOUSE_CLEAR);
    assert(ungetch(KEY_DOWN) == OK && getch() == KEY_DOWN);
    Tui_clear(&state);
    start_color();
    use_default_colors();
    init_pair(TUI_PAIR_HEADER, COLOR_CYAN, -1);
    init_pair(TUI_PAIR_CODE, COLOR_YELLOW, -1);
    init_pair(TUI_PAIR_INPUT, COLOR_GREEN, -1);
    assert(Tui_addStyled(&state, "You:", 4, TEXT_STYLE_USER | TEXT_STYLE_BOLD, false));
    const char *markdown = "# Title\n**bold** and *soft* `code`\n\n\nnext";
    assert(Tui_addStyled(&state, markdown, strlen(markdown), 0, true));
    assert(draw(&state, false, false, true, false, false, row, styles, sizeof(row)));
    assert((mvinch(2, 1) & A_BOLD) != 0);
    assert(PAIR_NUMBER(mvinch(2, 1)) == TUI_PAIR_INPUT);
    assert((mvinch(3, 1) & A_CHARTEXT) == 'T');
    assert((mvinch(3, 1) & A_BOLD) != 0);
    assert(PAIR_NUMBER(mvinch(3, 1)) == TUI_PAIR_HEADER);
    assert((mvinch(4, 1) & A_CHARTEXT) == 'b');
    assert((mvinch(4, 1) & A_BOLD) != 0);
    assert((mvinch(4, 10) & A_UNDERLINE) != 0);
    assert(PAIR_NUMBER(mvinch(4, 15)) == TUI_PAIR_CODE);
    assert((mvinch(6, 1) & A_CHARTEXT) == 'n');
    // The four-type browser paints only OpenCode's resident chat; local examples
    // keep its draft/history private from users, servers, saved chats and DMs.
    assert(resizeterm(24, 100) != ERR);
    int left = Primeagen_chatLeft(100, PRIMEAGEN_LIST_WIDTH_DEFAULT);
    assert(left == 32);
    assert(Primeagen_chatLeft(40, 32) == 0);
    assert(Primeagen_chatLeft(100, -1) == 0);
    assert(Primeagen_chatLeft(100, 1000) == 50);
    assert(Tui_setSize(&state, 24, 100 - left));
    int panel = PRIMEAGEN_PANEL_PEOPLE, selected = 0;
    assert(Primeagen_drawPanels(&state, false, true, true, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(0, 1) & A_CHARTEXT) == 'v');
    assert((mvinch(2, 1) & A_CHARTEXT) == '>');
    assert((mvinch(7, 1) & A_CHARTEXT) == '>');
    assert((mvinch(11, 1) & A_CHARTEXT) == '>');
    assert((mvinch(15, 1) & A_CHARTEXT) == '>');
    assert((mvinch(10, left - 1) & A_CHARTEXT) != ' '); // one divider separates tree and chat
    assert((mvinch(13, 5) & A_CHARTEXT) == '#'); // the tree has one channel
    assert((mvinch(0, left + 1) & A_CHARTEXT) == 'O');
    assert((mvinch(22, left + 2) & A_CHARTEXT) == 'f');
    size_t retainedCount = Tui_getCount(&state);
    assert(Primeagen_panelsClick(8, 9, 24, 100, 32, panel, selected, &panel, &selected));
    assert(selected == 1);
    assert(Primeagen_drawPanels(&state, false, true, true, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(2, left + 1) & A_CHARTEXT) == 'u');
    assert((mvinch(4, left + 1) & A_CHARTEXT) == 'U'); // User-chat preview only
    assert((mvinch(22, left + 2) & A_CHARTEXT) == 'm'); // mock composer, not live draft
    assert(Tui_getCount(&state) == retainedCount && strcmp(Tui_getInput(&state), "fixed") == 0);
    assert(Primeagen_panelsClick(8, 11, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_SERVERS);
    assert(Primeagen_panelsClick(8, 13, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHANNELS && selected == 3);
    assert(Primeagen_drawPanels(&state, false, false, false, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(0, left + 1) & A_CHARTEXT) == '#');
    assert((mvinch(0, left + 2) & A_CHARTEXT) == 'g'); // #general chat context
    assert((mvinch(13, 5) & A_CHARTEXT) == '#'); // the only server channel
    assert(Primeagen_panelsClick(8, 3, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_CHATS && selected == 0);
    assert(Primeagen_drawPanels(&state, false, false, false, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(0, left + 1) & A_CHARTEXT) == 'c');
    assert(Primeagen_panelsClick(8, 15, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_PRIVATE && selected == 0);
    assert(Primeagen_drawPanels(&state, false, false, false, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(0, left + 1) & A_CHARTEXT) == '#');
    assert((mvinch(4, left + 1) & A_CHARTEXT) == 'T'); // unsaved private chat
    assert(Primeagen_panelsClick(8, 7, 24, 100, 32, panel, selected, &panel, &selected));
    assert(Primeagen_panelsClick(8, 8, 24, 100, 32, panel, selected, &panel, &selected));
    assert(panel == PRIMEAGEN_PANEL_PEOPLE && selected == 0);
    assert(!Primeagen_panelsClick(0, 1, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 14, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(left, 2, 24, 100, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 40, 32, panel, selected, &panel, &selected));
    assert(!Primeagen_panelsClick(8, 2, 24, 100, 32, panel, selected, nullptr, &selected));
    assert(!Primeagen_drawPanels(&state, false, false, false, false, false, -1, 0, 32, row, styles, sizeof row));
    assert(Primeagen_drawPanels(&state, false, true, true, false, false, panel, selected, 32, row, styles, sizeof row));
    assert((mvinch(2, left + 1) & A_CHARTEXT) == 'Y'); // live history restored
    assert(resizeterm(2, 10) != ERR);
    assert(Tui_setSize(&state, 2, 10));
    assert(draw(&state, false, true, false, false, false, row, styles, sizeof(row)));
    endwin(); delscreen(screen);
    fclose(input); fclose(output); Tui_free(&state);
}
