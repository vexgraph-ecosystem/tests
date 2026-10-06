"""C23 migration gate and native null-pointer smoke; not whole-class readiness.

Comments/strings, vendor and attic are outside this lexical gate. Test-tree
migration is a separately reported inventory until its remaining cases land.
"""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
TOKENS = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\bNULL\b', re.S)
EXCLUDED = {".git", "attic", "vendor", "third_party", "third-party", "build", "target",
            ".idea", "node_modules", "_old", "legacy"}


def null_tokens(text):
    return [m.start() for m in TOKENS.finditer(text) if m.group() == "NULL"]


class C23MigrationTest(unittest.TestCase):
    def test_production_uses_native_nullptr(self):
        count = 0
        for name in ("ecosystem/repos", "ecosystem/projects", "personal/b",
                     "personal/relational-engine", "tools"):
            for path in (ROOT / name).rglob("*"):
                if path.suffix not in (".c", ".h", ".m") or EXCLUDED.intersection(path.parts):
                    continue
                self.assertEqual(null_tokens(path.read_text()), [], str(path.relative_to(ROOT)))
                count += 1
        self.assertGreater(count, 300)

    def test_lexer_preserves_strings_comments_and_null_handle_names(self):
        self.assertEqual(null_tokens('// NULL\n/* NULL */ "NULL" VK_NULL_HANDLE; nullptr;'), [])
        self.assertEqual(len(null_tokens('void *p = NULL; if (p == NULL) return NULL;')), 3)

    def test_c23_keywords_object_and_function_nulls(self):
        with tempfile.TemporaryDirectory(prefix="c23-nullptr-") as scratch:
            source = Path(scratch) / "probe.c"
            binary = Path(scratch) / "probe"
            source.write_text('#include <assert.h>\n#include <stddef.h>\n'
                              'static int call(void) { return 7; }\n'
                              'int main(void) {\n'
                              'bool enabled = true; void *p = nullptr;\n'
                              'int (*callback)(void) = nullptr;\n'
                              'assert(enabled && !false && p == nullptr && callback == nullptr);\n'
                              'callback = call; assert(callback() == 7);\n'
                              'nullptr_t empty = nullptr; assert(empty == p); return 0; }\n')
            subprocess.run(["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                            str(source), "-o", str(binary)], check=True, capture_output=True, timeout=60)
            subprocess.run([str(binary)], check=True, timeout=10)

    def test_root_ide_requires_c23_without_fallback_macros(self):
        text = (ROOT / "CMakeLists.txt").read_text()
        self.assertIn("set(CMAKE_C_STANDARD 23)", text)
        self.assertIn("set(CMAKE_C_STANDARD_REQUIRED YES)", text)
        self.assertIn("set(CMAKE_OBJC_STANDARD 23)", text)

    def test_bundled_clangd_parses_bvh_with_project_configuration(self):
        clangd = Path("/Applications/CLion.app/Contents/bin/clang/mac/aarch64/bin/clangd")
        if not clangd.exists():
            self.skipTest("CLion bundled clangd unavailable on this host")
        for source in ("ecosystem/repos/vexspoke/src/algo/bvh.c",
                       "tests/vexspoke/algo/algo_suite_test.c",
                       "tests/darling/compositor/filter_gallery.c",
                       "tests/darling/darling_tests.c"):
            with self.subTest(source=source):
                # Scope this proof to parsing/diagnostics, not clangd refactoring
                # tools. SwapBinaryOperands' self-test fails on valid COLOR_RGBA
                # macro expansions (overlapping edits), unrelated to the model.
                result = subprocess.run([str(clangd), "--enable-config",
                                         "--tweaks=ExpandAutoType",
                                         "--check=" + str(ROOT / source)],
                                        cwd=ROOT, text=True, capture_output=True, timeout=120)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn("0 errors", result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
