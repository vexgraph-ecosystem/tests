"""Harness workspace compiler proof; no standalone CMake, agent or GUI run."""
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
OWNER = ROOT / "ecosystem/repos/harness"
VEXSPOKE = ROOT / "ecosystem/repos/vexspoke/src"


def run(args, **kwargs):
    result = subprocess.run(args, text=True, capture_output=True, timeout=180, **kwargs)
    if result.returncode:
        raise AssertionError(f"{args}\n{result.stdout}\n{result.stderr}")
    return result


def syntax_args(entry):
    args = shlex.split(entry["command"])
    index = args.index("-o")
    del args[index:index + 2]
    args.remove("-c")
    return args + ["-fsyntax-only"]


class HarnessIdeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix="harness-ide-", dir=os.environ.get("TMPDIR"))
        cls.addClassCleanup(cls.temp.cleanup)
        cls.home = Path(cls.temp.name)
        cls.build = cls.home / "workspace"
        run(["cmake", "-S", str(ROOT), "-B", str(cls.build), "-DBUILD_TESTING=OFF"])
        cls.db = json.loads((cls.build / "compile_commands.json").read_text())

    def test_every_translation_unit_and_header_compiles(self):
        sources = set((OWNER / "src").rglob("*.c"))
        owned = [entry for entry in self.db if Path(entry["file"]) in sources]
        self.assertEqual(len(sources), 4)
        self.assertEqual({Path(entry["file"]) for entry in owned}, sources)
        for entry in owned:
            self.assertIn("vexgraph_extra_harness.dir", entry["command"])
            self.assertIn(str(VEXSPOKE), entry["command"])
            run(syntax_args(entry), cwd=entry["directory"])
        entry = owned[0]
        args = syntax_args(entry)
        args.remove(entry["file"])
        for header in (OWNER / "src").rglob("*.h"):
            run(args + ["-x", "c", "-"], cwd=entry["directory"],
                input=f'#include "{header.relative_to(OWNER / "src")}"\n')

    def test_excluded_target_has_no_runtime_dependency(self):
        run(["cmake", "--build", str(self.build), "--target", "vexgraph_extra_harness"])
        objects = list(self.build.rglob("*.o"))
        self.assertEqual(len(objects), 4)
        self.assertTrue(all("vexgraph_extra_harness.dir" in str(path) for path in objects))

    def test_recursive_new_source_and_header_discovery(self):
        source = self.home / "copied source"
        shutil.copytree(OWNER / "src", source)
        build = self.home / "discovery"
        configure = ["cmake", "-S", str(ROOT), "-B", str(build),
                     f"-DVEXGRAPH_HARNESS_SOURCE_DIR={source}", "-DBUILD_TESTING=OFF"]
        run(configure)
        nested = source / "new_directory/new_class.c"
        nested.parent.mkdir()
        (nested.parent / "new_class.h").write_text("int new_class(void);\n")
        nested.write_text('#include "new_directory/new_class.h"\nint new_class(void) { return 0; }\n')
        run(["cmake", "--build", str(build), "--target", "vexgraph_extra_harness"])
        db = json.loads((build / "compile_commands.json").read_text())
        entry = next(item for item in db if Path(item["file"]) == nested)
        run(syntax_args(entry), cwd=entry["directory"])
        self.assertEqual(list(source.rglob("CMakeLists.txt")), [])

    def test_missing_dependency_is_a_real_compiler_error(self):
        entry = next(item for item in self.db if item["file"].endswith("/space/model_user.c"))
        args = [arg for arg in syntax_args(entry) if arg != f"-I{VEXSPOKE}"]
        result = subprocess.run(args, text=True, capture_output=True,
                                cwd=entry["directory"], timeout=30)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("exception/throw.h", result.stderr)

    def test_clion_bundled_parser_accepts_actual_harness_files(self):
        clangd = Path("/Applications/CLion.app/Contents/bin/clang/mac/aarch64/bin/clangd")
        if not clangd.exists():
            self.skipTest("CLion bundled clangd unavailable; compiler proof remains separate")
        for source in (OWNER / "src").rglob("*.c"):
            result = run([str(clangd), "--enable-config=false", "--tweaks=ExpandAutoType",
                          f"--compile-commands-dir={self.build}", f"--check={source}"])
            self.assertIn("0 errors", result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
