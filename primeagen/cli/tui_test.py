"""Registered native owners: strict C23, sanitizer runs and headless ncurses.
No interactive terminal/gallery is launched. Appearance belongs to the user.
"""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
VEX = ROOT / "ecosystem/repos/vexspoke/src"


class NativeTuiTest(unittest.TestCase):
    def test_source_and_document_contract(self):
        readme = (APP / "README.md").read_text()
        for text in ("sh cli/build.sh", "./primeagen tui --owo", "offline interface preview",
                     "not yet the native", "Mouse reporting depends", "printable ASCII"):
            self.assertIn(text, readme)
        for name in ("cli/main.c", "tui/tui.c", "tui/event/mouse_event.c", "personality/owo.c"):
            source = (APP / name).read_text()
            for marker in (";;DEFINITION", ";;OVERVIEW", "FUNCTION REGISTRY:", "============================================================================"):
                self.assertIn(marker, source)
        self.assertIn('"net/json.h"', (APP / "tui/tui.c").read_text())

    def test_native_owners_strict_and_sanitized(self):
        owners = [
            ("tui/tui_test.c", []),
            ("tui/event/mouse_event_test.c", ["tui/tui.c", "tui/event/mouse_event.c"]),
            ("personality/owo_test.c", ["personality/owo.c"]),
            ("cli/main_test.c", ["tui/tui.c", "tui/event/mouse_event.c", "personality/owo.c"]),
        ]
        with tempfile.TemporaryDirectory() as directory:
            for sanitized in (False, True):
                for owner, sources in owners:
                    with self.subTest(owner=owner, sanitized=sanitized):
                        dest = Path(directory) / "owner"
                        argv = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                                "-mcpu=apple-m1", "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}"]
                        if sanitized:
                            argv += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                        argv += [str(ROOT / "tests/primeagen" / owner)]
                        argv += [str(APP / source) for source in sources]
                        argv += [str(VEX / "net/json.c"), "-Wl,-dead_strip", "-lncurses", "-o", str(dest)]
                        subprocess.run(argv, check=True, capture_output=True, timeout=60)
                        result = subprocess.run([str(dest)], capture_output=True, timeout=20,
                                                env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                        self.assertEqual(result.returncode, 0, result.stderr.decode())
                        if owner == "tui/tui_test.c":
                            lines = result.stderr.decode().splitlines()
                            self.assertTrue(lines)
                            self.assertTrue(all(line.startswith("[vex]") for line in lines), lines)
                        else:
                            self.assertEqual(result.stderr, b"")

    def test_build_and_nonterminal_admission(self):
        subprocess.run(["sh", str(APP / "cli/build.sh")], check=True, capture_output=True, timeout=60)
        help_result = subprocess.run([str(APP / "primeagen"), "tui", "--help"], capture_output=True, timeout=10)
        self.assertEqual(help_result.returncode, 0)
        self.assertIn(b"offline", help_result.stdout)
        result = subprocess.run([str(APP / "primeagen"), "tui"], capture_output=True, timeout=10)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn(b"[vex]", result.stderr)
        self.assertEqual(result.stdout, b"")


if __name__ == "__main__":
    unittest.main()
