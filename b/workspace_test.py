"""Migration seam: compatibility launcher, workspace metadata and CPU target.

No interactive gallery or visual approval; the builder is exercised through its
existing metadata and registered headless compositor target, not blanket proof.
"""

import json
from pathlib import Path
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[2]


class WorkspaceTest(unittest.TestCase):
    def invoke(self, *arguments):
        result = subprocess.run([str(ROOT / "tools/b"), *arguments], cwd=ROOT / "b",
                                capture_output=True, text=True, timeout=120)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_doctor_resolves_worktree_not_checkout(self):
        doctor = self.invoke("doctor")
        self.assertIn(str(ROOT), doctor)
        self.assertIn("-Werror", doctor)
        self.assertIn("target vexspoke", doctor)
        self.assertFalse((ROOT / "tools/b.c").exists())

    def test_metadata_and_preserved_shader_wiring(self):
        metadata = json.loads(self.invoke("ide"))
        self.assertIsInstance(metadata, dict)
        self.assertIn("vexspoke", json.dumps(metadata))
        engine = (ROOT / "b/workspace.c").read_text()
        for module in ("scatter.vert", "scatter.frag", "resolve.vert", "resolve.frag"):
            self.assertIn(module, engine)
        readme = (ROOT / "README.md").read_text()
        self.assertIn("`tools/b`", readme)
        self.assertIn("forwarding launcher", readme)

    def test_registered_cpu_target_still_builds_and_runs(self):
        self.assertIn("PASS", self.invoke("test", "compositor_scope_test"))


if __name__ == "__main__":
    unittest.main(verbosity=2)
