#!/usr/bin/env python3
"""Registered runner for the harness space owners.

Compiles and runs each C owner twice — strict `-Wall -Wextra -Werror`, then
ASan/UBSan — and runs the Python owners (the compile-negative arity battery).
Self-contained: no b, no network, no interactive launch. A C owner that fails to
build, fails an assertion, or trips a sanitizer fails the run.
"""
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
APP = ROOT / "ecosystem/repos/harness"
VEX = ROOT / "ecosystem/repos/vexspoke/src"
REL = ROOT / "ecosystem/repos/relational-engine/src"
SPACE_SRCS = [APP / "src/space/model_user.c", APP / "src/space/channel.c",
              APP / "src/space/message.c", APP / "src/space/task.c"]


def run_c_owners():
    failed = False
    with tempfile.TemporaryDirectory(prefix="harness-space-") as d:
        for sanitized in (False, True):
            for owner in sorted(HERE.rglob("*_test.c")):
                out = Path(d) / ("owner-" + ("san" if sanitized else "strict"))
                argv = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                        "-arch", "arm64", "-mmacosx-version-min=14.0",
                        f"-I{APP / 'src'}", f"-I{VEX}", f"-I{ROOT / 'tests'}", f"-I{REL}"]
                if sanitized:
                    argv += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                argv += [str(owner), *(str(s) for s in SPACE_SRCS), "-o", str(out)]
                build = subprocess.run(argv, capture_output=True, text=True, timeout=120)
                if build.returncode != 0:
                    print(f"BUILD FAIL {'asan' if sanitized else 'strict'} {owner.name}\n{build.stderr}")
                    failed = True
                    continue
                result = subprocess.run([str(out)], capture_output=True, text=True, timeout=60,
                                        env={"ASAN_OPTIONS": "detect_leaks=0", "PATH": "/usr/bin:/bin"})
                label = "asan" if sanitized else "strict"
                if result.returncode != 0:
                    print(f"RUN FAIL {label} {owner.name}\n{result.stdout}{result.stderr}")
                    failed = True
                else:
                    print(f"PASS {label} {owner.name}")
    return failed


def run_py_owners():
    failed = False
    for owner in sorted(HERE.rglob("*_test.py")):
        result = subprocess.run([sys.executable, "-B", str(owner)], timeout=240)
        failed |= result.returncode != 0
    return failed


if __name__ == "__main__":
    raise SystemExit(1 if (run_c_owners() | run_py_owners()) else 0)
