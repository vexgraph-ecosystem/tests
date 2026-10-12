/* Header owner: scalar decoding boundaries and malformed admission preservation;
 * terminal width under stable UTF-8/C locales. No grapheme/font/hardware claim. */
#include "tui/utf8.h"
#include <assert.h>
#include <locale.h>
#include <string.h>

int main(void) {
    uint32_t cp = 99;
    const char *valid[] = {"a", "é", "中", "👋", "\xf4\x8f\xbf\xbf"};
    const uint32_t scalars[] = {'a', 0xe9, 0x4e2d, 0x1f44b, 0x10ffff};
    for (size_t i = 0; i < sizeof(valid) / sizeof(*valid); ++i) {
        size_t length = strlen(valid[i]);
        assert(TuiUtf8_decode(valid[i], length, &cp) == length && cp == scalars[i]);
        for (size_t n = 0; n < length; ++n) {
            cp = 99;
            assert(TuiUtf8_decode(valid[i], n, &cp) == 0 && cp == 99);
        }
    }
    const char *bad[] = {"\x80", "\xc0\xaf", "\xe0\x80\x80", "\xed\xa0\xbd", "\xf0\x80\x80\x80", "\xf4\x90\x80\x80", "\xff", "\xc2x"};
    for (size_t i = 0; i < sizeof(bad) / sizeof(*bad); ++i) {
        cp = 99;
        assert(TuiUtf8_decode(bad[i], strlen(bad[i]), &cp) == 0 && cp == 99);
    }
    assert(TuiUtf8_decode(nullptr, 1, &cp) == 0);
    assert(TuiUtf8_decode("a", 1, nullptr) == 0);
    assert(!TuiUtf8_safe(0) && !TuiUtf8_safe(0x1b) && !TuiUtf8_safe(0x7f));
    assert(!TuiUtf8_safe(0x9b) && !TuiUtf8_safe(0x202e) && !TuiUtf8_safe(0x2066));
    assert(TuiUtf8_safe('\n') && TuiUtf8_safe(0x1f44b));
    assert(setlocale(LC_CTYPE, "en_US.UTF-8") != nullptr);
    assert(TuiUtf8_width(0x1f44b) == 2 && TuiUtf8_width(0x4e2d) == 2);
    assert(TuiUtf8_width(0x301) == 0 && TuiUtf8_width('a') == 1);
    assert(setlocale(LC_CTYPE, "C") != nullptr);
    assert(TuiUtf8_width(0x1f44b) == 1);
}
