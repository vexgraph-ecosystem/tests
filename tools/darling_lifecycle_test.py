#!/usr/bin/env python3
"""Starter migration and documented API compilation; no runtime/visual proof."""
from pathlib import Path
import re
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]
DARLING = ROOT / "ecosystem/interface/darling-framework"


def code(text):
    return re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)


class DarlingLifecycleDocumentation(unittest.TestCase):
    def test_every_c_starter_uses_application(self):
        starters = []
        for source in (ROOT / "tests/darling").rglob("*.c"):
            text = code(source.read_text())
            if not re.search(r"\bint\s+main\s*\(", text):
                continue
            starters.append(source)
            with self.subTest(source=source.relative_to(ROOT)):
                self.assertTrue('"darling/test_application.h"' in text or
                                '"frame_chrome_lab.h"' in text or
                                "Application_start(" in text)
                self.assertNotIn("Window_pollEvents(", text)
                self.assertNotIn("Window_waitEvents(", text)
                self.assertNotIn("Frame_run(", text)
        self.assertGreaterEqual(len(starters), 35)

    def test_documented_starter_compiles(self):
        documentation = (DARLING / "APPLICATION_LIFECYCLE.md").read_text()
        example = re.search(r"```c\n(.*?)\n```", documentation, re.S)
        self.assertIsNotNone(example)
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-fsyntax-only", "-x", "c", "-"]
        for include in (DARLING / "src", ROOT / "ecosystem/hotcwap",
                        ROOT / "ecosystem/vexspoke/src", ROOT / "ecosystem/drivers/graphvex/src"):
            command += ["-I", str(include)]
        result = subprocess.run(command, input='#include "frame/frame.h"\n' + example.group(1),
                                text=True, capture_output=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_documentation_links_and_intentions(self):
        for path in (DARLING / "STATUS.md", DARLING / "APPLICATION_LIFECYCLE.md",
                     ROOT / "tests/darling/frame/README.md"):
            text = path.read_text()
            for target in re.findall(r"\]\(([^)]+)\)", text):
                self.assertTrue((path.parent / target).is_file(), target)
        lifecycle = (DARLING / "APPLICATION_LIFECYCLE.md").read_text()
        self.assertEqual(lifecycle.count(";;INTENTION("), 3)
        self.assertIn("Implemented contract", lifecycle)
        self.assertIn("Automatic display-rate", lifecycle)
        preferences = (ROOT / "ecosystem/vexspoke/preferences.md").read_text()
        self.assertIn("`Application_start` is the", preferences)
        self.assertIn("`Application_close/stop` request all-window", preferences)
        readme = (ROOT / "tests/darling/frame/README.md").read_text()
        for name in re.findall(r"\./tools/b (?:run|test) (\w+)", readme):
            self.assertTrue(list((ROOT / "tests/darling").rglob(name + ".c")), name)


if __name__ == "__main__":
    unittest.main()
