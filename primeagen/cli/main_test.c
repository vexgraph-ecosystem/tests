/* Headless ncurses owner: real virtual-screen cells and injected mouse/key
 * decoding, not an interactive gallery or physical terminal acceptance. */
#define main previewMain
#include "cli/main.c"
#undef main
#include <assert.h>
int main(void) {
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
    assert(draw(&state, true, true, false, false, row, sizeof(row)));
    assert((mvinch(2, 1) & A_CHARTEXT) == 'v');
    assert((mvinch(18, 2) & A_CHARTEXT) == 'f');
    assert((mvinch(0, 45) & A_CHARTEXT) == '[');
    for (int i = 0; i < 30; ++i)
        assert(Tui_add(&state, "extra", 5));
    Tui_scrollBy(&state, 10);
    assert(draw(&state, false, false, true, true, row, sizeof(row)));
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
    assert(ungetch(KEY_PPAGE) == OK && getch() == KEY_PPAGE);
    assert(resizeterm(2, 10) != ERR);
    assert(Tui_setSize(&state, 2, 10));
    assert(draw(&state, false, true, false, false, row, sizeof(row)));
    endwin(); delscreen(screen);
    fclose(input); fclose(output); Tui_free(&state);
}
