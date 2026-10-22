"""Build/run the new shared helper boundaries with assertions and ASan/UBSan."""
import os
from pathlib import Path
import subprocess
from adapter_support import AdapterCase, ROOT


class UtilTest(AdapterCase):
    def test_command_boundaries_and_memory_safety(self):
        self.require_tool("cc", "--version")
        output = self.project / "util-test"
        source = Path(__file__).with_suffix(".c")
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-UNDEBUG",
                   "-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-I", str(ROOT),
                   str(source), str(ROOT / "util.c"), "-o", str(output)]
        compiled = subprocess.run(command, capture_output=True, text=True, timeout=60)
        self.assertEqual(compiled.returncode, 0, compiled.stdout + compiled.stderr)
        result = subprocess.run([str(output)], cwd=self.project, capture_output=True,
                                text=True, timeout=30, env=dict(os.environ, ASAN_OPTIONS="detect_leaks=0"))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        diagnostics = result.stderr.splitlines()
        self.assertEqual(len(diagnostics), 11, result.stderr)
        self.assertTrue(all(line.startswith("[vex]") for line in diagnostics), result.stderr)
        self.assertFalse((self.project / "NEVER").exists())
