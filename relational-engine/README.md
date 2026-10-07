# Relational engine owner tests

Tests for `ecosystem/repos/relational-engine`, maintained in the independent tests
repository rather than production source. Run from the workspace root:

```sh
python3 tests/relational-engine/rust/run.py
python3 tests/relational-engine/scaffold_test.py
```

`rust/Cargo.toml` registers nine independent owner targets:
- `memory_owner`: `rust/nio/mem_test.rs` (ownership, growth, stale IDs, macros).
- `string_owner`: `rust/primitives/string_test.rs` (byte and UTF-8 projections).
- `atomic_string_owner`: `rust/primitives/atomic_string_test.rs` (snapshot lifetime,
  caller budget rejection, and barrier-driven reader/writer publication).
- `ffi_memory_owner`: `rust/ffi/memory_test.rs` (FFI rejection/recovery).
- `chunk_owner`: fixed typed rows, alignment, OOM/retry, drops and projections.
- `chunked_list_owner`: stable addresses across growth, OOM/retry and lifetime.
- `variable_slot_owner`: 32-byte layout, all byte/name boundaries and arities.
- `variable_registry_owner`: actual C search, stable bindings and borrowed values.
- `ffi_variable_registry_owner`: every registry extern and output preservation.

These directories mirror `rust/src/{nio,primitives,ffi}` in the engine. The companion
C client is `rust/ffi/memory_test.c`; the compile-negative borrow fixture is
`rust/nio/borrow_rejection.rs`. The runner executes
debug/release tests, the engine doctest, an actual strict C23 static-library
client, C-client ASan/UBSan and expected E0502 borrow rejection. Subprocesses
have 90-second watchdogs; generated output stays in temporary directories.
The runner also builds and runs `tests/vexspoke/nio/relational_memory_test.c`
against the real engine, with and without C-client ASan/UBSan. This proves the
opt-in extern handshake, not the default allocator or Hotcwap reload integration.

`scaffold_test.py` checks C IDE metadata and ignores, documentation markers,
and warnings-denied Cargo discovery. It does not exercise imported C behavior.
`preferences_test.py` checks the R2 responsibility layout and explicit planned
scope. io/compress/virtual modules have no runtime behavior to prove; mmap, file
indexing, codecs and GPU transfer remain unimplemented. Primitive owners also
execute legacy `text` aliases. C `search/primitives/name_search_test.c` executes
span boundaries and exact diagnostic counts under ASan/UBSan. A real C registry
client asserts ABI layout, growth, pointer rebinding and borrowed-value teardown.
`storage_rejection.rs` proves unsupported macro arities/types and exclusive Rust
borrows fail for the intended diagnostic. New fault injection is thread-local,
deterministic, and scoped to isolated owner executables; no timing sleeps.

Needs the sibling workspace `../../ecosystem/repos/relational-engine` checkout, Cargo,
rustc, clang and CMake. Current evidence is macOS only. Rust internals are not
sanitizer-instrumented here; OOM injection, concurrency, Vexspoke allocator
equivalence, deterministic writer-busy injection, automatic record schema
migration and Windows remain gaps. No interactive visual tests.
