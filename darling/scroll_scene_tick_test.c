#include "scroll_scene.h"

#include "lang/graphics.h"
#include "raster/raster_graphics.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    assert(Graphics_registerRow(RasterGraphics_getRow()));
    assert(Graphics_setGraphics(LANG_BACKEND_RASTER));
    ScrollScene_build();
    ScrollScene_paint(800, 600);

    // No motion or fade: the display link must not request another paint.
    ScrollScene_tick(10000u);
    assert(!ScrollScene_tick(10016u));

    // A scroll shows the overlay bar; its hold leaves pixels unchanged. The
    // first fading tick, the final hide, and only those visible changes paint.
    ScrollScene_setNestedOffset(6, 0.0f, 100.0f, 11000u);
    ScrollScene_tick(11016u);
    assert(!ScrollScene_tick(11032u));
    assert(ScrollScene_tick(12500u));
    assert(ScrollScene_tick(13000u));
    assert(!ScrollScene_tick(13016u));

    // Released overscroll must produce frame changes while the spring returns.
    ScrollScene_setNestedOffset(2, 0.0f,
        SCROLL_SCENE_V_CONTENT_H - SCROLL_SCENE_VIEW_H, 14000u);
    ScrollScene_scrollInputNested(2, 0.0f, 200.0f, 14000u);
    assert(ScrollScene_tick(14016u));
    ScrollScene_free();
    RasterGraphics_shutdown();
    puts("PASS scroll_scene_tick_test");
    return 0;
}
