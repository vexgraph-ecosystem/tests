// 96 nested panels: verify every visible ring, deepest hit, visibility,
// repaint after mutation, resize retention, and recursive ownership teardown.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame/frame.h"
#include "nio/property_pool.h"

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); exit(1); } } while (0)

static void captureEvidence(const Image *image) {
    const char *dir = getenv("UI_BATTLE_ARTIFACT_DIR");
    if (!dir || !image || !Image_pixels(image)) return;
    const char *name = strrchr(__FILE__, '/');
    name = name ? name + 1 : __FILE__;
    char path[1024];
    int n = snprintf(path, sizeof path, "%s/%s.png", dir, name);
    if (n > 0 && (size_t) n < sizeof path &&
        Window_writePNG(Image_pixels(image), Image_stride(image), (int) Image_width(image), (int) Image_height(image), path))
        fprintf(stderr, "capture: %s\n", path);
}

static inline void pixel(const Image *image, int x, int y, Color expected) {
    CHECK(image && Image_pixels(image));
    CHECK(x >= 0 && y >= 0 && (uint32_t) x < Image_width(image) && (uint32_t) y < Image_height(image));
    const uint8_t *p = Image_pixels(image) + (size_t) y * Image_stride(image) + (size_t) x * 4;
    int want[4] = {Color_red(expected), Color_green(expected), Color_blue(expected), Color_alpha(expected)};
    for (int i = 0; i < 4; ++i) {
        if (abs((int) p[i] - want[i]) > 2) {
            fprintf(stderr, "FAIL pixel (%d,%d) channel %d: got %d expected %d\n", x, y, i, p[i], want[i]);
            captureEvidence(image);
            exit(1);
        }
    }
}


#define DEPTH 96
#define EXTENT (DEPTH * 4 + 16)
static Color layerColor(int i) { return i % 2 ? COLOR_RGBA(40, 180, 80, 255) : COLOR_RGBA(180, 40, 80, 255); }
int main(void) {
    uint32_t baseline = PropertyPool_live(PropertyPool_default());
    Frame *frame = Frame("matryoshka battle", EXTENT, EXTENT);
    CHECK(frame);
    Panel *layers[DEPTH];
    for (int i = 0; i < DEPTH; ++i) {
        layers[i] = Panel(EXTENT - i * 4, EXTENT - i * 4);
        CHECK(layers[i]);
        Panel_setBackground(layers[i], layerColor(i));
        if (i == 0) CHECK(Frame_add(frame, layers[i]) == layers[i]);
        else {
            Panel_setOffset(layers[i], 2, 2);
            CHECK(Panel_add(layers[i - 1], layers[i]) == layers[i]);
        }
    }
    Frame_show(frame);
    Image *shot = Frame_capture(frame);
    for (int i = 0; i < DEPTH; ++i) {
        pixel(shot, i * 2 + 1, EXTENT / 2, layerColor(i));
        CHECK(Panel_childCount(layers[i]) == (i == DEPTH - 1 ? 0 : 1));
    }
    CHECK(Element_hit(Frame_element(frame), EXTENT / 2, EXTENT / 2) == Panel_graphics(layers[DEPTH - 1]));
    Panel_setVisible(layers[DEPTH - 1], false);
    pixel(Frame_capture(frame), EXTENT / 2, EXTENT / 2, layerColor(DEPTH - 2));
    CHECK(Element_hit(Frame_element(frame), EXTENT / 2, EXTENT / 2) == Panel_graphics(layers[DEPTH - 2]));
    Panel_setVisible(layers[DEPTH - 1], true);
    Panel_setBackground(layers[DEPTH - 1], COLOR_WHITE);
    pixel(Frame_capture(frame), EXTENT / 2, EXTENT / 2, COLOR_WHITE);
    Frame_setSize(frame, EXTENT + 17, EXTENT + 11);
    shot = Frame_capture(frame);
    CHECK(Image_width(shot) == EXTENT + 17 && Image_height(shot) == EXTENT + 11);
    pixel(shot, EXTENT / 2, EXTENT / 2, COLOR_WHITE);
    Frame_destroy(frame);
    CHECK(PropertyPool_live(PropertyPool_default()) == baseline);
    puts("ui_matryoshka_battle_test: PASS (96 rings, mutation, hit, resize, reclamation)");
    return 0;
}
