"""Scoped snapshot documentation/IDE inventory; not cloud or runtime proof."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class SnapshotDocsTest(unittest.TestCase):
    def test_scope_and_run_contract(self):
        readme = (ROOT / "ecosystem/repos/sesh/README.md").read_text()
        for phrase in ("tests/sesh/run.py", "caller-owned", "not an implemented Google Drive connector",
                       "drive.file", "Pending operations", "engine-owned", "not rewritten into Rust",
                       "No canonical preferences file", "iCloud/CloudKit"):
            self.assertIn(phrase, readme)
        contributing = (ROOT / "ecosystem/repos/sesh/CONTRIBUTING.md").read_text()
        self.assertIn("caller-buffer snapshot core", contributing)
        self.assertIn("tests/sesh/run.py", contributing)

    def test_readiness_is_scoped(self):
        for repo in ("sesh", "api-haven"):
            text = (ROOT / f"ecosystem/ecosystem/{repo}.md").read_text()
            self.assertIn("snapshot", text.lower())
            self.assertIn("tests/sesh/run.py", text)
            self.assertIn("Drive", text)


if __name__ == "__main__":
    unittest.main(verbosity=2)
