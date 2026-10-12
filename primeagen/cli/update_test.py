"""Real view-module and scripted PTY /update proof; offline, no visual approval.
Local descriptors and all installation/ledger state belong to disposable fixtures.
Host ASan/UBSan does not establish ASan module unload/reload support on Apple.
"""
from pathlib import Path
import hashlib
import json
import os
import pty
import select
import shutil
import subprocess
import tempfile
import time
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
HOT = ROOT / "ecosystem/repos/hotcwap"
VEX = ROOT / "ecosystem/repos/vexspoke/src"
TEST = Path(__file__).resolve().parent


def build_views(base):
    for title in ("A", "B"):
        dest = base / title
        dest.mkdir()
        module = dest / "cli_view.dylib"
        command = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1", "-mcpu=apple-m1",
                   "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}", f"-I{HOT}",
                   f'-DPRIMEAGEN_VIEW_TITLE="Primeagen {title}"', "-dynamiclib",
                   str(APP / "cli/view.c"), str(APP / "tui/tui.c"), str(APP / "tui/markdown.c"),
                   str(VEX / "net/json.c"), "-lncurses", "-o", str(module)]
        subprocess.run(command, check=True, capture_output=True, timeout=60)
        (base / f"{title}.sha256").write_text(hashlib.sha256(module.read_bytes()).hexdigest() + "\n")


class UpdateTest(unittest.TestCase):
    def test_real_mouse_bytes_both_directions_and_visible_approval(self):
        # Exercise actual getch/prefix decoding through the production process,
        # not ungetmouse(): Apple v1 silently loses wheel-down in that path.
        subprocess.run(["sh", str(APP / "cli/build.sh")], check=True, capture_output=True, timeout=90)
        import fcntl
        import struct
        import termios
        master, slave = pty.openpty()
        fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack("HHHH", 24, 100, 0, 0))
        requestRead, requestWrite = os.pipe()
        responseRead, responseWrite = os.pipe()
        os.set_blocking(requestWrite, False)
        os.set_blocking(responseRead, False)
        child = subprocess.Popen([str(APP / ".build/tui"), "--bridge-read", str(responseRead),
                                  "--bridge-write", str(requestWrite)], stdin=slave, stdout=slave, stderr=slave,
                                 pass_fds=(responseRead, requestWrite), env={**os.environ, "TERM": "xterm-256color"})
        os.close(slave)
        os.close(responseRead)
        os.close(requestWrite)
        output = bytearray()

        def until(marker):
            deadline = time.monotonic() + 10
            while marker not in output:
                remaining = deadline - time.monotonic()
                self.assertGreater(remaining, 0, bytes(output))
                self.assertTrue(select.select([master], [], [], remaining)[0])
                output.extend(os.read(master, 65536))

        def wire():
            deadline = time.monotonic() + 10
            while time.monotonic() < deadline:
                ready = select.select([requestRead, master], [], [], deadline - time.monotonic())[0]
                if requestRead in ready:
                    return json.loads(os.read(requestRead, 16384))
                if master in ready:
                    output.extend(os.read(master, 65536))
            self.fail("terminal did not send wire request: " + repr(bytes(output)))

        try:
            until(b"People")
            self.assertIn(b"?1006h", output)
            os.write(master, b"probe\n")
            self.assertEqual(wire()["prompt"], "probe")
            text = "\n".join(("Older block" if i < 30 else "Latest block") + f" entry {i}" for i in range(60))
            os.write(responseWrite, json.dumps({"ok": True, "text": text}).encode() + b"\n")
            until(b"Latest block")
            output.clear()
            os.write(master, b"\x1b[<64;50;4M" * 20)
            until(b"Older")
            output.clear()
            os.write(master, b"\x1b[<65;50;4M" * 20)
            until(b"Latest")
            # Return to older history, then prove the approval brings the live
            # chat/tail back and offers decisions in a fixed input-row bar.
            output.clear()
            os.write(master, b"\x1b[<64;50;4M" * 20)
            until(b"Older")
            os.write(master, b"next\n")
            self.assertEqual(wire()["prompt"], "next")
            output.clear()
            os.write(responseWrite, b'{"ask":{"id":"per_probe","label":"this Bash script: printf probe"}}\n')
            until(b"Do you accept this Bash script")
            until(b"Always yes")
            os.write(master, b"n")
            self.assertEqual(wire(), {"decision": "reject"})
            os.write(master, b"\x11")
            deadline = time.monotonic() + 10
            while child.poll() is None and time.monotonic() < deadline:
                if select.select([master], [], [], .1)[0]:
                    try:
                        output.extend(os.read(master, 65536))
                    except OSError:
                        break
            self.assertEqual(child.wait(timeout=3), 0)
        finally:
            if child.poll() is None:
                child.kill()
                child.wait(timeout=3)
            for fd in (master, requestRead, responseWrite):
                os.close(fd)

    def test_terminal_native_selection_toggle(self):
        # Actual terminal protocol, not appearance/clipboard acceptance: disable
        # mouse capture for native selection, then restore it without losing draft.
        subprocess.run(["sh", str(APP / "cli/build.sh")], check=True, capture_output=True, timeout=90)
        import fcntl
        import struct
        import termios
        master, slave = pty.openpty()
        fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack("HHHH", 24, 100, 0, 0))
        child = subprocess.Popen([str(APP / ".build/tui")], stdin=slave, stdout=slave, stderr=slave,
                                 env={**os.environ, "TERM": "xterm-256color"})
        os.close(slave)
        output = bytearray()

        def until(marker):
            deadline = time.monotonic() + 10
            while marker not in output:
                remaining = deadline - time.monotonic()
                self.assertGreater(remaining, 0, bytes(output))
                ready, _, _ = select.select([master], [], [], remaining)
                self.assertTrue(ready)
                output.extend(os.read(master, 65536))

        try:
            until(b"People")
            self.assertIn(b"?1000h", output)
            output.clear()
            os.write(master, b"draft-selection\x14")
            until(b"Select text: drag")
            self.assertIn(b"?1000l", output)
            self.assertIn(b"draft-selection", output)
            output.clear()
            os.write(master, b"\x14")
            until(b"People")
            self.assertIn(b"?1000h", output)
            until(b"draft-selection")
            os.write(master, b"\x11")
            deadline = time.monotonic() + 10
            while child.poll() is None:
                remaining = deadline - time.monotonic()
                self.assertGreater(remaining, 0)
                ready, _, _ = select.select([master], [], [], remaining)
                if ready:
                    try:
                        output.extend(os.read(master, 65536))
                    except OSError:
                        break
            self.assertEqual(child.wait(timeout=3), 0)
        finally:
            if child.poll() is None:
                child.kill()
                child.wait(timeout=3)
            os.close(master)

    def test_headless_view_state_and_recovery(self):
        with tempfile.TemporaryDirectory(prefix="primeagen-update-") as directory:
            base = Path(directory)
            build_views(base)
            sources = [HOT / "hot" / name for name in ("hot.c", "hot_trampoline.c", "hot_retire.c", "manifest.c", "ledger.c")]
            for sanitizer in (False, True):
                home = base / ("sanitized" if sanitizer else "strict")
                home.mkdir()
                owner = base / "owner"
                command = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1", "-mcpu=apple-m1",
                           "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}", f"-I{HOT}"]
                if sanitizer:
                    command += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                command += [str(TEST / "update_test.c"), str(APP / "cli/update.c"), str(APP / "tui/tui.c"),
                            str(APP / "tui/markdown.c"), str(VEX / "net/json.c"), str(VEX / "security/crypto.c"),
                            *map(str, sources), "-pthread", "-lncurses", "-o", str(owner)]
                subprocess.run(command, check=True, capture_output=True, timeout=60)
                result = subprocess.run([str(owner), str(home), str(base / "A"), str(base / "A.sha256"),
                                         str(base / "B"), str(base / "B.sha256")],
                                        capture_output=True, timeout=30, env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                self.assertEqual(result.returncode, 0, result.stderr.decode())
                self.assertIn(b"rejection/recovery passed", result.stdout)

    def test_scripted_terminal_update_command(self):
        subprocess.run(["sh", str(APP / "cli/build.sh")], check=True, capture_output=True, timeout=90)
        with tempfile.TemporaryDirectory(prefix="primeagen-pty-update-") as directory:
            base = Path(directory)
            build_views(base)
            payload = base / "payload"
            shutil.copytree(base / "A", payload)
            checksum = base / "current.sha256"
            shutil.copyfile(base / "A.sha256", checksum)
            master, slave = pty.openpty()
            child = subprocess.Popen([str(APP / ".build/tui"), "--update-payload", str(payload), "--update-checksum", str(checksum)],
                                     stdin=slave, stdout=slave, stderr=slave,
                                     env={**os.environ, "TERM": "xterm-256color", "HOME": str(base), "VEX_MANIFEST": str(base)})
            os.close(slave)
            transcript = bytearray()

            def until(marker):
                deadline = time.monotonic() + 15
                while marker not in transcript:
                    remaining = deadline - time.monotonic()
                    self.assertGreater(remaining, 0, bytes(transcript))
                    ready, _, _ = select.select([master], [], [], remaining)
                    self.assertTrue(ready, bytes(transcript))
                    transcript.extend(os.read(master, 65536))
            try:
                until(b"Primeagen A")
                shutil.copyfile(base / "B/cli_view.dylib", payload / "cli_view.dylib")
                shutil.copyfile(base / "B.sha256", checksum)
                os.write(master, b"/update\n")
                until(b"Updated running CLI")
                # ncurses sends only the changed title cell, not the full title.
                # The headless owner independently asserts the actual B cell.
                until(b"generation 2 -> 3")
                # A broken reference must resolve and leave the updated view live.
                checksum.write_text("0" * 64 + "\n")
                os.write(master, b"/update\n")
                until(b"Update rejected")
                os.write(master, b"\x11")
                # endwin restores termios with TCSADRAIN: keep consuming PTY
                # output until exit, rather than blocking its drain with wait().
                deadline = time.monotonic() + 10
                while child.poll() is None:
                    remaining = deadline - time.monotonic()
                    self.assertGreater(remaining, 0, bytes(transcript))
                    ready, _, _ = select.select([master], [], [], remaining)
                    if ready:
                        try:
                            transcript.extend(os.read(master, 65536))
                        except OSError:
                            break  # slave closed; wait reaps the completed child
                self.assertEqual(child.wait(timeout=3), 0)
            finally:
                if child.poll() is None:
                    child.terminate()
                    try:
                        child.wait(timeout=3)
                    except subprocess.TimeoutExpired:
                        child.kill()
                        child.wait(timeout=3)
                os.close(master)


if __name__ == "__main__":
    unittest.main()
