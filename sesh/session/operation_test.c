/* All operation identity forms, boundaries, equality and copied intent vocabulary. */
#include "support.h"
int main(int argc, char **argv) {
    Operation operation = Operation(1, 2, 3, 4, 0);
    assert(!Operation_isValid(nullptr));
    Operation empty = Operation();
    assert(!Operation_isValid(&empty) && Operation_0().id == 0 && Operation_zero().id == 0);
    Operation direct = Operation_5(1, 2, 3, 4, 0);
    assert(Operation_equals(&direct, &operation));
    assert(!Operation_equals(nullptr, &operation) && !Operation_equals(&operation, nullptr));
    assert(Operation_getId(nullptr) == 0 && Operation_getClientId(nullptr) == 0 &&
           Operation_getWorkspaceId(nullptr) == 0 && Operation_getResourceId(nullptr) == 0 && Operation_getExpectedRevision(nullptr) == 0);
#define ROUNDTRIP(Name) assert(Operation_set##Name(&operation, UINT64_MAX)); assert(Operation_get##Name(&operation) == UINT64_MAX)
    ROUNDTRIP(Id); ROUNDTRIP(ClientId); ROUNDTRIP(WorkspaceId); ROUNDTRIP(ResourceId); ROUNDTRIP(ExpectedRevision);
#undef ROUNDTRIP
    assert(!Operation_equals(&direct, &operation));
    PROJECTIONS(Operation, operation, "resourceId=18446744073709551615,expectedRevision=");
    if (INVALID_MODE) {
#define BAD_SET(Name) assert(!Operation_set##Name(nullptr, 1)); assert(!Operation_set##Name(&operation, 0)); assert(Operation_get##Name(&operation) == UINT64_MAX)
        BAD_SET(Id); BAD_SET(ClientId); BAD_SET(WorkspaceId); BAD_SET(ResourceId);
#undef BAD_SET
        assert(!Operation_setExpectedRevision(nullptr, 0));
        assert(Operation(0, 1, 1, 1, 0).id == 0);
        assert(Operation(1, 0, 1, 1, 0).id == 0);
        assert(Operation(1, 1, 0, 1, 0).id == 0);
        assert(Operation(1, 1, 1, 0, 0).id == 0);
        BAD_PROJECTIONS(Operation, operation);
    }
    return 0;
}
