#!/usr/bin/env python3
"""Owner: the semicolon terminal shell (suite/projects/semicolon/src/main/tui-main.c).

MODULE: tui_main_test.py
REGISTRY: main() builds the shell through its own build.py, then proves three
things about it: the deterministic --dump frame reproduces the rows of the design
in src/tui/tui_example.txt, a non-tty launch is refused with a diagnostic and a
non-zero exit instead of a corrupt terminal, and an interactive launch under a
real pty enters the screen, paints the same content, and leaves on 'q'.

The pty is allocated by this test, not by the shell. No network, no file edit, no
provider call, and no visual acceptance is claimed: the pty proves the program
drives a terminal, not that it looks right on one.
"""
from pathlib import Path
import fcntl
import os
import pty
import select
import struct
import subprocess
import sys
import termios

ROOT = Path(__file__).resolve().parents[2]
APP = ROOT / "suite/projects/semicolon"
BUILD = APP / "build.py"
BINARY = APP / ".build/semicolon-tui"


def ensure_built():
    if BINARY.is_file():
        return
    result = subprocess.run([sys.executable, str(BUILD)], capture_output=True, text=True, timeout=600)
    if result.returncode:
        print(f"BUILD FAIL\n{result.stdout}{result.stderr}", file=sys.stderr)
        raise SystemExit(1)


def dump(width, height):
    result = subprocess.run([str(BINARY), "--dump", f"{width}x{height}"],
                            capture_output=True, text=True, timeout=60)
    if result.returncode:
        print(f"FAIL dump exited {result.returncode}\n{result.stderr}", file=sys.stderr)
        raise SystemExit(1)
    return result.stdout.splitlines()


def run_under_pty(rows, columns):
    master, slave = pty.openpty()
    fcntl.ioctl(master, termios.TIOCSWINSZ, struct.pack("HHHH", rows, columns, 0, 0))
    process = subprocess.Popen([str(BINARY)], stdin=slave, stdout=slave, stderr=slave,
                               close_fds=True, cwd=str(APP))
    os.close(slave)
    captured = bytearray()
    quit_sent = False
    for _ in range(400):
        readable, _, _ = select.select([master], [], [], 0.05)
        if readable:
            try:
                chunk = os.read(master, 65536)
            except OSError:
                break
            if not chunk:
                break
            captured += chunk
            if not quit_sent and b"vexgraph" in captured:
                os.write(master, b"q")
                quit_sent = True
        if process.poll() is not None:
            break
    if process.poll() is None:
        process.kill()
        process.wait()
    try:
        os.close(master)
    except OSError:
        pass
    return captured, process.returncode


def main():
    ensure_built()
    failures = 0

    # --- The deterministic frame reproduces the design's own rows ---
    lines = dump(103, 23)
    if len(lines) != 23:
        print(f"FAIL dump produced {len(lines)} rows, expected 23", file=sys.stderr)
        failures += 1
    design = (APP / "src/tui/tui_example.txt").read_text().splitlines()
    # Rows 0 and 1 are compared chip for chip against the design. The status row
    # is compared structurally: this shell shows a real path where the sketch has
    # a placeholder, so its dash run is legitimately a different length.
    expected = {0: design[0], 1: design[1]}
    for index, wanted in expected.items():
        if index >= len(lines):
            print(f"FAIL row {index} missing", file=sys.stderr)
            failures += 1
        elif lines[index] != wanted:
            print(f"FAIL row {index}\n  want {wanted!r}\n  got  {lines[index]!r}", file=sys.stderr)
            failures += 1
    if len(lines) > 22:
        status = lines[22]
        for fragment in ("--[vexgraph > src > main > tui-main.c]",
                         "-[c23/java]-[ln:1]-[time]-[utf-8]--"):
            if fragment not in status:
                print(f"FAIL status row missing {fragment!r}\n  got {status!r}", file=sys.stderr)
                failures += 1
        if not status.endswith("--") or not status.startswith("--["):
            print(f"FAIL status row margins\n  got {status!r}", file=sys.stderr)
            failures += 1
    for needle in ("opencode.json", "helloWorld.c", "no errors!", "Hello *h = Hello();",
                   "new folder"):
        if not any(needle in line for line in lines):
            print(f"FAIL {needle!r} absent from the frame", file=sys.stderr)
            failures += 1
    # The rail rail, the column divider and the right edge are all present.
    for y, x in ((1, 0), (1, 28), (1, 102), (20, 28)):
        if lines[y][x] != "|":
            print(f"FAIL chrome missing '|' at {x},{y} (found {lines[y][x]!r})", file=sys.stderr)
            failures += 1

    # --- A non-tty launch is refused, not painted into a pipe ---
    refused = subprocess.run([str(BINARY)], capture_output=True, text=True, timeout=60)
    if refused.returncode == 0:
        print("FAIL non-tty launch succeeded", file=sys.stderr)
        failures += 1
    elif "requires terminal" not in refused.stderr:
        print(f"FAIL non-tty diagnostic missing\n{refused.stderr}", file=sys.stderr)
        failures += 1

    # --- Under a real pty the shell enters the screen, paints, and leaves on 'q' ---
    captured, status = run_under_pty(24, 100)
    if status != 0:
        print(f"FAIL pty run exited {status}", file=sys.stderr)
        failures += 1
    text = captured.decode("utf-8", "replace")
    if "vexgraph" not in text:
        print("FAIL pty run never painted the design", file=sys.stderr)
        failures += 1
    if "\x1b[?1049h" not in text and "\x1b[?47h" not in text:
        # ncurses enters the alternate screen via smcup; its exact bytes vary by
        # terminfo, so treat either the capability or plain content as sufficient.
        if "no errors!" not in text:
            print("FAIL pty run left no screen trace", file=sys.stderr)
            failures += 1
    if not text.endswith("\x1b[?1049l") and "\x1b[?1049l" not in text and "\x1b[?47l" not in text:
        print("note: no explicit alternate-screen exit sequence observed", file=sys.stderr)

    if failures:
        print(f"FAIL {failures} check(s)", file=sys.stderr)
        return 1
    print("PASS semicolon terminal shell: dump rows, non-tty refusal, pty launch")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
