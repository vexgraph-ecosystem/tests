"""Owner runner: compiles and runs the C settings owner (strict then ASan/UBSan)."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "suite/personal/primeagen"
VEX = ROOT / "suite/ecosystem/vexspoke/src"
HOT = ROOT / "suite/ecosystem/hotcwap"
RE = ROOT / "suite/ecosystem/relational-engine"


class SettingsNativeTest(unittest.TestCase):
    def test_native_owner_strict_and_sanitized(self):
        with tempfile.TemporaryDirectory() as directory:
            for sanitized in (False, True):
                with self.subTest(sanitized=sanitized):
                    dest = Path(directory) / "settings_owner"
                    argv = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                            "-mcpu=apple-m1", "-mmacosx-version-min=14.0",
                            "-ffunction-sections", "-fdata-sections",
                            f"-I{APP}", f"-I{VEX}", f"-I{HOT}", f"-I{RE}/src"]
                    if sanitized:
                        argv += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                    argv += [str(ROOT / "tests/primeagen/src/settings_test.c"),
                             str(APP / "src/settings.c"),
                             str(HOT / "hot/manifest.c")]
                    argv += ["-Wl,-dead_strip", "-o", str(dest)]
                    subprocess.run(argv, check=True, capture_output=True, timeout=60)
                    result = subprocess.run([str(dest)], capture_output=True, timeout=20,
                                            env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                    self.assertEqual(result.returncode, 0, result.stderr.decode())
                    self.assertIn(b"all checks passed", result.stdout)


if __name__ == "__main__":
    unittest.main()
