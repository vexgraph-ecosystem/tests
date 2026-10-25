// Owner: TerminalMarkdownView — the terminal Markdown preview. Cold parsing and
// cell-aware wrapping are checked separately from hot rendering.
#include "ui/markdown/terminal_markdown_view.h"
#include "terminal/terminal_color.h"
#include "determinant/support.h"

enum { W = 48, H = 12 };

static void clearCanvas(TerminalCanvas *canvas, uint32_t *cells, uint8_t *styles,
                        uint32_t *fg, uint32_t *bg) {
    *canvas = TerminalCanvas(cells, styles, fg, bg, W, H);
    TerminalCanvas_fill(canvas, ' ', TerminalPen());
}

static bool canvasContainsText(const TerminalCanvas *canvas, const char *text) {
    size_t length = strlen(text);
    for (size_t row = 0; row < TerminalCanvas_getHeight(canvas); ++row)
        for (size_t column = 0; column + length <= TerminalCanvas_getWidth(canvas); ++column) {
            bool matches = true;
            for (size_t index = 0; index < length; ++index)
                if (TerminalCanvas_glyphAt(canvas, column + index, row)
                    != (unsigned char) text[index]) {
                    matches = false;
                    break;
                }
            if (matches)
                return true;
        }
    return false;
}

int main(void) {
    static const char *lines[] = {
        "# Semicolon",
        "",
        "A **bold** and *quiet* `code` paragraph with [docs](https://example.test).",
        "",
        "- [x] done",
        "- [ ] next",
        "",
        "> [!WARNING] Important section",
        "> Callout details stay grouped.",
        "",
        "```c",
        "int main(void) { return 0; }",
        "```",
        "",
        "| Name | Value |",
        "| ---- | ----- |",
        "| one | two |",
    };
    uint32_t cells[W * H];
    uint8_t styles[W * H];
    uint32_t fg[W * H];
    uint32_t bg[W * H];
    TerminalCanvas canvas;
    clearCanvas(&canvas, cells, styles, fg, bg);

    TerminalMarkdownView *view = TerminalMarkdownView();
    assert(view != nullptr);
    assert(TerminalMarkdownView_rowCount(view) == 0u);
    assert(TerminalMarkdownView_getWidth(view) == 0u);
    assert(TerminalMarkdownView_visibleRows(view) == 0u);
    assert(TerminalMarkdownView_getScroll(view) == 0u);
    assert(TerminalMarkdownView_maxScroll(view) == 0u);
    assert(!TerminalMarkdownView_isTruncated(view));
    assert(TerminalMarkdownView_exhaustionCount(view) == 0u);
    TerminalMarkdownStyle style = TerminalMarkdownView_getStyle(view);
    assert(style.heading.style == TERMINAL_STYLE_BOLD);
    style.body = TerminalPen(TERMINAL_STYLE_DIM, TERMINAL_RGB(9, 8, 7));
    assert(TerminalMarkdownView_setStyle(view, style));
    assert(TerminalMarkdownView_getStyle(view).body.fg == TERMINAL_RGB(9, 8, 7));
    assert(TerminalMarkdownView_setSource(view, lines, sizeof lines / sizeof lines[0]));
    assert(!TerminalMarkdownView_isReady(view));
    assert(TerminalMarkdownView_setViewport(view, W));
    assert(TerminalMarkdownView_isReady(view));
    assert(!TerminalMarkdownView_isTruncated(view));
    assert(TerminalMarkdownView_getWidth(view) == W);
    assert(TerminalMarkdownView_rowCount(view) >= 10u);
    TerminalMarkdownStyle renderedStyle = TerminalMarkdownView_getStyle(view);
    assert(renderedStyle.code.fg == TERMINAL_RGB(112, 190, 130));
    assert((renderedStyle.code.style & TERMINAL_STYLE_REVERSE) == 0);

    TerminalMarkdownView_setVisibleRows(view, H);
    assert(TerminalMarkdownView_maxScroll(view) + H >= TerminalMarkdownView_rowCount(view));
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalCanvas_glyphAt(&canvas, 0, 0) == '#'
           || TerminalCanvas_glyphAt(&canvas, 0, 0) == 'S');
    bool sawStyledCell = false;
    bool sawCalloutBar = false;
    bool sawColoredCallout = false;
    bool sawTableBorder = false;
    for (size_t i = 0; i < W * H; ++i)
        if ((styles[i] & TERMINAL_STYLE_BOLD) != 0)
            sawStyledCell = true;
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            if (glyph == '|') {
                sawCalloutBar = true;
                sawColoredCallout = (styles[row * W + column] & TERMINAL_STYLE_BOLD) != 0
                    && fg[row * W + column] == TERMINAL_RGB(105, 190, 220);
            }
        }
    assert(sawStyledCell);
    assert(sawCalloutBar);
    assert(sawColoredCallout);

    static const char *warningCallout[] = {
        "> [!WARNING]",
        "> text goes here and text goes here",
        "> and text goes here",
    };
    assert(TerminalMarkdownView_setSource(view, warningCallout,
                                          sizeof warningCallout / sizeof warningCallout[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalCanvas_glyphAt(&canvas, 0, 0) == '|');
    assert(TerminalCanvas_glyphAt(&canvas, 2, 0) == '!');
    assert(TerminalCanvas_glyphAt(&canvas, 4, 0) == 'W');
    assert(TerminalCanvas_glyphAt(&canvas, 12, 0) == '!');
    for (size_t column = 2u; column <= 12u; ++column) {
        assert((styles[column] & TERMINAL_STYLE_BOLD) != 0u);
        assert(fg[column] == TERMINAL_RGB(235, 190, 95));
    }
    assert(TerminalCanvas_glyphAt(&canvas, 0, 1) == '|');
    assert(TerminalCanvas_glyphAt(&canvas, 2, 1) == ' ');
    assert(TerminalCanvas_glyphAt(&canvas, 0, 2) == '|');
    assert(TerminalCanvas_glyphAt(&canvas, 2, 2) == 't');
    assert((styles[2u * W + 2u] & TERMINAL_STYLE_BOLD) == 0u);
    assert(fg[2u * W + 2u] == TERMINAL_RGB(9, 8, 7));

    // A fenced block is a shaded band, not a box. It gets one blank row above
    // and below, every cell of its rows carries the fence background, and the
    // copy projection exposes the content lines without the fences.
    static const char *shaded[] = {
        "before",
        "",
        "```c",
        "int x = 1;",
        "",
        "int y = 2;",
        "```",
        "",
        "after",
    };
    assert(TerminalMarkdownView_setSource(view, shaded, sizeof shaded / sizeof shaded[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    TerminalMarkdownView_setVisibleRows(view, H);
    assert(TerminalMarkdownView_codeBlockCount(view) == 1u);
    TerminalMarkdownCodeBlock block = TerminalMarkdownView_codeBlockAt(view, 0);
    assert(!block.valid);
    block = TerminalMarkdownView_codeBlockAt(view, 2);
    assert(block.valid);
    assert(block.firstRow == 2u);
    assert(block.rowCount == 3u);
    assert(block.firstSourceLine == 3u);
    assert(block.sourceLineCount == 3u);
    assert(block.quoteDepth == 0u);
    assert(TerminalMarkdownView_codeBlockAt(view, 4).valid);
    assert(!TerminalMarkdownView_codeBlockAt(view, 5).valid);
    TerminalMarkdownCodeLine line = TerminalMarkdownView_codeBlockLine(view, block, 0);
    assert(line.valid && line.length == strlen("int x = 1;"));
    assert(memcmp(line.text, "int x = 1;", line.length) == 0);
    line = TerminalMarkdownView_codeBlockLine(view, block, 1);
    assert(line.valid && line.length == 0u);
    line = TerminalMarkdownView_codeBlockLine(view, block, 3);
    assert(!line.valid);

    TerminalMarkdownStyle codeStyle = TerminalMarkdownView_getStyle(view);
    assert(codeStyle.fence.bg != TERMINAL_COLOR_DEFAULT);
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(bg[1u * W] != codeStyle.fence.bg);
    for (size_t row = 2u; row < 5u; ++row) {
        assert(bg[row * W] == codeStyle.fence.bg);
        assert(bg[row * W + W - 1u] == codeStyle.fence.bg);
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            assert(glyph != 0x250Cu && glyph != 0x2502u && glyph != 0x2518u);
        }
    }
    assert(bg[5u * W] != codeStyle.fence.bg);
    assert(canvasContainsText(&canvas, "int x = 1;"));
    assert(canvasContainsText(&canvas, "int y = 2;"));

    // A fence inside a callout keeps the quote gutter but gets the same band, and
    // its copy projection strips the quote markers.
    static const char *quotedFence[] = {
        "> [!WARNING]",
        "> Watch this block",
        "> ```c",
        "> int q = 3;",
        "> ```",
    };
    assert(TerminalMarkdownView_setSource(view, quotedFence,
                                          sizeof quotedFence / sizeof quotedFence[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    TerminalMarkdownView_setVisibleRows(view, H);
    assert(TerminalMarkdownView_codeBlockCount(view) == 1u);
    TerminalMarkdownCodeBlock quotedBlock = {0};
    for (size_t row = 0; row < TerminalMarkdownView_rowCount(view); ++row) {
        TerminalMarkdownCodeBlock candidate = TerminalMarkdownView_codeBlockAt(view, row);
        if (candidate.valid) {
            quotedBlock = candidate;
            break;
        }
    }
    assert(quotedBlock.valid);
    assert(quotedBlock.quoteDepth == 1u);
    assert(quotedBlock.sourceLineCount == 1u);
    line = TerminalMarkdownView_codeBlockLine(view, quotedBlock, 0);
    assert(line.valid && line.length == strlen("int q = 3;"));
    assert(memcmp(line.text, "int q = 3;", line.length) == 0);
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalCanvas_glyphAt(&canvas, 0, quotedBlock.firstRow) == '|');
    assert(bg[quotedBlock.firstRow * W + 2u] == codeStyle.fence.bg);

    assert(TerminalMarkdownView_setSource(view, lines, sizeof lines / sizeof lines[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    TerminalMarkdownView_setVisibleRows(view, H);
    assert(TerminalMarkdownView_setScroll(view, TerminalMarkdownView_maxScroll(view)));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalMarkdownView_codeBlockCount(view) == 1u);
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            if (glyph == 0x2502u)
                sawTableBorder = true;
        }
    assert(sawTableBorder);

    TerminalMarkdownView_setVisibleRows(view, 2);
    size_t oldScroll = TerminalMarkdownView_getScroll(view);
    assert(TerminalMarkdownView_scrollBy(view, 3));
    assert(TerminalMarkdownView_getScroll(view) == oldScroll + 3u);
    assert(TerminalMarkdownView_setScroll(view, SIZE_MAX));
    assert(TerminalMarkdownView_getScroll(view) == TerminalMarkdownView_maxScroll(view));
    assert(!TerminalMarkdownView_scrollBy(view, 1));

    static const char *longQuote[] = {
        "> A blockquote long enough to wrap across several terminal rows must keep its "
        "colored vertical marker at the start of every continuation row."
    };
    assert(TerminalMarkdownView_setSource(view, longQuote, 1));
    assert(TerminalMarkdownView_setViewport(view, W));
    assert(TerminalMarkdownView_rowCount(view) >= 2u);
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    size_t quoteRows = 0;
    for (size_t row = 0; row < H && row < TerminalMarkdownView_rowCount(view); ++row)
        if (TerminalCanvas_glyphAt(&canvas, 0, row) == '|') {
            ++quoteRows;
            assert(TerminalCanvas_glyphAt(&canvas, 1, row) == ' ');
        }
    assert(quoteRows >= 2u);

    static const char *ordered[] = {
        "1. first ordered item", "2. second ordered item", "3) third ordered item"
    };
    assert(TerminalMarkdownView_setSource(view, ordered, 3));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    bool sawFirstNumber = false;
    bool sawSecondNumber = false;
    bool sawThirdNumber = false;
    bool sawFirstText = false;
    bool sawSecondText = false;
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            if (column + 1u < W && glyph == '1'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == '.')
                sawFirstNumber = true;
            if (column + 1u < W && glyph == '2'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == '.')
                sawSecondNumber = true;
            if (column + 1u < W && glyph == '3'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == ')')
                sawThirdNumber = true;
            if (column + 1u < W && glyph == 'f'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == 'i')
                sawFirstText = true;
            if (column + 1u < W && glyph == 's'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == 'e')
                sawSecondText = true;
        }
    assert(sawFirstNumber && sawSecondNumber && sawThirdNumber);
    assert(sawFirstText && sawSecondText);

    static const char *unordered[] = { "- dash item", "* star item", "+ plus item" };
    assert(TerminalMarkdownView_setSource(view, unordered, 3));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    for (size_t row = 0; row < 3u; ++row) {
        assert(TerminalCanvas_glyphAt(&canvas, 0, row) == 0x2022u);
        assert(TerminalCanvas_glyphAt(&canvas, 1, row) == ' ');
    }
    assert(canvasContainsText(&canvas, "dash item"));
    assert(canvasContainsText(&canvas, "star item"));
    assert(canvasContainsText(&canvas, "plus item"));

    static const char *pipeTable[] = {
        "| Law Title | Scope | Enforcement |",
        "| :--- | :--- | :--- |",
        "| Table Law | Terminal | Required |",
    };
    assert(TerminalMarkdownView_setSource(view, pipeTable, 3));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(canvasContainsText(&canvas, "Law Title"));
    assert(canvasContainsText(&canvas, "Scope"));
    assert(canvasContainsText(&canvas, "Enforcement"));
    assert(canvasContainsText(&canvas, "Table Law"));
    assert(canvasContainsText(&canvas, "Terminal"));
    assert(canvasContainsText(&canvas, "Required"));
    assert(!canvasContainsText(&canvas, ":---"));
    size_t tableVerticals = 0;
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column < W; ++column)
            if (TerminalCanvas_glyphAt(&canvas, column, row) == 0x2502u)
                ++tableVerticals;
    assert(tableVerticals >= 6u);

    static const char *lawIndexTable[] = {
        "| # | Law Title |",
        "| :-- | :--- |",
        "| 1 | Semantic Consistency Law |",
        "| 2 | Single Class Per File Law (Java Law) |",
        "| 3 | Two-Semicolon Annotation Style Law |",
        "| 4 | Build & Naming Conventions Law |",
    };
    assert(TerminalMarkdownView_setSource(view, lawIndexTable,
                                          sizeof lawIndexTable / sizeof lawIndexTable[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    bool sawLawTitle = false;
    bool sawOrdinal = false;
    bool sawUnicodeTableEdge = false;
    bool sawPaddedTableCell = false;
    bool sawSourcePipe = false;
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            if (glyph == 'L' && column + 3u < W
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == 'a'
                && TerminalCanvas_glyphAt(&canvas, column + 2u, row) == 'w'
                && TerminalCanvas_glyphAt(&canvas, column + 3u, row) == ' ')
                sawLawTitle = true;
            if (glyph == '1' && column + 1u < W
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == ' ')
                sawOrdinal = true;
            if (glyph == 0x2502u)
                sawUnicodeTableEdge = true;
            if (row > 0u && glyph == 0x2502u && column + 2u < W
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == ' '
                && TerminalCanvas_glyphAt(&canvas, column + 2u, row) == '#')
                sawPaddedTableCell = true;
            if (glyph == '|')
                sawSourcePipe = true;
        }
    assert(sawLawTitle && sawOrdinal && sawUnicodeTableEdge && sawPaddedTableCell);
    assert(!sawSourcePipe);

    static const char *wrappedTable[] = {
        "| Name | Scope Summary | Status |",
        "| ---- | ------------- | ------ |",
        "| Runner | Long cell wraps cleanly and keeps all of its content COMPLETE. | 🟥 ready |",
        "| Editor | Short row stays aligned below the wrapped row. | 🟩 ready |",
    };
    assert(TerminalCanvas_widthOf(0x1F7E5u) == 2u);
    static const char *naturalWidthTable[] = {
        "| ID | Description |",
        "| -- | ----------- |",
        "| 1 | This description should keep its natural unwrapped width. |",
    };
    assert(TerminalMarkdownView_setSource(view, naturalWidthTable,
                                          sizeof naturalWidthTable / sizeof naturalWidthTable[0]));
    assert(TerminalMarkdownView_setViewport(view, 100u));
    assert(TerminalMarkdownView_rowCount(view) == 5u);

    static const char *weightedTable[] = {
        "| ID | Description | State |",
        "| -- | ----------- | ----- |",
        "| 1 | This longer description needs substantially more column room than the short state. | OK |",
    };
    assert(TerminalMarkdownView_setSource(view, weightedTable,
                                          sizeof weightedTable / sizeof weightedTable[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalCanvas_glyphAt(&canvas, 5u, 3u) == 0x2502u);
    assert(TerminalCanvas_glyphAt(&canvas, 39u, 3u) == 0x2502u);

    assert(TerminalMarkdownView_setSource(view, wrappedTable,
                                          sizeof wrappedTable / sizeof wrappedTable[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    assert(TerminalMarkdownView_setScroll(view, 0u));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(canvasContainsText(&canvas, "COMPLETE"));
    bool sawRedEmoji = false;
    size_t tableJoins = 0;
    for (size_t row = 0; row < H; ++row) {
        size_t lastContentColumn = W;
        for (size_t column = 0; column < W; ++column) {
            uint32_t glyph = TerminalCanvas_glyphAt(&canvas, column, row);
            if (glyph != ' ' && glyph != 0u)
                lastContentColumn = column;
            if (glyph == 0x1F7E5u) {
                sawRedEmoji = true;
                assert(column + 1u < W);
                assert(TerminalCanvas_glyphAt(&canvas, column + 1u, row) == 0u);
            }
            if (glyph == 0x253Cu)
                ++tableJoins;
        }
        if (lastContentColumn != W) {
            uint32_t rightEdge = TerminalCanvas_glyphAt(&canvas, lastContentColumn, row);
            assert(rightEdge == 0x2502u || rightEdge == 0x2524u
                   || rightEdge == 0x2510u || rightEdge == 0x2518u);
        }
    }
    assert(sawRedEmoji);
    assert(tableJoins >= 4u);

    static const char *numberWidthList[] = {
        "997. item 997", "1000. item 1000", "12345. item 12345",
        "1234567. item 1234567", "123456789012. item 123456789012",
    };
    assert(TerminalMarkdownView_setSource(view, numberWidthList,
                                          sizeof numberWidthList / sizeof numberWidthList[0]));
    assert(TerminalMarkdownView_setViewport(view, W));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    size_t itemColumn = W;
    size_t itemRows = 0;
    for (size_t row = 0; row < H; ++row)
        for (size_t column = 0; column + 3u < W; ++column)
            if (TerminalCanvas_glyphAt(&canvas, column, row) == 'i'
                && TerminalCanvas_glyphAt(&canvas, column + 1u, row) == 't'
                && TerminalCanvas_glyphAt(&canvas, column + 2u, row) == 'e'
                && TerminalCanvas_glyphAt(&canvas, column + 3u, row) == 'm') {
                if (itemColumn == W)
                    itemColumn = column;
                assert(column == itemColumn);
                ++itemRows;
            }
    assert(itemRows == 5u);

    static const char *narrow[] = { "A wide λ marker and a paragraph that must wrap." };
    assert(TerminalMarkdownView_setSource(view, narrow, 1));
    assert(TerminalMarkdownView_setViewport(view, 12));
    assert(TerminalMarkdownView_rowCount(view) > 1u);
    assert(TerminalMarkdownView_setVisibleRows(view, 2));
    assert(TerminalMarkdownView_setScroll(view, 0));
    clearCanvas(&canvas, cells, styles, fg, bg);
    TerminalMarkdownView_render(view, &canvas);
    assert(TerminalCanvas_glyphAt(&canvas, 0, 0) == 'A');

    beginDiagnostics();
    assert(!TerminalMarkdownView_setViewport(view, 0));
    endDiagnostics(1, "zero width");
    beginDiagnostics();
    assert(!TerminalMarkdownView_setSource(view, nullptr, 1));
    endDiagnostics(1, "null list");
    static const char *nullLine[] = { nullptr };
    beginDiagnostics();
    assert(!TerminalMarkdownView_setSource(view, nullLine, 1));
    endDiagnostics(1, "null line");
    assert(TerminalMarkdownView_isReady(view));
    assert(TerminalMarkdownView_rowCount(view) > 1u);
    assert(TerminalMarkdownView_exhaustionCount(view) == 0u);

    TerminalProjection projection = TerminalMarkdownView_toString(view);
    assert(projection.length != 0 && !projection.truncated);
    TerminalProjection structure = TerminalMarkdownView_toStringStruct(view);
    assert(strstr(structure.text, "rowCount") != nullptr);
    assert(strcmp(TerminalMarkdownView_toString(nullptr).text, "nullptr") == 0);
    TerminalMarkdownView_free(view);
    TerminalMarkdownView_free(nullptr);
    TerminalMarkdownView_clearExhaustion(nullptr);
    assert(TerminalMarkdownView_rowCount(nullptr) == 0u);
    assert(TerminalMarkdownView_getWidth(nullptr) == 0u);
    assert(TerminalMarkdownView_visibleRows(nullptr) == 0u);
    assert(TerminalMarkdownView_getScroll(nullptr) == 0u);
    assert(TerminalMarkdownView_maxScroll(nullptr) == 0u);
    assert(TerminalMarkdownView_exhaustionCount(nullptr) == 0u);
    assert(!TerminalMarkdownView_isReady(nullptr));
    assert(!TerminalMarkdownView_isTruncated(nullptr));
    TerminalMarkdownView_render(nullptr, &canvas);
    return 0;
}
