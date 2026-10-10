/* Tui owner: flat growth, retained contents, wrap/scroll/input, projections,
 * failure preservation and repeated reclamation. Owner-thread-only: racing
 * mutation/free is outside contract. No hardware appearance or backend trust. */
#include "tui/tui.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int failAfter = -1;
static bool fail(void) {
    if (failAfter < 0)
        return false;
    if (failAfter == 0)
        return true;
    --failAfter;
    return false;
}
static void *testMalloc(size_t n) { return fail() ? nullptr : malloc(n); }
static void *testCalloc(size_t n, size_t s) { return fail() ? nullptr : calloc(n, s); }
static void *testRealloc(void *p, size_t n) { return fail() ? nullptr : realloc(p, n); }
#define malloc testMalloc
#define calloc testCalloc
#define realloc testRealloc
#include "tui/tui.c"
#undef malloc
#undef calloc
#undef realloc

int main(void) {
    Tui a = Tui();
    Tui b = Tui(1);
    Tui c = Tui(1, 1);
    Tui d = Tui_zero();
    assert(Tui_isReady(&a) && Tui_isReady(&b) && Tui_isReady(&c) && Tui_isReady(&d));
    Tui_free(&a); Tui_free(&b); Tui_free(&d);
    assert(Tui_setSize(&c, 9, 20));
    int rows, columns;
    Tui_getSize(&c, &rows, &columns);
    assert(rows == 9 && columns == 20);
    assert(Tui_getViewportHeight(&c) == 3 && Tui_getViewportWidth(&c) == 18);
    assert(Tui_add(&c, "1234567890123456789\n", 20));
    char output[4096]; bool cut;
    assert(Tui_getVisualRowCount(&c) == 3);
    assert(Tui_copyRow(&c, 0, output, sizeof(output), &cut) && !cut && strlen(output) == 18);
    assert(Tui_copyRow(&c, 1, output, sizeof(output), nullptr) && strcmp(output, "9") == 0);
    assert(Tui_copyRow(&c, 2, output, sizeof(output), nullptr) && output[0] == 0);
    for (int i = 0; i < 1024; ++i)
        assert(Tui_add(&c, "row", 3));
    assert(Tui_getCount(&c) == 1025 && Tui_getCapacity(&c) >= 1025);
    Tui_setScroll(&c, 10);
    size_t first = Tui_getFirstVisibleRow(&c);
    assert(Tui_add(&c, "next", 4));
    assert(Tui_getFirstVisibleRow(&c) == first);
    Tui_scrollBy(&c, INT64_MAX);
    assert(Tui_getFirstVisibleRow(&c) == 0);
    Tui_scrollBy(&c, INT64_MIN);
    assert(Tui_getScroll(&c) == 0);
    assert(Tui_setInput(&c, "quote\"\\", 7));
    assert(Tui_toStringStruct(&c, output, sizeof(output), &cut));
    assert(strstr(output, "input=\"quote\\\"\\\\\"") != nullptr);
    assert(Tui_setInput(&c, Tui_getInput(&c) + 1, 6));
    for (int i = 0; i < 1024; ++i)
        assert(Tui_inputAddChar(&c, 'a'));
    assert(Tui_getInputLength(&c) == 1030 && Tui_getInputCapacity(&c) > 1030);
    assert(Tui_inputBackspace(&c));
    assert(Tui_toString(&c, output, sizeof(output), nullptr));
    Tui_setDirty(&c, false); assert(!Tui_isDirty(&c));
    Tui_inputClear(&c); assert(!Tui_inputBackspace(&c));
    assert(Tui_getInputLength(&c) == 0);
    Tui_clear(&c); assert(Tui_getCount(&c) == 0);
    const char hostile[] = {'\033', '\0', (char) 255};
    assert(Tui_add(&c, hostile, sizeof(hostile)));
    assert(Tui_copyRow(&c, 0, output, sizeof(output), nullptr));
    assert(strcmp(output, "\\x1b\\x00\\xff") == 0);
    assert(!Tui_copyRow(&c, 0, output, 2, &cut) && cut);
    assert(!Tui_toString(&c, output, 1, &cut) && cut);
    assert(!Tui_toStringStruct(&c, output, 10, &cut) && cut);
    assert(!Tui_copyRow(&c, SIZE_MAX, output, sizeof(output), &cut) && !cut);
    assert(!Tui_copyRow(&c, 0, nullptr, 0, &cut) && cut);
    assert(!Tui_setInput(&c, "\033", 1));
    assert(!Tui_inputAddChar(&c, 255));
    assert(!Tui_setSize(&c, -1, 20));
    assert(!Tui_add(&c, "a", SIZE_MAX));
    assert(!Tui_entriesReserve(&c, SIZE_MAX));
    assert(!Tui_inputReserve(&c, SIZE_MAX));
    assert(!Tui_setInput(&c, nullptr, 0));
    assert(!Tui_add(nullptr, "a", 1));
    assert(!Tui_setInput(nullptr, "a", 1));
    assert(!Tui_inputAddChar(nullptr, 'a'));
    assert(!Tui_inputBackspace(nullptr));
    assert(!Tui_entriesReserve(nullptr, 1));
    assert(!Tui_inputReserve(nullptr, 1));
    assert(!Tui_setSize(nullptr, 1, 1));
    assert(!Tui_isReady(nullptr) && !Tui_isDirty(nullptr));
    assert(Tui_getInput(nullptr) == nullptr && Tui_getCount(nullptr) == 0);
    assert(Tui_getCapacity(nullptr) == 0 && Tui_getInputLength(nullptr) == 0);
    assert(Tui_getInputCapacity(nullptr) == 0 && Tui_getScroll(nullptr) == 0);
    assert(Tui_getViewportHeight(nullptr) == 0 && Tui_getViewportWidth(nullptr) == 0);
    assert(Tui_getVisualRowCount(nullptr) == 0 && Tui_getFirstVisibleRow(nullptr) == 0);
    assert(Tui_toString(nullptr, output, sizeof(output), &cut) && strcmp(output, "nullptr") == 0);
    assert(Tui_toStringStruct(nullptr, output, sizeof(output), nullptr));
    Tui_getSize(nullptr, &rows, &columns); assert(rows == 0 && columns == 0);
    Tui_getSize(&c, nullptr, nullptr);
    Tui_inputClear(nullptr); Tui_clear(nullptr); Tui_free(nullptr);
    Tui_scrollBy(nullptr, 1); Tui_setScroll(nullptr, 1); Tui_setDirty(nullptr, true);
    size_t count = Tui_getCount(&c);
    failAfter = 0;
    assert(!Tui_add(&c, "preserve", 8) && Tui_getCount(&c) == count);
    assert(!Tui_setInput(&c, "preserve", 8) && Tui_getInputLength(&c) == 0);
    assert(!Tui_entriesReserve(&c, Tui_getCapacity(&c) + 1));
    assert(!Tui_inputReserve(&c, Tui_getInputCapacity(&c) + 1));
    Tui failed = Tui(1, 1); assert(!Tui_isReady(&failed));
    failAfter = 1;
    failed = Tui(1, 1); assert(!Tui_isReady(&failed));
    failAfter = -1;
    assert(Tui_add(&c, "recovered", 9));
    Tui_free(&c); Tui_free(&c);
    assert(!Tui_isReady(&c));
    failed = Tui(0); assert(!Tui_isReady(&failed));
    failed = Tui(SIZE_MAX, 1); assert(!Tui_isReady(&failed));
    puts("tui owner passed");
}
