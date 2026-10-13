#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/settings.h"

// Owner: persisted settings — defaults, setters/getters, manifest-root path
// resolution, atomic save + reload round trip, missing file, malformed/unknown
// rejection that applies nothing, and bounded-setter overflow. Uses a disposable
// VEX_MANIFEST base; never touches the real Application Support tree.

static int failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                    \
        }                                                                  \
    } while (0)

static void writeFile(const char *path, const char *text) {
    FILE *file = fopen(path, "wb");
    if (file == nullptr) {
        failures++;
        return;
    }
    fwrite(text, 1, strlen(text), file);
    fclose(file);
}

int main(void) {
    char base[] = "/tmp/primeagen-settings-XXXXXX";
    if (mkdtemp(base) == nullptr) {
        fprintf(stderr, "FAIL: mkdtemp\n");
        return 1;
    }
    setenv("VEX_MANIFEST", base, 1);  // must precede the first resolvePath

    Settings settings = Settings_0();
    CHECK(strcmp(Settings_getProvider(&settings), "") == 0);
    CHECK(strcmp(Settings_getModel(&settings), "") == 0);
    CHECK(!Settings_isOwo(&settings));
    CHECK(Settings_getConsent(&settings) == SETTINGS_CONSENT_PROMPT);
    CHECK(strcmp(Settings_getAppearance(&settings), SETTINGS_APPEARANCE_DEFAULT) == 0);

    CHECK(Settings_setProvider(&settings, "openai"));
    CHECK(Settings_setModel(&settings, "gpt-x"));
    Settings_setOwo(&settings, true);
    CHECK(Settings_setConsent(&settings, SETTINGS_CONSENT_DENY));
    CHECK(!Settings_setConsent(&settings, 99));  // out of range
    CHECK(Settings_setAppearance(&settings, "unicode-emdash"));

    char path[600];
    CHECK(Settings_resolvePath(path, sizeof path));
    CHECK(strstr(path, base) == path);  // lives under the disposable base

    CHECK(Settings_save(&settings, path));

    Settings loaded = Settings_0();
    CHECK(Settings_load(&loaded, path));
    CHECK(strcmp(Settings_getProvider(&loaded), "openai") == 0);
    CHECK(strcmp(Settings_getModel(&loaded), "gpt-x") == 0);
    CHECK(Settings_isOwo(&loaded));
    CHECK(Settings_getConsent(&loaded) == SETTINGS_CONSENT_DENY);
    CHECK(strcmp(Settings_getAppearance(&loaded), "unicode-emdash") == 0);

    // Missing file: defaults kept, still true.
    Settings absent = Settings_0();
    Settings_setProvider(&absent, "keep");
    CHECK(Settings_load(&absent, "/tmp/primeagen-settings-absent/file"));
    CHECK(strcmp(Settings_getProvider(&absent), "keep") == 0);

    // Malformed / unknown key: false, nothing applied.
    Settings reject = Settings_0();
    Settings_setProvider(&reject, "keep");
    writeFile(path, "provider=openai\nbogus=1\n");
    CHECK(!Settings_load(&reject, path));
    CHECK(strcmp(Settings_getProvider(&reject), "keep") == 0);
    writeFile(path, "no-assignment\n");
    CHECK(!Settings_load(&reject, path));

    // Bounded setter overflow keeps the previous value.
    char big[200];
    memset(big, 'a', sizeof big - 1);
    big[sizeof big - 1] = '\0';
    Settings bounded = Settings_0();
    CHECK(!Settings_setModel(&bounded, big));
    CHECK(strcmp(Settings_getModel(&bounded), "") == 0);

    unlink(path);
    if (failures == 0)
        printf("settings owner: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
