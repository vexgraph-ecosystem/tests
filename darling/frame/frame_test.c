// tests/darling/frame/frame_test.c — mirrors darling-framework/src/frame
//
// The Frame: a real window. Lifecycle, the ownership tree, the one resize
// surface, background/transparency/blur, panels, and capture.

#include <stdio.h>

#include "frame/frame.h"
#include "panel/panel.h"
#include "ui/element.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

static void onClosed(Frame *frame, void *ud) {
    (void)frame;
    *(int *)ud += 1;
}

int main(void) {
    Frame *f = Frame("frame test", 400, 300);
    CHECK(f != NULL);
    CHECK(Frame_window(f) != NULL);
    CHECK(!Frame_isClosed(f));
    CHECK(Frame_active() == f);

    // the content element IS the layout root
    Element *root = Frame_element(f);
    CHECK(root != NULL);

    // one resize surface: setSize -> root tracks the new size
    Frame_setSize(f, 400, 300);
    Rect r = Frame_root(f);
    CHECK(r.w == 400.0f && r.h == 300.0f);
    Frame_setSize(f, 640, 360);
    r = Frame_root(f);
    CHECK(r.w == 640.0f && r.h == 360.0f);

    // title / background
    Frame_setTitle(f, "renamed");
    Frame_setBackgroundColor(f, COLOR_RGBA(10, 20, 30, 255));
    CHECK(Frame_background(f) == COLOR_RGBA(10, 20, 30, 255));
    Frame_setBackground(f, COLOR_CLEAR);       // transparent colour -> see-through window
    CHECK(Frame_background(f) == COLOR_CLEAR);

    // transparency + blur are settable without crashing (OS-backed; both ways)
    Frame_setTransparent(f, true);
    Frame_setTransparent(f, false);
    Frame_setBlur(f, 12.0f);
    Frame_setBlur(f, 0.0f);

    // panels: add / count / index / remove
    ElementDesc d = {0};
    d.width = 100; d.height = 40;
    d.anchor = PART_CENTER; d.pivot = PART_CENTER;
    d.background = COLOR_RGBA(200, 100, 100, 255);
    Panel *p0 = Frame_addPanel(f, &d);
    Panel *p1 = Frame_addPanel(f, &d);
    CHECK(p0 != NULL && p1 != NULL);
    CHECK(Frame_count(f) == 2);
    CHECK(Frame_panel(f, 0) == p0 && Frame_panel(f, 1) == p1);
    CHECK(Frame_panel(f, 5) == NULL);
    Frame_removePanels(f);
    CHECK(Frame_count(f) == 0);

    // capture re-renders and hands back pixels
    Frame_addPanel(f, &d);
    Frame_render(f);
    Image *shot = Frame_capture(f);
    CHECK(shot != NULL);
    CHECK(Image_width(shot) == 640 && Image_height(shot) == 360);

    // hide / show don't crash and keep the frame alive
    Frame_hide(f);
    Frame_show(f);
    CHECK(!Frame_isClosed(f));

    // ownership tree: closing the OWNER closes the children
    Frame *child = Frame("child", 200, 150);
    Frame_setOwner(child, f);
    CHECK(Frame_owner(child) == f);
    int closed = 0;
    Frame_onClose(child, onClosed, &closed);
    Frame_close(f);                    // frees f AND child
    CHECK(closed == 1);                // the child's close hook fired

    printf("frame_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
