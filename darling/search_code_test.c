#include <stdio.h>
#include <string.h>

#include "darling/field/searchfield.h"
#include "darling/field/codefield.h"
#include "nio/mem.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[search_code_test] PASS %s\n", name); } \
    else { printf("[search_code_test] FAIL %s\n", name); g_failures++; } \
} while (0)

static void onSearchQuery(const char *query, void *ctx) {
    (void) query;
    (*(int*) ctx)++;
}

static void onCodeChanged(void *ctx) {
    (*(int*) ctx)++;
}

int main(void) {
    printf("=== Running SearchField & CodeField Test Suite ===\n");

    // 1. SearchField
    {
        SearchField *sf = SearchField_0();
        CHECK("SearchField created", sf != nullptr);
        CHECK("default text empty", SearchField_getText(sf) == nullptr || strcmp(SearchField_getText(sf), "") == 0);
        CHECK("default placeholder Search...", SearchField_getPlaceholder(sf) != nullptr && strcmp(SearchField_getPlaceholder(sf), "Search...") == 0);

        SearchField_setText(sf, "Darling Framework");
        CHECK("setText works", strcmp(SearchField_getText(sf), "Darling Framework") == 0);

        SearchField_setPlaceholder(sf, "Find symbol...");
        CHECK("setPlaceholder works", strcmp(SearchField_getPlaceholder(sf), "Find symbol...") == 0);

        SearchField_setShortcut(sf, "Cmd+K");
        CHECK("setShortcut works", strcmp(SearchField_getShortcut(sf), "Cmd+K") == 0);

        int searchCount = 0;
        SearchField_setOnSearch(sf, onSearchQuery);
        SearchField_setCtx(sf, &searchCount);

        SearchField_free(sf);
        SearchField_free(nullptr);
        CHECK("SearchField free null no crash", true);
    }

    // 2. CodeField
    {
        CodeField *cf = CodeField_0();
        CHECK("CodeField created", cf != nullptr);
        CHECK("default gutterWidth 40", CodeField_getGutterWidth(cf) == 40.0f);
        CHECK("inner editor exists", CodeField_getEditor(cf) != nullptr);

        const char *code = "int main() {\n    printf(\"Hello, Darling!\\n\");\n    return 0;\n}";
        CodeField_setText(cf, code);
        CHECK("setText works", strcmp(CodeField_getText(cf), code) == 0);

        CodeField_setGutterWidth(cf, 50.0f);
        CHECK("setGutterWidth 50", CodeField_getGutterWidth(cf) == 50.0f);

        CodeField_setGutterBackground(cf, 0xFF101012u);
        CHECK("setGutterBackground", CodeField_getGutterBackground(cf) == 0xFF101012u);

        CodeField_setGutterTextColor(cf, 0xFF888888u);
        CHECK("setGutterTextColor", CodeField_getGutterTextColor(cf) == 0xFF888888u);

        CodeField_setActiveLineColor(cf, 0xFF3B82F6u);
        CHECK("setActiveLineColor", CodeField_getActiveLineColor(cf) == 0xFF3B82F6u);

        int changeCount = 0;
        CodeField_setOnChange(cf, onCodeChanged);
        CodeField_setCtx(cf, &changeCount);

        CodeField_free(cf);
        CodeField_free(nullptr);
        CHECK("CodeField free null no crash", true);
    }

    printf("\n=== SearchField & CodeField Test Summary: %d failures ===\n", g_failures);
    return g_failures > 0 ? 1 : 0;
}
