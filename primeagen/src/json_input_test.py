"""Owner: duplicate/depth/encoding rejection independent of decoder version."""
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from json_input import loads, MAX_DEPTH


class JsonInputTest(unittest.TestCase):
    def test_exact_depth_and_next_rejection_then_recovery(self):
        value = loads("[" * MAX_DEPTH + "0" + "]" * MAX_DEPTH)
        self.assertIsInstance(value, list)
        for depth in (MAX_DEPTH + 1, 2000):
            with self.assertRaisesRegex(ValueError, "nesting"):
                loads("[" * depth + "0" + "]" * depth)
        self.assertEqual(loads('{"answer":"recovered"}'), {"answer": "recovered"})

    def test_duplicate_encoding_malformed_and_admission(self):
        for text in ('{"a":1,"a":2}', '{"args":{"path":"a","path":"b"}}', b"\xff", ']', '{', None):
            with self.assertRaises(ValueError):
                loads(text)
        for bound in (0, -1, True):
            with self.assertRaises(ValueError):
                loads("{}", bound)

    def test_brackets_escaped_quotes_and_utf8_inside_strings_are_not_depth(self):
        import json
        text = '[]{}"\\' * 100 + "🦊"
        self.assertEqual(loads(json.dumps({"text": text}).encode()), {"text": text})


if __name__ == "__main__":
    unittest.main()
