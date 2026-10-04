#!/usr/bin/env python3
"""Compile/validate compositor modules; does not execute a GPU pipeline."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SHADERS = ROOT / "ecosystem/drivers/graphvex/src/shaders/compositor"
TEMP_ROOT = Path(tempfile.gettempdir()) / "opencode"


class CompositorShaderTest(unittest.TestCase):
    def test_compile_and_validate_modules(self):
        compiler = shutil.which("glslangValidator")
        validator = shutil.which("spirv-val")
        self.assertIsNotNone(compiler, "glslangValidator is required by the umbrella build")
        self.assertIsNotNone(validator, "spirv-val is required for this SPIR-V validation check")
        # Honor the workspace-approved temporary directory when available;
        # other platforms can use their configured standard temporary directory.
        directory = str(TEMP_ROOT) if TEMP_ROOT.is_dir() else None
        with tempfile.TemporaryDirectory(prefix="compositor-shaders-", dir=directory) as tmp:
            for name in ("scatter.vert", "scatter.frag", "resolve.vert", "resolve.frag"):
                with self.subTest(shader=name):
                    output = Path(tmp) / (name + ".spv")
                    result = subprocess.run([compiler, "-V", str(SHADERS / name),
                                             "-o", str(output)], capture_output=True,
                                            text=True, timeout=30)
                    self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                    self.assertGreater(output.stat().st_size, 20)
                    result = subprocess.run([validator, str(output)], capture_output=True,
                                            text=True, timeout=30)
                    self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()
