"""Registered direct fs C client, strict and sanitized; temporary storage only."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest
from support import APP, VEX

class FsOwner(unittest.TestCase):
    def test_direct_contract(self):
        for sanitized in (False, True):
            with tempfile.TemporaryDirectory() as directory:
                dest = Path(directory) / "owner"
                flags = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1", "-mcpu=apple-m1",
                         "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}"]
                if sanitized:
                    flags += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                subprocess.run([*flags, str(Path(__file__).with_suffix(".c")), str(APP / "tools/fs.c"), "-o", str(dest)],
                               check=True, capture_output=True, timeout=60)
                result = subprocess.run([str(dest), directory], input=b"inp", capture_output=True, timeout=5,
                                        env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertTrue(all(line.startswith(b"[vex]") for line in result.stderr.splitlines()))

if __name__ == "__main__":
    unittest.main()
