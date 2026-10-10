"""Lab proof for the CLion CMake adapter; never executes UI/window tests."""

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import unittest


ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "cmake-build-debug"
GENERATOR = None
NINJA = None


def run(command, **kwargs):
    result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True,
                            timeout=300, **kwargs)
    if result.returncode:
        raise RuntimeError(f"{command!r}\n{result.stdout}\n{result.stderr}")
    return result.stdout


class ClionAdapterTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        configure = ["cmake", "-S", str(ROOT), "-B", str(BUILD), "-DCMAKE_BUILD_TYPE=Debug"]
        if GENERATOR:
            configure += ["-G", GENERATOR]
        if NINJA:
            configure += [f"-DCMAKE_MAKE_PROGRAM={NINJA}"]
        run(configure)
        # Compile the reported problem, but only execute two headless targets.
        run(["cmake", "--build", str(BUILD), "--target", "ui_anchor_pivot_pixels_test",
             "console_test", "hot_behavior_test", "-j", "4"])
        cls.graph = json.loads((BUILD / "b-test-graph.json").read_text())
        cls.targets = {t["name"]: t for t in cls.graph["tests"]}
        cls.ctest = json.loads(run(["ctest", "--test-dir", str(BUILD), "--show-only=json-v1"]))
        cls.tests = {t["name"]: t for t in cls.ctest["tests"]}
        assert not (ROOT / "tests/CMakeLists.txt").exists()

    def test_anchor_has_transitive_includes_and_libraries(self):
        target = self.targets["ui_anchor_pivot_pixels_test"]
        self.assertIn(str(ROOT / "ecosystem/repos/darling-framework/src"), target["includes"])
        self.assertIn(str(ROOT / "ecosystem/repos/graphvex/src"), target["includes"])
        self.assertIn("UNDEBUG", target["definitions"])
        self.assertIn("-std=gnu23", target["options"])
        archives = [Path(p).name for p in target["libraries"] if p.endswith(".a")]
        self.assertIn("libdarling.a", archives)
        self.assertIn("libhotcwap.a", archives)
        self.assertIn("libgraphvex.a", archives)
        self.assertIn("libvexspoke.a", archives)
        self.assertTrue((BUILD / "bin/ui_anchor_pivot_pixels_test").is_file())

    def test_ctest_safety_and_no_demo_registration(self):
        self.assertEqual(set(self.targets), set(self.tests))
        for name in ["window_test", "ui_anchor_pivot_pixels_test"]:
            props = {p["name"]: p["value"] for p in self.tests[name]["properties"]}
            self.assertTrue(props["DISABLED"])
            self.assertEqual(props["SKIP_RETURN_CODE"], 77)
            self.assertEqual(props["TIMEOUT"], 120)
        self.assertNotIn("hello_window", self.tests)
        self.assertNotIn("darling_tests", self.tests)

    def test_hotload_paths_are_isolated_and_built(self):
        for name in ["hot_behavior_test", "manifest_rollback_test"]:
            definitions = self.targets[name]["definitions"]
            modules = [d.split("=", 1)[1].strip('"') for d in definitions
                       if d.startswith(("HOT_BEHAVIOR_MODULE=", "HOT_BEHAVIOR_BAD_MODULE="))]
            self.assertTrue(modules)
            for module in modules:
                self.assertTrue(Path(module).is_relative_to(BUILD / "b-state"))
                self.assertTrue(Path(module).is_file())

    def test_release_metadata_matches_release_dependencies(self):
        env = dict(os.environ, B_HOME=str(BUILD / "b-state"))
        release = json.loads(run([str(ROOT / "tools/b"), "--release", "ide"], env=env))
        target = next(t for t in release["tests"] if t["name"] == "console_test")
        self.assertIn("-O2", target["options"])
        self.assertNotIn("DEBUG_BORROW_CHECK=1", target["definitions"])
        for path in release["byproducts"]:
            self.assertIn("/out/release/", path)

    def test_headless_native_ctest_execution(self):
        output = run(["ctest", "--test-dir", str(BUILD), "-R", "^(console_test|hot_behavior_test)$",
                      "--output-on-failure", "--no-tests=error"])
        self.assertIn("100% tests passed", output)
        self.assertIn("2/2", output)
        print(output)

    def test_production_bvh_has_an_owned_c23_code_model(self):
        database = json.loads((BUILD / "compile_commands.json").read_text())
        source = ROOT / "ecosystem/repos/vexspoke/src/algo/bvh.c"
        entry = next(item for item in database if Path(item["file"]) == source)
        command = entry["command"]
        self.assertIn("vexgraph_index_vexspoke", command)
        for flag in ("-std=gnu23", "-Werror", "-mcpu=apple-m1", "DEBUG_BORROW_CHECK=1"):
            self.assertIn(flag, command)
        self.assertIn(str(ROOT / "ecosystem/repos/vexspoke/src"), command)
        # Execute the actual indexing command for the reported problem file.
        import shlex
        result = subprocess.run(shlex.split(command), cwd=entry["directory"],
                                text=True, capture_output=True, timeout=120)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("index", self.graph)

    def test_coordinator_edits_trigger_cmake_reload(self):
        ninja = (BUILD / "build.ninja").read_text()
        self.assertIn(str(ROOT / "tools/workspace.c"), ninja)
        self.assertIn(str(ROOT / "tools/build_annotation.h"), ninja)

    def test_gallery_and_shared_helpers_have_owned_contexts(self):
        import shlex
        indexed = {item["name"]: item for item in self.graph["index"]}
        for target in ("filter_gallery", "darling_tests"):
            self.assertIn(target, indexed)
            self.assertNotIn(target, self.tests)
            for repo in ("darling-framework/src", "graphvex/src", "hotcwap",
                         "vexspoke/src"):
                self.assertIn(str(ROOT / "ecosystem/repos" / repo), indexed[target]["includes"])
        database = json.loads((BUILD / "compile_commands.json").read_text())
        sources = indexed["filter_gallery"]["sources"]
        self.assertIn(str(ROOT / "tests/darling/compositor/filter_gallery.c"), sources)
        # This gallery's fixture is header-only; compilation checks that helper
        # through its actual main. Iterate all TUs for apps with shared sources.
        for source in sources:
            entry = next(item for item in database if item["file"] == source and
                         "vexgraph_index_filter_gallery.dir" in item["command"])
            self.assertIn("-std=gnu23", entry["command"])
            result = subprocess.run(shlex.split(entry["command"]), cwd=entry["directory"],
                                    text=True, capture_output=True, timeout=120)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        documentation = (ROOT / "tools/BUILD.md").read_text()
        self.assertIn("tests/darling/compositor/filter_gallery.c", documentation)
        self.assertIn("gallery indexing does not register or launch it", documentation)

    def test_every_exported_source_has_a_compilation_context(self):
        for build in (BUILD,):
            database = json.loads((build / "compile_commands.json").read_text())
            compiled = {item["file"] for item in database}
            for target in self.graph["tests"] + self.graph["index"]:
                self.assertTrue(set(target["sources"]) <= compiled,
                                f"{build}: {target['name']} has unmodeled sources")

    def test_tests_checkout_borrows_the_only_workspace_entry(self):
        self.assertFalse((ROOT / "tests/CMakeLists.txt").exists())
        database = json.loads((BUILD / "compile_commands.json").read_text())
        source = ROOT / "tests/vexspoke/algo/bvh_test.c"
        # This owner may be co-owned by the algorithm suite instead.
        if not source.exists():
            source = ROOT / "tests/vexspoke/algo/algo_suite_test.c"
        entry = next(item for item in database if Path(item["file"]) == source)
        self.assertIn("-std=gnu23", entry["command"])
        self.assertIn(str(ROOT / "ecosystem/repos/vexspoke/src"), entry["command"])
        documentation = (ROOT / "tests/README.md").read_text()
        self.assertIn("Open the workspace root", documentation)
        source = ROOT / "tests/darling/compositor/filter_gallery.c"
        entry = next(item for item in database if Path(item["file"]) == source)
        self.assertIn("vexgraph_index_filter_gallery", entry["command"])
        self.assertIn(str(ROOT / "ecosystem/repos/darling-framework/src"), entry["command"])


if __name__ == "__main__":
    if not shutil.which("cmake") or not shutil.which("ctest"):
        print("SKIP: CMake and CTest are required for adapter lab proof")
        raise SystemExit(77)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=BUILD)
    parser.add_argument("--generator")
    parser.add_argument("--ninja")
    options = parser.parse_args()
    BUILD = options.build_dir.resolve()
    GENERATOR, NINJA = options.generator, options.ninja
    unittest.main(argv=[sys.argv[0]])
