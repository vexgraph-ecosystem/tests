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
    uint32_t cells[4] = {0, 0, 0, 0};
    TerminalCanvas c = TerminalCanvas(cells, nullptr, 2, 2);
    TerminalImage_render(&img, &c, 0, 0);
    assert(cells[0] == ' ' && cells[1] == '@');

    // Transparent pixels are skipped when requested.
    static const uint8_t clear[1 * 4] = { 255, 255, 255, 0 };
    TerminalImage ghost = TerminalImage(clear, 1, 1);
    uint32_t keep[1] = {'.'};
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
    uint32_t big[6 * 2];
    uint8_t bigStyles[6 * 2];
    TerminalCanvas region = TerminalCanvas(big, bigStyles, 6, 2);
    TerminalImage_setStyle(&img, TERMINAL_STYLE_DIM);
    TerminalImage_renderFit(&img, &region);
    assert(big[0] == ' ' && big[5] == '@');
    assert(TerminalCanvas_styleAt(&region, 0, 0) == TERMINAL_STYLE_DIM);

    // Colour plane: cells carry the sampled colour when color is on.
    static uint32_t cc[4];
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

    // Background NOISE: the blank "paper" becomes a glyph field, re-scattered by
    // the seed, so a revalidate draws a fresh decor while a fixed seed repeats.
    {
        static uint32_t noiseCells[8];
        static uint32_t noiseFg[8];
        TerminalCanvas noisyCanvas = TerminalCanvas(noiseCells, nullptr, noiseFg, nullptr, 8, 1);
        TerminalImage noisy = TerminalImage(two, 2, 1);
        TerminalImage_setInvert(&noisy, true);
        assert(!TerminalImage_setNoise(&noisy, "", 0));            // empty clears
        assert(TerminalImage_getNoise(&noisy) == nullptr);
        assert(!TerminalImage_setNoise(&noisy, nullptr, 0));
        assert(TerminalImage_setNoise(&noisy, "abcxyz", TERMINAL_RGB(0x40, 0x40, 0x40)));
        assert(TerminalImage_getNoise(&noisy) != nullptr);
        TerminalImage_setNoiseStyle(&noisy, TERMINAL_STYLE_DIM);
        assert(TerminalImage_getNoiseStyle(&noisy) == TERMINAL_STYLE_DIM);
        TerminalImage_setNoiseSeed(&noisy, 7);
        assert(TerminalImage_getNoiseSeed(&noisy) == 7);
        TerminalImage_render(&noisy, &noisyCanvas, 0, 0);
        assert(TerminalCanvas_glyphAt(&noisyCanvas, 0, 0) == '@');  // the ink half
        char first = (char) TerminalCanvas_glyphAt(&noisyCanvas, 1, 0);
        assert(strchr("abcxyz", first) != nullptr);                 // the paper half
        // Same seed -> the same scatter; a new seed -> (normally) a new one.
        TerminalImage_setNoiseSeed(&noisy, 7);
        TerminalImage_render(&noisy, &noisyCanvas, 0, 0);
        assert((char) TerminalCanvas_glyphAt(&noisyCanvas, 1, 0) == first);
        int reshuffled = 0;
        for (uint32_t seed = 1; seed <= 8 && !reshuffled; ++seed) {
            TerminalImage_setNoiseSeed(&noisy, seed);
            TerminalImage_render(&noisy, &noisyCanvas, 0, 0);
            if ((char) TerminalCanvas_glyphAt(&noisyCanvas, 1, 0) != first)
                reshuffled = 1;
        }
        assert(reshuffled);
        // Density: 100 is the default cover; 0 leaves the paper blank again.
        assert(TerminalImage_getNoiseDensity(&noisy) == 100);
        TerminalImage_setNoiseDensity(&noisy, 250);              // clamped to 100
        assert(TerminalImage_getNoiseDensity(&noisy) == 100);
        TerminalImage_setNoiseDensity(&noisy, 0);
        TerminalImage_render(&noisy, &noisyCanvas, 0, 0);
        assert(TerminalCanvas_glyphAt(&noisyCanvas, 1, 0) == ' ');
        // A clean image carries no noise.
        assert(TerminalImage_getNoise(&img) == nullptr);
    }

    // CONTAIN: uniform zoom-to-fit — one scale for both axes, centred, and the
    // cellAspect correction keeps a wide picture wide (letterboxed, never squashed).
    {
        static uint32_t containCells[8 * 4];
        static uint32_t containFg[8 * 4];
        TerminalCanvas contain = TerminalCanvas(containCells, nullptr, containFg, nullptr, 8, 4);
        TerminalCanvas_fill(&contain, ' ', TerminalPen());
        assert(TerminalImage_getCellAspect(&img) == 2.0);
        TerminalImage_renderContain(&img, &contain);
        // A 2x1 source at cellAspect 2 fits as 8x2, centred on rows 1..2.
        assert(TerminalCanvas_glyphAt(&contain, 7, 1) != ' ');   // the white half
        assert(TerminalCanvas_glyphAt(&contain, 7, 0) == ' ');   // letterbox top
        assert(TerminalCanvas_glyphAt(&contain, 7, 3) == ' ');   // letterbox bottom
        TerminalImage_setCellAspect(&img, 1.0);
        assert(TerminalImage_getCellAspect(&img) == 1.0);
        TerminalCanvas_fill(&contain, ' ', TerminalPen());
        TerminalImage_renderContain(&img, &contain);
        assert(TerminalCanvas_glyphAt(&contain, 7, 0) != ' ');   // square pixels: fills
        TerminalImage_setCellAspect(&img, 0.0);                  // rejected, keeps 1.0
        assert(TerminalImage_getCellAspect(&img) == 1.0);
        TerminalImage_renderContain(&img, nullptr);
        TerminalImage_renderContain(nullptr, &contain);
    }

    // Palette treatment: a mono mask renders in the purple ramp with a
    // deterministic per-cell scatter (the `>w<` idea).
    static uint32_t pc[2];
    static uint32_t pfg[2];
    TerminalCanvas paletteCanvas = TerminalCanvas(pc, nullptr, pfg, nullptr, 2, 1);
    size_t rampCount = 0;
    const uint32_t *purple = TerminalImage_rampPurple(&rampCount);
    assert(purple != nullptr && rampCount >= 4);
    assert(TerminalImage_setColorRamp(&img, purple, rampCount));
    assert(TerminalImage_getColorRamp(&img, &rampCount) == purple && rampCount >= 4);
    TerminalImage_setJitter(&img, 0, 0);
    TerminalImage_render(&img, &paletteCanvas, 0, 0);
    assert(TerminalCanvas_fgAt(&paletteCanvas, 0, 0) == purple[0]);                 // black -> darkest
    assert(TerminalCanvas_fgAt(&paletteCanvas, 1, 0) == purple[rampCount - 1]);     // white -> brightest
    TerminalImage_setJitter(&img, 60, 7);
    assert(TerminalImage_getJitter(&img) == 60 && TerminalImage_getJitterSeed(&img) == 7);
    TerminalImage_render(&img, &paletteCanvas, 0, 0);
    uint32_t scattered = TerminalCanvas_fgAt(&paletteCanvas, 0, 0);
    TerminalImage_render(&img, &paletteCanvas, 0, 0);
    assert(TerminalCanvas_fgAt(&paletteCanvas, 0, 0) == scattered);                 // deterministic for a seed
    TerminalImage_setJitter(&img, 0, 0);
    assert(TerminalImage_setColorRamp(&img, nullptr, 0));
    assert(TerminalImage_getColorRamp(&img, &rampCount) == nullptr && rampCount == 0);
    TerminalImage_render(&img, &paletteCanvas, 0, 0);
    assert(TerminalCanvas_fgAt(&paletteCanvas, 1, 0) == TERMINAL_RGB(255, 255, 255));
    REJECT(TerminalImage_setColorRamp(&img, nullptr, 4), "color ramp");

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
