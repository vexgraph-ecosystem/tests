"""OpenCode V2 host owner: scripted API, fake installed CLI and native pipe
owners only. Never calls the real OpenCode server/provider or opens a gallery.
The default policy, the bounded approval round-trip and its refusals are proven
here; the durable saved rule and remote enforcement stay OpenCode's.
"""
from pathlib import Path
import json
import os
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
sys.path.insert(0, str(APP / "src"))
import session_host as host


class SessionHostTest(unittest.TestCase):
    def scripted(self, messages, permissions=None):
        """Scripted API: one session, one prompt boundary, caller-supplied replies."""
        self.calls = []
        self.message_id = None
        policy = host.SESSION_PERMISSIONS if permissions is None else permissions
        def request(method, path, body=None, timeout=10, empty=False):
            self.calls.append((method, path, body))
            if path == "/api/session":
                return {"data": {"id": "ses_fake", "permissions": policy}}
            if path == "/api/session/ses_fake":
                return {"data": {"permissions": policy}}
            if path == "/api/session/ses_fake/permission":
                return self.pending if self.pending is not None else {"data": []}
            if path.endswith("/reply"):
                self.replies.append(body["decision"])
                return {}
            if path.endswith("/interrupt"):
                return {}
            if path.endswith("/prompt"):
                self.message_id = body["id"]
                return {"data": {"id": self.message_id}}
            if "/message?" in path:
                return messages() if callable(messages) else messages
            raise AssertionError(path)
        return request

    def setUp(self):
        self.pending = None
        self.replies = []

    def test_default_policy_two_step_turn_and_session_reuse(self):
        def messages():
            return {"data": [
                {"id": "msg_idle", "type": "idle", "outcome": "succeeded"},
                {"id": "msg_answer", "type": "assistant", "content": [{"type": "text", "text": "hello"}]},
                {"id": self.message_id, "type": "user"},
                {"id": "msg_old_idle", "type": "idle", "outcome": "failed"}]}
        self.api = self.scripted(messages)
        with patch.object(host, "api", self.api):
            session = host.create("opencode-go/example")
            self.assertEqual(session, "ses_fake")
            created = self.calls[0][2]
            self.assertEqual(created["permissions"], host.SESSION_PERMISSIONS)
            self.assertEqual(created["model"], {"providerID": "opencode-go", "id": "example"})
            self.assertTrue(host.inspect(session))
            turn = host.begin(session, "prompt", "OwO guidance\n")
            posted = next(body for method, path, body in self.calls if path.endswith("/prompt"))
            self.assertEqual(posted["text"], "OwO guidance\nprompt")
            self.assertEqual(host.advance(session, turn), ("done", "hello"))
            again = host.begin(session, "second", "")
            self.assertEqual(host.advance(session, again), ("done", "hello"))
            self.assertEqual(sum(path == "/api/session" for _, path, _ in self.calls), 1)
            self.assertEqual(host.SESSION_PERMISSIONS[0], {"action": "*", "resource": "*", "effect": "deny"})
            effects = {(rule["action"], rule["effect"]) for rule in host.SESSION_PERMISSIONS}
            self.assertIn(("read", "allow"), effects)
            self.assertIn(("edit", "ask"), effects)
            self.assertIn(("shell", "ask"), effects)

    def test_policy_evaluation_and_reported_policy_refusals(self):
        self.assertTrue(host.confirmed(host.SESSION_PERMISSIONS))
        for bad in ([], "no", None, [{"action": "*", "resource": "*"}],
                    [{"action": "*", "resource": "*", "effect": "maybe"}],
                    [{"action": "", "resource": "*", "effect": "deny"}],
                    ["not-a-rule"]):
            self.assertFalse(host.confirmed(bad))
        self.assertFalse(host.confirmed(host.SESSION_PERMISSIONS[1:]))  # dropped catch-all deny
        self.assertFalse(host.confirmed(host.SESSION_PERMISSIONS
                                        + [{"action": "shell", "resource": "*", "effect": "allow"}]))
        self.assertTrue(host.matches("*", "anything/at all"))
        self.assertTrue(host.matches("git*", "git push"))
        self.assertFalse(host.matches("git*", "hg push"))
        self.assertFalse(host.matches("a?c", "ac"))
        self.assertEqual(host.effective(host.SESSION_PERMISSIONS, "shell", "rm -rf /"), "ask")
        self.assertEqual(host.effective(host.SESSION_PERMISSIONS, "read", "notes.md"), "allow")
        self.assertEqual(host.effective(host.SESSION_PERMISSIONS, "definitely_not_a_tool"), "deny")
        self.assertEqual(host.effective([], "read"), "ask")  # OpenCode's no-match default

    def test_approval_request_is_paused_bounded_and_decided_once(self):
        self.pending = {"data": [{"id": "per_1", "sessionID": "ses_fake", "action": "shell",
                                  "resources": ["git push \x1b[31mnow"]}]}
        self.api = self.scripted({"data": []})
        with patch.object(host, "api", self.api):
            turn = host.begin("ses_fake", "push", "")
            state, detail = host.advance("ses_fake", turn)
            self.assertEqual(state, "ask")
            self.assertEqual(detail["id"], "per_1")
            self.assertEqual(host.label(detail), "shell: git push  [31mnow")
            self.assertNotIn("\x1b", host.label(detail))
            self.assertEqual(host.advance("ses_fake", turn), ("waiting", None))
            self.assertEqual(self.replies, [])
            with self.assertRaises(ValueError):
                host.decide("ses_fake", detail, "later")
            host.decide("ses_fake", detail, "always")
            self.assertEqual(self.replies, ["always"])
            host.decide("ses_fake", detail, "reject")
            self.assertEqual(self.replies, ["always", "reject"])
            self.pending = None
            turn["awaiting"] = None  # the worker clears this after forwarding the decision
            self.assertEqual(host.advance("ses_fake", turn), ("running", None))

    def test_request_and_advance_reject_invalid_shapes(self):
        for identity in ("per/../x", "ses_1", "per", None, 5, "per " + "x" * 200):
            with self.assertRaises(ValueError):
                host.checked_request(identity)
        for identity in ("ses/../x", "ses?x", "ses", None, 5):
            with self.assertRaises(ValueError):
                host.checked_id(identity)
        for request in ({"data": "no"}, {"data": [1]}, {"data": [{"id": "per_1", "action": "", "resources": ["x"]}]},
                        {"data": [{"id": "per_1", "action": "shell", "resources": []}]},
                        {"data": [{"id": "per_1", "action": "shell", "resources": [5]}]}):
            with patch.object(host, "api", return_value=request):
                with self.assertRaises(ValueError):
                    host.request("ses_fake")
        for content in ([{"type": "tool", "name": ""}], [{"type": "text", "text": 5}], "no"):
            with patch.object(host, "api", return_value={"data": [
                    {"type": "idle", "outcome": "succeeded"}, {"type": "assistant", "content": content},
                    {"id": "msg_a"}]}):
                turn = {"messageId": "msg_a", "deadline": float("inf"), "awaiting": None,
                        "awaitingSince": 0.0, "nextCheck": float("inf")}
                with self.assertRaises(ValueError):
                    host.advance("ses_fake", turn)

    def test_tool_activity_is_reported_not_executed_and_text_only_survives(self):
        self.api = self.scripted({"data": [
            {"type": "idle", "outcome": "succeeded"},
            {"type": "assistant", "content": [{"type": "text", "text": "done"},
                                              {"type": "tool", "name": "read"}]},
            {"id": "msg_a"}]})
        with patch.object(host, "api", self.api):
            turn = {"messageId": "msg_a", "deadline": float("inf"), "awaiting": None,
                    "awaitingSince": 0.0, "nextCheck": float("inf")}
            self.assertEqual(host.advance("ses_fake", turn), ("done", "done\nused read"))
        for content, expected in (([{"type": "tool", "name": "shell"}], "used shell"),
                                  ([], "(no text returned)")):
            self.api = self.scripted({"data": [{"type": "idle", "outcome": "succeeded"},
                                               {"type": "assistant", "content": content}, {"id": "msg_a"}]})
            with patch.object(host, "api", self.api):
                turn = {"messageId": "msg_a", "deadline": float("inf"), "awaiting": None,
                        "awaitingSince": 0.0, "nextCheck": float("inf")}
                self.assertEqual(host.advance("ses_fake", turn), ("done", expected))
        self.api = self.scripted({"data": [
            {"type": "idle", "outcome": "succeeded"},
            {"type": "assistant", "content": [{"type": "text", "text": "x" * host.WIRE_CAP}]},
            {"id": "msg_a"}]})
        with patch.object(host, "api", self.api):
            turn = {"messageId": "msg_a", "deadline": float("inf"), "awaiting": None,
                    "awaitingSince": 0.0, "nextCheck": float("inf")}
            with self.assertRaises(ValueError):
                host.advance("ses_fake", turn)

    def test_policy_drift_missing_boundary_page_limit_and_deadline(self):
        with patch.object(host, "api", return_value={"data": {"permissions": []}}):
            with self.assertRaises(ValueError):
                host.inspect("ses_fake")
        self.pending = None
        self.api = self.scripted({"data": [{"id": "old", "type": "idle", "outcome": "succeeded"}]})
        clock = {"now": 0.0}
        with patch.object(host, "api", self.api), patch.object(host.time, "monotonic", lambda: clock["now"]):
            turn = host.begin("ses_fake", "test", "")
            clock["now"] = 121
            with self.assertRaises(ValueError):
                host.advance("ses_fake", turn)
        self.api = self.scripted({"data": [{"id": f"msg_{i}"} for i in range(host.PAGE_LIMIT)]})
        with patch.object(host, "api", self.api), patch.object(host.time, "monotonic", lambda: 0.0):
            turn = host.begin("ses_fake", "test", "")
            with self.assertRaises(ValueError):
                host.advance("ses_fake", turn)

    def test_invalid_session_creation_and_model_admission(self):
        for model in ("nope", "opencode-go/", "/example", "has space/x"):
            with self.assertRaises(ValueError), patch.object(host, "api") as api:
                host.create(model)
            api.assert_not_called()
        with patch.object(host, "api", return_value={"data": {"id": "ses_bad", "permissions": []}}):
            with self.assertRaises(ValueError):
                host.create(None)
        with patch.object(host, "api", return_value={"data": {"id": "ses/../x", "permissions": host.SESSION_PERMISSIONS}}):
            with self.assertRaises(ValueError):
                host.create(None)

    def test_cli_literal_protocol_admission_no_secret_diagnostics(self):
        with patch.object(host, "run", return_value=b'{"data":[]}') as child:
            self.assertEqual(host.api("post", "/api/session", {"text": "$(id)"}), {"data": []})
            argv = child.call_args.args[0]
            self.assertEqual(argv[:4], ["opencode", "api", "post", "/api/session"])
            self.assertEqual(json.loads(argv[-1])["text"], "$(id)")
        with patch.object(host, "run", return_value=b""):
            self.assertEqual(host.api("post", "/api/session/x/permission/per_1/reply", {"decision": "once"}, empty=True), {})
        for bad in (b'[]', b'{"data":[],"data":[]}', b'\xff', b'[' * 33):
            with patch.object(host, "run", return_value=bad), self.assertRaises(ValueError):
                host.api("get", "/api/info")
        result = subprocess.run([str(APP / "primeagen"), "session"], capture_output=True, timeout=5)
        self.assertNotEqual(result.returncode, 0)
        self.assertNotIn(b"Traceback", result.stderr)

    def test_fake_cli_worker_approval_round_trip_and_malformed_wire(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "opencode"
            permissions = json.dumps(host.SESSION_PERMISSIONS)
            executable.write_text(
                '#!/usr/bin/env python3\n'
                'import json, os, sys\n'
                'method, path = sys.argv[2], sys.argv[3]\n'
                f'permissions = json.loads({permissions!r})\n'
                'if path == "/api/session":\n'
                '    print(json.dumps({"data": {"id": "ses_fixture", "permissions": permissions}}))\n'
                'elif path == "/api/session/ses_fixture":\n'
                '    print(json.dumps({"data": {"permissions": permissions}}))\n'
                'elif path.endswith("/prompt"):\n'
                '    open("boundary", "w").write(json.loads(sys.argv[-1])["id"])\n'
                '    print(json.dumps({"data": {}}))\n'
                'elif path.endswith("/permission"):\n'
                '    rows = [] if os.path.exists("approved") else [{"id": "per_1", "sessionID": "ses_fixture",\n'
                '        "action": "shell", "resources": ["rm -rf build"]}]\n'
                '    print(json.dumps({"data": rows}))\n'
                'elif path.endswith("/reply"):\n'
                '    open("approved", "w").write(json.loads(sys.argv[-1])["decision"])\n'
                'elif "/message?" in path:\n'
                '    if os.path.exists("approved"):\n'
                '        print(json.dumps({"data": [{"type": "idle", "outcome": "succeeded"},\n'
                '            {"type": "assistant", "content": [{"type": "text", "text": "offline reply \\U0001f44b caf\\u00e9 \\u4e2d\\u6587"}]},\n'
                '            {"id": open("boundary").read()}]}))\n'
                '    else:\n'
                '        print(json.dumps({"data": []}))\n'
                'else:\n'
                '    print(json.dumps({"data": {}}))\n')
            executable.chmod(0o700)
            environment = {**os.environ, "PATH": directory + ":" + os.environ["PATH"]}
            child = subprocess.Popen([sys.executable, str(APP / "src/session_host.py"), "--worker"],
                                     stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                     stderr=subprocess.DEVNULL, cwd=directory, env=environment)
            try:
                child.stdin.write(b'{"prompt":"test","instructions":""}\n')
                child.stdin.flush()
                ask = json.loads(child.stdout.readline())
                self.assertEqual(ask, {"ask": {"id": "per_1", "label": "shell: rm -rf build"}})
                child.stdin.write(b'{"decision":"once"}\n')
                child.stdin.flush()
                answer = json.loads(child.stdout.readline())
                self.assertEqual(answer, {"ok": True, "text": "offline reply 👋 café 中文"})
                self.assertIn("👋 café 中文".encode("utf-8"), json.dumps(answer, ensure_ascii=False).encode("utf-8"))
                self.assertEqual(Path(directory, "approved").read_text(), "once")
            finally:
                child.stdin.close()
                child.wait(timeout=10)
                child.stdout.close()
            self.assertEqual(child.returncode, 0)
            for wire in (b'{"prompt":"secret","prompt":"duplicate"}\n', b'x' * 16384, b'[]\n',
                         b'{"decision":"once"}\n', b'{"prompt":"a","instructions":"b","extra":1}\n'):
                result = subprocess.run([sys.executable, str(APP / "src/session_host.py"), "--worker"], input=wire,
                                        cwd=directory, env={**os.environ, "PATH": directory}, capture_output=True, timeout=10)
                self.assertNotEqual(result.returncode, 0)
                self.assertEqual(json.loads(result.stdout)["ok"], False)
                self.assertNotIn(b"secret", result.stdout + result.stderr)

    def test_native_real_haven_owner_strict_sanitized(self):
        with tempfile.TemporaryDirectory() as directory:
            for sanitized in (False, True):
                output = Path(directory) / "owner"
                vex = ROOT / "ecosystem/repos/vexspoke/src"
                api = ROOT / "ecosystem/repos/api-haven/src"
                argv = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                        "-mcpu=apple-m1", "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{vex}", f"-I{api}"]
                if sanitized:
                    argv += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                argv += [str(ROOT / "tests/primeagen/session/session_bridge_test.c"), str(APP / "session/session_bridge.c"),
                         str(APP / "personality/owo.c"), str(vex / "net/json.c"), str(api / "harness/harness.c"),
                         str(api / "harness/engine_provider.c"), "-o", str(output)]
                subprocess.run(argv, check=True, capture_output=True, timeout=60)
                result = subprocess.run([str(output)], capture_output=True, timeout=10,
                                        env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertTrue(all(line.startswith(b"[vex]") for line in result.stderr.splitlines()))

    def test_launcher_failure_reclaims_worker_and_pipes_without_model_contact(self):
        # Launcher fault injection: worker started, native executable unavailable.
        # The fake child checks terminate/reap; real disposable pipes check closure.
        from unittest.mock import MagicMock
        child = MagicMock()
        opened = []
        real_pipe = os.pipe
        def pipe():
            fds = real_pipe()
            opened.extend(fds)
            return fds
        with patch.object(host.sys.stdin, "isatty", return_value=True), \
             patch.object(host.sys.stdout, "isatty", return_value=True), \
             patch.object(host.os, "pipe", side_effect=pipe), \
             patch.object(host.subprocess, "Popen", side_effect=[child, OSError("unavailable")]):
            with self.assertRaises(OSError):
                host.launch(True, "opencode-go/example")
        child.terminate.assert_called_once()
        child.wait.assert_called_once_with(timeout=3)
        for fd in opened:
            with self.assertRaises(OSError):
                os.fstat(fd)


if __name__ == "__main__":
    unittest.main()
