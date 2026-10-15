#!/usr/bin/env python3
"""Fresh, bounded owner build for the Darkbase Database persistence seam.

MODULE: database/run.py
Builds the real Relational Engine Rust mapping archive and a strict C23 client
from the current owner sources; generated inputs and binaries stay disposable.
The workspace Vexspoke ownership ratchet remains untouched and independently
blocks the normal graph until its own baseline is repaired.
"""
from pathlib import Path
import os
import subprocess
import tempfile
from collections import Counter

ROOT = Path(__file__).resolve().parents[3]
DB = ROOT / "suite/ecosystem/darkbase-engine"
CPU = ROOT / "suite/ecosystem/vexspoke"
ENGINE = ROOT / "suite/ecosystem/relational-engine"
TEST = ROOT / "tests/darkbase-engine/database/database_test.c"


def run(argv, *, cwd=ROOT, env=None, capture=False, timeout=120):
    return subprocess.run([str(value) for value in argv], cwd=cwd, env=env,
                          check=True, capture_output=capture, text=capture,
                          timeout=timeout)


def main():
    with tempfile.TemporaryDirectory(prefix="darkbase-database-",
                                     dir=os.environ.get("TMPDIR")) as temp:
        build = Path(temp)
        cargo_env = dict(os.environ, CARGO_TARGET_DIR=str(build / "cargo"),
                         RUSTFLAGS="-D warnings")
        run(["cargo", "build", "--offline", "--locked", "--manifest-path",
             ENGINE / "rust/Cargo.toml"], env=cargo_env)
        archive = build / "cargo/debug/librelational_engine_scratchpad.a"
        sources = [
            (ENGINE / "src/nio/mem.c", ENGINE),
            (ENGINE / "src/io/file.c", CPU),
            (ENGINE / "src/type/type.c", ENGINE),
            (CPU / "src/atomic/spin.c", CPU),
            (CPU / "src/oop/stride.c", CPU),
            (CPU / "src/reflection/field.c", CPU),
            (CPU / "src/reflection/method.c", CPU),
            (CPU / "src/reflection/class.c", CPU),
            (CPU / "src/reflection/variable.c", CPU),
            (CPU / "src/reflection/struct.c", CPU),
            (CPU / "src/relational/variable_slot.c", CPU),
            (CPU / "src/struct/collection.c", CPU),
            (CPU / "src/struct/exhaustion.c", CPU),
            (CPU / "src/struct/chunked_list.c", CPU),
            (DB / "src/util/crc32.c", DB),
            (DB / "src/page/page.c", DB),
            (DB / "src/wal/wal.c", DB),
            (DB / "src/store/store.c", DB),
            (DB / "src/database/database_result.c", DB),
            (DB / "src/database/database.c", DB),
        ]
        flags = ["-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-UNDEBUG",
                 "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
                 "-mcpu=apple-m1", "-mmacosx-version-min=14.0"]
        objects = []
        for index, (source, owner) in enumerate(sources):
            obj = build / f"{index}.o"
            include_roots = [owner / "src", DB / "src", CPU / "src",
                             ENGINE / "src", ENGINE / "rust/include"]
            includes = []
            for include_root in include_roots:
                option = "-I" + str(include_root)
                if option not in includes:
                    includes.append(option)
            run(["clang", *flags, *includes, "-c", source, "-o", obj])
            objects.append(obj)
        binary = build / "database_test"
        run(["clang", *flags, "-I" + str(DB / "src"),
             "-I" + str(CPU / "src"), "-I" + str(ENGINE / "src"),
             "-I" + str(ENGINE / "rust/include"), "-I" + str(ROOT / "tests"),
             TEST, *objects, archive, "-o", binary])
        result = run([binary], capture=True, timeout=60)
        if result.stdout:
            print(result.stdout, end="")
        expected = Counter({
            "Database_define: duplicate entity": 1,
            "Database_insert: no such entity": 1,
            "Database_insert: null argument": 1,
            "Database_insertOwned: row length does not equal schema stride": 2,
            "Database_insertOwned: cannot mix borrowed and owned rows": 1,
            "Database_insert: cannot bind a live row into a file-loaded entity": 2,
            "Database_define: null database or schema": 1,
            "Database_load: payload rejected": 1,
            "Database_load: checksum mismatch": 1,
            "Database_save: entity has no row stride": 1,
            "Wal_append: region exhausted, request 576 bytes (remaining 296)": 1,
            "Store exhausted: request 544 bytes, bound 2048 bytes": 1,
            "Database_saveStore: data-page write failed before publication": 1,
            "Database_loadStore: registered schemas rejected snapshot": 1,
        })
        actual = Counter(line.split(": ", 1)[1] for line in result.stderr.splitlines())
        if actual != expected:
            raise AssertionError(f"Database diagnostic mismatch: {actual - expected}; "
                                 f"missing {expected - actual}")
        print("PASS: strict optimized C ASan/UBSan Database owner including page-store reopen")


if __name__ == "__main__":
    main()
