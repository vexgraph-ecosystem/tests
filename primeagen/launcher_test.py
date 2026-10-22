"""Explicit command install: real shell symlink resolution and cwd independence.
No interactive application, providers, shell-config edits or real-user install.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
APP = ROOT / "personal/primeagen"


class LauncherTest(unittest.TestCase):
    def test_install_cwd_arguments_and_preservation(self):
        with tempfile.TemporaryDirectory(prefix="primeagen-command-") as directory:
            base = Path(directory).resolve()
            checkout = base / "checkout with spaces"
            checkout.mkdir()
            for name in ("primeagen", "install-command.sh"):
                shutil.copy2(APP / name, checkout / name)
            (checkout / ".build").mkdir()
            fake = checkout / ".build/tui"
            fake.write_text('#!/bin/sh\nprintf "cwd=%s\\n" "$PWD"\nprintf "arg=%s\\n" "$@"\n')
            fake.chmod(0o755)
            bindir = base / "bin with spaces"
            env = {**os.environ, "HOME": str(base), "PATH": f"{bindir}:{os.environ['PATH']}"}
            result = subprocess.run(["sh", str(checkout / "install-command.sh"), str(bindir)], env=env, capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stderr)
            link = bindir / "primeagen"
            self.assertEqual(link.resolve(), (checkout / "primeagen").resolve())
            result = subprocess.run(["sh", str(checkout / "install-command.sh"), str(bindir)], env=env, capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn(b"Already installed", result.stdout)
            elsewhere = base / "another cwd"
            elsewhere.mkdir()
            result = subprocess.run(["primeagen", "tui", "--list-width", "28"], cwd=elsewhere, env=env, capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn(f"cwd={elsewhere}".encode(), result.stdout)
            self.assertIn(f"arg={checkout}/src/session_host.py".encode(), result.stdout)
            self.assertIn(b"arg=--list-width\narg=28", result.stdout)
            # Relative symlink chains also resolve without changing caller cwd.
            (bindir / "alias").symlink_to("primeagen")
            result = subprocess.run([str(bindir / "alias")], cwd=elsewhere, env=env, capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn(f"arg={checkout}/.build/payload".encode(), result.stdout)
            link.unlink()
            link.write_text("keep this unrelated command")
            result = subprocess.run(["sh", str(checkout / "install-command.sh"), str(bindir)], env=env, capture_output=True, timeout=5)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(link.read_text(), "keep this unrelated command")
            self.assertFalse((base / ".zshrc").exists())


if __name__ == "__main__":
    unittest.main()
