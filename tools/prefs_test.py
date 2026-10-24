"""Proof that `tools/prefs check` works on a clean checkout.

`PREFERENCES.md` is Git-ignored, so CI only sees the compressed constitution
and the published Gist files. The check must validate what the repository
actually ships, name the missing full document instead of pretending parity,
and still refuse to publish or verify when the full document is absent.

Fixtures are throwaway trees; no shell, no network, no `gh`.
"""

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
PREFS = ROOT / "tools" / "prefs"

GIST = """# Vexgraph - Compressed constitution

## 1. Demo Law

### 1.1 Demo section
* one clause

## 2. Second Law

### 2.1 Another section
* another clause
"""

FULL = """# Vexgraph - Full documentation

## 1. Demo Law

### 1.1 Demo section
* one clause

## 2. Second Law

### 2.1 Another section
* another clause
"""


class PrefsCheckTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.write("metadata/gists/preferences/gist.md", GIST)
        self.write("metadata/gists/preferences/_introduction.md", "intro\n")
        self.write("metadata/gists/preferences/overview-template.c", "/* overview */\n")
        self.write("metadata/gists/preferences/main-template.c", "/* main */\n")
        self.write("tests/test-preferences.md", "# Test Preferences\n\n- one law\n")
        self.write("suite/ecosystem/demo/demo-preferences.md",
                   "# demo - Repo-Local Living Preferences\n\n"
                   "Inherits the universal laws.\n")

    def write(self, rel, text):
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8", newline="\n")
        return path

    def prefs(self, *args):
        return subprocess.run(
            [sys.executable, str(PREFS), *args],
            cwd=self.root, capture_output=True, text=True, timeout=60)

    def test_clean_checkout_passes_compressed_only(self):
        result = self.prefs("check")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("full document absent", result.stdout)
        self.assertIn("compressed-only", result.stdout)
        self.assertIn("PASS", result.stdout)
        self.assertNotIn("FAIL", result.stdout)

    def test_full_document_present_is_compared(self):
        self.write("PREFERENCES.md", FULL)
        result = self.prefs("check")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("2 compressed / 2 full", result.stdout)
        self.assertTrue(result.stdout.rstrip().endswith("PASS"))

    def test_full_document_drift_is_reported(self):
        self.write("PREFERENCES.md", FULL.replace("## 1. Demo Law", "## 1. Other Law"))
        result = self.prefs("check")
        self.assertEqual(result.returncode, 1)
        self.assertIn("FAIL", result.stdout)

    def test_missing_published_file_fails(self):
        (self.root / "metadata/gists/preferences/_introduction.md").unlink()
        result = self.prefs("check")
        self.assertEqual(result.returncode, 1)
        self.assertIn("_introduction.md is missing", result.stdout)

    def test_level_four_heading_fails(self):
        self.write("metadata/gists/preferences/gist.md",
                   GIST + "\n#### too deep\n")
        result = self.prefs("check")
        self.assertEqual(result.returncode, 1)
        self.assertIn("level-4 heading", result.stdout)

    def test_publish_and_verify_refuse_without_full_document(self):
        for command in ("publish", "verify"):
            result = self.prefs(command)
            self.assertEqual(result.returncode, 1, command)
            self.assertIn("PREFERENCES.md is absent", result.stderr)

    def test_list_works_without_full_document(self):
        result = self.prefs("list")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("Demo Law", result.stdout)


if __name__ == "__main__":
    unittest.main()
