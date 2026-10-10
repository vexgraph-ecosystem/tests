"""Owner: host consent and confined reads plus real Func build/run separation.

Temporary test-owned files/artifacts only; no user workspace writes or model
calls. Stable-tree checks deliberately do not claim a race-proof sandbox.
"""
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from tools import Tools, FILE_CAP
from agent import Agent


class ToolsTest(unittest.TestCase):
    def setUp(self):
        self.home = tempfile.TemporaryDirectory(prefix="primeagen-owner-")
        self.addCleanup(self.home.cleanup)
        self.path = Path(self.home.name)
        self.workspace = self.path / "workspace"
        self.workspace.mkdir()
        self.file = self.workspace / "name ; literal.txt"
        self.file.write_text("retained bytes")
        self.decisions = []
        self.allow = True
        def approve(name, args):
            self.decisions.append(name)
            return self.allow
        self.tools = Tools(self.workspace, approve)
        self.addCleanup(self.tools.close)

    def test_read_denial_escape_symlink_limits_and_recovery(self):
        self.assertEqual(self.tools.execute("read", {"path": self.file.name}), "retained bytes")
        self.allow = False
        self.assertEqual(self.tools.execute("read", {"path": self.file.name}), "denied by user")
        outside = self.path / "outside"
        outside.write_text("private")
        (self.workspace / "alias").symlink_to(outside)
        for name in ("../outside", "alias", str(outside), "missing", "", None):
            with self.assertRaises((ValueError, OSError)):
                self.tools.execute("read", {"path": name})
        self.allow = True
        self.file.write_bytes(b"x" * FILE_CAP)
        self.assertEqual(len(self.tools.execute("read", {"path": self.file.name})), FILE_CAP)
        self.file.write_bytes(b"x" * (FILE_CAP + 1))
        with self.assertRaisesRegex(ValueError, "truncate"):
            self.tools.execute("read", {"path": self.file.name})
        for name, args in [("shell", {"command": "anything"}), ("read", {"path": self.file.name, "approved": True}),
                           ("func_run", {"artifact": "1"}), ("func_build", {"source": "", "syntax": "surface"})]:
            with self.assertRaises(ValueError):
                self.tools.execute(name, args)

    def test_real_func_compile_run_denial_cleanup_and_failure_preservation(self):
        self.allow = False
        args = {"source": "add(34,7)", "syntax": "surface"}
        self.assertEqual(self.tools.execute("func_build", args), "denied by user")
        self.assertEqual(self.tools.artifacts, {})
        self.allow = True
        result = self.tools.execute("func_build", args)
        self.assertEqual(result, "built artifact 1; not executed")
        home = Path(self.tools.artifacts["1"].name)
        self.assertTrue((home / "artifact").is_file())
        self.allow = False
        self.assertEqual(self.tools.execute("func_run", {"artifact": "1"}), "denied by user")
        self.allow = True
        self.assertEqual(self.tools.execute("func_run", {"artifact": "1"}), "41\n")
        with self.assertRaises(ValueError):
            self.tools.execute("func_build", {"source": "add()", "syntax": "surface"})
        self.assertEqual(set(self.tools.artifacts), {"1"})
        self.assertEqual(self.tools.execute("func_run", {"artifact": "1"}), "41\n")
        self.tools.close()
        self.assertFalse(home.exists())
        self.assertEqual(self.tools.artifacts, {})
        self.tools.close()

    def test_agent_to_actual_func_artifact_full_offline_workflow(self):
        from types import SimpleNamespace
        actions = iter(['{"tool":"func_build","arguments":{"source":"1,34,7","syntax":"numeric"}}',
                        '{"tool":"func_run","arguments":{"artifact":"1"}}', '{"answer":"The native result is 41."}'])
        provider = SimpleNamespace(complete=lambda text: next(actions))
        agent = Agent(provider, self.tools)
        self.assertEqual(agent.ask("compute 34+7 using func"), "The native result is 41.")
        self.assertEqual(self.decisions, ["func_build", "func_run"])
        self.assertEqual(agent.history[4]["result"], "41\n")


if __name__ == "__main__":
    unittest.main()
