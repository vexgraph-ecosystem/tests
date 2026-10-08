/* Sesh composition owner: all parts/constructors, scoped authenticated admission,
 * copied history and replay, stale revisions, failure atomicity and borrow detach.
 * Multiple Sesh objects share state only under one caller-owned serialization
 * domain. This tests local metadata, not OAuth, data sync or distributed CAS.
 */
#include "support.h"
#include <pthread.h>

typedef struct Worker {
    Sesh session;
    pthread_mutex_t *mutex;
    uint64_t clientId;
    pthread_cond_t *condition;
    size_t *ready;
    bool *start;
} Worker;

static void *submitWorker(void *context) {
    Worker *worker = context;
    assert(pthread_mutex_lock((*worker).mutex) == 0);
    ++*(*worker).ready;
    assert(pthread_cond_broadcast((*worker).condition) == 0);
    while (!*(*worker).start)
        assert(pthread_cond_wait((*worker).condition, (*worker).mutex) == 0);
    assert(pthread_mutex_unlock((*worker).mutex) == 0);
    for (uint64_t id = 1; id <= 32; id++) {
        assert(pthread_mutex_lock((*worker).mutex) == 0);
        Sesh *session = &(*worker).session;
        Resource *resource = Sesh_getResource(session);
        Operation operation = Operation(id, (*worker).clientId, 3, 4, Resource_getRevision(resource));
        assert(Sesh_submit(session, &operation) == SESH_APPLIED);
        assert(Sesh_submit(session, &operation) == SESH_REPLAY);
        assert(pthread_mutex_unlock((*worker).mutex) == 0);
    }
    return nullptr;
}

static void serializedConcurrency(AuthService *auth, Workspace *workspace) {
    Resource resource = Resource(4, 3, 0);
    OperationsSlot rows[128];
    Operations ledger = Operations(rows, 128);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
    size_t ready = 0;
    bool start = false;
    SeshClient clients[4];
    Worker workers[4];
    pthread_t threads[4];
    for (size_t i = 0; i < 4; i++) {
        clients[i] = SeshClient(i + 1);
        workers[i] = (Worker) {Sesh(auth, &clients[i], workspace, &resource, &ledger), &mutex, i + 1, &condition, &ready, &start};
        assert(pthread_create(&threads[i], nullptr, submitWorker, &workers[i]) == 0);
    }
    assert(pthread_mutex_lock(&mutex) == 0);
    while (ready != 4)
        assert(pthread_cond_wait(&condition, &mutex) == 0);
    start = true;
    assert(pthread_cond_broadcast(&condition) == 0);
    assert(pthread_mutex_unlock(&mutex) == 0);
    for (size_t i = 0; i < 4; i++)
        assert(pthread_join(threads[i], nullptr) == 0);
    assert(Resource_getRevision(&resource) == 128 && Operations_getCount(&ledger) == 128);
    assert(pthread_mutex_destroy(&mutex) == 0);
    assert(pthread_cond_destroy(&condition) == 0);
}

int main(int argc, char **argv) {
    ApiAuth credential = { .kind = API_AUTH_BEARER, .credential = "NEVER-PRINT-TOKEN" };
    uint64_t principal = 7;
    AuthService auth = AuthService(&credential, verify, &principal);
    assert(AuthService_authenticate(&auth));
    SeshClient client = SeshClient(2);
    Workspace workspace = Workspace(3, 7);
    Resource resource = Resource(4, 3, 0);
    OperationsSlot storage[2];
    OperationsSlot grown[4];
    Operations operations = Operations(storage, 2);
    Sesh session = Sesh(&auth, &client, &workspace, &resource, &operations);
    Sesh direct = Sesh_5(&auth, &client, &workspace, &resource, &operations);
    Sesh empty = Sesh();
    assert(Sesh_0().client == nullptr && Sesh_zero().client == nullptr);
    assert(Sesh_setAuthService(&empty, &auth));
    assert(Sesh_setClient(&empty, &client));
    assert(Sesh_setWorkspace(&empty, &workspace));
    assert(Sesh_setResource(&empty, &resource));
    assert(Sesh_setOperations(&empty, &operations));
    assert(Sesh_getAuthService(&empty) == &auth && Sesh_getClient(&empty) == &client &&
           Sesh_getWorkspace(&empty) == &workspace && Sesh_getResource(&empty) == &resource && Sesh_getOperations(&empty) == &operations);
    assert(Sesh_getAuthService(nullptr) == nullptr && Sesh_getClient(nullptr) == nullptr &&
           Sesh_getWorkspace(nullptr) == nullptr && Sesh_getResource(nullptr) == nullptr && Sesh_getOperations(nullptr) == nullptr);
    Operation operation = Operation(1, 2, 3, 4, 0);
    assert(Operations_add(&operations, &operation) == OPERATIONS_ADDED);
    assert(!Operations_isApplied(&operations, 2, 1));
    assert(Sesh_submit(&session, &operation) == SESH_APPLIED);
    assert(Resource_getRevision(&resource) == 1 && Operations_getCount(&operations) == 1);
    assert(Sesh_submit(&direct, &operation) == SESH_REPLAY);
    assert(Resource_getRevision(&resource) == 1 && Operations_getCount(&operations) == 1);
    PROJECTIONS(Sesh, session, "authService=principal 7,client=client 2,workspace=workspace 3,resource=resource 4 revision 1,operations=1 operations");
    serializedConcurrency(&auth, &workspace);
    if (INVALID_MODE) {
        Resource shared = Resource(4, 3, 0);
        OperationsSlot raceSlots[2];
        Operations raceLedger = Operations(raceSlots, 2);
        SeshClient secondClient = SeshClient(3);
        Sesh firstSession = Sesh(&auth, &client, &workspace, &shared, &raceLedger);
        Sesh secondSession = Sesh(&auth, &secondClient, &workspace, &shared, &raceLedger);
        Operation firstIntent = Operation(1, 2, 3, 4, 0);
        Operation secondIntent = Operation(1, 3, 3, 4, 0);
        assert(Sesh_submit(&firstSession, &firstIntent) == SESH_APPLIED);
        assert(Sesh_submit(&secondSession, &secondIntent) == SESH_CONFLICT);
        assert(Resource_getRevision(&shared) == 1 && Operations_getCount(&raceLedger) == 1);
        assert(Operation_setExpectedRevision(&secondIntent, 1));
        assert(Sesh_submit(&secondSession, &secondIntent) == SESH_APPLIED);
        assert(Resource_getRevision(&shared) == 2 && Operations_getCount(&raceLedger) == 2);
        assert(Sesh(nullptr, &client, &workspace, &resource, &operations).client == nullptr);
        assert(Sesh(&auth, nullptr, &workspace, &resource, &operations).client == nullptr);
        assert(Sesh(&auth, &client, nullptr, &resource, &operations).client == nullptr);
        assert(Sesh(&auth, &client, &workspace, nullptr, &operations).client == nullptr);
        assert(Sesh(&auth, &client, &workspace, &resource, nullptr).client == nullptr);
#define BAD_PART(Name, value) assert(!Sesh_set##Name(nullptr, value)); assert(!Sesh_set##Name(&session, nullptr)); assert(Sesh_get##Name(&session) == value)
        BAD_PART(AuthService, &auth); BAD_PART(Client, &client); BAD_PART(Workspace, &workspace);
        BAD_PART(Resource, &resource); BAD_PART(Operations, &operations);
#undef BAD_PART
        assert(Sesh_submit(nullptr, &operation) == SESH_REJECTED);
        assert(Sesh_submit(&session, nullptr) == SESH_REJECTED);
        Sesh unbound = Sesh();
        assert(Sesh_submit(&unbound, &operation) == SESH_REJECTED);
        AuthService_revoke(&auth);
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
        assert(AuthService_authenticate(&auth));
        assert(Workspace_setPrincipalId(&workspace, 8));
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
        assert(Workspace_setPrincipalId(&workspace, 7));
        Operation wrong = Operation(2, 9, 3, 4, 1);
        assert(Sesh_submit(&session, &wrong) == SESH_REJECTED);
        wrong = Operation(2, 2, 9, 4, 1);
        assert(Sesh_submit(&session, &wrong) == SESH_REJECTED);
        wrong = Operation(2, 2, 3, 9, 1);
        assert(Sesh_submit(&session, &wrong) == SESH_REJECTED);
        assert(Resource_setWorkspaceId(&resource, 9));
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
        assert(Resource_setWorkspaceId(&resource, 3));
        wrong = Operation(2, 2, 3, 4, 0);
        assert(Sesh_submit(&session, &wrong) == SESH_CONFLICT);
        wrong = Operation(1, 2, 3, 4, 1);
        assert(Sesh_submit(&session, &wrong) == SESH_REJECTED);
        assert(Resource_getRevision(&resource) == 1 && Operations_getCount(&operations) == 1);
        operation = Operation(2, 2, 3, 4, 1);
        assert(Sesh_submit(&session, &operation) == SESH_APPLIED);
        operation = Operation(3, 2, 3, 4, 2);
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
        assert(Resource_getRevision(&resource) == 2 && Operations_getCount(&operations) == 2);
        assert(Operations_reserve(&operations, grown, 4));
        assert(Sesh_submit(&session, &operation) == SESH_APPLIED);
        assert(Resource_setRevision(&resource, UINT64_MAX));
        operation = Operation(4, 2, 3, 4, UINT64_MAX);
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
        assert(Resource_getRevision(&resource) == UINT64_MAX && Operations_getCount(&operations) == 3);
        BAD_PROJECTIONS(Sesh, session);
        Sesh_close(&session);
        assert(Sesh_submit(&session, &operation) == SESH_REJECTED);
    }
    Sesh_close(&session); Sesh_close(&session); Sesh_close(nullptr);
    Sesh_close(&direct); Sesh_close(&empty);
    assert(Sesh_getResource(&session) == nullptr);
    assert(SeshClient_getId(&client) == 2 && AuthService_isAuthenticated(&auth));
    return 0;
}
