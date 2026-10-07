# Relational engine owner tests

Tests for `personal/relational-engine`, maintained in the independent tests
repository rather than production source. Run from the workspace root:

```sh
python3 tests/relational-engine/rust/run.py
python3 tests/relational-engine/scaffold_test.py
```

`rust/Cargo.toml` registers `memory_owner`: byte ownership, growth, stale IDs,
UTF-8 and macro construction, plus FFI rejection/recovery. The runner executes
debug/release tests, the engine doctest, an actual strict C23 static-library
client, C-client ASan/UBSan and expected E0502 borrow rejection. Subprocesses
have 90-second watchdogs; generated output stays in temporary directories.

`scaffold_test.py` checks C IDE metadata and ignores, documentation markers,
and warnings-denied Cargo discovery. It does not exercise imported C behavior.

Needs the sibling workspace `personal/relational-engine` checkout, Cargo,
rustc, clang and CMake. Current evidence is macOS only. Rust internals are not
sanitizer-instrumented here; OOM injection, concurrency, Vexspoke allocator
equivalence and Windows remain gaps. No interactive visual tests.
