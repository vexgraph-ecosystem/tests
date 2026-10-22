/* Owner test for space/model_user.c. Exhausts the persona value: identity,
 * boundary ids, the 23+1 handle extent, every constructor form, setter
 * reject-and-preserve, null-safe getters, and bounded projections. Rejections
 * emit the [vex] diagnostic and return the empty value / false (the Contract
 * Before Cases Law: invalid input never mutates live state).
 */
#include "test_support.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "space/model_user.h"

static int g_failures = 0;

#define CHECK(name, cond) do { \
    if (cond) { printf("[model_user] PASS %s\n", name); } \
    else { printf("[model_user] FAIL %s\n", name); g_failures++; } \
} while (0)

static bool sameUser(const ModelUser *a, const ModelUser *b) {
    return (*a).id == (*b).id && (*a).modelId == (*b).modelId &&
           memcmp((*a).name, (*b).name, MODEL_USER_NAME_CAP) == 0;
}

static bool isZeroUser(ModelUser v) {
    return v.id == 0 && v.modelId == 0 && v.name[0] == '\0';
}

int main(void) {
    printf("=== ModelUser owner suite ===\n");
    const char *n23 = "aaaaaaaaaaaaaaaaaaaaaaa";   /* 23 chars: the extent  */

    // --- Constructors and the empty value.
    ModelUser zero = ModelUser_0();
    ModelUser chooserZero = ModelUser();
    CHECK("zero id/name/model", zero.id == 0 && zero.name[0] == '\0' && zero.modelId == 0);
    CHECK("chooser() == zero", sameUser(&zero, &chooserZero));
    ModelUser zeroAlias = ModelUser_zero();
    CHECK("zero() == _0", sameUser(&zeroAlias, &zero));

    ModelUser u = ModelUser(7, "vex", 3);
    CHECK("chooser 3-arg id", u.id == 7);
    CHECK("chooser 3-arg name", strcmp(u.name, "vex") == 0);
    CHECK("chooser 3-arg modelId", u.modelId == 3);
    CHECK("name is zero-padded", u.name[4] == '\0' && u.name[23] == '\0');

    ModelUser big = ModelUser(UINT64_MAX, n23, UINT64_MAX);
    CHECK("boundary max id/model accepted", big.id == UINT64_MAX && big.modelId == UINT64_MAX);

    // --- Invalid construction returns the empty value, never a partial persona.
    CHECK("zero id rejected", isZeroUser(ModelUser_3(0, "vex", 3)));
    CHECK("zero modelId rejected", isZeroUser(ModelUser_3(1, "vex", 0)));
    CHECK("null name rejected", isZeroUser(ModelUser_3(1, nullptr, 3)));
    CHECK("empty name rejected", isZeroUser(ModelUser_3(1, "", 3)));
    CHECK("uppercase name rejected", isZeroUser(ModelUser_3(1, "Vex", 3)));
    CHECK("overlong name rejected", isZeroUser(ModelUser_3(1, "aaaaaaaaaaaaaaaaaaaaaaaa", 3)));

    // --- setName: accept, re-pad on shrink, reject-and-preserve.
    ModelUser s = ModelUser(1, "longername", 2);
    CHECK("setName accepts", ModelUser_setName(&s, "short") && strcmp(s.name, "short") == 0);
    CHECK("setName re-pads leftovers", s.name[5] == '\0' && s.name[10] == '\0');
    ModelUser before = s;
    CHECK("setName rejects null", !ModelUser_setName(&s, nullptr));
    CHECK("setName rejects uppercase", !ModelUser_setName(&s, "Bad"));
    CHECK("setName rejects overlong", !ModelUser_setName(&s, "aaaaaaaaaaaaaaaaaaaaaaaa"));
    CHECK("setName preserves on reject", sameUser(&s, &before));
    CHECK("setName rejects null self", !ModelUser_setName(nullptr, "ok"));

    // --- setModelId: zero is never a binding; valid updates; reject preserves.
    CHECK("setModelId rejects zero", !ModelUser_setModelId(&s, 0) && s.modelId == before.modelId);
    CHECK("setModelId rejects null self", !ModelUser_setModelId(nullptr, 9));
    CHECK("setModelId accepts", ModelUser_setModelId(&s, 99) && s.modelId == 99);

    // --- Getters: null-safe defaults, live values otherwise.
    CHECK("getters null self", ModelUser_getId(nullptr) == 0 && ModelUser_getModelId(nullptr) == 0 &&
                              ModelUser_getName(nullptr) == nullptr);
    CHECK("getName on empty value is not null", ModelUser_getName(&zero) != nullptr && ModelUser_getName(&zero)[0] == '\0');
    CHECK("getters live", ModelUser_getId(&s) == 1 && ModelUser_getModelId(&s) == 99 &&
                          strcmp(ModelUser_getName(&s), "short") == 0);

    // --- Projections: value, structure, null self, and truncation.
    char buf[128];
    bool cut = false;
    CHECK("toString null self", ModelUser_toString(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toString value", ModelUser_toString(&s, buf, sizeof(buf), &cut) && strcmp(buf, "@short") == 0 && !cut);
    CHECK("toStringStruct null self", ModelUser_toStringStruct(nullptr, buf, sizeof(buf), &cut) && strcmp(buf, "nullptr") == 0);
    CHECK("toStringStruct fields",
          ModelUser_toStringStruct(&s, buf, sizeof(buf), &cut) &&
          strcmp(buf, "ModelUser{id=1,name=\"short\",modelId=99}") == 0 && !cut);
    CHECK("toString truncates", !ModelUser_toString(&s, buf, 3, &cut) && cut);
    CHECK("toStringStruct truncates", !ModelUser_toStringStruct(&s, buf, 8, &cut) && cut);
    CHECK("toStringStruct null dest rejects", !ModelUser_toStringStruct(&s, nullptr, 8, &cut) && cut);
    CHECK("toStringStruct null flag tolerated", ModelUser_toStringStruct(&s, buf, sizeof(buf), nullptr));

    if (g_failures != 0) {
        printf("=== %d MODELUSER FAILURES ===\n", g_failures);
        return 1;
    }
    printf("=== ALL MODELUSER PASS ===\n");
    return 0;
}
