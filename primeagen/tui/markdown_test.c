/* Cold Markdown subset owner: literal/code preservation, visible per-byte flags,
 * compact prose, unmatched syntax, exact capacity, hostile admission and bounded
 * delimiter pressure. No CommonMark, Unicode, terminal appearance or concurrency
 * promise; pure caller-owned disjoint buffers, no resources to exhaust. */
#include "tui/markdown.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void check(const char *source, const char *expected) {
    char dest[256];
    uint8_t styles[256];
    size_t length = 99;
    bool truncated = true;
    assert(Markdown_render(source, strlen(source), dest, styles, sizeof(dest), &length, &truncated));
    assert(strcmp(dest, expected) == 0 && length == strlen(expected) && !truncated);
    assert(styles[length] == 0);
}

int main(void) {
    check("", "");
    check("**bold** and __also__", "bold and also");
    check("*soft* _emphasis_", "soft emphasis");
    check("unmatched ** text", "unmatched ** text");
    check("snake_case remains", "snake_case remains");
    check("\\*literal\\*", "*literal*");
    check("a\n\n\n\nnext\n\n", "a\n\nnext");
    check("```c\n  **literal**\n\n\nend\n```\nafter", "  **literal**\n\n\nend\nafter");
    check("`**literal**`", "**literal**");
    check("####### literal", "####### literal");
    char dest[256];
    uint8_t styles[256];
    size_t length;
    const char *source = "# Title\n**bold** *soft* `code`";
    assert(Markdown_render(source, strlen(source), dest, styles, sizeof(dest), &length, nullptr));
    assert(styles[0] == (TEXT_STYLE_HEADING | TEXT_STYLE_BOLD));
    assert(styles[6] == TEXT_STYLE_BOLD && styles[11] == TEXT_STYLE_ITALIC && styles[16] == TEXT_STYLE_CODE);
    bool truncated;
    strcpy(dest, "keep");
    memset(styles, 0xaa, sizeof(styles));
    assert(!Markdown_render("a", 1, dest, styles, 1, &length, &truncated));
    assert(truncated && length == 0 && strcmp(dest, "keep") == 0 && styles[0] == 0xaa);
    assert(Markdown_render("a", 1, dest, styles, 2, &length, &truncated));
    assert(!truncated && strcmp(dest, "a") == 0);
    assert(!Markdown_render(nullptr, 0, dest, styles, sizeof(dest), &length, nullptr));
    assert(!Markdown_render("a", 1, nullptr, styles, sizeof(dest), &length, nullptr));
    assert(!Markdown_render("a", 1, dest, nullptr, sizeof(dest), &length, nullptr));
    assert(!Markdown_render("a", 1, dest, styles, sizeof(dest), nullptr, nullptr));
    assert(!Markdown_render("a", SIZE_MAX, dest, styles, sizeof(dest), &length, nullptr));
    const char hostile[] = {'x', '\033', '[', 'm'};
    strcpy(dest, "keep");
    assert(!Markdown_render(hostile, sizeof(hostile), dest, styles, sizeof(dest), &length, nullptr));
    assert(strcmp(dest, "keep") == 0);
    const size_t pressure = 65536;
    char *input = malloc(pressure + 1);
    char *output = malloc(pressure + 1);
    uint8_t *flags = malloc(pressure + 1);
    assert(input != nullptr && output != nullptr && flags != nullptr);
    memset(input, '`', pressure);
    assert(Markdown_render(input, pressure, output, flags, pressure + 1, &length, nullptr));
    assert(length <= pressure && output[length] == '\0');
    free(input); free(output); free(flags);
}
