# Relational engine owner tests

Tests for `personal/relational-engine`, maintained in the independent tests
repository rather than production source. Run from the workspace root:

```sh
python3 tests/relational-engine/rust/run.py
python3 tests/relational-engine/scaffold_test.py
```

`rust/Cargo.toml` registers three independent owner targets:
- `memory_owner`: `rust/nio/mem_test.rs` (ownership, growth, stale IDs, macros).
- `string_owner`: `rust/text/string_test.rs` (byte and UTF-8 projections).
- `ffi_memory_owner`: `rust/ffi/memory_test.rs` (FFI rejection/recovery).

These directories mirror `rust/src/{nio,text,ffi}` in the engine. The companion
C client is `rust/ffi/memory_test.c`; the compile-negative borrow fixture is
`rust/nio/borrow_rejection.rs`. The runner executes
debug/release tests, the engine doctest, an actual strict C23 static-library
client, C-client ASan/UBSan and expected E0502 borrow rejection. Subprocesses
have 90-second watchdogs; generated output stays in temporary directories.

`scaffold_test.py` checks C IDE metadata and ignores, documentation markers,
and warnings-denied Cargo discovery. It does not exercise imported C behavior.

Needs the sibling workspace `personal/relational-engine` checkout, Cargo,
rustc, clang and CMake. Current evidence is macOS only. Rust internals are not
sanitizer-instrumented here; OOM injection, concurrency, Vexspoke allocator
equivalence and Windows remain gaps. No interactive visual tests.
