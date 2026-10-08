/* Trusted-verifier binding, revocation and secret-free projections. Offline only. */
#include "support.h"
static bool verifyZero(const ApiAuth *auth, void *context, uint64_t *dest) {
    (void) auth; (void) context;
    *dest = 0;
    return true;
}
int main(int argc, char **argv) {
    ApiAuth credential = { .kind = API_AUTH_API_KEY, .credential = "DO-NOT-PRINT-SECRET" };
    uint64_t principal = 7;
    AuthService auth = AuthService(&credential, verify, &principal);
    assert(AuthService_0().principalId == 0 && AuthService().principalId == 0 && AuthService_zero().principalId == 0);
    AuthService direct = AuthService_3(&credential, verify, &principal);
    assert(AuthService_getAuth(&direct) == &credential);
    assert(AuthService_getVerifier(&auth) == verify && AuthService_getContext(&auth) == &principal);
    assert(AuthService_getAuth(nullptr) == nullptr && AuthService_getVerifier(nullptr) == nullptr &&
           AuthService_getContext(nullptr) == nullptr && AuthService_getPrincipalId(nullptr) == 0 && !AuthService_isAuthenticated(nullptr));
    assert(!AuthService_isAuthenticated(&auth));
    assert(AuthService_authenticate(&auth) && AuthService_getPrincipalId(&auth) == 7);
    char text[512];
    assert(AuthService_toStringStruct(&auth, text, sizeof(text), nullptr));
    assert(strstr(text, credential.credential) == nullptr && strstr(text, "auth=redacted,verifier=bound,context=redacted,principalId=7"));
    PROJECTIONS(AuthService, auth, "principalId=7");
    assert(AuthService_setAuth(&auth, &credential) && !AuthService_isAuthenticated(&auth));
    assert(AuthService_authenticate(&auth));
    assert(AuthService_setContext(&auth, nullptr) && !AuthService_isAuthenticated(&auth));
    assert(AuthService_getContext(&auth) == nullptr);
    assert(AuthService_setContext(&auth, &principal) && AuthService_authenticate(&auth));
    assert(AuthService_setVerifier(&auth, verify, &principal) && !AuthService_isAuthenticated(&auth));
    assert(AuthService_authenticate(&auth));
    AuthService_revoke(&auth); AuthService_revoke(&auth); AuthService_revoke(nullptr);
    assert(!AuthService_isAuthenticated(&auth));
    if (INVALID_MODE) {
        assert(AuthService_authenticate(&auth));
        assert(AuthService(nullptr, verify, &principal).auth == nullptr);
        assert(AuthService(&credential, nullptr, &principal).auth == nullptr);
        assert(!AuthService_setAuth(nullptr, &credential));
        assert(!AuthService_setAuth(&auth, nullptr) && AuthService_getAuth(&auth) == &credential);
        assert(!AuthService_setVerifier(nullptr, verify, &principal));
        assert(!AuthService_setVerifier(&auth, nullptr, &principal) && AuthService_getVerifier(&auth) == verify);
        assert(!AuthService_setContext(nullptr, &principal));
        assert(AuthService_getPrincipalId(&auth) == 7);
        assert(!AuthService_authenticate(nullptr));
        assert(AuthService_authenticate(&direct));
        principal = 0;
        assert(!AuthService_authenticate(&direct) && !AuthService_isAuthenticated(&direct));
        assert(AuthService_setVerifier(&direct, verifyZero, nullptr));
        assert(!AuthService_authenticate(&direct) && !AuthService_isAuthenticated(&direct));
        AuthService unbound = AuthService();
        assert(!AuthService_authenticate(&unbound));
        BAD_PROJECTIONS(AuthService, auth);
    }
    return 0;
}
