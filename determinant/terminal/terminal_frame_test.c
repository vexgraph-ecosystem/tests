// Owner: TerminalFrame — bare region layout. Regions, bands, rails collapse,
// rules, background paint, small-root degradation, projections.
#include "terminal/terminal_frame.h"
#include "determinant/support.h"

int main(void) {
    enum { W = 40, H = 10 };
    static char cells[W * H];
    static uint8_t styles[W * H];
    static uint32_t fg[W * H];
    static uint32_t bg[W * H];

    TerminalFrame frame = TerminalFrame(10, 10);
    assert(TerminalFrame_getBackground(&frame) == nullptr);
    size_t hdr = 0, ftr = 0;
    TerminalFrame_getBands(&frame, &hdr, &ftr);
    assert(hdr == TERMINAL_FRAME_HEADER_ROWS_DEFAULT && ftr == 0);
    assert(TerminalFrame_setBands(&frame, 2, 3));

    TerminalCanvas root = TerminalCanvas(cells, styles, fg, bg, W, H);
    TerminalCanvas_fill(&root, ' ', TerminalPen());

    size_t l = 0, r = 0;
    bool rails = false;
    TerminalFrame_getLayout(&frame, &root, &l, &r, &rails);
    assert(rails && l == 10 && r == 10);

    TerminalCanvas header = TerminalFrame_header(&frame, &root);
    TerminalCanvas left = TerminalFrame_left(&frame, &root);
    TerminalCanvas center = TerminalFrame_center(&frame, &root);
    TerminalCanvas right = TerminalFrame_right(&frame, &root);
    TerminalCanvas footer = TerminalFrame_footer(&frame, &root);
    assert(TerminalCanvas_getHeight(&header) == 2 && TerminalCanvas_getWidth(&header) == W);
    assert(TerminalCanvas_getWidth(&left) == 10 && TerminalCanvas_getHeight(&left) == 5);
    assert(TerminalCanvas_getWidth(&right) == 10 && TerminalCanvas_getHeight(&right) == 5);
    assert(TerminalCanvas_getWidth(&center) == W - 10 - 10 - 2 && TerminalCanvas_getHeight(&center) == 5);
    assert(TerminalCanvas_getWidth(&footer) == W - 10 - 10 - 2 && TerminalCanvas_getHeight(&footer) == 3);

    // Regions are disjoint views over one grid.
    TerminalCanvas_text(&center, 0, 0, "C", 1, TerminalPen());
    assert(TerminalCanvas_charAt(&root, 11, 2) == 'C');
    assert(TerminalCanvas_charAt(&left, 0, 0) == ' ');

    TerminalFrame_drawRules(&frame, &root, TerminalPen_1(TERMINAL_STYLE_DIM));
    assert(TerminalCanvas_charAt(&root, 10, 1) == '+');       // junction: run meets the left rail
    assert(TerminalCanvas_charAt(&root, W - 10 - 1, 1) == '+'); // junction: run meets the right rail
    assert(TerminalCanvas_charAt(&root, 5, 1) == '-');        // run between the rails
    assert(TerminalCanvas_charAt(&root, 10, 2) == '|');       // rail below the junction
    assert(TerminalCanvas_charAt(&root, W - 10 - 1, 2) == '|');

    // A striped connector run, and a custom glyph set.
    TerminalRule striped = TerminalRule('-', '|', '.', 4);
    assert(TerminalFrame_setRule(&frame, striped));
    TerminalRule got = TerminalRule_0();
    TerminalFrame_getRule(&frame, &got);
    assert(got.stripe == '.' && got.stripeEvery == 4);
    TerminalFrame_drawRules(&frame, &root, TerminalPen_1(TERMINAL_STYLE_DIM));
    assert(TerminalCanvas_charAt(&root, 3, 1) == '.');        // every 4th cell is the stripe
    assert(TerminalCanvas_charAt(&root, 4, 1) == '-');

    // A very narrow root collapses the rails so the centre stays usable.
    static char narrow[6 * 6];
    TerminalCanvas small = TerminalCanvas(narrow, nullptr, 6, 6);
    TerminalFrame_getLayout(&frame, &small, &l, &r, &rails);
    assert(!rails && l == 0 && r == 0);
    TerminalCanvas centerSmall = TerminalFrame_center(&frame, &small);
    TerminalCanvas leftSmall = TerminalFrame_left(&frame, &small);
    assert(TerminalCanvas_isValid(&centerSmall) && !TerminalCanvas_isValid(&leftSmall));

    // Background paint + null safety.
    static const uint8_t white[4] = { 255, 255, 255, 255 };
    TerminalImage image = TerminalImage(white, 1, 1);
    TerminalImage_setRamp(&image, "#");
    assert(TerminalFrame_setBackground(&frame, &image));
    TerminalFrame_paintBackground(&frame, &root);
    assert(TerminalCanvas_charAt(&root, W - 1, H - 1) == '#');
    assert(TerminalFrame_setBackground(&frame, nullptr));

    TerminalCanvas none = TerminalFrame_center(nullptr, &root);
    assert(!TerminalCanvas_isValid(&none));
    TerminalCanvas noneRoot = TerminalFrame_left(&frame, nullptr);
    assert(!TerminalCanvas_isValid(&noneRoot));
    TerminalFrame_paintBackground(nullptr, &root);
    TerminalFrame_drawRules(nullptr, &root, TerminalPen());
    REJECT(TerminalFrame_setRails(nullptr, 1, 1), "null self");
    REJECT(TerminalFrame_setRule(nullptr, TerminalRule_0()), "null self");
    TerminalFrame_getRule(nullptr, &got);
    TerminalRule_draw(nullptr, &root, 0, TerminalPen());
    TerminalRule_draw(&striped, nullptr, 0, TerminalPen());
    TerminalFrame_getRails(nullptr, nullptr, nullptr);
    TerminalFrame_getBands(nullptr, nullptr, nullptr);

    PROJECT(TerminalFrame, &frame);
    return 0;
}
