"""Strict migrated native IO/NIO owners, with assertions and ASan/UBSan.

The engine owns every storage/I/O object; only SpinLock and Crypto computation
come from Vexspoke. No Vexspoke io/nio source or header exists in this build.
"""
from pathlib import Path
import os
import subprocess
import tempfile

SUITE = Path(__file__).resolve().parent
WORKSPACE = SUITE.parents[1]
ENGINE = WORKSPACE / "ecosystem/repos/relational-engine"
CPU = WORKSPACE / "ecosystem/repos/vexspoke"

assert not (CPU / "src/nio/mem.c").exists()
assert not list((CPU / "src/io").glob("*"))
sources = [ENGINE / "src/nio/mem.c", *sorted((ENGINE / "src/io").glob("*.c")),
           ENGINE / "src/io/clipboard_mac.m", CPU / "src/atomic/spin.c",
           CPU / "src/security/crypto.c", CPU / "src/c23/free.c"]
owners = [*sorted((SUITE / "io").glob("*_test.c")),
          SUITE / "nio/mem_test.c", SUITE / "nio/transient_lifetime_test.c"]
with tempfile.TemporaryDirectory(prefix="re-native-", dir=os.environ.get("TMPDIR")) as tmp:
    tmp = Path(tmp)
    for mode, sanitizer in (("strict", []), ("sanitized", ["-fsanitize=address,undefined"])):
        flags = ["-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-UNDEBUG",
                 "-DDEBUG_BORROW_CHECK", "-mcpu=apple-m1", "-mmacosx-version-min=14.0",
                 "-I" + str(CPU / "src"), "-I" + str(ENGINE / "src"),
                 "-I" + str(WORKSPACE / "tests"), *sanitizer]
        objects = []
        for index, source in enumerate(sources):
            obj = tmp / f"{mode}-{index}.o"
            arc = ["-fobjc-arc"] if source.suffix == ".m" else []
            subprocess.run(["clang", *flags, *arc, "-c", str(source), "-o", str(obj)],
                           check=True, timeout=60)
            objects.append(str(obj))
        for owner in owners:
            binary = tmp / f"{mode}-{owner.stem}"
            subprocess.run(["clang", *flags, str(owner), *objects, "-framework", "AppKit", "-o", str(binary)],
                           check=True, timeout=60)
            result = subprocess.run([str(binary)], timeout=15)
            if result.returncode == 77:
                assert owner.name == "clipboard_test.c", owner
                print(f"SKIP: {mode} {owner.relative_to(SUITE)} (system clipboard)", flush=True)
                continue
            result.check_returncode()
            print(f"PASS: {mode} {owner.relative_to(SUITE)}", flush=True)
print(f"PASS: {len(owners) - 1} migrated native owners per configuration; clipboard SKIP; macOS only")
