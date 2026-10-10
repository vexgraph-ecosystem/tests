"""OpenCode V2 host owner: scripted API, fake installed CLI and native pipe
owners only. Never calls the real OpenCode server/provider or opens a gallery.
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
    def response_api(self, errors=None):
        self.calls = []
        self.message_id = None
        def request(method, path, body=None, timeout=10):
            self.calls.append((method, path, body))
            if path.endswith("/interrupt"):
                return {}
            if path == "/api/session":
                return {"data": {"id": "ses_fake", "permissions": host.DENY_ALL}}
            if path == "/api/session/ses_fake":
                return {"data": {"permissions": host.DENY_ALL}}
            if path.endswith("/prompt"):
                self.message_id = body["id"]
                return {"data": {"id": self.message_id}}
            if "/message?" in path:
                if errors is not None:
                    return errors
                return {"data": [{"id": "msg_idle", "type": "idle", "outcome": "succeeded"},
                                 {"id": "msg_answer", "type": "assistant", "content": [{"type": "text", "text": "hello"}]},
                                 {"id": self.message_id, "type": "user"},
                                 {"id": "msg_old_idle", "type": "idle", "outcome": "failed"}]}
            raise AssertionError(path)
        return request

    def test_isolated_denied_session_model_guidance_and_retained_second_turn(self):
        with patch.object(host, "api", self.response_api()):
            sid, text = host.turn(None, "prompt", "OwO guidance\n", "opencode-go/example")
            self.assertEqual((sid, text), ("ses_fake", "hello"))
            created = self.calls[0][2]
            self.assertEqual(created["permissions"], host.DENY_ALL)
            self.assertEqual(created["model"], {"providerID": "opencode-go", "id": "example"})
            posted = next(body for method, path, body in self.calls if path.endswith("/prompt"))
            self.assertEqual(posted["text"], "OwO guidance\nprompt")
            host.turn(sid, "second", "", None)
            self.assertEqual(sum(path == "/api/session" for _, path, _ in self.calls), 1)
            self.assertFalse(any(path.endswith("/interrupt") for _, path, _ in self.calls))

    def test_invalid_identity_input_permissions_tool_and_error_rejections(self):
        for identity in ("ses/../x", "ses?x", "ses", None, 5):
            with self.assertRaises(ValueError):
                host.checked_id(identity)
        for prompt in ("", None, "x" * 8193):
            with self.assertRaises(ValueError), patch.object(host, "api") as api:
                host.turn(None, prompt, "", None)
            api.assert_not_called()
        with patch.object(host, "api", return_value={"data": {"id": "ses_bad", "permissions": []}}):
            with self.assertRaises(ValueError):
                host.turn(None, "test", "", None)
        for content in ([{"type": "tool", "text": "secret"}], [{"type": "text", "text": "x" * host.WIRE_CAP}]):
            def api(method, path, body=None, timeout=10):
                if "/message?" in path:
                    return {"data": [{"type": "idle", "outcome": "succeeded"},
                                     {"type": "assistant", "content": content}, {"id": self.message_id}]}
                return delegate(method, path, body, timeout)
            delegate = self.response_api()
            with patch.object(host, "api", api):
                with self.assertRaises(ValueError):
                    host.turn(None, "test", "", None)
            self.assertTrue(any(path.endswith("/interrupt") for _, path, _ in self.calls))

    def test_old_idle_missing_boundary_timeout_and_interrupt(self):
        with patch.object(host, "api", self.response_api({"data": [{"id": "old", "type": "idle", "outcome": "succeeded"}]})), \
             patch.object(host.time, "monotonic", side_effect=[0, 1, 2, 121]), patch.object(host.time, "sleep"):
            with self.assertRaises(ValueError):
                host.turn(None, "test", "", None)
        self.assertTrue(any(path.endswith("/interrupt") for _, path, _ in self.calls))

    def test_cli_literal_protocol_admission_no_secret_diagnostics(self):
        with patch.object(host, "run", return_value=b'{"data":[]}') as child:
            self.assertEqual(host.api("post", "/api/session", {"text": "$(id)"}), {"data": []})
            argv = child.call_args.args[0]
            self.assertEqual(argv[:4], ["opencode", "api", "post", "/api/session"])
            self.assertEqual(json.loads(argv[-1])["text"], "$(id)")
        for bad in (b'[]', b'{"data":[],"data":[]}', b'\xff', b'[' * 33):
            with patch.object(host, "run", return_value=bad), self.assertRaises(ValueError):
                host.api("get", "/api/info")
        result = subprocess.run([str(APP / "primeagen"), "session"], capture_output=True, timeout=5)
        self.assertNotEqual(result.returncode, 0)
        self.assertNotIn(b"Traceback", result.stderr)

    def test_fake_cli_worker_and_malformed_wire_no_real_server(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "opencode"
            executable.write_text('#!/usr/bin/env python3\nimport sys,json\npath=sys.argv[3]\n'
                                  'if path=="/api/session": result={"data":{"id":"ses_fixture","permissions":[{"action":"*","resource":"*","effect":"deny"}]}}\n'
                                  'elif path=="/api/session/ses_fixture": result={"data":{"permissions":[{"action":"*","resource":"*","effect":"deny"}]}}\n'
                                  'elif path.endswith("/prompt"):\n body=json.loads(sys.argv[-1]);open("boundary","w").write(body["id"]);result={"data":{}}\n'
                                  'else: result={"data":[{"type":"idle","outcome":"succeeded"},{"type":"assistant","content":[{"type":"text","text":"offline reply"}]},{"id":open("boundary").read()}]}\n'
                                  'print(json.dumps(result))\n')
            executable.chmod(0o700)
            result = subprocess.run([sys.executable, str(APP / "src/session_host.py"), "--worker"],
                                    input=b'{"prompt":"test","instructions":""}\n', cwd=directory,
                                    env={**os.environ, "PATH": directory + ":" + os.environ["PATH"]},
                                    capture_output=True, timeout=10)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(json.loads(result.stdout), {"ok": True, "text": "offline reply"})
            for wire in (b'{"prompt":"secret","prompt":"duplicate"}\n', b'x' * 16384, b'[]\n'):
                result = subprocess.run([sys.executable, str(APP / "src/session_host.py"), "--worker"], input=wire,
                                        cwd=directory, env={**os.environ, "PATH": directory}, capture_output=True, timeout=5)
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
