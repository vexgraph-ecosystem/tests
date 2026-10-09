"""Proof for the executable-tests-only inventory, timestamps, staleness and failures.

The ledger records compilable/runnable test files, runner scripts and the shared
harness. Markdown, documentation, configuration and production source are
deliberately excluded, so these fixtures create real test files and assert that
non-test files produce no rows.
"""

import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("checklist", ROOT / "tools/test_checklist.py")
CHECKLIST = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(CHECKLIST)


class ChecklistTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for rel in [".", "tests", "ecosystem/interface/demo"]:
            subprocess.run(["git", "init", "-q", str(self.root / rel)], check=True)
        (self.root / "tools").mkdir()
        # Executable test units.
        (self.root / "tests/subject_test.py").write_text("original\n")
        (self.root / "tests/unit_test.c").write_text("int main(void) { return 0; }\n")
        (self.root / "tests/run.py").write_text("print('runner')\n")
        (self.root / "tests/test_support.h").write_text("harness\n")
        (self.root / "tests/helper_support.py").write_text("shared helper, not a test\n")
        # Non-test files that must never be inventoried.
        (self.root / "tests/notes.md").write_text("documentation\n")
        (self.root / "tests/data.json").write_text("{}\n")
        (self.root / ".gitignore").write_text("tests/\necosystem/\nignored.txt\n")
        (self.root / "ignored.txt").write_text("not source\n")

    def run_cli(self, args):
        with patch.object(CHECKLIST, "__file__", str(self.root / "tools/test_checklist.py")):
            return CHECKLIST.main(args)

    def test_predicate_accepts_tests_and_rejects_documentation(self):
        for good in ("tests/a_test.c", "x/b_test.py", "c_test.rs", "d_test.m",
                     "tests/run.py", "tests/native_run.py", "tests/test_support.h",
                     "tests/test_widget.py"):
            self.assertTrue(CHECKLIST.is_executable_test(good), good)
        for bad in ("tests/notes.md", "README.md", "tests/data.json",
                    "src/panel.c", "tests/helper_support.py", "docs/guide.txt"):
            self.assertFalse(CHECKLIST.is_executable_test(bad), bad)

    def test_inventory_lists_only_executable_tests(self):
        (self.root / "ecosystem/interface/demo/widget_test.c").write_text("int main(void){return 0;}\n")
        (self.root / "ecosystem/interface/demo/header.h").write_text("header\n")
        groups = CHECKLIST.inventory(self.root)
        self.assertIn("tests/subject_test.py", groups["tests"])
        self.assertIn("tests/unit_test.c", groups["tests"])
        self.assertIn("tests/run.py", groups["tests"])
        self.assertIn("tests/test_support.h", groups["tests"])
        self.assertNotIn("tests/notes.md", groups.get("tests", []))
        self.assertNotIn("tests/helper_support.py", groups.get("tests", []))
        self.assertNotIn("ignored.txt", groups["workspace"])
        self.assertIn("ecosystem/interface/demo/widget_test.c", groups["ecosystem/interface/demo"])
        self.assertNotIn("ecosystem/interface/demo/header.h", groups["ecosystem/interface/demo"])
        self.assertNotIn("tools/notes.md", groups["workspace"])

    def test_unknown_is_not_verified(self):
        self.assertEqual(self.run_cli(["sync"]), 0)
        records = CHECKLIST.load(self.root / CHECKLIST.REPORT)
        self.assertEqual(records["tests/subject_test.py"][:3], ["❌", "—", "—"])
        self.assertNotIn(CHECKLIST.REPORT.as_posix(), records)
        self.assertNotIn("tests/notes.md", records)
        self.assertEqual(self.run_cli(["check"]), 0)

    def test_standalone_b_repo_owns_its_test_inventory(self):
        subprocess.run(["git", "init", "-q", str(self.root / "b")], check=True)
        (self.root / "b/util_test.c").write_text("int main(void) { return 0; }\n")
        (self.root / "b/util.c").write_text("int helper(void) { return 0; }\n")
        (self.root / "b/.gitignore").write_text("b.json\n")
        (self.root / "b/b.json").write_text("generated\n")
        with (self.root / ".gitignore").open("a") as ignore:
            ignore.write("b/\n")
        groups = CHECKLIST.inventory(self.root)
        self.assertIn("b/util_test.c", groups["b"])
        self.assertNotIn("b/util.c", groups["b"])
        self.assertNotIn("b/b.json", groups["b"])
        self.assertNotIn("b/util_test.c", groups["workspace"])

    def test_ignored_repos_use_their_own_ignores_for_tests(self):
        with (self.root / ".gitignore").open("a") as ignore:
            ignore.write("repos/\nprojects/\npersonal/\n")
        for relative in ("repos/.ecosystem", "projects/demo", "personal/demo"):
            repo = self.root / relative
            subprocess.run(["git", "init", "-q", str(repo)], check=True)
            (repo / ".gitignore").write_text("ignored_test.c\n")
            (repo / "unit_test.c").write_text("int main(void){return 0;}\n")
            (repo / "ignored_test.c").write_text("ignored\n")
            (repo / "document.md").write_text("owned document\n")
            subprocess.run(["git", "-C", str(repo), "add", "unit_test.c"], check=True)
        groups = CHECKLIST.inventory(self.root)
        for relative in ("repos/.ecosystem", "projects/demo", "personal/demo"):
            self.assertIn(f"{relative}/unit_test.c", groups[relative])
            self.assertNotIn(f"{relative}/ignored_test.c", groups[relative])
            self.assertNotIn(f"{relative}/document.md", groups[relative])
            self.assertNotIn(f"{relative}/unit_test.c", groups["workspace"])
        files = [path for group in groups.values() for path in group]
        self.assertEqual(len(files), len(set(files)))
        self.assertEqual(self.run_cli(["sync"]), 0)
        records = CHECKLIST.load(self.root / CHECKLIST.REPORT)
        self.assertIn("repos/.ecosystem/unit_test.c", records)
        self.assertNotIn("projects/demo/document.md", records)
        self.assertEqual(self.run_cli(["check"]), 0)

    def test_real_run_timestamp_failure_skip_and_invalidation(self):
        with patch.object(CHECKLIST.time, "time", return_value=1800000000):
            for code, expected in [(0, "✅"), (1, "❌"), (77, "❌")]:
                self.assertEqual(self.run_cli(["run", "--file", "tests/subject_test.py", "--",
                                              sys.executable, "-c", f"raise SystemExit({code})"]), code)
                row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tests/subject_test.py"]
                self.assertEqual(row[0:2], [expected, "1800000000"])
            self.run_cli(["run", "--file", "tests/subject_test.py", "--", sys.executable, "-c", "pass"])
        (self.root / "tests/subject_test.py").write_text("changed\n")
        self.assertEqual(self.run_cli(["check"]), 1)
        self.run_cli(["sync"])
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tests/subject_test.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("stale", row[-1])

    def test_missing_rows_are_rejected(self):
        self.run_cli(["sync"])
        (self.root / "tests/new_test.py").write_text("new\n")
        self.assertEqual(self.run_cli(["check"]), 1)

    def test_non_test_subject_is_rejected(self):
        (self.root / "tools/readme.md").write_text("# docs\n")
        with self.assertRaises(SystemExit) as error:
            self.run_cli(["run", "--file", "tools/readme.md", "--", sys.executable, "-c", "pass"])
        self.assertEqual(error.exception.code, 2)

    def test_evidence_cannot_break_table(self):
        self.assertEqual(CHECKLIST.cell("a|b\nc"), "a&#124;b c")
        self.assertEqual(CHECKLIST.cell("<script>"), "&lt;script&gt;")

    def test_authorship_is_removed_on_migration(self):
        report = self.root / CHECKLIST.REPORT
        report.parent.mkdir(exist_ok=True)
        report.write_text("| Filename | Tested? | Time | Hash | Evidence | Written by | Result |\n"
                          "| `tests/subject_test.py` | ❌ | — | — | No evidence | private-name (agent ses_private) | untested |\n")
        self.run_cli(["sync"])
        text = report.read_text()
        self.assertNotIn("Written by", text)
        self.assertNotIn("private-name", text)
        self.assertNotIn("ses_private", text)
        self.assertIn("Description", text)
        self.assertEqual(self.run_cli(["check"]), 0)

    def test_visual_check_is_rejected_before_execution(self):
        with patch.object(CHECKLIST.subprocess, "run") as command:
            with self.assertRaises(SystemExit) as error:
                self.run_cli(["run", "--kind", "visual", "--file", "tests/subject_test.py", "--", "gallery"])
        self.assertEqual(error.exception.code, 2)
        command.assert_not_called()

    def test_description_is_saved_without_environment_identity(self):
        with patch.dict("os.environ", {"AGENTS_NAME": "private-name", "OPENCODE_SESSION_ID": "ses_private"}):
            self.run_cli(["run", "--file", "tests/subject_test.py", "--description", "Contract lab check", "--",
                          sys.executable, "-c", "pass"])
        text = (self.root / CHECKLIST.REPORT).read_text()
        self.assertIn("Contract lab check", text)
        self.assertNotIn("private-name", text)
        self.assertNotIn("ses_private", text)

    def test_subject_changed_during_test_is_not_green(self):
        result = self.run_cli(["run", "--file", "tests/subject_test.py", "--", sys.executable, "-c",
                               "from pathlib import Path; Path('tests/subject_test.py').write_text('changed')"])
        self.assertEqual(result, 1)
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tests/subject_test.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("stale", row[-1])

    def test_timeout_is_not_green(self):
        result = self.run_cli(["run", "--file", "tests/subject_test.py", "--timeout", "0.02", "--",
                               sys.executable, "-c", "import time; time.sleep(5)"])
        self.assertEqual(result, 124)
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tests/subject_test.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("124", row[-1])


if __name__ == "__main__":
    unittest.main()
