"""Workspace-only CMake ownership and complete source-context inventory.

The historical filename is retained for runner/ledger continuity. Proves metadata
and representative strict compiler checks, not every source's runtime or IDE UI.
"""
import json
import os
from pathlib import Path
import shlex
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
CODE = {
    "ecosystem/repos/vexspoke": "src/algo/bvh.c",
    "ecosystem/repos/api-haven": "src/ai/ai_provider.c",
    "ecosystem/repos/graphvex": "src/graphics/viewport.c",
    "ecosystem/repos/hotcwap": "kernel/process.c",
    "ecosystem/repos/darling-framework": "src/panel/panel.c",
    "ecosystem/repos/relational-engine": "src/io/file.c",
    "ecosystem/repos/sesh": "src/snapshot/snapshot.c",
    "ecosystem/repos/darkbase": "src/database/database.c",
    "ecosystem/repos/samplerate": "src/audio/audio_format.c",
    "ecosystem/repos/harness": "src/space/model_user.c",
    "ecosystem/projects/impedance": "src/impedance.c",
    "personal/b": "b.c",
}
BLUEPRINTS = ("ecosystem/repos/language", "personal/func",
              "ecosystem/projects/anti", "ecosystem/projects/drawling",
              "ecosystem/projects/semicolon")


def run(args, **kwargs):
    result = subprocess.run(args, text=True, capture_output=True, timeout=180, **kwargs)
    if result.returncode:
        raise AssertionError(f"{args}\n{result.stdout}\n{result.stderr}")
    return result


class PerRepoIdeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix="workspace-ide-", dir=os.environ.get("TMPDIR"))
        cls.addClassCleanup(cls.temp.cleanup)
        cls.build = Path(cls.temp.name)
        run(["cmake", "-S", str(ROOT), "-B", str(cls.build), "-DBUILD_TESTING=OFF"])
        cls.db = json.loads((cls.build / "compile_commands.json").read_text())

    def test_workspace_is_the_only_owned_entry_and_disclaimer_home(self):
        self.assertTrue((ROOT / "CMakeLists.txt").is_file())
        for relative in (*CODE, *BLUEPRINTS, "tests"):
            with self.subTest(owner=relative):
                owner = ROOT / relative
                self.assertFalse((owner / "CMakeLists.txt").exists())
                readme = (owner / "README.md").read_text()
                self.assertNotIn("## CLion: CMake is IDE metadata only", readme)
                self.assertNotIn("## Disclaimer: CMake", readme)
                self.assertNotIn("LANGUAGES NONE", readme)
        constitution = (ROOT / "preferences.md").read_text()
        self.assertIn("only maintained `CMakeLists.txt`", constitution)
        self.assertIn("opt-in native", constitution)
        self.assertIn("inlay hints", (ROOT / "README.md").read_text())

    def test_every_owned_host_translation_unit_has_a_context(self):
        compiled = {Path(entry["file"]).resolve() for entry in self.db}
        count = 0
        for relative in (*CODE, *BLUEPRINTS):
            owner = ROOT / relative
            source_root = owner if relative in ("personal/b", "ecosystem/repos/hotcwap") else owner / "src"
            for source in source_root.rglob("*"):
                if source.suffix not in (".c", ".m"):
                    continue
                if any(part.startswith("cmake-build") or part in
                       (".git", "CMakeFiles", "attic", "vendor", "third_party", "_old")
                       for part in source.relative_to(source_root).parts):
                    continue
                if source.name in ("window.c", "window_linux.c", "window_wayland.c", "window_win32.c", "audio_stream_stub.c"):
                    continue
                self.assertIn(source.resolve(), compiled, source)
                count += 1
        self.assertGreater(count, 400)
        print(f"Workspace host source inventory: {count} translation units have contexts")

    def test_representative_c23_commands_use_real_owner_headers(self):
        for owner, relative in CODE.items():
            with self.subTest(owner=owner):
                source = ROOT / owner / relative
                entry = next(item for item in self.db if Path(item["file"]) == source)
                command = entry["command"]
                for flag in ("-Wall", "-Wextra", "-Werror", "-mcpu=apple-m1"):
                    self.assertIn(flag, command)
                self.assertRegex(command, r"-std=gnu(23|2x)")
                args = shlex.split(command)
                index = args.index("-o")
                del args[index:index + 2]
                args.remove("-c")
                run(args + ["-fsyntax-only"], cwd=entry["directory"])


if __name__ == "__main__":
    unittest.main(verbosity=2)
