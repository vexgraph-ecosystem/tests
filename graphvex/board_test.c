// tests/graphvex/board_test.c — mirrors src/board.c
//
// A retained offscreen target: owns its Image, publishes an atomic generation
// (the thing that wakes the render loop). NOT a swapchain.

#include <stdio.h>

#include "board.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

int main(void) {
    Board *b = Board_2(8, 8);
    CHECK(b != NULL);
    CHECK(Board_isValid(b));
    CHECK(Board_width(b) == 8 && Board_height(b) == 8);
    CHECK(Board_generation(b) == 0);             // never published
    CHECK(Board_image(b) != NULL);
    CHECK(Board_native(b) == NULL);

    // every publish advances the generation by exactly one
    Board_publish(b);
    CHECK(Board_generation(b) == 1);
    Board_publish(b);
    CHECK(Board_generation(b) == 2);

    // the retained image is the board's own pixels
    Board_fill(b, COLOR_RGBA(1, 2, 3, 4));
    uint8_t *px = Image_pixels(Board_image(b));
    CHECK(px != NULL);
    CHECK(px[0] == 1 && px[1] == 2 && px[2] == 3 && px[3] == 4);

    // resize recreates at the new size; same-size is a no-op true
    CHECK(Board_resize(b, 4, 4));
    CHECK(Board_width(b) == 4 && Board_height(b) == 4);
    CHECK(Board_resize(b, 4, 4));

    // default board is 1x1
    Board *d = Board_0();
    CHECK(Board_width(d) == 1 && Board_height(d) == 1);
    Board_destroy(d);

    // null-safe
    CHECK(!Board_isValid(NULL));
    CHECK(Board_width(NULL) == 0u);
    CHECK(Board_generation(NULL) == 0u);
    CHECK(Board_image(NULL) == NULL);
    CHECK(!Board_resize(NULL, 4, 4));
    Board_publish(NULL);            // must not crash
    Board_fill(NULL, COLOR_WHITE);
    Board_destroy(NULL);

    Board_destroy(b);
    printf("board_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
