/* SeshClient public identity/value owner; host persistence is not implemented. */
#include "support.h"
int main(int argc, char **argv) {
    SeshClient client = SeshClient(1);
    assert(SeshClient_getId(&client) == 1);
    assert(SeshClient_getId(nullptr) == 0);
    assert(SeshClient_0().id == 0 && SeshClient().id == 0 && SeshClient_zero().id == 0);
    assert(SeshClient_1(2).id == 2);
    assert(SeshClient_setId(&client, UINT64_MAX));
    PROJECTIONS(SeshClient, client, "id=18446744073709551615");
    if (INVALID_MODE) {
        assert(!SeshClient_setId(nullptr, 1));
        assert(!SeshClient_setId(&client, 0) && SeshClient_getId(&client) == UINT64_MAX);
        assert(SeshClient(0).id == 0);
        BAD_PROJECTIONS(SeshClient, client);
    }
    return 0;
}
