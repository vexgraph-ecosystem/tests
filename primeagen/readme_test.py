"""Mechanical competency review of new app docs against actual source/owners.

This independently checks scope claims and complete source maps; it does not
self-certify live providers, appearance, all platforms or backend trust.
"""
import ast
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
import os
import shutil

ROOT = Path(__file__).resolve().parents[2]
APP = ROOT / "personal/primeagen"


class ReadmeTest(unittest.TestCase):
    def test_source_grounded_scope_and_expanded_blueprints(self):
        readme = (APP / "README.md").read_text()
        for claim in ("## Current State", "## Scope and Limitations", "not production-ready", "strict JSON", "unproven", "ThePrimeagen", "PAIcom", "independent"):
            self.assertIn(claim, readme)
        for path in [*sorted((APP / "src").glob("*.py")), APP / "build.py"]:
            source = path.read_text()
            ast.parse(source)
            self.assertIn("FUNCTION REGISTRY:", source, path)
            self.assertIn("============================================================================", source, path)
            for node in ast.walk(ast.parse(source)):
                if isinstance(node, ast.FunctionDef) and node.name != "__init__":
                    self.assertIn(node.name, source[:source.index("import ")], path)
        native = (APP / "native/request.c").read_text()
        self.assertIn(";;DEFINITION", native)
        self.assertIn(";;OVERVIEW", native)
        self.assertIn("MODULE: Primeagen", native)
        self.assertIn("Message(1, 1, 0, 0", native)
        self.assertIn("AiChat_buildRequest", native)
        self.assertIn("AiChatAnthropic_buildRequest", native)
        self.assertNotIn("Http_perform(", native)
        tools = (APP / "src/tools.py").read_text()
        self.assertIn("if not self.approve", tools)
        self.assertIn('"built artifact "', tools)
        self.assertIn('run([str(Path(home.name) / "artifact")]', tools)
        self.assertNotIn("shell=True", "\n".join(p.read_text() for p in (APP / "src").glob("*.py")))

    def test_cli_no_contact_doctor_and_standalone_source_root_build(self):
        result = subprocess.run([sys.executable, str(APP / "src/cli.py"), "doctor"], capture_output=True, timeout=5)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn(b"Live integration unproven", result.stdout)
        result = subprocess.run([sys.executable, str(APP / "src/cli.py"), "chat"], capture_output=True, timeout=5)
        self.assertNotEqual(result.returncode, 0)
        with tempfile.TemporaryDirectory(prefix="primeagen-standalone-") as temp:
            target = Path(temp) / "independent checkout"
            shutil.copytree(APP, target, ignore=shutil.ignore_patterns(".git", ".build", "__pycache__", ".env"))
            roots = {"API_HAVEN_SOURCE_DIR": ROOT / "ecosystem/repos/api-haven/src",
                     "HARNESS_SOURCE_DIR": ROOT / "ecosystem/repos/harness/src",
                     "VEXSPOKE_SOURCE_DIR": ROOT / "ecosystem/repos/vexspoke/src"}
            env = {**os.environ, **{key: str(value) for key, value in roots.items()}}
            result = subprocess.run([sys.executable, str(target / "build.py")], capture_output=True, timeout=120, env=env)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertTrue((target / ".build/request").is_file())
            result = subprocess.run([str(target / ".build/request"), "openai", "test", "https://example.com/v1"],
                                    input=b"standalone request", capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
