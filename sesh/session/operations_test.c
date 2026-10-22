/* Flat ledger owner: growth, copied admission, replay/reuse, preserved outputs.
 * No arbitrary-pointer rejection, allocation ownership or concurrency promised. */
#include "support.h"
int main(int argc, char **argv) {
    OperationsSlot first[1], grown[16];
    Operations operations = Operations(first, 1);
    assert(Operations_0().count == 0 && Operations().count == 0 && Operations_zero().count == 0);
    Operations direct = Operations_2(grown, 16);
    assert(Operations_getCapacity(&direct) == 16 && Operations_getCount(nullptr) == 0 && Operations_getCapacity(nullptr) == 0);
    Operation value = Operation(1, 1, 1, 1, 0);
    assert(Operations_add(&operations, &value) == OPERATIONS_ADDED);
    assert(Operations_add(&operations, &value) == OPERATIONS_REPLAY);
    assert(!Operations_isApplied(&operations, 1, 1) && !Operations_isApplied(nullptr, 1, 1));
    Resource resource = Resource(1, 1, 0);
    assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_ADDED);
    assert(Operations_isApplied(&operations, 1, 1) && Resource_getRevision(&resource) == 1);
    assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_REPLAY);
    Operation copy = Operation(99, 1, 1, 1, 0);
    assert(!Operations_find(&operations, 1, 99, &copy) && copy.id == 99);
    assert(Operations_find(&operations, 1, 1, &copy) && Operation_equals(&copy, &value));
    assert(Operation_setExpectedRevision(&value, 100));
    assert(Operations_find(&operations, 1, 1, &copy) && copy.expectedRevision == 0);
    assert(Operations_reserve(&operations, grown, 16));
    first[0].operation.id = 99; /* old caller storage is no longer borrowed */
    assert(Operations_find(&operations, 1, 1, &copy) && copy.id == 1);
    for (uint64_t id = 2; id <= 16; id++) {
        value = Operation(id, 1, 1, 1, id);
        assert(Operations_add(&operations, &value) == OPERATIONS_ADDED);
    }
    assert(Operations_getCount(&operations) == 16 && Operations_getCapacity(&operations) == 16);
    assert(Operations_isApplied(&operations, 1, 1) && !Operations_isApplied(&operations, 1, 99));
    PROJECTIONS(Operations, operations, "count=16,capacity=16");
    if (INVALID_MODE) {
        assert(Operations(nullptr, 1).capacity == 0);
        assert(Operations(first, 0).capacity == 0);
        assert(!Operations_reserve(nullptr, first, 1));
        assert(!Operations_reserve(&operations, nullptr, 16));
        assert(!Operations_reserve(&operations, first, SIZE_MAX));
        assert(!Operations_reserve(&operations, first, 1));
        assert(Operations_getCount(&operations) == 16);
        value = Operation(17, 1, 1, 1, 0);
        assert(Operations_add(&operations, &value) == OPERATIONS_REJECTED);
        assert(Operations_add(nullptr, &value) == OPERATIONS_REJECTED);
        assert(Operations_add(&operations, nullptr) == OPERATIONS_REJECTED);
        value = Operation(1, 1, 1, 1, 99);
        assert(Operations_add(&operations, &value) == OPERATIONS_REJECTED);
        assert(!Operations_find(nullptr, 1, 1, &copy));
        assert(!Operations_find(&operations, 1, 1, nullptr));
        assert(!Operations_find(&operations, 0, 1, &copy));
        assert(!Operations_find(&operations, 1, 0, &copy));
        assert(copy.id == 1 && copy.expectedRevision == 0);
        assert(Operations_apply(nullptr, &value, &resource) == OPERATIONS_REJECTED);
        assert(Operations_apply(&operations, nullptr, &resource) == OPERATIONS_REJECTED);
        assert(Operations_apply(&operations, &value, nullptr) == OPERATIONS_REJECTED);
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_REJECTED);
        value = Operation(17, 1, 1, 1, 0);
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_REJECTED);
        value = Operation(2, 1, 1, 1, 2);
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_CONFLICT);
        assert(Resource_setRevision(&resource, 2));
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_ADDED);
        assert(Resource_getRevision(&resource) == 3);
        value = Operation(17, 1, 9, 1, 0);
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_REJECTED);
        value = Operation(17, 1, 1, 9, 0);
        assert(Operations_apply(&operations, &value, &resource) == OPERATIONS_REJECTED);
        OperationsSlot overflowSlot[1];
        Operations overflow = Operations(overflowSlot, 1);
        value = Operation(1, 1, 1, 1, UINT64_MAX);
        assert(Operations_add(&overflow, &value) == OPERATIONS_ADDED);
        assert(Resource_setRevision(&resource, UINT64_MAX));
        assert(Operations_apply(&overflow, &value, &resource) == OPERATIONS_REJECTED);
        assert(!Operations_isApplied(&overflow, 1, 1) && Resource_getRevision(&resource) == UINT64_MAX);
        BAD_PROJECTIONS(Operations, operations);
    }
    Operations_clear(&operations); Operations_clear(&operations); Operations_clear(nullptr);
    assert(Operations_getCount(&operations) == 0);
    value = Operation(1, 2, 1, 1, 0);
    assert(Operations_add(&operations, &value) == OPERATIONS_ADDED);
    return 0;
}
