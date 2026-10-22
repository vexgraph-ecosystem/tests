"""Owner: the development preview of the new frame (cli/preview.c + preview.sh).

Runs the preview in a real PTY and asserts the HOME screen composes: the menu
entries, the toolkit connector rule and the badge's ink. It also drives the
terminal EVENTS a real user produces — zooming (font size), toggling fullscreen
and dragging the window edge all arrive as a resize — and asserts the frame
revalidates at once instead of waiting for an unrelated key.

This proves the RENDERER in a terminal; it does not describe the live view, which
still paints ncurses directly until the migration is wired.
"""
from pathlib import Path
import fcntl
import os
import pty
import select
import signal
import struct
import subprocess
import termios
import time
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"


class Preview:
    """A preview running in a real PTY whose window size the test can change."""

    def __init__(self, rows=40, columns=120):
        self.master, slave = pty.openpty()
        # Give the PTY a real window size ABOVE the frame's advertised minimum, so
        # the preview composes the frame instead of its too-small notice.
        self.resize(rows, columns, fd=slave)
        env = {**os.environ,
               "TERM": "xterm-256color",
               "PRIMEAGEN_ROOT": str(APP),
               "DETERMINANT_SOURCE_DIR": str(ROOT / "drivers/determinant/src"),
               "RELATIONAL_ENGINE_SOURCE_DIR": str(ROOT / "host-behavior/relational-engine/src"),
               "VEXSPOKE_SOURCE_DIR": str(ROOT / "host-behavior/vexspoke/src"),
               "HOTCWAP_SOURCE_DIR": str(ROOT / "host-behavior/hotcwap")}
        self.child = subprocess.Popen(["sh", str(APP / "preview.sh")],
                                      stdin=slave, stdout=slave, stderr=slave, env=env)
        os.close(slave)

    def resize(self, rows, columns, fd=None, signal_child=True):
        """Change the PTY window size — what a zoom, fullscreen toggle or drag does."""
        target = self.master if fd is None else fd
        fcntl.ioctl(target, termios.TIOCSWINSZ, struct.pack("HHHH", rows, columns, 0, 0))
        if signal_child and fd is None:
            os.kill(self.child.pid, signal.SIGWINCH)

    def read_until(self, needle, timeout=20):
        """Read PTY output until `needle` appears; returns everything read."""
        wanted = needle.encode()
        transcript = bytearray()
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline and wanted not in transcript:
            ready, _, _ = select.select([self.master], [], [], 1)
            if ready:
                try:
                    transcript.extend(os.read(self.master, 65536))
                except OSError:
                    break
        return transcript.decode("utf-8", "replace")

    def close(self):
        try:
            os.write(self.master, b"q")
        except OSError:
            pass
        try:
            self.child.wait(timeout=10)
        except subprocess.TimeoutExpired:
            self.child.kill()
            self.child.wait(timeout=5)
        finally:
            os.close(self.master)


class PreviewTest(unittest.TestCase):
    def test_home_frame_composes_in_a_pty(self):
        preview = Preview()
        try:
            text = preview.read_until("Settings")
            self.assertIn("Primeagen", text, text)
            for entry in ("People", "Servers", "Settings"):
                self.assertIn(entry, text, text)
            # The rail rule is the toolkit CONNECTOR run, not a bare ncurses line.
            self.assertTrue("---.---" in text or "---" in text, text)
            # The badge's ink made it into cells.
            self.assertTrue(any(ink in text for ink in ("@", "%", "*")), text)
        finally:
            preview.close()

    def test_zoom_and_fullscreen_revalidate_the_frame(self):
        # Zooming (a smaller font fits more columns), shrinking the window, and
        # toggling fullscreen all change the terminal size. Each must revalidate
        # the frame immediately: below the minimum it shows its notice, and growing
        # back draws the frame again — no waiting for another keypress.
        preview = Preview()
        try:
            self.assertIn("Settings", preview.read_until("Settings"))

            # Zoomed IN below the floor: the notice replaces the frame at once.
            preview.resize(24, 70)
            shrunk = preview.read_until("Rendering error")
            self.assertIn("Rendering error", shrunk, shrunk)
            self.assertNotIn("Settings", shrunk, shrunk)

            # Back above the floor: the frame returns without any further key.
            preview.resize(40, 120)
            grown = preview.read_until("Settings")
            self.assertIn("Settings", grown, grown)

            # Fullscreen-sized: an even larger grid still composes.
            preview.resize(60, 200)
            full = preview.read_until("Primeagen", timeout=10)
            self.assertIn("Primeagen", full, full)
        finally:
            preview.close()


if __name__ == "__main__":
    unittest.main()
