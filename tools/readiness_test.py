"""Proof for the readiness scoreboard's attribution and honest status counts.

The fixtures are real git checkouts so `git ls-files` supplies the inventory the
same way the workspace does. No build is executed: `--execute` is the only path
that shells out to `./tools/b`, and it is deliberately not exercised here.

Covers: owner attribution (suite repo vs shared tests vs worktree), production
file counts, test-unit counts, ledger green/stale, declared platform proof, and
the text/markdown/JSON projections.
"""

import hashlib
import importlib.util
import json
import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3] if len(Path(__file__).resolve().parents) > 3 else Path(__file__).resolve().parents[2]
_CANDIDATES = [ROOT / "tools/python/readiness.py", ROOT / "tools/readiness.py"]
SCRIPT = next((p for p in _CANDIDATES if p.exists()), _CANDIDATES[0])
SPEC = importlib.util.spec_from_file_location("readiness", SCRIPT)
READINESS = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(READINESS)


def init_repo(path):
    path.mkdir(parents=True, exist_ok=True)
    subprocess.run(["git", "init", "-q", str(path)], check=True)


class ReadinessTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

        init_repo(self.root)
        init_repo(self.root / "tests")
        init_repo(self.root / "suite/ecosystem/demo")

        (self.root / "tools").mkdir()
        (self.root / "metadata/gists").mkdir(parents=True)

        # Two production files; the untracked file still counts because the
        # inventory asks git for cached and untracked paths.
        (self.root / "suite/ecosystem/demo/src").mkdir(parents=True)
        (self.root / "suite/ecosystem/demo/src/demo.c").write_text(
            ";;DEFINITION\n;;OVERVIEW\nint Demo_value(void) { return 1; }\n")
        (self.root / "suite/ecosystem/demo/src/helper.h").write_text(
            "int Demo_helper(void);\n")

        # Test units: one for the subsystem, one for the worktree tooling.
        (self.root / "tests/demo").mkdir(parents=True)
        self.test_path = self.root / "tests/demo/thing_test.c"
        self.test_path.write_text("int main(void) { return 0; }\n")
        (self.root / "tests/tools").mkdir(parents=True)
        (self.root / "tests/tools/tool_test.py").write_text("print('x')\n")

        self.test_sha = hashlib.sha256(self.test_path.read_bytes()).hexdigest()
        (self.root / "metadata/gists/test-checklist.md").write_text(
            "# ledger\n\n"
            "| Filename | Lab tested? | Last checked (Unix seconds) | "
            "SHA-256 at check | Evidence / scope | Description | Result |\n"
            "| :--- | :---: | ---: | :--- | :--- | :--- | :--- |\n"
            f"| `tests/demo/thing_test.c` | ✅ | 1700000000 | {self.test_sha} | "
            "fixture command | demo test | passed |\n"
            "| `tests/demo/missing_test.c` | ❌ | — | — | none | not run | untested |\n",
            encoding="utf-8")

        claims = {
            "version": 1,
            "platforms": ["macos", "linux", "windows"],
            "repos": {
                "suite/ecosystem/demo": {
                    "tier": "R2", "subsystem": "demo",
                    "support": ["macos", "windows"], "proven": ["macos"],
                },
                "tests": {
                    "tier": "proof", "subsystem": None,
                    "support": ["macos", "linux", "windows"], "proven": ["macos"],
                },
                "(worktree)": {
                    "tier": "workspace", "subsystem": "tools",
                    "support": ["macos", "linux", "windows"], "proven": ["macos"],
                },
            },
        }
        (self.root / "tools/platforms.json").write_text(json.dumps(claims))

    def report(self):
        return READINESS.build_report(self.root)

    def row(self, report, key):
        return next(r for r in report["rows"] if r["repo"] == key)

    def test_owner_attribution(self):
        claims = {"demo": "suite/ecosystem/demo", "tools": "(worktree)"}
        self.assertEqual(
            READINESS.owner_of_path("suite/ecosystem/demo/src/demo.c", claims),
            "suite/ecosystem/demo")
        self.assertEqual(
            READINESS.owner_of_path("tests/demo/thing_test.c", claims),
            "suite/ecosystem/demo")
        self.assertEqual(
            READINESS.owner_of_path("tests/tools/tool_test.py", claims),
            "(worktree)")
        self.assertEqual(
            READINESS.owner_of_path("tools/linter.py", claims), "(worktree)")

    def test_production_files_exclude_tests(self):
        report = self.report()
        demo = self.row(report, "suite/ecosystem/demo")
        self.assertEqual(demo["sources"], 2)  # demo.c + helper.h, not the test
        self.assertEqual(self.row(report, "tests")["sources"], 0)

    def test_test_units_follow_the_tested_subsystem(self):
        report = self.report()
        self.assertEqual(self.row(report, "suite/ecosystem/demo")["test_units"], 1)
        self.assertEqual(self.row(report, "(worktree)")["test_units"], 1)
        self.assertEqual(report["summary"]["test_units"], 2)

    def test_ledger_green_requires_a_matching_hash(self):
        report = self.report()
        demo = self.row(report, "suite/ecosystem/demo")
        self.assertEqual(demo["green"], 1)
        self.assertEqual(demo["untested"], 1)
        self.assertEqual(demo["stale"], 0)
        self.assertEqual(demo["ledger_total"], 2)

        self.test_path.write_text("int main(void) { return 1; }\n")
        demo = self.row(self.report(), "suite/ecosystem/demo")
        self.assertEqual(demo["green"], 0)
        self.assertEqual(demo["stale"], 1)

    def test_platform_proof_is_declared_not_observed(self):
        report = self.report()
        self.assertEqual(report["summary"]["proven"],
                         {"macos": 3, "linux": 0, "windows": 0})
        self.assertEqual(report["summary"]["supported"],
                         {"macos": 3, "linux": 2, "windows": 3})
        self.assertEqual(self.row(report, "suite/ecosystem/demo")["proven"],
                         ["macos"])

    def test_lint_debt_attributed_to_the_owner(self):
        (self.root / "suite/ecosystem/demo/src/demo.c").write_text(
            "void Demo_f(Node *n) { n->x = 1; }\n")
        report = self.report()
        self.assertGreaterEqual(
            self.row(report, "suite/ecosystem/demo")["lint"], 1)

    def test_layout_debt_attributed_to_the_owner(self):
        (self.root / "suite/ecosystem/demo/src/demo.c").write_text(
            "void Demo_f(int x) {\n    if (x) return;\n}\n")
        report = self.report()
        self.assertGreaterEqual(
            self.row(report, "suite/ecosystem/demo")["layout"], 1)
        self.assertGreaterEqual(report["summary"]["layout_total"], 1)

    def test_text_and_markdown_report_the_same_headline(self):
        report = self.report()
        text = READINESS.text_report(report)
        markdown = READINESS.markdown_report(report)
        self.assertIn("3 repos", text)
        self.assertIn("2 test units", text)
        self.assertIn("1 green, 0 stale, 1 untested", text)
        self.assertIn("### Vexgraph readiness", markdown)
        self.assertIn("| `suite/ecosystem/demo` | R2 |", markdown)
        self.assertIn("windows 0/3", markdown)

    def test_json_is_serializable(self):
        report = self.report()
        encoded = json.dumps(report)
        self.assertIn("\"rows\"", encoded)
        self.assertEqual(report["platforms"],
                         ["macos", "linux", "windows"])

    def test_execute_is_off_by_default(self):
        self.assertEqual(self.report()["summary"]["build"], "not run")
        self.assertEqual(self.report()["summary"]["test"], "not run")


if __name__ == "__main__":
    unittest.main()
