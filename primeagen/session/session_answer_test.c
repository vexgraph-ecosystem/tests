/* Headless native answer client: real SessionBridge/Haven admission into caller
 * supplied host pipes. No ncurses/gallery; answer printed as bounded JSON only.
 * Registered Python owner runs fake CLI by default; live mode is explicit. */
#include "session/session_bridge.h"
#include "net/json.h"
#include <assert.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv) {
    assert(argc == 3);
    SessionBridge bridge = SessionBridge(atoi(argv[1]), atoi(argv[2]));
    SessionBridge_setOwo(&bridge, true);
    assert(SessionBridge_submit(&bridge, "What is 2 + 2? Reply with one short sentence containing the digit 4. Do not use tools or inspect any files."));
    int result = 0;
    while (!result) {
        result = SessionBridge_poll(&bridge);
        if (!result) {
            struct pollfd fd = {atoi(argv[1]), POLLIN, 0};
            assert(poll(&fd, 1, 100) >= 0);
        }
    }
    SessionBridge_free(&bridge);
    assert(result == 1);
    const char *text = SessionBridge_getText(&bridge);
    assert(strstr(text, "4"));
    char quoted[SESSION_RESPONSE_CAP];
    assert(Json_writeString(quoted, sizeof(quoted), text) > 0);
    puts(quoted);
    return 0;
}
