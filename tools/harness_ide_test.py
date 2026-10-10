"""Real workspace/standalone Harness code-model proof, not GUI or agent execution.

Configure CMake, inspect every draft translation unit and compile each public
header with its actual include context. Verify recursive discovery and default
exclusion in a disposable standalone copy. No gallery, provider or app runs.
"""
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
        cls.workspace = cls.home / "workspace"
        cls.standalone = cls.home / "standalone"
        run(["cmake", "-S", str(ROOT), "-B", str(cls.workspace),
             "-DCMAKE_BUILD_TYPE=Debug", "-DBUILD_TESTING=OFF"])
        run(["cmake", "-S", str(OWNER), "-B", str(cls.standalone),
             f"-DVEXSPOKE_SOURCE_DIR={VEXSPOKE}"])

    def test_all_translation_units_and_headers_in_both_contexts(self):
        sources = set((OWNER / "src").rglob("*.c"))
        self.assertEqual(len(sources), 4)
        for build in (self.workspace, self.standalone):
            db = json.loads((build / "compile_commands.json").read_text())
            owned = [entry for entry in db if Path(entry["file"]) in sources]
            self.assertEqual({Path(entry["file"]) for entry in owned}, sources)
            for entry in owned:
                with self.subTest(build=build.name, source=entry["file"]):
                    command = entry["command"]
                    self.assertIn("harness_ide.dir", command)
                    for flag in ("-Wall", "-Wextra", "-Werror"):
                        self.assertIn(flag, command)
                    self.assertRegex(command, r"-std=gnu(23|2x)")
                    self.assertIn(str(OWNER / "src"), command)
                    self.assertIn(str(VEXSPOKE), command)
                    run(syntax_args(entry), cwd=entry["directory"])
            entry = owned[0]
            args = syntax_args(entry)
            args.remove(entry["file"])
            args += ["-x", "c", "-"]
            for header in (OWNER / "src").rglob("*.h"):
                run(args, input=f'#include "{header.relative_to(OWNER / "src")}"\n',
                    cwd=entry["directory"])

    def test_standalone_default_noop_and_explicit_object_build(self):
        run(["cmake", "--build", str(self.standalone)])
        self.assertEqual(list(self.standalone.rglob("*.o")), [])
        run(["cmake", "--build", str(self.standalone), "--target", "harness_ide"])
        self.assertEqual(len(list(self.standalone.rglob("*.o"))), 4)

    def test_new_nested_files_need_no_new_cmake(self):
        owner = self.home / "copied owner"
        shutil.copytree(OWNER / "src", owner / "src")
        shutil.copyfile(OWNER / "CMakeLists.txt", owner / "CMakeLists.txt")
        build = self.home / "discovery"
        run(["cmake", "-S", str(owner), "-B", str(build),
             f"-DVEXSPOKE_SOURCE_DIR={VEXSPOKE}"])
        nested = owner / "src/new_directory/new_class.c"
        nested.parent.mkdir()
        nested.write_text("int new_class(void) { return 0; }\n")
        run(["cmake", "--build", str(build)])
        db = json.loads((build / "compile_commands.json").read_text())
        entry = next(item for item in db if Path(item["file"]) == nested)
        run(syntax_args(entry), cwd=entry["directory"])
        self.assertEqual(list(build.rglob("*.o")), [])
        self.assertEqual(list(owner.rglob("CMakeLists.txt")), [owner / "CMakeLists.txt"])

    def test_missing_vexspoke_is_a_real_error(self):
        build = self.home / "missing"
        run(["cmake", "-S", str(OWNER), "-B", str(build)])
        db = json.loads((build / "compile_commands.json").read_text())
        result = subprocess.run(syntax_args(db[0]), text=True, capture_output=True,
                                cwd=db[0]["directory"], timeout=30)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("exception/throw.h", result.stderr)


if __name__ == "__main__":
    if shutil.which("cmake") is None:
        print("SKIP: CMake required")
        raise SystemExit(77)
    unittest.main(verbosity=2)
