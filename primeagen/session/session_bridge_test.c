/* Owner: real API Haven EngineProvider/Harness with deterministic pipe driver.
 * No provider or interactive terminal. Partial IO, busy preservation, rejection,
 * malformed/duplicate/trailing/overflow frames, poisoned-wire failure, teardown
 * and borrowed-fd survival. External runner bounds all cases. */
#include "session/session_bridge.h"
#include "personality/owo.h"
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

static void fixture(int *request, int *response, SessionBridge *bridge) {
    assert(pipe(request) == 0 && pipe(response) == 0);
    assert(fcntl(request[1], F_SETFL, O_NONBLOCK) == 0);
    assert(fcntl(response[0], F_SETFL, O_NONBLOCK) == 0);
    *bridge = SessionBridge(response[0], request[1]);
}
static void cleanup(int *request, int *response, SessionBridge *bridge) {
    SessionBridge_free(bridge);
    for (int i = 0; i < 2; ++i) {
        close(request[i]);
        close(response[i]);
    }
}
int main(void) {
    SessionBridge empty = SessionBridge();
    assert(!SessionBridge_isBusy(nullptr) && !SessionBridge_isOwo(nullptr));
    assert(strcmp(SessionBridge_getText(nullptr), "") == 0);
    assert(SessionBridge_poll(nullptr) == 0);
    assert(SessionBridge_getTimeout(nullptr) == 0);
    assert(SessionBridge_getTimeout(&empty) == SESSION_TIMEOUT_MS_DEFAULT);
    assert(!SessionBridge_setTimeout(nullptr, 10));
    assert(!SessionBridge_setTimeout(&empty, 0));
    assert(!SessionBridge_setTimeout(&empty, SESSION_TIMEOUT_MS_MAX + 1));
    assert(SessionBridge_setTimeout(&empty, 1));
    assert(SessionBridge_getTimeout(&empty) == 1);
    SessionBridge_free(nullptr);
    SessionBridge_setOwo(nullptr, true);
    assert(!SessionBridge_submit(&empty, "test"));
    assert(!SessionBridge_submit(nullptr, "test"));
    char projection[512]; bool cut;
    assert(SessionBridge_toString(nullptr, projection, sizeof(projection), &cut));
    assert(!cut && strcmp(projection, "nullptr") == 0);
    assert(!SessionBridge_toString(&empty, projection, 1, &cut) && cut);
    assert(!SessionBridge_toStringStruct(&empty, nullptr, 0, nullptr));
    assert(SessionBridge_toStringStruct(nullptr, projection, sizeof(projection), nullptr));
    int request[2], response[2]; SessionBridge bridge;
    fixture(request, response, &bridge);
    SessionBridge_setOwo(&bridge, true);
    assert(SessionBridge_isOwo(&bridge));
    assert(!SessionBridge_submit(&bridge, nullptr));
    assert(!SessionBridge_submit(&bridge, ""));
    assert(!SessionBridge_submit(&bridge, "bad\x01"));
    assert(!SessionBridge_submit(&bridge, "bad\xff"));
    assert(SessionBridge_submit(&bridge, "hello \"quoted\""));
    assert(SessionBridge_isBusy(&bridge));
    assert(!SessionBridge_setTimeout(&bridge, 10));
    assert(!SessionBridge_submit(&bridge, "busy"));
    assert(SessionBridge_poll(&bridge) == 0);
    char frame[SESSION_REQUEST_CAP];
    ssize_t count = read(request[0], frame, sizeof(frame) - 1);
    assert(count > 0); frame[count] = '\0';
    assert(strstr(frame, "hello \\\"quoted\\\"") && strstr(frame, "personality mode: owo"));
    assert(SessionBridge_toString(&bridge, projection, sizeof(projection), &cut));
    assert(strstr(projection, "pending"));
    assert(write(response[1], "{\"ok\":true,", 11) == 11);
    assert(SessionBridge_poll(&bridge) == 0);
    const char *tail = "\"text\":\"answer\\nnext\"}\n";
    assert(write(response[1], tail, strlen(tail)) == (ssize_t) strlen(tail));
    assert(SessionBridge_poll(&bridge) == 1);
    assert(strcmp(SessionBridge_getText(&bridge), "answer\nnext") == 0);
    assert(!SessionBridge_isBusy(&bridge));
    assert(SessionBridge_toStringStruct(&bridge, projection, sizeof(projection), &cut));
    assert(!cut && strstr(projection, "text:redacted") && !strstr(projection, "answer"));
    assert(!SessionBridge_toStringStruct(&bridge, projection, 1, &cut) && cut);
    assert(SessionBridge_submit(&bridge, "again"));
    assert(SessionBridge_poll(&bridge) == 0);
    SessionBridge_free(&bridge);
    assert(!SessionBridge_isBusy(&bridge) && fcntl(request[1], F_GETFL) >= 0);
    assert(!SessionBridge_submit(&bridge, "after cancel"));
    cleanup(request, response, &bridge);
    const char *bad[] = {"{}\n", "{\"ok\":true,\"ok\":true}\n", "{\"ok\":false,\"text\":\"secret\"}\n",
                        "{\"ok\":true,\"text\":\"a\\u0000b\"}\n", "{\"ok\":true,\"text\":\"ok\"}\nextra\n", "not-json\n"};
    for (size_t i = 0; i < sizeof(bad) / sizeof(*bad); ++i) {
        fixture(request, response, &bridge);
        assert(SessionBridge_submit(&bridge, "test"));
        assert(write(response[1], bad[i], strlen(bad[i])) == (ssize_t) strlen(bad[i]));
        assert(SessionBridge_poll(&bridge) == -1);
        assert(!strstr(SessionBridge_getText(&bridge), "secret"));
        assert(!SessionBridge_submit(&bridge, "retry"));
        cleanup(request, response, &bridge);
    }
    fixture(request, response, &bridge);
    char oversized[SESSION_REQUEST_CAP]; memset(oversized, 'x', sizeof(oversized) - 1);
    oversized[sizeof(oversized) - 1] = '\0';
    assert(!SessionBridge_submit(&bridge, oversized));
    assert(SessionBridge_submit(&bridge, "recover after admission rejection"));
    bridge.startedMs = 1; /* Inject clock deadline, no scheduling sleeps. */
    assert(SessionBridge_poll(&bridge) == -1);
    cleanup(request, response, &bridge);
    fixture(request, response, &bridge);
    assert(SessionBridge_submit(&bridge, "EOF"));
    close(response[1]); response[1] = -1;
    assert(SessionBridge_poll(&bridge) == -1);
    cleanup(request, response, &bridge);
    fixture(request, response, &bridge);
    assert(SessionBridge_submit(&bridge, "wire overflow"));
    char block[1024]; memset(block, 'x', sizeof(block));
    for (size_t i = 0; i < 64; ++i) {
        size_t size = i == 63 ? sizeof(block) - 1 : sizeof(block);
        assert(write(response[1], block, size) == (ssize_t) size);
        assert(SessionBridge_poll(&bridge) == 0);
    }
    assert(SessionBridge_poll(&bridge) == -1);
    cleanup(request, response, &bridge);
    return 0;
}
