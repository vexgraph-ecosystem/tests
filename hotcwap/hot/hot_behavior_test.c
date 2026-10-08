// tests/hotcwap/hot/hot_behavior_test.c — the Hot_behavior module _test.
//
// Owns the reloadable behavior module's exported contract, exercised through a
// real dlopen of the built dylib (HOT_BEHAVIOR_MODULE):
//   - the pure math exports (pulse in [0,1], bar dimensions, texture path);
//   - the phase-bias setter/getter round trip;
//   - the versioned state schema (Hot_save v3 magic, Hot_restore v1/v2/v3,
//     foreign-magic rejection, unknown-length rejection, Hot_migrate forward);
//   - the trampoline export table.
//
// Deterministic. No loader, no manifest — this is the module itself.

#include <dlfcn.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef HOT_BEHAVIOR_MODULE
#error "HOT_BEHAVIOR_MODULE must be defined (path to the built hot_behavior dylib)"
#endif

#define HOT_BEHAVIOR_SCHEMA_MAGIC 0x56455842u // "VEXB"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

typedef struct HotModuleExport {
    const char *name;
    void *fn;
} HotModuleExport;

int main(void) {
    void *h = dlopen(HOT_BEHAVIOR_MODULE, RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        printf("hot_behavior_test: FAIL cannot dlopen %s: %s\n", HOT_BEHAVIOR_MODULE, dlerror());
        return 1;
    }

    float (*pulse)(double) = (float (*)(double)) dlsym(h, "hot_behavior_pulse");
    void (*bar)(float, float, float, float *, float *) =
        (void (*)(float, float, float, float *, float *)) dlsym(h, "hot_behavior_bar");
    const char *(*texturePath)(void) = (const char *(*)(void)) dlsym(h, "hot_texture_path");
    void (*setBias)(float) = (void (*)(float)) dlsym(h, "hot_behavior_set_phase_bias");
    float (*getBias)(void) = (float (*)(void)) dlsym(h, "hot_behavior_get_phase_bias");
    bool (*save)(void *, size_t, size_t *) = (bool (*)(void *, size_t, size_t *)) dlsym(h, "Hot_save");
    bool (*restore)(const void *, size_t) = (bool (*)(const void *, size_t)) dlsym(h, "Hot_restore");
    bool (*migrate)(const char *, const void *, size_t, void *, size_t, size_t *) =
        (bool (*)(const char *, const void *, size_t, void *, size_t, size_t *)) dlsym(h, "Hot_migrate");
    const void *(*tramps)(uint32_t *) = (const void *(*)(uint32_t *)) dlsym(h, "VkModuleGetTrampolines");

    CHECK(pulse && bar && texturePath && setBias && getBias && save && restore && migrate && tramps);
    if (!(pulse && bar && texturePath && setBias && getBias && save && restore && migrate && tramps)) {
        printf("hot_behavior_test: FAIL missing export(s)\n");
        dlclose(h);
        return 1;
    }

    // Pure math.
    setBias(0.0f);
    CHECK(fabsf(getBias() - 0.0f) < 1e-6f);
    setBias(0.25f);
    CHECK(fabsf(getBias() - 0.25f) < 1e-6f);
    for (int i = 0; i < 16; i++) {
        float p = pulse((double) i * 0.037);
        CHECK(p >= 0.0f && p <= 1.0f);
    }
    float barH = -1.0f;
    float barW = -1.0f;
    bar(100.0f, 50.0f, 0.5f, &barH, &barW);
    CHECK(fabsf(barH - 4.0f) < 1e-4f);   // h * 0.08
    CHECK(fabsf(barW - 50.0f) < 1e-4f);  // w * pulse
    CHECK(strcmp(texturePath(), "assets/sunflower.png") == 0);

    // State schema: save emits the v3 blob with the ownership magic.
    uint8_t blob[32];
    size_t len = 0;
    CHECK(save(blob, sizeof blob, &len));
    CHECK(len == 16);
    uint32_t magic = 0;
    memcpy(&magic, blob, 4);
    CHECK(magic == HOT_BEHAVIOR_SCHEMA_MAGIC);
    CHECK(save(blob, 8, &len) == false); // too small a buffer

    // Restore: v3 round-trips; foreign magic rejected; unknown length rejected.
    CHECK(restore(blob, 16));
    blob[0] ^= 0xFF; // corrupt the magic
    CHECK(restore(blob, 16) == false);
    CHECK(restore(blob, 7) == false);  // unknown length
    CHECK(restore(nullptr, 16) == false);

    // Legacy v1 (8 Bytes) and v2 (12 Bytes) are adopted.
    uint8_t v1[8] = { 0 };
    uint8_t v2[12] = { 0 };
    CHECK(restore(v1, 8));
    CHECK(restore(v2, 12));

    // Migration: v1/v2 forward into a v3 blob.
    uint8_t migrated[16];
    size_t outLen = 0;
    CHECK(migrate("1.0.0", v1, 8, migrated, sizeof migrated, &outLen));
    CHECK(outLen == 16);
    memcpy(&magic, migrated, 4);
    CHECK(magic == HOT_BEHAVIOR_SCHEMA_MAGIC);
    CHECK(migrate("1.1.0", v2, 12, migrated, sizeof migrated, &outLen));
    CHECK(outLen == 16);

    // Trampoline export table.
    uint32_t count = 0;
    const HotModuleExport *exports = (const HotModuleExport*) tramps(&count);
    CHECK(exports != nullptr);
    CHECK(count >= 10);
    int found = 0;
    for (uint32_t i = 0; i < count && exports && i < 64; i++) {
        const HotModuleExport *e = &exports[i];
        if ((*e).name && strcmp((*e).name, "VkModuleGetTrampolines") != 0 && (*e).fn)
            found++;
    }
    CHECK(found >= 9);

    dlclose(h);

    if (g_failures == 0) {
        printf("hot_behavior_test: all assertions held\n");
        return 0;
    }
    printf("hot_behavior_test: %d FAILURES\n", g_failures);
    return 1;
}
