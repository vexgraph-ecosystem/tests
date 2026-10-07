#!/usr/bin/env python3
"""Noninteractive bundle/fixture proof; never opens the gallery window."""
from pathlib import Path
import os
import plistlib
import shutil
import struct
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PHOTO = ROOT / "tests/resources/other-sunflower.png"
STATE = Path(os.environ.get("B_HOME", Path.home() / "Library/Application Support/vexgraph/b"))


class GalleryResourcesTest(unittest.TestCase):
    def test_bundled_asset_and_incremental_refresh(self):
        subprocess.run([str(ROOT / "tools/b"), "build", "filter_gallery"],
                       cwd=ROOT, check=True, timeout=180)
        app = STATE / "out/debug/apps/filter_gallery.app"
        bundled = app / "Contents/Resources/other-sunflower.png"
        original = PHOTO.read_bytes()
        self.assertEqual(original[:8], b"\x89PNG\r\n\x1a\n")
        self.assertEqual(struct.unpack(">II", original[16:24]), (700, 448))
        self.assertEqual(bundled.read_bytes(), original)
        # Only an owned generated artifact is changed, never the source fixture.
        bundled.write_bytes(b"stale generated bundle resource")
        subprocess.run([str(ROOT / "tools/b"), "build", "filter_gallery"],
                       cwd=ROOT, check=True, timeout=180)
        self.assertEqual(bundled.read_bytes(), original)
        subprocess.run(["codesign", "--verify", "--strict", str(app)],
                       check=True, timeout=30)
        doc = (ROOT / "tests/resources/README.md").read_text()
        self.assertIn("license is not established", doc)
        self.assertIn("color-run geometry memory overhead", doc)

    def test_sanitized_decode_and_relocated_bundle(self):
        with tempfile.TemporaryDirectory(prefix="gallery-photo-", dir=os.environ.get("TMPDIR")) as directory:
            base = Path(directory)
            app = base / "PhotoProbe.app"
            contents = app / "Contents"
            macos = contents / "MacOS"
            resources = contents / "Resources"
            macos.mkdir(parents=True)
            resources.mkdir()
            executable = macos / "photo_probe"
            command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2",
                       "-mcpu=apple-m1", "-mmacosx-version-min=14.0",
                       "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
                       f'-DFILTER_GALLERY_SOURCE_RESOURCE="{PHOTO}"',
                       str(ROOT / "tests/darling/compositor/gallery_photo_test.c"),
                       str(ROOT / "tests/darling/compositor/gallery_photo.c"),
                       "-framework", "CoreFoundation", "-framework", "CoreGraphics",
                       "-framework", "ImageIO", "-o", str(executable)]
            subprocess.run(command, check=True, timeout=60)
            shutil.copyfile(PHOTO, resources / PHOTO.name)
            (contents / "Info.plist").write_bytes(plistlib.dumps({
                "CFBundleExecutable": executable.name,
                "CFBundleIdentifier": "dev.vexgraph.photo-probe",
                "CFBundlePackageType": "APPL",
            }))
            for args in ([], ["--bundle"]):
                result = subprocess.run([str(executable), *args], cwd=base,
                                        capture_output=True, text=True, timeout=30)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual(result.stderr, "")
                self.assertIn("PASS", result.stdout)
            # The bundle path must fail rather than consult the source fallback.
            (resources / PHOTO.name).unlink()
            missing = subprocess.run([str(executable), "--bundle"], cwd=base,
                                     capture_output=True, text=True, timeout=30)
            self.assertNotEqual(missing.returncode, 0)
            self.assertIn("GalleryPhoto_decode(path, WIDTH, HEIGHT", missing.stderr)


if __name__ == "__main__":
    unittest.main()
