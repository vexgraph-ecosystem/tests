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

// recording revalidate steps, to prove order + count
static int g_steps[8];
static int g_stepCount = 0;
static void step1(Board *board, void *ud) { (void)board; (void)ud; g_steps[g_stepCount++] = 1; }
static void step2(Board *board, void *ud) { (void)board; (void)ud; g_steps[g_stepCount++] = 2; }

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

    // revalidation: steps run in registration order, then the board publishes
    uint64_t genBefore = Board_generation(b);
    Board_addRevalidator(b, step1, NULL);
    Board_addRevalidator(b, step2, NULL);
    Board_revalidate(b);
    CHECK(g_stepCount == 2 && g_steps[0] == 1 && g_steps[1] == 2);
    CHECK(Board_generation(b) == genBefore + 1);

    // clearing drops every step; revalidate then runs none
    g_stepCount = 0;
    Board_clearRevalidators(b);
    Board_revalidate(b);
    CHECK(g_stepCount == 0);

    // null-safe
    Board_addRevalidator(NULL, step1, NULL);
    Board_clearRevalidators(NULL);
    Board_revalidate(NULL);            // must not crash

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
