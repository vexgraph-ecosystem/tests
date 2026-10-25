"""Registered native owners: strict C23, sanitizer runs and headless ncurses.
No interactive terminal/gallery is launched. Appearance belongs to the user.
"""
from pathlib import Path
import os
import json
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "suite/personal/primeagen"
VEX = ROOT / "suite/ecosystem/vexspoke/src"
API = ROOT / "suite/ecosystem/api-haven/src"
HOT = ROOT / "suite/ecosystem/hotcwap"
DET = ROOT / "suite/ecosystem/determinant/src"
REL = ROOT / "suite/ecosystem/relational-engine/src"
# The R3 terminal toolkit the view now draws through (canvas → frame → appearance).
DET_SOURCES = ["ui/surface/terminal_canvas.c", "terminal/terminal_frame.c",
               "ui/image/terminal_image.c", "terminal/terminal_appearance.c",
               "terminal/terminal_tree_view.c"]


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
            ("tui/markdown_test.c", []),
            ("tui/utf8_test.c", []),
            ("tui/event/mouse_event_test.c", ["tui/tui.c", "tui/event/mouse_event.c"]),
            ("personality/owo_test.c", ["personality/owo.c"]),
            ("cli/main_test.c", ["tui/tui.c", "tui/event/mouse_event.c", "personality/owo.c", "session/session_bridge.c", "cli/view.c", "cli/update.c"]),
            ("cli/view_test.c", ["tui/tui.c", "cli/view.c"]),
            ("cli/appearance_test.c", ["cli/appearance.c"]),
            ("cli/screen_test.c", ["tui/tui.c", "cli/screen.c", "cli/appearance.c"]),
        ]
        with tempfile.TemporaryDirectory() as directory:
            for sanitized in (False, True):
                for owner, sources in owners:
                    with self.subTest(owner=owner, sanitized=sanitized):
                        dest = Path(directory) / "owner"
                        argv = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                                "-mcpu=apple-m1", "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}", f"-I{API}", f"-I{HOT}", f"-I{DET}", f"-I{REL}"]
                        if sanitized:
                            argv += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                        argv += [str(ROOT / "tests/primeagen" / owner)]
                        argv += [str(APP / source) for source in sources]
                        if "cli/view.c" in sources or "cli/appearance.c" in sources:
                            if "cli/appearance.c" not in sources:
                                argv += [str(APP / "cli/appearance.c")]
                            argv += [str(DET / source) for source in DET_SOURCES]
                        argv += [str(APP / "tui/markdown.c")]
                        argv += [str(REL / "nio/mem.c"), str(REL / "type/type.c"), str(VEX / "atomic/spin.c")]
                        argv += [str(API / "harness/harness.c"), str(API / "harness/engine_provider.c")]
                        argv += [str(VEX / "net/json.c"), "-Wl,-dead_strip", "-lncurses", "-o", str(dest)]
                        if owner == "cli/main_test.c":
                            argv += [str(HOT / "hot" / name) for name in ("hot.c", "hot_trampoline.c", "hot_retire.c", "manifest.c", "ledger.c")]
                            argv += [str(VEX / "security/crypto.c"), "-pthread"]
                        subprocess.run(argv, check=True, capture_output=True, timeout=60)
                        result = subprocess.run([str(dest)], capture_output=True, timeout=20,
                                                env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                        self.assertEqual(result.returncode, 0, result.stderr.decode())
                        if owner in ("tui/tui_test.c", "tui/markdown_test.c"):
                            lines = result.stderr.decode().splitlines()
                            self.assertTrue(lines)
                            self.assertTrue(all(line.startswith("[vex]") for line in lines), lines)
                        else:
                            self.assertEqual(result.stderr, b"")
                        if owner == "cli/main_test.c":
                            fixture = Path(directory) / "fake host with spaces.py"
                            fixture.write_text("import json,sys\nprint(json.dumps(sys.argv[1:]))\n")
                            handoff = subprocess.run([str(dest), "--connect-probe", str(fixture)],
                                                     capture_output=True, timeout=10)
                            self.assertEqual(handoff.returncode, 0, handoff.stderr)
                            self.assertEqual(json.loads(handoff.stdout),
                                             ["--engine", "opencode", "--model",
                                              "opencode-go/deepseek-v4.1-flash", "--owo"])
                            rejected = subprocess.run([str(dest), "--connect-probe", ""],
                                                      capture_output=True, timeout=10)
                            self.assertNotEqual(rejected.returncode, 0)
                            self.assertIn(b"connect requires", rejected.stderr)

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
