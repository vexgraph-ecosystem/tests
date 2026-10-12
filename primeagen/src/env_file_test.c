#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "src/env_file.h"

// Owner: literal .env admission parity with tests/primeagen/src/env_file_test.py —
// missing/empty, quotes/CRLF/comments, environment precedence, atomic rejection
// without echo, size/type boundaries, literal (no shell). Uses disposable files
// only; never reads the real app .env.

static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                \
        }                                                              \
    } while (0)

static void writeFile(const char *path, const void *data, size_t length) {
    FILE *file = fopen(path, "wb");
    if (file == nullptr) {
        failures++;
        return;
    }
    if (length)
        fwrite(data, 1, length, file);
    fclose(file);
}

static bool loadAbsent(const char *path) {
    unlink(path);
    return EnvFile_load(path);
}

int main(void) {
    char directory[] = "/tmp/primeagen-env-XXXXXX";
    if (mkdtemp(directory) == nullptr) {
        fprintf(stderr, "FAIL: mkdtemp\n");
        return 1;
    }
    char path[512];
    snprintf(path, sizeof path, "%s/.env", directory);

    unsetenv("OPENAI_API_KEY");
    unsetenv("ANTHROPIC_API_KEY");

    // Missing file is a no-op.
    CHECK(loadAbsent(path));

    // Empty file is accepted.
    writeFile(path, "", 0);
    CHECK(EnvFile_load(path));

    // Comments / blank / CRLF / quotes, with existing environment precedence.
    setenv("OPENAI_API_KEY", "from-terminal", 1);
    const char *body = "# test\r\n\r\n OPENAI_API_KEY='fake-openai'\r\nANTHROPIC_API_KEY=\"fake-anthropic\"\r\n";
    writeFile(path, body, strlen(body));
    CHECK(EnvFile_load(path));
    CHECK(strcmp(getenv("OPENAI_API_KEY"), "from-terminal") == 0);
    CHECK(strcmp(getenv("ANTHROPIC_API_KEY"), "fake-anthropic") == 0);

    // Empty value accepted; a real value does not overwrite an empty present entry.
    unsetenv("OPENAI_API_KEY");
    unsetenv("ANTHROPIC_API_KEY");
    static const char emptyBody[] = "OPENAI_API_KEY=\nANTHROPIC_API_KEY=\n";
    writeFile(path, emptyBody, sizeof emptyBody - 1);
    CHECK(EnvFile_load(path));
    CHECK(getenv("OPENAI_API_KEY") != nullptr && getenv("OPENAI_API_KEY")[0] == '\0');

    // Invalid files reject atomically and leave the environment untouched.
    const char *bad[] = {
        "OPENAI_API_KEY=fake-secret\nBOGUS=fake-secret",
        "export OPENAI_API_KEY=fake-secret",
        "OPENAI_API_KEY=fake-secret\nOPENAI_API_KEY=duplicate",
        "OPENAI_API_KEY='fake-secret",
        "OPENAI_API_KEY=fake secret",
        "OPENAI_API_KEY=\x1b",
        "no-assignment",
        "ANTHROPIC_API_KEY=fake-secret\nOPENAI_API_KEY=\"multi\nline\"",
    };
    for (size_t i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
        unsetenv("OPENAI_API_KEY");
        unsetenv("ANTHROPIC_API_KEY");
        setenv("UNCHANGED", "sentinel", 1);
        writeFile(path, bad[i], strlen(bad[i]));
        CHECK(!EnvFile_load(path));
        CHECK(strcmp(getenv("UNCHANGED"), "sentinel") == 0);
        CHECK(getenv("OPENAI_API_KEY") == nullptr);
        unsetenv("UNCHANGED");
    }

    // Embedded NUL in a value rejects (parsed length-aware, not via strlen).
    unsetenv("OPENAI_API_KEY");
    static const char nulBody[] = "OPENAI_API_KEY=fake-secret\0";
    writeFile(path, nulBody, sizeof nulBody - 1);
    CHECK(!EnvFile_load(path));

    // Recovery after rejection.
    static const char recoveredBody[] = "OPENAI_API_KEY=recovered\n";
    writeFile(path, recoveredBody, sizeof recoveredBody - 1);
    CHECK(EnvFile_load(path));
    CHECK(getenv("OPENAI_API_KEY") != nullptr && strcmp(getenv("OPENAI_API_KEY"), "recovered") == 0);

    // Literal, not shell.
    unsetenv("OPENAI_API_KEY");
    unsetenv("ANTHROPIC_API_KEY");
    static const char literalBody[] = "OPENAI_API_KEY=$(id)\nANTHROPIC_API_KEY=`id`\n";
    writeFile(path, literalBody, sizeof literalBody - 1);
    CHECK(EnvFile_load(path));
    CHECK(getenv("OPENAI_API_KEY") != nullptr && strcmp(getenv("OPENAI_API_KEY"), "$(id)") == 0);

    // Size boundary: exactly at the cap is accepted, one over is rejected.
    static char big[16384 + 1];
    memset(big, 'x', sizeof big);
    big[0] = '#';
    writeFile(path, big, 16384);
    CHECK(EnvFile_load(path));
    writeFile(path, big, 16385);
    CHECK(!EnvFile_load(path));

    // Type boundaries: symlink, directory and FIFO all reject.
    unlink(path);
    char target[512];
    snprintf(target, sizeof target, "%s/target", directory);
    writeFile(target, "OPENAI_API_KEY=x\n", 17);
    char link[512];
    snprintf(link, sizeof link, "%s/link", directory);
    symlink(target, link);
    CHECK(!EnvFile_load(link));
    CHECK(!EnvFile_load(directory));
    char fifo[512];
    snprintf(fifo, sizeof fifo, "%s/fifo", directory);
    if (mkfifo(fifo, 0600) == 0)
        CHECK(!EnvFile_load(fifo));

    unsetenv("OPENAI_API_KEY");
    unsetenv("ANTHROPIC_API_KEY");
    unlink(link);
    unlink(target);
    unlink(fifo);
    rmdir(directory);

    if (failures == 0)
        printf("env_file owner: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
