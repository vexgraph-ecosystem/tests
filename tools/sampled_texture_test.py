#!/usr/bin/env python3
"""Optimized ASan/UBSan host proof with real Vulkan pixels, no gallery launch."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
STATE = Path(os.environ.get("B_HOME", Path.home() / "Library/Application Support/vexgraph/b"))


class SampledTextureTest(unittest.TestCase):
    def test_sanitized_owner_and_scatter_sampling(self):
        with tempfile.TemporaryDirectory(prefix="sampled-texture-", dir=os.environ.get("TMPDIR")) as directory:
            base = Path(directory)
            shader = base / "out/debug/shader/compositor"
            shader.mkdir(parents=True)
            graph = ROOT / "ecosystem/repos/graphvex/src"
            for name in ("scatter.vert", "scatter.frag", "resolve.vert", "scope.frag"):
                subprocess.run(["glslangValidator", "-V", str(graph / "shaders/compositor" / name),
                                "-o", str(shader / (name + ".spv"))], check=True, timeout=30)
            common = ["image.c", "vulkan/device.c", "vulkan/sampled_image.c", "vulkan/vk_renderer.c",
                      "vulkan/vk_batch.c", "graphics/graphics.c", "graphics/image_runs.c", "compositor/gpu_scope.c"]
            flags = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-g",
                     "-mcpu=apple-m1", "-mmacosx-version-min=14.0", "-fsanitize=address,undefined",
                     "-fno-omit-frame-pointer", "-I" + str(graph),
                     "-I" + str(ROOT / "ecosystem/repos/vexspoke/src"), "-I" + str(ROOT / "tests"),
                     "-I/opt/homebrew/include", "-I" + str(STATE / "out/debug/shader")]
            links = ["-L/opt/homebrew/lib", "-lvulkan", "-Wl,-rpath,/opt/homebrew/lib"]
            for owner, diagnostics in (("vulkan/sampled_image_test", 3), ("image_test", 3),
                                       ("compositor/gpu_scope_test", 0),
                                       ("compositor/filter_gallery_fixture_test", 0)):
                executable = base / Path(owner).name
                source = ROOT / "tests/graphvex" / (owner + ".c")
                extras = []
                if "filter_gallery_fixture" in owner:
                    extras = [str(ROOT / "tests/darling/compositor/gallery_photo.c"),
                              '-DFILTER_GALLERY_SOURCE_RESOURCE="' + str(ROOT / "tests/resources/other-sunflower.png") + '"',
                              "-framework", "CoreFoundation", "-framework", "CoreGraphics", "-framework", "ImageIO"]
                subprocess.run(flags + [str(source)] + [str(graph / item) for item in common] +
                               extras + links + ["-o", str(executable)], check=True, timeout=60)
                result = subprocess.run([str(executable)], env=dict(os.environ, B_HOME=str(base)),
                                        capture_output=True, text=True, timeout=30)
                if result.returncode == 77:
                    self.skipTest(owner + ": Vulkan unavailable")
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                lines = result.stderr.splitlines()
                self.assertEqual(len(lines), diagnostics, result.stderr)
                self.assertTrue(all(line.startswith("[vex] ") for line in lines), result.stderr)


if __name__ == "__main__":
    unittest.main()
