/* Owner test for space/channel.c. Exhausts the relationship value: dedicated
 * destination identity (id == persona), server scoping, boundary ids, both
 * constructor forms, null-safe getters and bounded projections. Invalid
 * construction returns the empty channel.
 */
#include "test_support.h"

#include <stdint.h>
#include <stdio.h>

#include "space/channel.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[channel] PASS %s\n", name); } \
    else { printf("[channel] FAIL %s\n", name); g_failures++; } \
} while (0)

int main(void) {
    printf("=== Channel owner suite ===\n");

    Channel zero = Channel_0();
    Channel chooserZero = Channel();
    CHECK("zero is all-zero", zero.id == 0 && zero.serverId == 0 && zero.modelUserId == 0);
    CHECK("chooser() == _0", chooserZero.id == zero.id && chooserZero.serverId == zero.serverId);
    CHECK("zero() == _0", Channel_zero().id == 0);

    Channel c = Channel(11, 22);
    CHECK("id is the persona", c.id == 22);
    CHECK("serverId recorded", c.serverId == 11);
    CHECK("modelUserId recorded", c.modelUserId == 22);
    CHECK("chain 2-arg form", Channel(11, 22).id == Channel_2(11, 22).id);

    Channel big = Channel(UINT64_MAX, UINT64_MAX);
    CHECK("boundary max accepted", big.serverId == UINT64_MAX && big.modelUserId == UINT64_MAX && big.id == UINT64_MAX);

    CHECK("zero server rejected", Channel(0, 22).id == 0 && Channel(0, 22).serverId == 0);
    CHECK("zero persona rejected", Channel(11, 0).id == 0 && Channel(11, 0).modelUserId == 0);

    CHECK("getters null self", Channel_getId(nullptr) == 0 && Channel_getServerId(nullptr) == 0 &&
                               Channel_getModelUserId(nullptr) == 0);
    CHECK("getters live", Channel_getId(&c) == 22 && Channel_getServerId(&c) == 11 &&
                          Channel_getModelUserId(&c) == 22);
    CHECK("same id, different servers are distinct", Channel(1, 9).id == Channel(2, 9).id &&
                                                     Channel(1, 9).serverId != Channel(2, 9).serverId);

    char buf[128];
    bool cut = false;
    CHECK("toString null self", Channel_toString(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toString value", Channel_toString(&c, buf, sizeof(buf), &cut) && strcmp(buf, "channel 11/22") == 0);
    CHECK("toStringStruct null self", Channel_toStringStruct(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toStringStruct fields",
          Channel_toStringStruct(&c, buf, sizeof(buf), &cut) &&
          strcmp(buf, "Channel{id=22,serverId=11,modelUserId=22}") == 0 && !cut);
    CHECK("toString truncates", !Channel_toString(&c, buf, 4, &cut) && cut);
    CHECK("toStringStruct truncates", !Channel_toStringStruct(&c, buf, 5, &cut) && cut);

    if (g_failures != 0) {
        printf("=== %d CHANNEL FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL CHANNEL PASS ===\n");
    return 0;
}
