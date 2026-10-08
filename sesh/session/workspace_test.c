/* Private-workspace identity owner; no remote ACL verification claim. */
#include "support.h"
int main(int argc, char **argv) {
    Workspace workspace = Workspace(1, 2);
    assert(Workspace_0().id == 0 && Workspace().id == 0 && Workspace_zero().id == 0);
    assert(Workspace_2(3, 4).principalId == 4);
    assert(Workspace_getId(nullptr) == 0 && Workspace_getPrincipalId(nullptr) == 0);
    assert(Workspace_setId(&workspace, UINT64_MAX));
    assert(Workspace_setPrincipalId(&workspace, UINT64_MAX));
    assert(Workspace_getId(&workspace) == UINT64_MAX && Workspace_getPrincipalId(&workspace) == UINT64_MAX);
    PROJECTIONS(Workspace, workspace, "id=18446744073709551615,principalId=");
    if (INVALID_MODE) {
        assert(!Workspace_setId(nullptr, 1));
        assert(!Workspace_setPrincipalId(nullptr, 1));
        assert(!Workspace_setId(&workspace, 0));
        assert(!Workspace_setPrincipalId(&workspace, 0));
        assert(workspace.id == UINT64_MAX && workspace.principalId == UINT64_MAX);
        assert(Workspace(0, 1).id == 0);
        assert(Workspace(1, 0).id == 0);
        BAD_PROJECTIONS(Workspace, workspace);
    }
    return 0;
}
