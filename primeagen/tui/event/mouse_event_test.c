/* Mouse owner: named cell boundaries, wheel viewport, one-action clicks;
 * normalized input only, not real terminal mouse support/appearance proof. */
#include "tui/event/mouse_event.h"
#include <assert.h>
int main(void) {
    Tui state = Tui(1, 1);
    assert(Tui_setSize(&state, 10, 40));
    for (int i = 0; i < 20; ++i)
        assert(Tui_add(&state, "line", 4));
    assert(TuiMouse_dispatch(&state, 2, 2, TUI_MOUSE_WHEEL_UP) == TUI_MOUSE_NONE);
    assert(Tui_getScroll(&state) == 1);
    TuiMouse_dispatch(&state, 2, 2, TUI_MOUSE_WHEEL_DOWN);
    assert(Tui_getScroll(&state) == 0);
    TuiMouse_dispatch(&state, 2, 8, TUI_MOUSE_WHEEL_UP);
    assert(Tui_getScroll(&state) == 0);
    assert(TuiMouse_dispatch(&state, 25, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_CLEAR);
    assert(TuiMouse_dispatch(&state, 31, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_CLEAR);
    assert(TuiMouse_dispatch(&state, 32, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(&state, 33, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_QUIT);
    assert(TuiMouse_dispatch(&state, 39, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_QUIT);
    assert(TuiMouse_dispatch(&state, 2, 8, TUI_MOUSE_CLICK) == TUI_MOUSE_INPUT);
    assert(TuiMouse_dispatch(&state, 40, 8, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(&state, -1, 8, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(&state, 1, -1, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(&state, 1, 10, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(&state, 33, 0, (TuiMouseKind) 99) == TUI_MOUSE_NONE);
    assert(TuiMouse_dispatch(nullptr, 0, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    assert(Tui_setSize(&state, 1, 1));
    assert(TuiMouse_dispatch(&state, 0, 0, TUI_MOUSE_CLICK) == TUI_MOUSE_NONE);
    Tui_free(&state);
}
