"""Per-repo IDE metadata proof, not runtime readiness or visual acceptance.

Configure every owning entry independently, prove excluded/default-no-op targets,
local dependency options, C23 compile commands and representative syntax. Empty
blueprints must not invent targets. No Cargo, dependency fetch or app execution.
"""
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
B_LINK = "https://github.com/vex-graph/b"
CODE = {
    "ecosystem/repos/vexspoke": "src/algo/bvh.c",
    "ecosystem/repos/api-haven": "src/ai/ai_provider.c",
    "ecosystem/repos/graphvex": "src/graphics/viewport.c",
    "ecosystem/repos/hotcwap": "kernel/process.c",
    "ecosystem/repos/darling-framework": "src/panel/panel.c",
    "ecosystem/projects/impedance": "src/impedance.c",
    "personal/b": "b.c",
    "ecosystem/repos/relational-engine": "src/io/file.c",
}
EMPTY = (
    "ecosystem/repos/darkbase", "ecosystem/repos/language",
    "ecosystem/repos/samplerate", "ecosystem/repos/sesh",
    "ecosystem/projects/anti", "ecosystem/projects/drawling",
    "ecosystem/projects/semicolon",
)
DEPENDENCIES = {
    "VEXSPOKE_SOURCE_DIR": ROOT / "ecosystem/repos/vexspoke/src",
    "RELATIONAL_ENGINE_SOURCE_DIR": ROOT / "ecosystem/repos/relational-engine/src",
    "GRAPHVEX_SOURCE_DIR": ROOT / "ecosystem/repos/graphvex/src",
    "HOTCWAP_SOURCE_DIR": ROOT / "ecosystem/repos/hotcwap",
}


def run(args, **kwargs):
    result = subprocess.run(args, text=True, capture_output=True, timeout=120, **kwargs)
    if result.returncode:
        raise AssertionError(f"{args}\n{result.stdout}\n{result.stderr}")
    return result


class PerRepoIdeTest(unittest.TestCase):
    def test_workspace_test_seams_and_constitution_remain_distinct(self):
        constitution = (ROOT / "preferences.md").read_text()
        self.assertIn("IDE-only CMake files are metadata adapters", constitution)
        self.assertIn("not these indexing adapters", constitution)
        self.assertIn("native test integration", constitution)
        for name in ("README.md", "tests/README.md"):
            self.assertIn(B_LINK, (ROOT / name).read_text())
        self.assertIn("native-test integration", (ROOT / "README.md").read_text())
        self.assertIn("opt-in testing seam is retained", (ROOT / "tests/README.md").read_text())

    def test_owned_entries_and_build_links(self):
        for name in (*CODE, *EMPTY):
            with self.subTest(repo=name):
                repo = ROOT / name
                readme = (repo / "README.md").read_text()
                section = " ".join(readme.split("## CLion: CMake is IDE metadata only", 1)[1].split())
                self.assertIn(B_LINK, readme)
                self.assertIn("inlay hints", section)
                self.assertIn("user-verified", section)
                cmake = (repo / "CMakeLists.txt").read_text()
                ignored = subprocess.run(["git", "-C", str(repo), "check-ignore", "--no-index",
                                          "cmake-build-debug/CMakeCache.txt"],
                                         text=True, capture_output=True, timeout=30)
                self.assertEqual(ignored.returncode, 0, "IDE outputs must stay ignored")
                self.assertNotRegex(cmake, r"(?i)\b(FetchContent|ExternalProject|add_executable|target_link_libraries|add_custom_target|add_custom_command)\s*\(")
                self.assertNotRegex(cmake, r"(?i)COMMAND\s+[\"']?cargo\b")
                if name in CODE:
                    self.assertIn("EXCLUDE_FROM_ALL", cmake)
                else:
                    self.assertIn("LANGUAGES NONE", cmake)
                    self.assertNotIn("add_library", cmake)
                    self.assertIn("no production sources", section)

    def test_configure_default_noop_and_actual_c23_syntax(self):
        with tempfile.TemporaryDirectory(prefix="per-repo-ide-", dir=os.environ.get("TMPDIR")) as tmp:
            for number, name in enumerate((*CODE, *EMPTY)):
                with self.subTest(repo=name):
                    repo = ROOT / name
                    build = Path(tmp) / str(number)
                    cmake = (repo / "CMakeLists.txt").read_text()
                    options = [f"-D{key}={value}" for key, value in DEPENDENCIES.items()
                               if key in cmake]
                    run(["cmake", "-S", str(repo), "-B", str(build), *options])
                    before = set(build.rglob("*.o"))
                    output = run(["cmake", "--build", str(build)]).stdout
                    self.assertEqual(before, set(build.rglob("*.o")))
                    self.assertNotIn("Linking", output)
                    if name in EMPTY:
                        self.assertFalse((build / "compile_commands.json").exists())
                        continue
                    db = json.loads((build / "compile_commands.json").read_text())
                    self.assertTrue(db)
                    for entry in db:
                        self.assertIn("-Wall", entry["command"])
                        self.assertIn("-Wextra", entry["command"])
                        self.assertIn("-Werror", entry["command"])
                        self.assertRegex(entry["command"], r"-std=gnu(23|2x)")
                        self.assertNotIn("/attic/", entry["file"])
                        self.assertTrue(Path(entry["file"]).is_relative_to(repo))
                    source = str(repo / CODE[name])
                    entry = next(item for item in db if item["file"] == source)
                    args = shlex.split(entry["command"])
                    index = args.index("-o")
                    del args[index:index + 2]
                    args.remove("-c")
                    args.append("-fsyntax-only")
                    run(args, cwd=entry["directory"])
                    for key, value in DEPENDENCIES.items():
                        if key in cmake:
                            self.assertIn(str(value), entry["command"])

    def test_missing_dependencies_remain_unresolved_not_faked(self):
        repo = ROOT / "ecosystem/repos/api-haven"
        with tempfile.TemporaryDirectory(prefix="per-repo-missing-", dir=os.environ.get("TMPDIR")) as tmp:
            build = Path(tmp)
            run(["cmake", "-S", str(repo), "-B", str(build)])
            db = json.loads((build / "compile_commands.json").read_text())
            self.assertTrue(db)
            self.assertFalse(any(str(DEPENDENCIES["VEXSPOKE_SOURCE_DIR"]) in x["command"] for x in db))
            entry = next(x for x in db if x["file"].endswith("/src/ai/ai_provider.c"))
            args = shlex.split(entry["command"])
            index = args.index("-o")
            del args[index:index + 2]
            args.remove("-c")
            args.append("-fsyntax-only")
            result = subprocess.run(args, text=True, capture_output=True, timeout=120)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("file not found", result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
