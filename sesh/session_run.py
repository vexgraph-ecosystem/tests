"""Registered seven-class Sesh owner battery; isolated/offline, no b state."""
from pathlib import Path
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
SUITE = ROOT / "tests/sesh"
SRC = ROOT / "ecosystem/repos/sesh/src"
SOURCES = sorted((SRC / "session").glob("*.c"))
TEXT_ERRORS = ["session text: missing destination"] * 2 + ["session text: truncated"]
EXPECTED = {
    "client": ["client identity: null or zero ID"] * 3 + TEXT_ERRORS,
    "workspace": ["workspace identity: null or zero ID", "workspace principal: null or zero ID"] * 2
                 + ["workspace identity: zero ID"] * 2 + TEXT_ERRORS,
    "resource": ["resource identity: null or zero ID", "resource workspace: null or zero ID",
                 "resource revision: null resource", "resource identity: null or zero ID",
                 "resource workspace: null or zero ID"] + ["resource identity: zero ID"] * 2 + TEXT_ERRORS,
    "operation": ["operation identity: null or zero ID"] * 8 + ["operation revision: null operation"]
                 + ["operation identity: zero ID"] * 4 + TEXT_ERRORS,
    "auth_service": ["auth service: missing credential or verifier"] * 2
                    + ["auth service credentials: null input"] * 2 + ["auth service verifier: null input"] * 2
                    + ["auth service context: null service", "auth service authenticate: unconfigured", "auth service authenticate: rejected identity", "auth service authenticate: rejected identity",
                       "auth service authenticate: unconfigured"] + TEXT_ERRORS,
    "operations": ["operations reserve: invalid storage"] * 5 + ["operations reserve: cannot shrink storage",
                   "operations add: reserve larger storage"] + ["operations add: invalid operation"] * 2
                  + ["operations add: identity reused with changed intent"] + ["operations find: invalid input"] * 4
                  + ["operations apply: invalid scope"] * 3 + ["operations apply: changed intent", "operations apply: intent not admitted",
                     "operations apply: revision conflict"] + ["operations apply: invalid scope"] * 2 + ["operations apply: revision overflow"] + TEXT_ERRORS,
    "sesh": ["sesh submit: revision conflict"] + ["sesh binding: missing component"] * 5 + ["sesh binding: null component"] * 10
            + ["sesh submit: invalid operation"] * 2 + ["sesh submit: unauthenticated or wrong identity scope"] * 7
            + ["sesh submit: revision conflict", "sesh submit: identity reused with changed intent",
               "operations add: reserve larger storage", "sesh submit: revision overflow"] + TEXT_ERRORS
            + ["sesh submit: unauthenticated or wrong identity scope"],
    "text": ["session text: missing destination"] * 2 + ["session text: truncated"] * 2 + ["session text: missing format"],
}
with tempfile.TemporaryDirectory(prefix="sesh-classes-", dir=os.environ.get("TMPDIR")) as directory:
    directory = Path(directory)
    for mode, sanitizer in (("strict", []), ("asan-ubsan", ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]),
                            ("tsan", ["-fsanitize=thread"])):
        flags = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-UNDEBUG",
                 "-mcpu=apple-m1", "-mmacosx-version-min=14.0", *sanitizer,
                 "-I" + str(SRC), "-I" + str(ROOT / "ecosystem/repos/vexspoke/src"),
                 "-I" + str(ROOT / "ecosystem/repos/api-haven/src"),
                 "-I" + str(ROOT / "ecosystem/repos/graphvex/src")]
        owners = sorted((SUITE / "session").glob("*_test.c")) + sorted((SUITE / "lang").glob("*_test.c"))
        if mode == "tsan":
            owners = [SUITE / "session/sesh_test.c"]
        for owner in owners:
            name = owner.stem.removesuffix("_test")
            binary = directory / (mode + "-" + name)
            subprocess.run([*flags, str(owner), *map(str, SOURCES), "-o", str(binary)], check=True, timeout=60)
            for scenario in (("normal", "invalid") if name in EXPECTED else ("normal",)):
                result = subprocess.run([str(binary), scenario], capture_output=True, text=True, timeout=15)
                result.check_returncode()
                lines = result.stderr.splitlines()
                assert all(re.fullmatch(r"\[vex\] (\w+\.c|text\.h):\d+: .+", line) for line in lines), result.stderr
                messages = [re.sub(r"^\[vex\] [^:]+:\d+: ", "", line) for line in lines]
                assert messages == (EXPECTED[name] if scenario == "invalid" else []), (name, messages, result.stderr)
            print(f"PASS: {mode} {owner.relative_to(SUITE)}", flush=True)
        if mode == "strict":
            for name, invalid_count in (("SeshClient", 2), ("Workspace", 1), ("Workspace", 3), ("Resource", 1), ("Resource", 4),
                                        ("Operation", 1), ("Operation", 6), ("Operations", 1), ("Operations", 3),
                                        ("AuthService", 1), ("AuthService", 4), ("Sesh", 1), ("Sesh", 6)):
                probe = directory / "unsupported.c"
                args = ",".join("1" for _ in range(invalid_count))
                probe.write_text(f'#include "lang/sesh.h"\nint main(void) {{ {name} x = {name}({args}); (void)x; }}\n')
                result = subprocess.run([*flags, "-fsyntax-only", str(probe)], capture_output=True, text=True, timeout=15)
                assert result.returncode != 0 and f"{name}_{invalid_count}" in result.stderr, result.stderr
            probe = directory / "wrong-type.c"
            probe.write_text('#include "graphics/render_loop.h"\n#include "lang/sesh.h"\nint main(void) { Client graphics = {0}; SeshClient *session = &graphics; (void)session; }\n')
            result = subprocess.run([*flags, "-fsyntax-only", str(probe)], capture_output=True, text=True, timeout=15)
            assert result.returncode != 0 and "incompatible pointer types" in result.stderr, result.stderr
print("PASS: seven classes + formatter/arity/interop owners; local serialized metadata only, no distributed/provider proof")
