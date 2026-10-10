"""Owner: model response extraction/key gates; deterministic injected transport.

Native envelope execution is separately owned by native/request_test. Transport
fixtures are not live hosted/local model competence or TLS handshake evidence.
"""
import json
import os
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from provider import Provider


class ProviderTest(unittest.TestCase):
    def test_response_shapes_and_environment_credential_gate(self):
        for family, response in [("openai", {"choices": [{"message": {"content": '{"answer":"hello"}'}}]}),
                                 ("anthropic", {"content": [{"type": "text", "text": '{"answer":'}, {"type": "text", "text": '"hello"}'}]}),
                                 ("local", {"choices": [{"message": {"content": '{"answer":"hello"}'}}]})]:
            provider = Provider(family, "test")
            provider.render = lambda text: ("https://example.com", '{"model":"test"}')
            with patch.dict(os.environ, {"OPENAI_API_KEY": "fake-test", "ANTHROPIC_API_KEY": "fake-test"}), \
                 patch("provider.run", return_value=json.dumps(response).encode()) as child:
                self.assertEqual(provider.complete("hello"), '{"answer":"hello"}')
                payload = json.loads(child.call_args.kwargs["data"])
                self.assertNotIn("fake-test", json.dumps(payload))
                self.assertEqual(payload["family"], family)
        for family, key in (("openai", "OPENAI_API_KEY"), ("anthropic", "ANTHROPIC_API_KEY")):
            with patch.dict(os.environ, {key: ""}), patch("provider.run") as child:
                with self.assertRaisesRegex(ValueError, "missing"):
                    Provider(family, "test").complete("hello")
                child.assert_not_called()

    def test_bad_responses_are_rejected_then_recovery(self):
        provider = Provider("local", "test")
        provider.render = lambda text: ("http://127.0.0.1/v1/chat/completions", "{}")
        for response in (b"invalid", b"[" * 2000 + b"0" + b"]" * 2000, b"{}", b'{"choices":[]}', b'{"choices":[{"message":{"content":null}}]}',
                         b'{"choices":[{"message":{"content":""}}]}'):
            with patch("provider.run", return_value=response):
                with self.assertRaises((ValueError, KeyError, IndexError)):
                    provider.complete("hello")
        with patch("provider.run", return_value=b'{"choices":[{"message":{"content":"valid"}}]}'):
            self.assertEqual(provider.complete("hello"), "valid")
        provider = Provider("anthropic", "test")
        provider.render = lambda text: ("https://example.com/v1/messages", "{}")
        with patch.dict(os.environ, {"ANTHROPIC_API_KEY": "fake"}), \
             patch("provider.run", return_value=b'{"content":[{"type":"tool_use","text":"not-text"}]}'):
            with self.assertRaisesRegex(ValueError, "unsupported"):
                provider.complete("hello")


if __name__ == "__main__":
    unittest.main()
