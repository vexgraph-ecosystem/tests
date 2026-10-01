// tests/main.c — the one entry point.
//
//     tests/main                  list what you can run
//     tests/main <name> [args...]   run whatever <name> is associated with
//
// <name> is a friendly string tied to a file somewhere across the repos
// ("darling-gallery" -> _main/darling_gallery.c). The entry contract is
// `int main() { return run(argv); }`, so you can also overwrite this file
// with your own `<name>.c` (no extension) and keep the same shape.
//
// Anything not in the alias table is handed straight to the build system's
// own resolution, so every runnable target is reachable by a partial name.

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    const char *name;   // the string you type
    const char *file;   // the file it is associated with
} Entry;

static const Entry kRegistry[] = {
    // apps
    {"darling-gallery", "_main/darling_gallery.c"},
    {"hello-window", "_main/hello_window.c"},
    // graphvex
    {"gpu", "tests/graphvex/vulkan/gpu_render_test.c"},
    {"vk", "tests/graphvex/vulkan/vk_renderer_test.c"},
    {"capture", "tests/graphvex/graphics/capture_test.c"},
    {"panels", "tests/graphvex/panel_test.c"},
};
static const int kRegistryCount = (int)(sizeof kRegistry / sizeof kRegistry[0]);

static const char *g_root = NULL;

// walk up from cwd to the directory that holds tools/b
static const char *repo_root(void) {
    static char buf[PATH_MAX];
    if (g_root) return g_root;
    if (!getcwd(buf, sizeof buf)) return NULL;
    for (;;) {
        char probe[PATH_MAX + 32];
        snprintf(probe, sizeof probe, "%s/tools/b", buf);
        if (access(probe, X_OK) == 0) {
            g_root = buf;
            return g_root;
        }
        char *slash = strrchr(buf, '/');
        if (!slash || slash == buf) return NULL;
        *slash = '\0';
    }
}

static void list(void) {
    printf("tests/main — run anything by name\n\n");
    printf("  tests/main <name> [args...]\n\n");
    printf("  aliases (string -> file):\n");
    for (int i = 0; i < kRegistryCount; i++)
        printf("    %-18s %s\n", kRegistry[i].name, kRegistry[i].file);
    printf("\n  any other name is passed to `b run` (partial names ok), e.g.\n");
    printf("    tests/main image      # -> tests/graphvex/image_test\n");
    printf("    tests/main ledger     # -> tests/hotcwap/hot/ledger_test\n");
}

// the dispatch: <name> -> the file it is associated with -> run it.
// `argsv` is opaque on purpose: main hands it argv, but you may call run()
// with whatever you like — a struct of your own, a single string, NULL.
int run(int argc, void *argsv) {
    char **argv = argsv;
    if (argc < 2 || !argv) {
        list();
        return 0;
    }

    const char *name = argv[1];
    const char *file = name;
    for (int i = 0; i < kRegistryCount; i++) {
        if (!strcmp(kRegistry[i].name, name)) {
            file = kRegistry[i].file;
            break;
        }
    }

    const char *root = repo_root();
    char bpath[PATH_MAX + 32];
    if (root) snprintf(bpath, sizeof bpath, "%s/tools/b", root);
    else snprintf(bpath, sizeof bpath, "tools/b");

    // hand off to b: it compiles on demand and runs, so this stays a thin entry
    char **args = calloc((size_t)argc + 3, sizeof *args);
    if (!args) {
        fprintf(stderr, "tests/main: out of memory\n");
        return 1;
    }
    int n = 0;
    args[n++] = bpath;
    args[n++] = (char *)"run";
    args[n++] = (char *)file;
    for (int i = 2; i < argc; i++) args[n++] = argv[i];
    args[n] = NULL;

    execv(bpath, args);
    fprintf(stderr, "tests/main: cannot exec %s: %s\n", bpath, strerror(errno));
    return 1;
}

int main(int argc, char **argv) {
    // ── edit me ─────────────────────────────────────────────────────────────
    // This is the whole entry. `args` is yours: point it at argv, at a struct
    // of your own, at one string, or leave it NULL to just list.
    void *args = argv;
    return run(argc, args);
}
