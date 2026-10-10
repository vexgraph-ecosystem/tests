"""Owner: actual R4 Message/API Haven binding for all three backend selections.

Strict C23 native builds and ASan/UBSan render proof; no live providers. Explicit
source-root standalone build closure is exercised separately from workspace IDE.
"""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
sys.path.insert(0, str(APP / "src"))
from provider import Provider


class RequestTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.home = tempfile.TemporaryDirectory(prefix="primeagen-binding-")
        cls.addClassCleanup(cls.home.cleanup)
        cls.path = Path(cls.home.name)
        cls.bridge = cls.path / "request"
        cls.sanitized = cls.path / "request-sanitized"
        for output, extra in ((cls.bridge, []), (cls.sanitized, ["--sanitizer"])):
            subprocess.run([sys.executable, str(APP / "build.py"), "--output", str(output), *extra],
                           check=True, capture_output=True, timeout=120)

    def test_actual_native_envelopes_for_openai_anthropic_and_local(self):
        text = 'prompt "quoted"\nemoji 🦊 and a slash \\ and tab\t'
        for family, endpoint in [("openai", "https://api.openai.com/v1/chat/completions"),
                                 ("anthropic", "https://api.anthropic.com/v1/messages"),
                                 ("local", "http://127.0.0.1:11434/v1/chat/completions")]:
            provider = Provider(family, "test-model", bridge=self.bridge)
            url, body = provider.render(text)
            self.assertEqual(url, endpoint)
            rendered = json.loads(body)
            self.assertEqual(rendered["model"], "test-model")
            self.assertEqual(rendered["messages"], [{"role": "user", "content": text}])
            if family == "anthropic":
                self.assertGreater(rendered["max_tokens"], 0)
            self.assertNotIn("Authorization", body)

    def test_invalid_endpoints_models_missing_keys_and_binding(self):
        for family, base in [("openai", "http://example.com"), ("openai", "https://user:secret@example.com"),
                             ("openai", "https://example.com?key=secret"), ("openai", "https://example.com/#fragment"),
                             ("local", "http://192.168.1.1/v1"), ("local", "file:///tmp/model"),
                             ("anthropic", "https://api.anthropic.com/v1")]:
            with self.assertRaises(ValueError):
                Provider(family, "test", base=base)
        for model in ("", "model\n", "x" * 257):
            with self.assertRaises(ValueError):
                Provider("openai", model)
        for timeout in (0, float("nan"), float("inf")):
            with self.assertRaises(ValueError):
                Provider("openai", "test", timeout=timeout)
        with self.assertRaisesRegex(ValueError, "missing"):
            Provider("local", "test", bridge=self.path / "missing").render("hello")

    def test_owo_instructions_preserve_task_bytes_and_protocol_guidance(self):
        text = 'actual task: edit path/owo.c; keep JSON "arguments" intact'
        for bridge in (self.bridge, self.sanitized):
            for family in ("openai", "anthropic", "local"):
                _, body = Provider(family, "test", bridge=bridge, owo=True).render(text)
                content = json.loads(body)["messages"][0]["content"]
                self.assertTrue(content.endswith(text))
                self.assertIn("Complete the user's actual task competently", content)
                self.assertIn("required JSON action protocol", content)
                self.assertIn("separate tool consent", content)
        with self.assertRaises(ValueError):
            Provider("openai", "test", owo="true")

    def test_real_binding_cold_rejections_growth_and_sanitizers(self):
        for bridge in (self.bridge, self.sanitized):
            for data in (b"", b"embedded\0nul", b"x" * (1024 * 1024 + 1)):
                result = subprocess.run([str(bridge), "openai", "test", "https://example.com/v1"], input=data,
                                        capture_output=True, timeout=10, env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(b"[vex]", result.stderr)
                self.assertNotIn(b"AddressSanitizer", result.stderr)
                self.assertNotIn(b"runtime error:", result.stderr)
            result = subprocess.run([str(bridge), "anthropic", "test", "https://example.com"], input=b"x" * 65536,
                                    capture_output=True, timeout=10, env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, b"")
            body = json.loads(result.stdout.split(b"\n", 1)[1])
            self.assertEqual(len(body["messages"][0]["content"]), 65536)


if __name__ == "__main__":
    unittest.main()
