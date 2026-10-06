"""Headless CLion entry-point proof; never opens a gallery or approves IDE UI.

Exercise native C/Rust files, literal arguments, error propagation, project
graph ownership and saved XML action wiring. Every process has a watchdog.
"""
import os
import shutil
from pathlib import Path
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[2]


class RunCurrentTest(unittest.TestCase):
    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory(prefix="b-current-")
        self.addCleanup(self.scratch.cleanup)
        self.home = Path(self.scratch.name)
        self.env = dict(os.environ, B_HOME=str(self.home / "state"))

    def invoke(self, *args):
        return subprocess.run([str(ROOT / "tools/run-current"), *map(str, args)],
                              cwd=self.home, env=self.env, capture_output=True,
                              text=True, timeout=120)

    def test_standalone_c_file_and_literal_arguments(self):
        source = self.home / "hello space.c"
        source.write_text('#include <stdio.h>\nint main(int n, char **v) {\n'
                          'if (n != 2) return 9; puts(v[1]); return 0;\n}\n')
        result = self.invoke(source, "space ; $HOME")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, "space ; $HOME\n")

    def test_rust_exit_status_and_args(self):
        directory = self.home / "rust project with spaces"
        directory.mkdir()
        source = directory / "hello.rs"
        source.write_text('fn main() { println!("{}", std::env::args().nth(1).unwrap());'
                          ' std::process::exit(5); }\n')
        result = self.invoke(source, "literal argument")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertEqual(result.stdout, "literal argument\n")

    def test_finder_style_path_still_finds_rust(self):
        self.env["PATH"] = "/usr/bin:/bin:/usr/sbin:/sbin"
        result = self.invoke(ROOT / "personal/relational-engine/rust/src/helloworld.rs", "GUI path")
        self.assertEqual(result.returncode, 5, result.stderr)
        self.assertEqual(result.stdout, "GUI path\n")

    def test_missing_file_and_no_arguments_reject(self):
        for args in ((), (self.home / "absent.rs",)):
            result = self.invoke(*args)
            self.assertEqual(result.returncode, 2)
            self.assertTrue(result.stderr)

    def test_compilation_failure_does_not_run_old_output(self):
        source = self.home / "retry.c"
        source.write_text('#include <stdio.h>\nint main(void) { puts("valid"); return 0; }\n')
        self.assertEqual(self.invoke(source).returncode, 0)
        source.write_text('not valid C\n')
        rejected = self.invoke(source)
        self.assertNotEqual(rejected.returncode, 0)
        self.assertNotIn("valid", rejected.stdout)
        source.write_text('int main(void) { return 0; }\n')
        self.assertEqual(self.invoke(source).returncode, 0)

    def test_generic_b_does_not_own_ecosystem_graph(self):
        self.assertFalse((ROOT / "personal/b/workspace.c").exists())
        launcher = (ROOT / "personal/b/b").read_text()
        self.assertNotIn("workspace-cli", launcher)
        self.assertNotIn("ecosystem/", launcher)
        project = (ROOT / "tools/b").read_text()
        self.assertIn('run exec "$root/tools/workspace.c" --', project)
        docs = (ROOT / "tools/BUILD.md").read_text()
        for phrase in ("tools/run-current", "deliberately exits 5", "project-owned executable graph"):
            self.assertIn(phrase, docs)

    def test_workspace_routing_without_launching_windows(self):
        workspace = self.home / "workspace with spaces"
        tools = workspace / "tools"
        tools.mkdir(parents=True)
        shutil.copy2(ROOT / "tools/run-current", tools / "run-current")
        generic = workspace / "personal/b"
        generic.mkdir(parents=True)
        for launcher, label in ((tools / "b", "graph"), (generic / "b", "generic")):
            launcher.write_text(f'#!/bin/sh\nprintf "{label}\\n"\nprintf "%s\\n" "$@"\n')
            launcher.chmod(0o755)
        for relative, route in (("tests/ui/gallery.c", "graph"),
                                ("ecosystem/projects/demo/main.c", "graph"),
                                ("personal/demo/main.c", "generic")):
            source = workspace / relative
            source.parent.mkdir(parents=True)
            source.write_text("unused fixture\n")
            result = subprocess.run([str(tools / "run-current"), str(source), "one argument"],
                                    env=self.env, capture_output=True, text=True, timeout=10)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stdout.splitlines()[0], route)
            self.assertEqual(result.stdout.splitlines()[-1], "one argument")

    def test_saved_clion_action_matches_registered_tool(self):
        settings = Path.home() / "Library/Application Support/JetBrains/CLion2026.2/tools/b Runner.xml"
        toolset = ET.parse(settings).getroot()
        tool = toolset.find("tool")
        options = {node.get("name"): node.get("value") for node in tool.findall("exec/option")}
        self.assertEqual(options["COMMAND"], "$USER_HOME$/vexgraph/tools/run-current")
        self.assertEqual(options["PARAMETERS"], '"$FilePath$"')
        project = ET.parse(ROOT / ".idea/workspace.xml").getroot()
        actions = [node.get("actionId") for node in project.findall(".//option[@name='ToolBeforeRunTask']")]
        self.assertIn(f"Tool_{toolset.get('name')}_{tool.get('name')}", actions)


if __name__ == "__main__":
    unittest.main(verbosity=2)
