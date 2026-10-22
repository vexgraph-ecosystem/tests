"""Static owner for Gist canonical paths, R6 blueprint scope and terminal follow-up.

This verifies document/config/inventory contracts, not loaded native modules,
physical terminals, live providers, transitive dependency closure or performance.
Runtime terminal proof belongs to tests/determinant and later Primeagen owners.
"""
from pathlib import Path
import json
import re
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]


class MigrationContractTest(unittest.TestCase):
    def test_gist_sources_are_local_canonical_and_ignored(self):
        for relative in ("gist/PREFERENCES.md", "gist/test-checklist.md", "gist/BLOCKERS.md",
                         "gist/readiness/Home.md", "gist/readiness/primeagen.md"):
            self.assertTrue((ROOT / relative).is_file(), relative)
            self.assertFalse((ROOT / relative).is_symlink(), relative)
            ignored = subprocess.run(["git", "check-ignore", relative], cwd=ROOT,
                                     capture_output=True, text=True, timeout=10)
            self.assertEqual(ignored.returncode, 0, relative)
        self.assertFalse((ROOT / "PREFERENCES.md").exists())
        self.assertFalse((ROOT / "tests/test-checklist.md").exists())
        config = json.loads((ROOT / ".opencode/opencode.json").read_text())
        self.assertEqual(config["instructions"][0], "gist/PREFERENCES.md")

    def test_r6_blueprints_have_no_inflated_app_readiness(self):
        for name in ("anti", "semicolon"):
            owner = ROOT / "superapp" / name
            self.assertTrue((owner / ".git").exists())
            sources = [p for p in owner.rglob("*")
                       if p.suffix in (".c", ".h", ".m", ".rs") and ".git" not in p.parts]
            self.assertEqual(sources, [], name)
            readme = (owner / "README.md").read_text()
            for phrase in ("R6", "unfinished", "Tests (future)", "## Current State",
                           "## Scope and Limitations"):
                self.assertIn(phrase, readme)
            page = (ROOT / "gist/readiness" / f"{name}.md").read_text()
            self.assertIn("unmeasured", page)
            for row in page.splitlines():
                if re.match(r"\|\s*\d", row):
                    self.assertNotRegex(row, "[💚🟩🟨🟧]", row)

    def test_sidebar_anchors_resolve_to_published_filenames(self):
        readiness = ROOT / "gist/readiness"
        names = {path.name.lower().replace(".", "-") for path in readiness.glob("*.md")}
        sidebar = (readiness / "_Sidebar.md").read_text()
        for anchor in re.findall(r"\(#file-([^)]*)\)", sidebar):
            self.assertIn(anchor, names, anchor)
        self.assertIn("#file-determinant-md", sidebar)
        self.assertIn("R6 Superapps", sidebar)

    def test_terminal_boundary_and_primeagen_followup_are_explicit(self):
        laws = (ROOT / "drivers/determinant/determinant-preferences.md").read_text()
        self.assertIn("R2 never calls Determinant", laws)
        self.assertIn("Terminal coordinates are cells", laws)
        self.assertIn("external serialization", laws)
        scheduled = (ROOT / "gist/readiness/primeagen.md").read_text().split(
            "## Scheduled Determinant consumer integration", 1)[1].split("| Feature", 1)[0]
        for phrase in ("restart-required", "same production process", "consent",
                       "rollback", "do not fix or prove"):
            self.assertIn(phrase, scheduled)
        self.assertIn("standalone R5", (ROOT / "gist/PREFERENCES.md").read_text())


if __name__ == "__main__":
    unittest.main(verbosity=2)
