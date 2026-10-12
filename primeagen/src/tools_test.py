"""Owner: host consent and confined reads plus real Func build/run separation.

Temporary test-owned files/artifacts only; no user workspace writes or model
calls. Stable-tree checks deliberately do not claim a race-proof sandbox.
"""
from pathlib import Path
import sys
import tempfile
import unittest
import subprocess

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from tools import Tools, FILE_CAP
from agent import Agent


class ToolsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run(["sh", str(ROOT / "personal/primeagen/tools/build.sh")], check=True, capture_output=True, timeout=60)

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

    def test_native_tagged_files_and_approvals(self):
        import json
        actions = [
            ("create", {"path": "new.txt", "text": "hello 👋"}),
            ("read", {"path": "new.txt"}),
            ("edit", {"path": "new.txt", "old": "hello", "new": "goodbye"}),
            ("list", {"path": "."}),
            ("write", {"path": "new.txt", "text": "final"}),
            ("delete", {"path": "new.txt"}),
            ("bash", {"command": "printf 'approved bash'"}),
        ]
        from types import SimpleNamespace
        replies = iter([json.dumps({"tool": tag, "arguments": args}) for tag, args in actions] + ['{"answer":"done"}'])
        agent = Agent(SimpleNamespace(complete=lambda transcript: next(replies)), self.tools)
        self.assertEqual(agent.ask("exercise local tools"), "done")
        self.assertEqual(self.decisions, [tag for tag, _ in actions])
        self.assertFalse((self.workspace / "new.txt").exists())
        results = [event for event in agent.history if event["role"] == "tool"]
        self.assertEqual(results[1]["result"], "hello 👋")
        self.assertEqual(results[-1]["result"], "approved bash")
        self.assertTrue(all("error" not in item for item in results))

    def test_native_denial_malformed_search_and_bash_budgets(self):
        import json
        native = ROOT / "personal/primeagen/.build/tools"
        search = self.path / "search tool"
        search.write_text('#!/bin/sh\nprintf "query=%s" "$1"\n')
        search.chmod(0o700)
        self.tools.websearch = search
        self.allow = False
        requests = [
            ("read", {"path": self.file.name}), ("list", {"path": "."}),
            ("write", {"path": self.file.name, "text": "changed"}),
            ("create", {"path": "absent", "text": "new"}),
            ("edit", {"path": self.file.name, "old": "retained", "new": "changed"}),
            ("delete", {"path": self.file.name}),
            ("bash", {"command": "touch must-not-exist"}),
            ("websearch", {"query": "words"}),
        ]
        from unittest.mock import patch
        with patch("tools.run", side_effect=AssertionError("denial must not spawn")):
            for tag, args in requests:
                self.assertEqual(self.tools.execute(tag, args), "denied by user")
        self.assertEqual(self.file.read_text(), "retained bytes")
        self.assertFalse((self.workspace / "absent").exists())
        self.assertFalse((self.workspace / "must-not-exist").exists())
        before = len(self.decisions)
        for tag, args in [("bash", {"command": "x", "approved": True}), ("edit", {"path": self.file.name, "old": "", "new": "x"}),
                          ("create", {"path": "../escaped", "text": "x"}), ("write", {"path": "alias", "text": "x"}),
                          ("read", {"path": None}), ("bash", {"command": "\0"})]:
            with self.assertRaises((ValueError, OSError)):
                self.tools.execute(tag, args)
        self.assertEqual(len(self.decisions), before)
        self.allow = True
        self.assertEqual(self.tools.execute("websearch", {"query": "literal ; $(touch nope)"}), "query=literal ; $(touch nope)")
        self.assertFalse((self.workspace / "nope").exists())
        self.tools.websearch = None
        with self.assertRaisesRegex(ValueError, "configured"):
            self.tools.execute("websearch", {"query": "words"})
        bounded = Tools(self.workspace, lambda tag, args: True, timeout=0.2, native=native, byte_cap=128)
        with self.assertRaisesRegex(ValueError, "timed out"):
            bounded.execute("bash", {"command": "while :; do :; done"})
        with self.assertRaisesRegex(ValueError, "truncate"):
            bounded.execute("bash", {"command": "printf '%1024s' x"})
        self.assertEqual(bounded.execute("bash", {"command": "printf recovery"}), "recovery")
        bounded.close()


if __name__ == "__main__":
    unittest.main()
