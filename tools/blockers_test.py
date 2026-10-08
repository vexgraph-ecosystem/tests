"""Owner proof for the root backlog, not ecosystem runtime/readiness proof."""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]


class BlockersTest(unittest.TestCase):
    def setUp(self):
        self.text = (ROOT / "blockers.md").read_text()

    def test_inventory_and_explicit_limits(self):
        for name in ("relational-engine", "vexspoke", "graphvex", "api-haven",
                     "darkbase", "language", "hotcwap", "darling-framework", "sesh",
                     "samplerate", "impedance", "semicolon", "drawling", "anti",
                     "personal/b", "personal/vex-graph", "ecosystem/.github",
                     "ecosystem/ecosystem", "ecosystem/repos/harness"):
            self.assertIn(name, self.text)
        for phrase in ("DECIDE", "BUILD", "FIX", "PROVE", "RECHECK",
                       "not a claim that you forgot", "not** a full replay",
                       "Nothing here authorizes", "No private lesson",
                       "Timestamped Test Checklist Law"):
            self.assertIn(phrase, self.text)
        register = self.text.split("## Available session register", 1)[1]
        rows = [line for line in register.splitlines()
                if line.startswith("| ") and not line.startswith(("| Session title", "| :---"))]
        self.assertEqual(len(rows), 50)
        self.assertNotIn(chr(0xA7), self.text)
        self.assertNotRegex(self.text, r"ses_[A-Za-z0-9]+")

    def test_referenced_checkout_paths_exist(self):
        paths = re.findall(r"`((?:ecosystem/|personal/|tests/)[^`]+)`", self.text)
        for required in ("ecosystem/repos/relational-engine/README.md",
                         "ecosystem/repos/vexspoke/src/net/http.c",
                         "ecosystem/repos/api-haven/src/api/rest.c",
                         "ecosystem/repos/darkbase/src/database/database.{h,c}",
                         "ecosystem/repos/darling-framework/STATUS.md",
                         "tests/sesh/run.py"):
            self.assertIn(required, paths)
        for path in paths:
            # Compact pair references are deliberately spelled foo.{h,c}.
            expanded = [path.replace("{h,c}", suffix) for suffix in ("h", "c")] if "{h,c}" in path else [path]
            for subject in expanded:
                self.assertTrue((ROOT / subject).exists(), subject)

    def test_critical_findings_match_inspected_seams(self):
        http = (ROOT / "ecosystem/repos/vexspoke/src/net/http.c").read_text()
        self.assertRegex(http, r'if \(strcasecmp\(scheme, "https"\) == 0\)\s*return false;')
        graphics = (ROOT / "ecosystem/repos/graphvex/src/graphics/graphics.c").read_text()
        self.assertIn("Text metrics/glyphs are a later slice", graphics)
        renderer = (ROOT / "ecosystem/repos/graphvex/src/vulkan/vk_renderer.c").read_text()
        self.assertIn("VkBatch_glyph(s_batch, *rect, 0u", renderer)
        database = (ROOT / "ecosystem/repos/darkbase/src/database/database.c").read_text()
        self.assertIn("FILE_MODE_WRITE | FILE_MODE_CREATE | FILE_MODE_TRUNCATE", database)
        for phrase in ("HTTPS admission", "real text", "safe save publication", "PROVE"):
            self.assertIn(phrase, self.text)


if __name__ == "__main__":
    unittest.main(verbosity=2)
