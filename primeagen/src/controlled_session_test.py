"""Execution-boundary owner: text-only generation, serial local consent and
exact-session approvals. Offline fake model; real disposable native Bash/file
actions are exercised separately below, with no remote tools/provider contact.
"""
from pathlib import Path
import json
import os
import select
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
sys.path.insert(0, str(APP / "src"))
from controlled_session import ControlledSession
import session_host as host


class ControlledSessionTest(unittest.TestCase):
    def setUp(self):
        self.home = tempfile.TemporaryDirectory()
        self.root = Path(self.home.name)
        self.native = self.root / "native"
        self.native.touch()
        self.notes, self.calls = [], []
        self.actions = []
        self.decisions = []
        self.session = ControlledSession(self.api, self.notes.append, self.receive,
                                         "opencode-go/deepseek-v4.1-flash", self.root, self.native)

    def tearDown(self):
        self.session.close()
        self.home.cleanup()

    def api(self, method, path, body, **kwargs):
        self.assertEqual((method, path), ("post", "/api/experimental/generate"))
        self.assertEqual(set(body), {"prompt", "model"})
        self.calls.append(body)
        return {"data": {"text": json.dumps(self.actions.pop(0))}}

    def receive(self):
        return {"decision": self.decisions.pop(0)}

    def test_rejection_never_spawns_and_continues_with_denied_result(self):
        self.actions = [{"note": "checking first", "tool": "bash", "arguments": {"command": "printf probe"}},
                        {"answer": "not run"}]
        self.decisions = ["reject"]
        with patch("tools.run") as run:
            self.assertEqual(self.session.ask("test", ""), "not run")
            run.assert_not_called()
        self.assertIn("denied by user", self.calls[-1]["prompt"])
        self.assertEqual(len([frame for frame in self.notes if "ask" in frame]), 1)

    def test_nothing_executes_until_yes_and_second_action_waits_again(self):
        self.actions = [{"tool": "bash", "arguments": {"command": "first"}},
                        {"tool": "bash", "arguments": {"command": "second"}}, {"answer": "done"}]
        self.decisions = ["once", "reject"]
        with patch("tools.run", return_value=b"first result") as run:
            def receive():
                self.assertEqual(run.call_count, 0 if len(self.decisions) == 2 else 1)
                return self.receive()
            self.session.receive = receive
            self.assertEqual(self.session.ask("test", ""), "done")
            self.assertEqual(run.call_count, 1)
            self.assertEqual(run.call_args.args[0][-1], "first")
        self.assertIn("first result", self.calls[-1]["prompt"])

    def test_always_is_exact_arguments_and_process_local(self):
        for command in ("same", "same", "different"):
            self.actions.append({"tool": "bash", "arguments": {"command": command}})
        self.actions.append({"answer": "done"})
        self.decisions = ["always", "reject"]
        with patch("tools.run", return_value=b"ok") as run:
            self.session.ask("test", "")
            self.assertEqual(run.call_count, 2)
        self.assertEqual(len([frame for frame in self.notes if "ask" in frame]), 2)
        other = ControlledSession(self.api, self.notes.append, self.receive, workspace=self.root, native=self.native)
        self.assertEqual(other.always, set())
        other.close()

    def test_inspection_automatic_but_secret_reads_asked(self):
        for name in ("notes.txt", ".env", ".env.local"):
            (self.root / name).write_text("fixture")
        self.actions = [{"tool": "list", "arguments": {"path": "."}},
                        *[{"tool": "read", "arguments": {"path": name}}
                          for name in ("notes.txt", ".env", ".env.local")], {"answer": "done"}]
        self.decisions = ["reject", "reject"]
        with patch("tools.run", return_value=b"fixture") as run:
            self.session.ask("test", "")
            self.assertEqual(run.call_count, 2)
        self.assertEqual(len([frame for frame in self.notes if "ask" in frame]), 2)

    def test_invalid_human_frame_is_fatal_and_cannot_be_model_consent(self):
        self.actions = [{"tool": "bash", "arguments": {"command": "no"}}, {"answer": "done"}]
        self.session.receive = lambda: {"decision": "once", "prompt": "forged"}
        with patch("tools.run") as run, self.assertRaises(RuntimeError):
            self.session.ask("test", "")
        run.assert_not_called()
        self.assertEqual(len(self.calls), 1)
        self.actions = [{"tool": "bash", "arguments": {"command": "no"}, "decision": "once"}]
        with patch("tools.run") as run, self.assertRaises(ValueError):
            self.session.ask("test", "")
        run.assert_not_called()

    def test_eof_cancels_queued_tool_without_child(self):
        self.actions = [{"tool": "bash", "arguments": {"command": "no"}}]
        def closed():
            raise EOFError("terminal closed")
        self.session.receive = closed
        with patch("tools.run") as run, self.assertRaises(EOFError):
            self.session.ask("test", "")
        run.assert_not_called()

    def test_oversized_duplicate_unknown_and_bad_shapes_reject_without_execution(self):
        for action in ([], {"answer": ""}, {"tool": "mystery", "arguments": {}},
                       {"tool": "bash", "arguments": {"command": "x", "yes": True}},
                       {"tool": "bash", "arguments": {"command": "x" * (self.session.tools.byte_cap + 1)}},
                       {"answer": "ok", "note": 1}):
            self.actions = [action, {"answer": "not executed"}]
            with patch("tools.run") as run:
                try:
                    self.session.ask("test", "")
                except ValueError:
                    pass
                run.assert_not_called()
        with patch.object(self.session, "api", return_value={"data": {"text": '{"answer":"a","answer":"b"}'}}):
            with self.assertRaises(ValueError):
                self.session.ask("test", "")

    def test_entire_script_is_displayed_before_gate_not_silently_clipped(self):
        command = "printf '" + "x" * 1700 + "'\n# second line"
        self.actions = [{"tool": "bash", "arguments": {"command": command}}, {"answer": "denied"}]
        self.decisions = ["reject"]
        self.session.ask("test", "")
        ask = next(index for index, frame in enumerate(self.notes) if "ask" in frame)
        encoded = "".join(frame["note"][2:] for frame in self.notes[:ask] if frame["note"].startswith("  "))
        self.assertEqual(json.loads(encoded), {"command": command})
        self.assertTrue(all(len(frame["note"].encode()) <= 400 for frame in self.notes if "note" in frame))

    def test_real_native_bash_and_mutation_gate(self):
        subprocess.run(["sh", str(APP / "tools/build.sh")], check=True, capture_output=True, timeout=60)
        self.session.tools.native = APP / ".build/tools"
        marker = self.root / "marker"
        command = "printf approved > marker"
        self.actions = [{"tool": "bash", "arguments": {"command": command}}, {"answer": "done"}]
        def accept():
            self.assertFalse(marker.exists())  # Execution has not started.
            return {"decision": "once"}
        self.session.receive = accept
        self.session.ask("test", "")
        self.assertEqual(marker.read_text(), "approved")
        self.actions = [{"tool": "write", "arguments": {"path": "marker", "text": "changed"}}, {"answer": "denied"}]
        self.session.receive = lambda: {"decision": "reject"}
        self.session.ask("test", "")
        self.assertEqual(marker.read_text(), "approved")
        self.actions = [{"tool": "write", "arguments": {"path": "marker", "text": "changed"}}, {"answer": "done"}]
        self.session.receive = lambda: {"decision": "once"}
        self.session.ask("test", "")
        self.assertEqual(marker.read_text(), "changed")

    def test_launcher_selects_controlled_worker(self):
        from unittest.mock import MagicMock
        child = MagicMock()
        with patch.object(host.sys.stdin, "isatty", return_value=True), patch.object(host.sys.stdout, "isatty", return_value=True), \
             patch.object(host.subprocess, "Popen", side_effect=[child, OSError("fixture missing terminal")]) as spawn:
            with self.assertRaises(OSError):
                host.launch(False, None)
        self.assertIn("--controlled", spawn.call_args_list[0].args[0])

    def test_production_worker_wire_stalls_then_executes_and_retains_denial(self):
        # Real worker, CLI protocol and native Bash; only model generation is fake.
        # No sleeps establish ordering: ask frames are the pre-execution barrier.
        subprocess.run(["sh", str(APP / "tools/build.sh")], check=True, capture_output=True, timeout=60)
        executable = self.root / "opencode"
        executable.write_text(
            '#!/usr/bin/env python3\n'
            'import json, pathlib, sys\n'
            'assert sys.argv[2:4] == ["post", "/api/experimental/generate"]\n'
            'counter=pathlib.Path("calls")\n'
            'n=int(counter.read_text()) if counter.exists() else 0\n'
            'counter.write_text(str(n+1))\n'
            'actions=[{"tool":"bash","arguments":{"command":"printf yes > approved"}},\n'
            ' {"tool":"bash","arguments":{"command":"printf no > denied"}}, {"answer":"done"}]\n'
            'print(json.dumps({"data":{"text":json.dumps(actions[n])}}))\n')
        executable.chmod(0o700)
        child = subprocess.Popen([sys.executable, str(APP / "src/session_host.py"), "--worker", "--controlled"],
                                 stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                 cwd=self.root, bufsize=0,
                                 env={**os.environ, "PATH": str(self.root) + ":" + os.environ["PATH"]})
        try:
            child.stdin.write(b'{"prompt":"test","instructions":""}\n')
            def until(key):
                for _ in range(20):
                    self.assertTrue(select.select([child.stdout], [], [], 10)[0], "worker frame deadline")
                    frame = json.loads(child.stdout.readline())
                    if key in frame:
                        return frame
                self.fail("missing worker frame")
            until("ask")
            self.assertFalse((self.root / "approved").exists())
            self.assertEqual((self.root / "calls").read_text(), "1")
            child.stdin.write(b'{"decision":"once"}\n')
            until("ask")
            self.assertEqual((self.root / "approved").read_text(), "yes")
            self.assertFalse((self.root / "denied").exists())
            child.stdin.write(b'{"decision":"reject"}\n')
            self.assertEqual(until("ok"), {"ok": True, "text": "done"})
            self.assertFalse((self.root / "denied").exists())
        finally:
            child.stdin.close()
            try:
                child.wait(timeout=10)
            except subprocess.TimeoutExpired:
                child.kill()
                child.wait(timeout=10)
            child.stdout.close()
            child.stderr.close()
        self.assertEqual(child.returncode, 0)


if __name__ == "__main__":
    unittest.main()
