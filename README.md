# tests

Independent, tracked test-source repository for the ecosystem. Presence of a
source file is **not** evidence that its target was built or executed.

## Current State

**Implemented:** the shared test-source repository — per-subsystem test trees,
`test-checklist.md`, `test_support.h`, the runner scripts and the proof gates
(`b check`, `b coverage`). **Proven:** the gates prove ownership, surface
invocation and function execution for **vexspoke only** (macOS host); everything
else below is unproven. **Platforms proven:** macOS (the executor host).

## Scope and Limitations

**Scope:** tracked test sources and their build/run wiring, one owner test per
production unit (the Test Tree Mirror Law), plus the timestamped evidence ledger.
**Deliberately not covered:** production sources (they live in their own repos);
visual/monitor acceptance (the user's); app/GPU/audio runtime proof.
**Known limits and gaps:** value boundaries, failure recovery, sanitizer
cleanliness, concurrency, GPU behavior and non-macOS platforms are not yet proven
— see `test-preferences.md` and "What is not yet proven" below. A passing
documentation check proves no application runtime.

## Run

### Editor

Build/run with [b](https://github.com/vex-graph/b). The CMake adapters provide
CLion navigation, diagnostics and inlay hints; they do not replace b's graph.
Unlike the per-repo metadata-only object targets, this existing test adapter
also offers native test Run/Debug targets. That opt-in testing seam is retained.

The workspace build system is `b` (standalone repo at `personal/b/`); it is the
source of truth for include paths, libraries, and targets. Open either the
workspace-root `CMakeLists.txt` or this repository's `CMakeLists.txt` in CLion
for C23 syntax analysis and native test Run/Debug targets. Both consume the
same `b ide` metadata; CMake is an IDE adapter, not another build graph.
The tests-only entry requires the parent workspace, or an explicit
`-DVEXGRAPH_WORKSPACE_ROOT=/path/to/vexgraph`. Window/GPU/UI CTest execution is
disabled by default. Terminal execution remains `./tools/b run <target>` or
`tests/run.sh <name>`.

### Terminal

```sh
tests/run.sh              # list the friendly names
tests/run.sh <name>       # build + run one target through the umbrella `b`
```

Any other name is handed straight to `b run` (partial names ok). This script
replaces the former C runner (`main.c`). `b` compiles on demand; see the
umbrella `README.md`. `b test [substr]` builds and runs every test target; the
static proof gate below runs first, so a suite cannot go green while a unit
lacks an owner or a public function goes uninvoked.

## R2 ownership and proof

R2 has two cooperating owners: Vexspoke CPU computation, math, algorithms,
synchronization and behavior, and Relational Engine memory/storage, stable rows,
variable bindings and native C search over Rust-owned spans. Migration is staged;
retained Vexspoke memory/container ABI and default allocator still need their
existing owners. Engine tests do not confer allocator parity, C/Rust atomic-layout
equivalence, automatic schema migration or R1 reload integration.

Engine owner/build instructions live in [relational-engine/README.md](relational-engine/README.md).
Use `python3 tests/relational-engine/rust/run.py` from the workspace root; do not
assume `b test` or the Vexspoke-only gates below include every Rust/C owner.
R1 owns residency/lifetimes; GPU shaders/dispatch remain Graphvex R3. The R5 apps
are unfinished, and a passing documentation check proves no application runtime.

## Timestamped evidence ledger

`test-checklist.md` inventories every framework, with directory tables and one
row per file, plus shared tests and workspace tools. ✅ means its automated lab
command passed for its recorded hash and scope, not universal battle readiness
or visual approval. The user tests visual appearance and reports defects.
Descriptions replace authorship; no agent names or session IDs are stored.

```sh
python3 tools/test_checklist.py sync
python3 tools/test_checklist.py run --file tools/agents.sh --scope 'Bash syntax only; runtime unproved' -- bash -n tools/agents.sh
python3 tools/test_checklist.py check
python3 tests/tools/test_checklist_test.py
python3 tests/tools/agents_test.py
```

Consult and update it in the same work cycle, per the Timestamped Test Checklist
Law. Timestamps are actual Unix seconds, not estimates. File changes invalidate
old greens; failures and skips stay ❌. The ledger itself is a generated test
report, not a manually maintained source file.

## Proof gates

The Per-File Battle Test Law and the Public Surface Proof Law are enforced in
the C23 build tool (the workspace engine at `tools/workspace.c`), not by a script.
The baselines are data files under `tests/vexspoke/`; each **may only shrink**.

```sh
b check                    # static: every compiled unit has an owner test, and
                           # the owner names every public function in the unit header
b check --strict           # fail on *any* current gap, not just a new one
b check --list             # print the whole surface worklist
b coverage [substr]        # instrument with clang, run the tests, gate that every
                           # public function is actually executed by its owner test
b coverage --list          # print the unexecuted worklist
```

| baseline | enforced by | records |
| :--- | :--- | :--- |
| `vexspoke/coverage_baseline.txt` | `b check` | units with no owner test |
| `vexspoke/surface_baseline.txt` | `b check` | public functions the owner never names |
| `vexspoke/function_baseline.txt` | `b coverage` | public functions the owner never executes |
| `vexspoke/mirror_exceptions.txt` | `b check` | deliberately non-mirrored owner tests |

Regenerate a baseline after clearing debt: `b check --emit-units`,
`b check --emit-surface`, `b coverage --emit-functions`.

## Skips are not passes

A test that cannot exercise its contract on this host (no window server, no
Vulkan row, blocked loopback) must `return B_TEST_SKIP` — the value 77 from
`tests/test_support.h` — after printing the reason. `b test` then reports
`SKIP` separately from `PASS`, and `b coverage` excludes it. Returning `0` from
a skipped test is a false green and is banned by the Executable Evidence and
Readiness Law. `tests/` is on every test target's include path, so
`#include "test_support.h"` always resolves.

## What is not yet proven

The gates prove **ownership, surface invocation, and function execution** for
vexspoke. They do **not** yet prove value boundaries, failure recovery,
sanitizer cleanliness, concurrency, GPU behavior, or non-macOS platforms, and
they cover vexspoke only. Those gaps are enumerated in `test-preferences.md`;
never mark them passing merely because `b check` and `b coverage` are green.

Keep test assertions enabled in Release test targets; an `assert` erased by
`NDEBUG` is not proof. `b` compiles tests with `-UNDEBUG` for exactly this
reason.
