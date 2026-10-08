/* Internal bounded formatter owner: optional flag, exact capacity, escaping
 * responsibility at caller (only numeric/public metadata currently supplied). */
#include "session/text.h"
#include <assert.h>
#include <string.h>
int main(int argc, char **argv) {
    char text[5]; bool truncated = true;
    assert(SeshText_format(text, sizeof(text), &truncated, "%s", "text") && !truncated);
    assert(strcmp(text, "text") == 0);
    assert(SeshText_format(text, sizeof(text), nullptr, "%d", 0));
    char ten[11];
    assert(SeshText_format(ten, sizeof(ten), nullptr, "%d%d%d%d%d%d%d%d%d%d", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
    assert(strcmp(ten, "0123456789") == 0);
    if (argc == 2 && strcmp(argv[1], "invalid") == 0) {
        assert(!SeshText_format(nullptr, 5, &truncated, "%s", "text") && truncated);
        assert(!SeshText_format(text, 0, &truncated, "%s", "text") && truncated);
        assert(!SeshText_format(text, 4, &truncated, "%s", "text") && truncated);
        assert(strcmp(text, "tex") == 0);
        assert(!SeshText_format(text, 1, nullptr, "%s", "text") && text[0] == '\0');
        assert(!SeshText_format(text, sizeof(text), &truncated, nullptr) && !truncated && text[0] == '\0');
    }
    return 0;
}
