/* Owner test for space/task.c. Exhausts the coordination value: the four-state
 * lifecycle, every illegal transition (each preserving state), actor/assignee
 * authority, permission-ceiling narrowing on delegation, and full rejection
 * preservation for both parent and destination. Delegation never mutates the
 * parent and never broadens permissions.
 */
#include "test_support.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "space/task.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[task] PASS %s\n", name); } \
    else { printf("[task] FAIL %s\n", name); g_failures++; } \
} while (0)

#define PERM_READ 1u
#define PERM_WRITE 2u
#define PERM_ADMIN 4u

int main(void) {
    printf("=== Task owner suite ===\n");

    Task zero = Task_0();
    CHECK("zero is all-zero", zero.id == 0 && zero.state == 0);
    CHECK("chooser() == _0", Task().id == 0);
    CHECK("zero() == _0", Task_zero().id == 0);

    Task root = Task(100, 7, 42, 9, PERM_READ | PERM_WRITE);
    CHECK("chooser 5-arg id", root.id == 100);
    CHECK("channel recorded", Task_getChannelId(&root) == 7);
    CHECK("requester recorded", Task_getRequesterId(&root) == 42);
    CHECK("assignee recorded", Task_getAssigneeId(&root) == 9);
    CHECK("permissions recorded", Task_getPermissions(&root) == (PERM_READ | PERM_WRITE));
    CHECK("root parent is zero", Task_getParentId(&root) == 0);
    CHECK("starts queued", Task_getState(&root) == TASK_QUEUED);

    CHECK("zero id rejected", Task(0, 7, 1, 9, PERM_READ).id == 0);
    CHECK("zero channel rejected", Task(1, 0, 1, 9, PERM_READ).id == 0);
    CHECK("zero assignee rejected", Task(1, 7, 1, 0, PERM_READ).id == 0);
    CHECK("requester 0 (host) allowed", Task(1, 7, 0, 9, PERM_READ).id == 1);
    Task maxed = Task(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX);
    CHECK("boundary max accepted", maxed.id == UINT64_MAX && maxed.permissions == UINT64_MAX);

    // --- Lifecycle: only the assigned actor starts a queued task.
    Task t = root;
    CHECK("start rejects wrong actor", !Task_start(&t, 42) && Task_getState(&t) == TASK_QUEUED);
    CHECK("start rejects actor 0", !Task_start(&t, 0) && Task_getState(&t) == TASK_QUEUED);
    CHECK("start rejects null self", !Task_start(nullptr, 9));
    CHECK("start accepts assignee", Task_start(&t, 9) && Task_getState(&t) == TASK_RUNNING);
    CHECK("start twice rejected", !Task_start(&t, 9) && Task_getState(&t) == TASK_RUNNING);
    CHECK("finish rejects wrong actor", !Task_finish(&t, 42) && Task_getState(&t) == TASK_RUNNING);
    CHECK("finish accepts assignee", Task_finish(&t, 9) && Task_getState(&t) == TASK_DONE);
    CHECK("finish stays done", !Task_finish(&t, 9) && Task_getState(&t) == TASK_DONE);
    CHECK("cancel done rejected", !Task_cancel(&t) && Task_getState(&t) == TASK_DONE);

    Task q = root;
    CHECK("cancel queued accepted", Task_cancel(&q) && Task_getState(&q) == TASK_CANCELLED);
    CHECK("cancel twice rejected", !Task_cancel(&q) && Task_getState(&q) == TASK_CANCELLED);
    Task emptyTask = Task_0();
    CHECK("cancel null self rejected", !Task_cancel(nullptr) && !Task_cancel(&emptyTask));
    Task r = root;
    CHECK("start then cancel", Task_start(&r, 9) && Task_cancel(&r) && Task_getState(&r) == TASK_CANCELLED);

    // --- Delegation: a running assignee creates a narrowed child.
    Task parent = root;
    CHECK("delegate requires running", !Task_delegate(&parent, 9, 200, 7, 3, PERM_READ, &(Task){0}) &&
                                        Task_getState(&parent) == TASK_QUEUED);
    CHECK("start parent", Task_start(&parent, 9));

    Task child;
    memset(&child, 0xAB, sizeof(child));
    bool ok = Task_delegate(&parent, 9, 200, 8, 3, PERM_READ, &child);
    CHECK("delegate child id", ok && Task_getId(&child) == 200);
    CHECK("delegate child channel", Task_getChannelId(&child) == 8);
    CHECK("delegate child assignee", Task_getAssigneeId(&child) == 3);
    CHECK("delegate child requester is actor", Task_getRequesterId(&child) == 9);
    CHECK("delegate child parent", Task_getParentId(&child) == 100);
    CHECK("delegate child permissions", Task_getPermissions(&child) == PERM_READ);
    CHECK("delegate child queued", Task_getState(&child) == TASK_QUEUED);
    CHECK("parent unchanged", Task_getState(&parent) == TASK_RUNNING && Task_getParentId(&parent) == 0);

    Task deny = (Task) { .id = 55 };
    CHECK("delegate rejects superset permissions",
          !Task_delegate(&parent, 9, 201, 8, 3, PERM_READ | PERM_WRITE | PERM_ADMIN, &deny));
    CHECK("delegate allows equal permissions",
          Task_delegate(&parent, 9, 202, 8, 3, PERM_READ | PERM_WRITE, &(Task){0}));
    CHECK("delegate rejects wrong actor", !Task_delegate(&parent, 42, 201, 8, 3, PERM_READ, &deny));
    CHECK("delegate rejects actor 0", !Task_delegate(&parent, 0, 201, 8, 3, PERM_READ, &deny));
    CHECK("delegate rejects reused id", !Task_delegate(&parent, 9, 100, 8, 3, PERM_READ, &deny));
    CHECK("delegate rejects zero child id", !Task_delegate(&parent, 9, 0, 8, 3, PERM_READ, &deny));
    CHECK("delegate rejects zero channel", !Task_delegate(&parent, 9, 201, 0, 3, PERM_READ, &deny));
    CHECK("delegate rejects zero assignee", !Task_delegate(&parent, 9, 201, 8, 0, PERM_READ, &deny));
    CHECK("delegate rejects null parent", !Task_delegate(nullptr, 9, 201, 8, 3, PERM_READ, &deny));
    CHECK("delegate rejects null dest", !Task_delegate(&parent, 9, 201, 8, 3, PERM_READ, nullptr));
    CHECK("delegate rejects dest == parent", !Task_delegate(&parent, 9, 201, 8, 3, PERM_READ, &parent));
    CHECK("rejections preserve parent", Task_getState(&parent) == TASK_RUNNING);
    CHECK("rejections preserve dest", deny.id == 55 && deny.state == 0 && deny.permissions == 0);

    // --- Grandchild: a started child can delegate within its own ceiling.
    CHECK("start child", Task_start(&child, 3));
    Task grand;
    CHECK("grandchild delegated", Task_delegate(&child, 3, 300, 9, 4, PERM_READ, &grand) &&
                                  Task_getParentId(&grand) == 200 && Task_getPermissions(&grand) == PERM_READ);

    // --- Getters and projections.
    CHECK("getters null self", Task_getId(nullptr) == 0 && Task_getChannelId(nullptr) == 0 &&
                               Task_getRequesterId(nullptr) == 0 && Task_getAssigneeId(nullptr) == 0 &&
                               Task_getParentId(nullptr) == 0 && Task_getPermissions(nullptr) == 0 &&
                               Task_getState(nullptr) == 0);
    char buf[256];
    bool cut = false;
    CHECK("toString null self", Task_toString(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toString value", Task_toString(&root, buf, sizeof(buf), &cut) && strcmp(buf, "task 100 state=1") == 0);
    CHECK("toStringStruct null self", Task_toStringStruct(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toStringStruct fields",
          Task_toStringStruct(&root, buf, sizeof(buf), &cut) &&
          strcmp(buf, "Task{id=100,channelId=7,requesterId=42,assigneeId=9,parentId=0,permissions=3,state=1}") == 0);
    CHECK("toString truncates", !Task_toString(&root, buf, 4, &cut) && cut);
    CHECK("toStringStruct truncates", !Task_toStringStruct(&root, buf, 10, &cut) && cut);

    if (g_failures != 0) {
        printf("=== %d TASK FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL TASK PASS ===\n");
    return 0;
}
