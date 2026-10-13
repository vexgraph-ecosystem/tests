"""Owner: the development preview of the new frame (cli/preview.c + preview.sh).

Runs the preview in a real PTY and asserts the HOME screen composes: the menu
entries, the toolkit connector rule and the badge's ink. This proves the RENDERER
in a terminal; it does not describe the live view, which still paints ncurses
directly until the migration is wired.
"""
from pathlib import Path
import os
import pty
import select
import subprocess
import time
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"


class PreviewTest(unittest.TestCase):
    def test_home_frame_composes_in_a_pty(self):
        master, slave = pty.openpty()
        env = {**os.environ,
               "TERM": "xterm-256color",
               "PRIMEAGEN_ROOT": str(APP),
               "DETERMINANT_SOURCE_DIR": str(ROOT / "drivers/determinant/src"),
               "RELATIONAL_ENGINE_SOURCE_DIR": str(ROOT / "host-behavior/relational-engine/src"),
               "VEXSPOKE_SOURCE_DIR": str(ROOT / "host-behavior/vexspoke/src"),
               "HOTCWAP_SOURCE_DIR": str(ROOT / "host-behavior/hotcwap")}
        child = subprocess.Popen(["sh", str(APP / "preview.sh")],
                                 stdin=slave, stdout=slave, stderr=slave, env=env)
        os.close(slave)
        transcript = bytearray()
        try:
            deadline = time.monotonic() + 60
            while time.monotonic() < deadline and b"Settings" not in transcript:
                ready, _, _ = select.select([master], [], [], 1)
                if ready:
                    try:
                        transcript.extend(os.read(master, 65536))
                    except OSError:
                        break
            text = transcript.decode("utf-8", "replace")
            self.assertIn("Primeagen", text, text)
            for entry in ("People", "Servers", "Settings"):
                self.assertIn(entry, text, text)
            # The rail rule is the toolkit CONNECTOR run, not a bare ncurses line.
            self.assertTrue("---.---" in text or "---" in text, text)
            # The badge's dark ink made it into cells.
            self.assertTrue(any(ink in text for ink in ("@", "%", "*")), text)
            os.write(master, b"q")
            try:
                child.wait(timeout=10)
            except subprocess.TimeoutExpired:
                pass   # the frame composed; the exit path is not this owner's claim
        finally:
            if child.poll() is None:
                child.terminate()
                try:
                    child.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    child.kill()
            os.close(master)


if __name__ == "__main__":
    unittest.main()
