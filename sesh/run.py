"""Registered isolated Sesh owner runner; no engine, b state or network required."""
from pathlib import Path
import os
import re
import subprocess
import tempfile

SUITE = Path(__file__).resolve().parent
ROOT = SUITE.parents[1]
CPU = ROOT / "ecosystem/repos/vexspoke/src"
API = ROOT / "ecosystem/repos/api-haven/src"
SESH = ROOT / "ecosystem/repos/sesh/src"
fixture = SUITE / "test.txt"
# Disposable runtime input, intentionally ignored. Never touch preferences.md.
assert not fixture.is_symlink(), "Disposable fixture must not be a symlink"
if not fixture.exists():
    with fixture.open("x") as stream:
        stream.write("Sesh disposable backup fixture.\nNo credentials.\n")
assert subprocess.check_output(["git", "-C", str(SUITE.parent), "check-ignore", "sesh/test.txt"], text=True).strip() == "sesh/test.txt"
assert subprocess.run(["git", "-C", str(SUITE.parent), "check-ignore", "sesh/snapshot/snapshot_test.c"], capture_output=True).returncode == 1
with tempfile.TemporaryDirectory(prefix="sesh-owner-", dir=os.environ.get("TMPDIR")) as directory:
    for mode, extra in (("strict", []), ("sanitized", ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"])):
        binary = str(Path(directory) / mode)
        command = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-UNDEBUG",
                   "-mcpu=apple-m1", "-mmacosx-version-min=14.0", *extra,
                   "-I" + str(CPU), "-I" + str(API), "-I" + str(SESH),
                   str(SUITE / "snapshot/snapshot_test.c"), str(SESH / "snapshot/snapshot.c"), "-o", binary]
        subprocess.run(command, check=True, timeout=60)
        header_binary = str(Path(directory) / (mode + "-snapshot-io"))
        subprocess.run(command[:-4] + [str(ROOT / "tests/api-haven/storage/snapshot_io_test.c"), "-o", header_binary], check=True, timeout=60)
        header_result = subprocess.run([header_binary], capture_output=True, text=True, timeout=10)
        header_result.check_returncode()
        assert not header_result.stderr, header_result.stderr
        print(mode + ": PASS: API Haven snapshot header owner")
        for scenario in (str(fixture), "invalid"):
            result = subprocess.run([binary, scenario], capture_output=True, text=True, timeout=10)
            result.check_returncode()
            reports = result.stderr.splitlines()
            if scenario == "invalid":
                assert len(reports) == 27, result.stderr
                assert all(re.fullmatch(r"\[vex\] snapshot\.c:\d+: snapshot .+", line) for line in reports), result.stderr
                expected = [
                    "configuration: null or active job", "buffer: missing storage", "buffer: missing storage",
                    "configuration: null or active job", "provider: missing callback",
                    "configuration: null or active job", "retry policy: zero delay or attempts",
                    "retry policy: zero delay or attempts", *(["queue: invalid input"] * 3),
                    *(["queue: busy, unconfigured or oversized"] * 4),
                    *(["configuration: null or active job"] * 3), "step: null job",
                    *(["step: retry exhausted or clock overflow"] * 2),
                    *(["step: provider rejected snapshot"] * 2),
                    *(["string: missing destination"] * 2), *(["string: truncated"] * 2),
                ]
                assert [line.split(": snapshot ", 1)[1] for line in reports] == expected, result.stderr
            else:
                assert not reports, result.stderr
            print(mode + ": " + result.stdout.strip())
        # Unsupported arity must fail specifically at the constructor.
        probe = Path(directory) / "arity.c"
        probe.write_text('#include "snapshot/snapshot.h"\nint main(void) { SeshSnapshot x = SeshSnapshot(1); (void)x; }\n')
        result = subprocess.run(command[:-4] + ["-fsyntax-only", str(probe)], capture_output=True, text=True, timeout=10)
        assert result.returncode != 0 and "too many arguments" in result.stderr, result.stderr
print("PASS: strict + ASan/UBSan, offline only; no engine/Drive/iCloud/durable-journal proof")
