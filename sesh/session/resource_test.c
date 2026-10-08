/* Resource metadata owner; revisions are local externally serialized values. */
#include "support.h"
int main(int argc, char **argv) {
    Resource resource = Resource(1, 2, 0);
    assert(Resource_0().id == 0 && Resource().id == 0 && Resource_zero().id == 0);
    assert(Resource_3(3, 4, 5).revision == 5);
    assert(Resource_getId(nullptr) == 0 && Resource_getWorkspaceId(nullptr) == 0 && Resource_getRevision(nullptr) == 0);
    assert(Resource_setId(&resource, UINT64_MAX));
    assert(Resource_setWorkspaceId(&resource, UINT64_MAX));
    assert(Resource_setRevision(&resource, UINT64_MAX));
    assert(Resource_getId(&resource) == UINT64_MAX && Resource_getWorkspaceId(&resource) == UINT64_MAX && Resource_getRevision(&resource) == UINT64_MAX);
    PROJECTIONS(Resource, resource, "workspaceId=18446744073709551615,revision=");
    if (INVALID_MODE) {
        assert(!Resource_setId(nullptr, 1));
        assert(!Resource_setWorkspaceId(nullptr, 1));
        assert(!Resource_setRevision(nullptr, 0));
        assert(!Resource_setId(&resource, 0));
        assert(!Resource_setWorkspaceId(&resource, 0));
        assert(resource.id == UINT64_MAX && resource.workspaceId == UINT64_MAX && resource.revision == UINT64_MAX);
        assert(Resource(0, 1, 0).id == 0);
        assert(Resource(1, 0, 0).id == 0);
        BAD_PROJECTIONS(Resource, resource);
    }
    return 0;
}
