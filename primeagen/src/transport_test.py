"""Owner: isolated host transport. Offline loopback success/status/redirect/cap.

No real credentials/provider contact. Cleartext fixture proves framing and refusal
only; verified TLS defaults are inspected, not a live TLS handshake proof.
"""
import json
import os
from pathlib import Path
import subprocess
import sys
import threading
import unittest
from http.server import BaseHTTPRequestHandler, HTTPServer

ROOT = Path(__file__).resolve().parents[3]
SCRIPT = ROOT / "personal/primeagen/src/transport.py"


class TransportTest(unittest.TestCase):
    def setUp(self):
        self.requests = []
        owner = self
        class Handler(BaseHTTPRequestHandler):
            def do_POST(self):
                body = self.rfile.read(int(self.headers.get("Content-Length", 0)))
                owner.requests.append((self.path, dict(self.headers), body))
                if self.path == "/redirect":
                    self.send_response(307)
                    self.send_header("Location", "/must-not-follow")
                    self.end_headers()
                elif self.path == "/bad":
                    self.send_response(429)
                    self.end_headers()
                else:
                    self.send_response(200)
                    self.end_headers()
                    if self.path == "/large":
                        self.wfile.write(b"x" * (1024 * 1024 + 1))
                    else:
                        self.wfile.write(b'{"choices":[{"message":{"content":"{\\"answer\\":\\"hello\\"}"}}]}')
            def log_message(self, *args):
                pass
        self.server = HTTPServer(("127.0.0.1", 0), Handler)
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()
        self.addCleanup(self.close)

    def close(self):
        self.server.shutdown()
        self.server.server_close()
        self.thread.join(timeout=5)
        self.assertFalse(self.thread.is_alive())

    def request(self, path, family="local"):
        payload = json.dumps({"family": family, "url": f"http://127.0.0.1:{self.server.server_port}{path}",
                              "body": '{"model":"test"}', "timeout": 2}).encode()
        return subprocess.run([sys.executable, str(SCRIPT)], input=payload, capture_output=True, timeout=5,
                              env={**os.environ, "OPENAI_API_KEY": "fake-test-secret", "ANTHROPIC_API_KEY": "fake-anthropic"})

    def test_success_no_local_credential_and_distinct_host_headers(self):
        for family in ("local", "openai", "anthropic"):
            result = self.request("/success", family)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, b"")
            json.loads(result.stdout)
            _, headers, body = self.requests[-1]
            self.assertEqual(json.loads(body), {"model": "test"})
            if family == "local":
                self.assertNotIn("Authorization", headers)
            elif family == "openai":
                self.assertEqual(headers["Authorization"], "Bearer fake-test-secret")
            else:
                lower = {key.lower(): value for key, value in headers.items()}
                self.assertEqual(lower["x-api-key"], "fake-anthropic")
                self.assertEqual(lower["anthropic-version"], "2023-06-01")

    def test_redirect_status_and_saturation_fail_without_secret_logs(self):
        for path in ("/redirect", "/bad", "/large"):
            result = self.request(path, "openai")
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(b"[vex]", result.stderr)
            self.assertNotIn(b"fake-test-secret", result.stderr)
            self.assertNotIn(b"fake-anthropic", result.stderr)
            self.assertEqual(result.stdout, b"")
        self.assertNotIn("/must-not-follow", [request[0] for request in self.requests])
        self.assertEqual(self.request("/success").returncode, 0)


if __name__ == "__main__":
    unittest.main()
