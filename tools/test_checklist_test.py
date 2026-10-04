"""Proof for checklist inventory, timestamps, stale results and failed checks."""

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
        (self.root / "tools/subject.py").write_text("original\n")
        (self.root / "ecosystem/interface/demo/header.h").write_text("header\n")
        (self.root / ".gitignore").write_text("tests/\necosystem/\nignored.txt\n")
        (self.root / "ignored.txt").write_text("not source\n")

    def run_cli(self, args):
        with patch.object(CHECKLIST, "__file__", str(self.root / "tools/test_checklist.py")):
            return CHECKLIST.main(args)

    def test_inventory_includes_nested_repos_and_headers(self):
        groups = CHECKLIST.inventory(self.root)
        self.assertIn("ecosystem/interface/demo/header.h", groups["ecosystem/interface/demo"])
        self.assertNotIn("ignored.txt", groups["workspace"])
        self.assertIn("tools/subject.py", groups["workspace"])

    def test_unknown_is_not_verified(self):
        self.assertEqual(self.run_cli(["sync"]), 0)
        records = CHECKLIST.load(self.root / CHECKLIST.REPORT)
        self.assertEqual(records["tools/subject.py"][:3], ["❌", "—", "—"])
        self.assertNotIn(CHECKLIST.REPORT.as_posix(), records)
        self.assertEqual(self.run_cli(["check"]), 0)

    def test_standalone_b_repo_owns_its_inventory(self):
        subprocess.run(["git", "init", "-q", str(self.root / "b")], check=True)
        (self.root / "b/b.c").write_text("int main(void) { return 0; }\n")
        (self.root / "b/.gitignore").write_text("b.json\n")
        (self.root / "b/b.json").write_text("generated\n")
        with (self.root / ".gitignore").open("a") as ignore:
            ignore.write("b/\n")
        groups = CHECKLIST.inventory(self.root)
        self.assertIn("b/b.c", groups["b"])
        self.assertNotIn("b/b.json", groups["b"])
        self.assertNotIn("b/b.c", groups["workspace"])

    def test_ignored_wiki_and_project_repos_use_their_own_ignores(self):
        with (self.root / ".gitignore").open("a") as ignore:
            ignore.write("repos/\nprojects/\n")
        for relative in ("repos/.ecosystem", "projects/demo"):
            repo = self.root / relative
            subprocess.run(["git", "init", "-q", str(repo)], check=True)
            (repo / ".gitignore").write_text("ignored.txt\n")
            (repo / "document.md").write_text("owned document\n")
            (repo / "ignored.txt").write_text("not inventoried\n")
            (repo / "tracked.txt").write_text("tracked files remain included\n")
            subprocess.run(["git", "-C", str(repo), "add", "tracked.txt"], check=True)
            with (repo / ".gitignore").open("a") as ignore:
                ignore.write("tracked.txt\n")
        groups = CHECKLIST.inventory(self.root)
        for relative in ("repos/.ecosystem", "projects/demo"):
            self.assertIn(f"{relative}/document.md", groups[relative])
            self.assertIn(f"{relative}/tracked.txt", groups[relative])
            self.assertNotIn(f"{relative}/ignored.txt", groups[relative])
            self.assertNotIn(f"{relative}/document.md", groups["workspace"])
        files = [path for group in groups.values() for path in group]
        self.assertEqual(len(files), len(set(files)))
        self.assertEqual(self.run_cli(["sync"]), 0)
        records = CHECKLIST.load(self.root / CHECKLIST.REPORT)
        self.assertIn("repos/.ecosystem/document.md", records)
        self.assertIn("projects/demo/document.md", records)
        self.assertEqual(self.run_cli(["check"]), 0)

    def test_real_run_timestamp_failure_skip_and_invalidation(self):
        with patch.object(CHECKLIST.time, "time", return_value=1800000000):
            for code, expected in [(0, "✅"), (1, "❌"), (77, "❌")]:
                self.assertEqual(self.run_cli(["run", "--file", "tools/subject.py", "--",
                                              sys.executable, "-c", f"raise SystemExit({code})"]), code)
                row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tools/subject.py"]
                self.assertEqual(row[0:2], [expected, "1800000000"])
            self.run_cli(["run", "--file", "tools/subject.py", "--", sys.executable, "-c", "pass"])
        (self.root / "tools/subject.py").write_text("changed\n")
        self.assertEqual(self.run_cli(["check"]), 1)
        self.run_cli(["sync"])
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tools/subject.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("stale", row[-1])

    def test_missing_rows_are_rejected(self):
        self.run_cli(["sync"])
        (self.root / "tools/new.py").write_text("new\n")
        self.assertEqual(self.run_cli(["check"]), 1)

    def test_evidence_cannot_break_table(self):
        self.assertEqual(CHECKLIST.cell("a|b\nc"), "a&#124;b c")
        self.assertEqual(CHECKLIST.cell("<script>"), "&lt;script&gt;")

    def test_authorship_is_removed_on_migration(self):
        report = self.root / CHECKLIST.REPORT
        report.parent.mkdir(exist_ok=True)
        report.write_text("| Filename | Tested? | Time | Hash | Evidence | Written by | Result |\n"
                          "| `tools/subject.py` | ❌ | — | — | No evidence | private-name (agent ses_private) | untested |\n")
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
                self.run_cli(["run", "--kind", "visual", "--file", "tools/subject.py", "--", "gallery"])
        self.assertEqual(error.exception.code, 2)
        command.assert_not_called()

    def test_description_is_saved_without_environment_identity(self):
        with patch.dict("os.environ", {"AGENTS_NAME": "private-name", "OPENCODE_SESSION_ID": "ses_private"}):
            self.run_cli(["run", "--file", "tools/subject.py", "--description", "Contract lab check", "--",
                          sys.executable, "-c", "pass"])
        text = (self.root / CHECKLIST.REPORT).read_text()
        self.assertIn("Contract lab check", text)
        self.assertNotIn("private-name", text)
        self.assertNotIn("ses_private", text)

    def test_subject_changed_during_test_is_not_green(self):
        result = self.run_cli(["run", "--file", "tools/subject.py", "--", sys.executable, "-c",
                               "from pathlib import Path; Path('tools/subject.py').write_text('changed')"])
        self.assertEqual(result, 1)
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tools/subject.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("stale", row[-1])

    def test_timeout_is_not_green(self):
        result = self.run_cli(["run", "--file", "tools/subject.py", "--timeout", "0.02", "--",
                               sys.executable, "-c", "import time; time.sleep(5)"])
        self.assertEqual(result, 124)
        row = CHECKLIST.load(self.root / CHECKLIST.REPORT)["tools/subject.py"]
        self.assertEqual(row[0], "❌")
        self.assertIn("124", row[-1])


if __name__ == "__main__":
    unittest.main()
