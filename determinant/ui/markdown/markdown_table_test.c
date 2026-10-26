// Owner: TerminalMarkdownTable — delimiter alignment, escape- and code-aware
// splitting, natural/longest-word measurement, honest truncation and width
// allocation.
#include "ui/markdown/markdown_table.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    TerminalMarkdownTableAlign aligns[TERMINAL_MARKDOWN_TABLE_COLUMNS] = {0};
    size_t columns = TerminalMarkdownTable_parseDelimiter("| :--- | :---: | ---: |",
                                                          strlen("| :--- | :---: | ---: |"),
                                                          aligns, 8u);
    assert(columns == 3u);
    assert(aligns[0] == TERMINAL_MARKDOWN_TABLE_LEFT);
    assert(aligns[1] == TERMINAL_MARKDOWN_TABLE_CENTER);
    assert(aligns[2] == TERMINAL_MARKDOWN_TABLE_RIGHT);
    assert(TerminalMarkdownTable_parseDelimiter("--- | ---", strlen("--- | ---"), aligns, 8u) == 2u);
    assert(TerminalMarkdownTable_parseDelimiter("| --- |", strlen("| --- |"), aligns, 8u) == 0u);
    assert(TerminalMarkdownTable_parseDelimiter("| x | --- |", strlen("| x | --- |"), aligns, 8u) == 0u);
    assert(TerminalMarkdownTable_parseDelimiter("| --- | :: |", strlen("| --- | :: |"), aligns, 8u) == 0u);
    assert(TerminalMarkdownTable_parseDelimiter("| --- | -- |", strlen("| --- | -- |"),
                                                nullptr, 0u) == 2u);

    // A literal pipe is escaped or inside a code run; neither may invent a
    // column. The optional outer pipes are not cells.
    const char *cells[TERMINAL_MARKDOWN_TABLE_COLUMNS];
    size_t lengths[TERMINAL_MARKDOWN_TABLE_COLUMNS];
    const char *row = "| a \\| b | `x|y` |";
    size_t count = TerminalMarkdownTable_split(row, strlen(row), cells, lengths, 8u);
    assert(count == 2u);
    assert(lengths[0] == strlen("a \\| b") && memcmp(cells[0], "a \\| b", lengths[0]) == 0);
    assert(lengths[1] == strlen("`x|y`") && memcmp(cells[1], "`x|y`", lengths[1]) == 0);
    row = "a | b || c";
    count = TerminalMarkdownTable_split(row, strlen(row), cells, lengths, 8u);
    assert(count == 4u);
    assert(lengths[1] == 1u && cells[1][0] == 'b');
    assert(lengths[2] == 0u);
    assert(lengths[3] == 1u && cells[3][0] == 'c');
    assert(TerminalMarkdownTable_split("| a | b |", strlen("| a | b |"), cells, lengths,
                                       1u) == 2u);
    assert(TerminalMarkdownTable_split(nullptr, 0, cells, lengths, 8u) == 0u);

    assert(TerminalMarkdownTable_natural("日本", 6) == 4u);
    assert(TerminalMarkdownTable_longestWord("ab cdef g", 9) == 4u);
    assert(TerminalMarkdownTable_longestWord("", 0) == 1u);
    assert(TerminalMarkdownTable_lineCount("ab cd", 5, 8u) == 1u);
    assert(TerminalMarkdownTable_lineCount("ab cd ef", 8, 3u) == 3u);

    // A word break leaves the following word whole; a word too wide for the
    // column is cut with the ellipsis cell handed back to the caller.
    TerminalMarkdownTableCellLine line = TerminalMarkdownTable_cellLine("ab cd", 5, 0, 3u);
    assert(!line.truncated && line.cells == 2u && line.length == 2u && line.next == 3u);
    line = TerminalMarkdownTable_cellLine("abcdefgh", 8, 0, 4u);
    assert(line.truncated);
    assert(line.cells == 3u && line.length == 3u && line.next == 3u);
    line = TerminalMarkdownTable_cellLine("ab cd", 5, 3, 3u);
    assert(!line.truncated && line.cells == 2u && line.length == 2u && line.next == 5u);
    line = TerminalMarkdownTable_cellLine("", 0, 0, 3u);
    assert(!line.truncated && line.cells == 0u && line.length == 0u);

    // Natural widths when they fit, minimums plus slack when they do not, and a
    // refusal — never a zero-width column — when even the minimums overflow.
    size_t needed[4] = { 10u, 10u, 1u, 1u };
    size_t minimums[4] = { 3u, 3u, 1u, 1u };
    size_t widths[4] = {0};
    assert(TerminalMarkdownTable_allocate(needed, minimums, widths, 2u, 30u));
    assert(widths[0] == 10u && widths[1] == 10u);
    assert(TerminalMarkdownTable_allocate(needed, minimums, widths, 2u, 15u));
    assert(widths[0] == 4u && widths[1] == 4u);
    assert(!TerminalMarkdownTable_allocate(needed, minimums, widths, 2u, 10u));
    assert(!TerminalMarkdownTable_allocate(nullptr, minimums, widths, 2u, 30u));
    assert(!TerminalMarkdownTable_allocate(needed, minimums, widths, 0u, 30u));
    return 0;
}
