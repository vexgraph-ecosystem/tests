// Engine-owned bounded frame slot: full buffer, preservation, retry, timeout and
// cancellation. Single-owner API; no concurrent cancellation claim is tested.
// Existing silent rejection policy is retained; THROW/toString gaps remain.
#include "io/ws_client.h"
#include "nio/mem.h"
#include <assert.h>
#include <string.h>

int main(void) {
    WsClient *a = WsClient();
    WsClient *b = WsClient(UINT64_MAX);
    assert(a && b);
    assert(WsClient_getTimeoutNs(b) == WS_CLIENT_POLL_MAX_NS);
    assert(WsClient_getState(a) == WS_CLIENT_IDLE);
    assert(WsClient_getState(nullptr) == WS_CLIENT_CLOSED);
    assert(WsClient_getTimeoutNs(nullptr) == 0);
    assert(WsClient_isCancelled(nullptr));
    assert(WsClient_getPending(nullptr) == 0);
    WsClient_setState(a, WS_CLIENT_OPEN);
    WsClient_setTimeoutNs(a, 1);
    assert(WsClient_getState(a) == WS_CLIENT_OPEN && WsClient_getTimeoutNs(a) == 1);
    WsClient_setTimeoutNs(a, UINT64_MAX);
    assert(WsClient_getTimeoutNs(a) == WS_CLIENT_POLL_MAX_NS);
    uint8_t source[WS_CLIENT_RX_CAP], dest[WS_CLIENT_RX_CAP];
    memset(source, 42, sizeof source);
    assert(!WsClient_feed(nullptr, source, 1));
    assert(!WsClient_feed(a, nullptr, 1));
    assert(!WsClient_feed(a, source, 0));
    assert(!WsClient_feed(a, source, UINT32_MAX));
    assert(WsClient_feed(a, source, sizeof source));
    assert(!WsClient_feed(a, source, 1));
    assert(WsClient_getPending(a) == sizeof source);
    uint32_t length = 99;
    memset(dest, 7, sizeof dest);
    assert(!WsClient_poll(a, 0, dest, sizeof dest - 1, &length));
    assert(length == 0 && dest[0] == 7 && WsClient_getPending(a) == sizeof source);
    assert(!WsClient_poll(nullptr, 0, dest, sizeof dest, &length));
    assert(!WsClient_poll(a, 0, nullptr, sizeof dest, &length));
    assert(!WsClient_poll(a, 0, dest, 0, &length));
    assert(!WsClient_poll(a, 0, dest, sizeof dest, nullptr));
    assert(WsClient_poll(a, 0, dest, sizeof dest, &length));
    assert(length == sizeof source && memcmp(source, dest, length) == 0);
    assert(!WsClient_poll(a, 1, dest, sizeof dest, &length) && length == 0);
    assert(WsClient_feed(a, source, 1));
    WsClient_cancel(a);
    assert(!WsClient_poll(a, UINT64_MAX, dest, sizeof dest, &length));
    assert(WsClient_getPending(a) == 1 && WsClient_isCancelled(a));
    WsClient_setCancelled(a, false);
    assert(WsClient_poll(a, 0, dest, sizeof dest, &length) && length == 1);
    assert(WsClient_feed(a, source, 1));
    WsClient_clear(a);
    assert(WsClient_getPending(a) == 0 && WsClient_getState(a) == WS_CLIENT_OPEN);
    WsClient_clear(nullptr); WsClient_cancel(nullptr);
    WsClient_setState(nullptr, WS_CLIENT_OPEN);
    WsClient_setTimeoutNs(nullptr, 1); WsClient_setCancelled(nullptr, false);
    WsClient_free(a); WsClient_free(b); WsClient_free(nullptr);
    Memory_freeAll();
    return 0;
}
