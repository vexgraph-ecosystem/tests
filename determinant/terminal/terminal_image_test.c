// Owner: TerminalImage — picture → cells. Luminance/glyph mapping, box sample,
// 1:1 render, region-fit render, transparency skip, invert, ramp, projections.
#include "terminal/terminal_image.h"
#include "determinant/support.h"

int main(void) {
    // 2x1 image: black then white.
    static const uint8_t two[2 * 1 * 4] = {
        0, 0, 0, 255,
        255, 255, 255, 255,
    };
    TerminalImage img = TerminalImage(two, 2, 1);
    assert(TerminalImage_isValid(&img));
    assert(TerminalImage_getWidth(&img) == 2 && TerminalImage_getHeight(&img) == 1);
    assert(TerminalImage_luminance(0, 0, 0) == 0);
    assert(TerminalImage_luminance(255, 255, 255) == 255);

    assert(TerminalImage_glyph(&img, 0) == ' ');
    assert(TerminalImage_glyph(&img, 255) == '@');
    TerminalImage_setInvert(&img, true);
    assert(TerminalImage_glyph(&img, 0) == '@');
    assert(TerminalImage_glyph(&img, 255) == ' ');
    TerminalImage_setInvert(&img, false);

    // 1:1 render into cells.
    char cells[4] = {0, 0, 0, 0};
    TerminalCanvas c = TerminalCanvas(cells, nullptr, 2, 2);
    TerminalImage_render(&img, &c, 0, 0);
    assert(cells[0] == ' ' && cells[1] == '@');

    // Transparent pixels are skipped when requested.
    static const uint8_t clear[1 * 4] = { 255, 255, 255, 0 };
    TerminalImage ghost = TerminalImage(clear, 1, 1);
    char keep[1] = {'.'};
    TerminalCanvas k = TerminalCanvas(keep, nullptr, 1, 1);
    TerminalImage_render(&ghost, &k, 0, 0);
    assert(keep[0] == '.');
    TerminalImage_setSkipTransparent(&ghost, false);
    TerminalImage_render(&ghost, &k, 0, 0);
    assert(keep[0] == '@');

    // Box sampling: a 2x2 checker averaged into one cell lands mid-luminance.
    static const uint8_t checker[4 * 4] = {
        0, 0, 0, 255,   255, 255, 255, 255,
        255, 255, 255, 255,   0, 0, 0, 255,
    };
    TerminalImage box = TerminalImage(checker, 2, 2);
    uint8_t r = 0, g = 0, b = 0, a = 0;
    assert(TerminalImage_sampleCell(&box, 0, 0, 1, 1, &r, &g, &b, &a));
    assert(r == 127 && g == 127 && b == 127 && a == 255);
    assert(TerminalImage_sampleCell(&box, 5, 0, 1, 1, &r, &g, &b, &a) == false);

    // renderFit covers a whole region deterministically.
    char big[6 * 2];
    uint8_t bigStyles[6 * 2];
    TerminalCanvas region = TerminalCanvas(big, bigStyles, 6, 2);
    TerminalImage_setStyle(&img, TERMINAL_STYLE_DIM);
    TerminalImage_renderFit(&img, &region);
    assert(big[0] == ' ' && big[5] == '@');
    assert(TerminalCanvas_styleAt(&region, 0, 0) == TERMINAL_STYLE_DIM);

    // Colour plane: cells carry the sampled colour when color is on.
    static char cc[4];
    static uint32_t cf[4];
    TerminalCanvas colored = TerminalCanvas(cc, nullptr, cf, nullptr, 2, 2);
    TerminalImage_render(&img, &colored, 0, 0);
    assert(TerminalCanvas_fgAt(&colored, 0, 0) == TERMINAL_RGB(0, 0, 0));
    assert(TerminalCanvas_fgAt(&colored, 1, 0) == TERMINAL_RGB(255, 255, 255));
    assert(TerminalImage_hasColor(&img));
    TerminalImage_setColor(&img, false);
    assert(!TerminalImage_hasColor(&img));
    TerminalImage_render(&img, &colored, 0, 0);
    assert(TerminalCanvas_fgAt(&colored, 1, 0) == TERMINAL_COLOR_DEFAULT);
    TerminalImage_setColor(&img, true);

    // Ramp + config.
    assert(TerminalImage_setRamp(&img, "01") && strcmp(TerminalImage_getRamp(&img), "01") == 0);
    assert(TerminalImage_glyph(&img, 0) == '0' && TerminalImage_glyph(&img, 255) == '1');
    assert(TerminalImage_getStyle(&img) == TERMINAL_STYLE_DIM);
    assert(TerminalImage_skipsTransparent(&img));
    assert(!TerminalImage_inverted(&img));

    // Rejections + invalid image safety.
    assert(!TerminalImage_isValid(nullptr));
    assert(TerminalImage_getPixels(nullptr) == nullptr);
    TerminalImage zero = TerminalImage_0();
    assert(!TerminalImage_isValid(&zero));
    REJECT(TerminalImage_setRamp(&zero, nullptr), "null or empty");
    TerminalImage_render(&zero, &region, 0, 0);   // no-op
    TerminalImage_renderFit(&zero, &region);
    TerminalImage_renderFit(&img, nullptr);
    REJECT(TerminalImage_setPixels(&zero, nullptr, 2, 2), "extent");
    REJECT(TerminalImage_setRamp(&zero, ""), "empty");
    char text[64]; bool cut = true; size_t before = 0;
    (void) before;
    REJECT(TerminalImage_toString(&zero, text, 1, &cut), "truncated");
    assert(cut);
    TerminalImage rejected = TerminalImage(nullptr, 2, 2);
    assert(!TerminalImage_isValid(&rejected));

    PROJECT(TerminalImage, &img);
    return 0;
}
