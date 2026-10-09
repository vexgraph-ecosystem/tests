# vexgraph — Test Preferences (Ecosystem Test Laws)

This is the ecosystem's proof lawbook. It defines the evidence required before a production file,
class, component, or subsystem is called tested across the R1–R5 ecosystem. It governs **proof and
readiness only**; universal architecture is governed by the real workspace-root `preferences.md`
(the Living Documentation Law). When a test law is cited, cite its **Title** — never a position,
never a number (the Law Identity Doctrine).

## How to use this document

1. **Pick one subsystem** in Part II. Each is self-contained.
2. **Open its "Seams to Prove" table.** Every row is one owning test; the row names the target test
   file and the seam it must exercise.
3. **Write the owner test**, wire it into a named build/run target that compiles with
   `-Wall -Wextra -Werror` and exits nonzero on any failed check, then run it.
4. **Record readiness** on the status ladder of the Per-File Battle Test Law: **unmapped** →
   **mapped but unwired** → **failing** → **passing with stated gaps** → **battle tested**.
5. **Never promote a whole layer from partial file results.** A subsystem is battle tested only when
   every applicable law here has executed evidence.
6. **Read and maintain `tests/test-checklist.md` across every part**, per the
   Timestamped Test Checklist Law in workspace `preferences.md`. Record actual
   Unix timestamps, subject hashes, command/scope, result, and description after
   each automated lab check. Do not record authors or session identifiers.
   Visual appearance is user-tested; manual visual checks are outside this ledger.
   Unknown, failed, skipped, or stale evidence is ❌; a scoped ✅ is not automatically
   battle-tested readiness or visual acceptance.

Part I binds everything. Part II specializes for a subsystem. Part III governs repos that are still
blueprints, so their first real commit lands already governed.

## Table of Contents

**Part I — Universal Test Laws** (bind every repo and every file)
- Per-File Battle Test Law
- Test Tree Mirror Law
- Contract Before Cases Law
- Public Surface Proof Law
- Value Boundary Matrix Law
- Pointer, Identity, and Dereference Safety Law
- Arity and Variadic Dispatch Law
- Failure Atomicity and Recovery Law
- Failure Observability Law
- Determinism and Reproducibility Law
- Resource and Lifetime Law
- Concurrency and Bounded Progress Law
- Adversarial and Hostile-Input Proof Law
- Hot-Path Minimal Guard Law
- Darling Component Narrative and Focus Law
- Runtime-Level Seam Law
- Executable Evidence and Readiness Law

**Part II — Subsystem Test Laws**
- hotcwap Test Laws (R1 host)
- vexspoke Test Laws (R2 computation/behavior; retained storage ABI)
- relational-engine Test Laws (R2 memory/storage/native C search)
- graphvex Test Laws (R3 GPU)
- api-haven Test Laws (R3 API)
- darling Test Laws (R4 UI)
- b Build Tool Test Laws (standalone tooling)

**Part III — Blueprint Repos** (stub-stage; rules provisional)
- language Test Laws
- darkbase Test Laws
- sesh Test Laws
- samplerate Test Laws

---

# Part I — Universal Test Laws

## Per-File Battle Test Law

### Definition:
The smallest unit of testing readiness is one production file at one runtime level. A public class's
`.h`/`.c` pair is one file unit; a header-only file, implementation-only file, Objective-C file,
shader, or generated interface is also a file unit with its own proof.

### The Why:
Subsystem-wide green results can hide an untested file. A neighboring class working does not
establish that this file's constructors, operations, failure paths, and cleanup work.

### The Rule:
- Every production file unit has an owning test file in the matching `tests/<subsystem>/` tree, in the
  directory that mirrors the unit's source directory (the Test Tree Mirror Law), named for that unit,
  plus a short inventory of its public surface and applicable requirements from this document. A file
  with no callable public API still gets an owner test that exercises its behavior
  through the nearest stable seam; record that seam explicitly.
- Each owner test proves the file's normal behavior, applicable value boundaries, invalid inputs,
  failure state, lifetime, and concurrency contract. It runs independently enough that its result
  identifies the file that failed. Shared fixtures are allowed; a single broad integration test does
  not replace the owner test.
- For Rust units, mirror each owning module under `tests/relational-engine/rust/`
  and use its registered Cargo owner target. Test both debug and release with
  warnings denied. Compile/run native C clients of the public ABI with assertions
  and `-Wall -Wextra -Werror`; a Cargo build alone proves no C boundary.
- For an `.h`/`.c` pair, compile the public header as a client and run the implementation behavior.
  Header-only macros and inline functions require compile and runtime proof as applicable.
  Platform-specific implementations require proof on every supported platform or an explicit unproved
  status.
- Record file-level status as **unmapped**, **mapped but unwired**, **failing**, **passing with
  stated gaps**, or **battle tested**. The last status requires every applicable law here to have
  executed evidence. Never promote an entire layer from a partial set of file results.
- Add cross-file and cross-level integration tests after file-level proof. They test seams and
  composition; they do not confer readiness on an individual file whose own owner test is missing.

---

## Test Tree Mirror Law

### Definition:
A subsystem's test tree mirrors its production source tree. For a production unit at
`<repo>/<dir>/<unit>.c`, the owning test is `tests/<subsystem>/<dir>/<unit>_test.c` — the same
relative directory, with the unit name suffixed by `_test`. A seam test that spans directories or
levels sits in the mirrored directory of its primary subject.

### The Why:
A flat `tests/<subsystem>/` dump becomes unreadable as the ecosystem grows: hundreds of files in one
directory, no signal which source directory an owner test belongs to, and no mechanical way to answer
"is this file tested?". Mirroring the source tree makes ownership a path lookup, keeps each directory
self-describing, and preserves the Per-File Battle Test Law's one-owner-test-per-unit mapping.

### The Rule:
- Mirror the production directory: `ecosystem/hotcwap/window/window.c` maps to
  `tests/hotcwap/window/window_test.c`.
- One owner test per file unit, named `<unit>_test.c` in the mirrored directory.
- A unit with no directory (a repo-root file) keeps its owner test at `tests/<subsystem>/<unit>_test.c`.
- A seam test lives in the mirrored directory of its primary subject; record the seam in the test header.
- The Test Segregation Law still forbids any test file inside a production source tree.

---

## Contract Before Cases Law

### Definition:
Each production file's public API has a written contract that states valid inputs, invalid inputs,
ownership, thread affinity, result and error semantics, and the state after failure. Tests prove
that contract.

### The Why:
An assertion cannot establish correctness when the expected behavior is unstated. A crash, silent
no-op, partial write, and explicit error can all look like plausible responses to the same invalid
call unless the contract chooses one.

### The Rule:
- Test the documented behavior at the public seam. If the contract is missing, define it before
  claiming the test is complete.
- Distinguish an invalid input from caller misuse outside the API's contract. A test may expose an
  unsafe contract, but must not silently invent one.
- Record preconditions, observable result, output parameters, callbacks, and postconditions for both
  success and failure.
- A failed operation must have an explicit disposition: unchanged, rolled back, partially applied
  with documented progress, or terminal. Assert that exact disposition.

---

## Public Surface Proof Law

### Definition:
Every public function, constructor form, macro dispatch, callback entry, and destructor declared by a
file has test evidence in that file's owner test.

### The Why:
Code that links but is never invoked by a test can fail on its first real use. An unused public
function is a coverage gap even when nearby functions pass.

### The Rule:
- Maintain a per-file-unit inventory of public declarations and the tests that exercise them. Every
  declaration maps to an executable test or a stated, reviewable reason for a different proof method.
- Exercise each meaningful return path and output parameter. For getters and setters, assert the
  round trip and the underlying observable state.
- Prove callback registration, delivery, removal, and exactly-once behavior where callbacks are part
  of the API.
- Count only tests that are built and run by the relevant test target. A source file sitting in
  `tests/` is not evidence by itself.

---

## Value Boundary Matrix Law

### Definition:
Every applicable public input domain is tested at its identity, normal, boundary, and invalid values.

### The Why:
Most defects hide at the transitions between absent, empty, full, and invalid states rather than at
an ordinary example value.

### The Rule:
- Numeric inputs: test zero, one, negative values where representable, documented minima and maxima,
  and values just beyond each valid boundary. Include overflow and underflow for arithmetic and size
  calculations.
- Strings and byte spans: test `""`, a one-element value, embedded NUL when length-based, malformed
  encoding when decoded, exact capacity, one byte over capacity, and a zero-capacity destination
  where applicable.
- Collections and object values: test an empty instance (`{}` or equivalent), one element, growth
  past initial capacity, removal to empty, duplicates, missing keys, and out-of-range indices where
  applicable.
- Floating-point inputs: test `-0.0`, non-finite values, and precision or rounding boundaries when
  the API accepts floating-point data.
- Pointer inputs: test `nullptr` separately for each nullable public parameter, including optional
  output pointers. Assert the specified safe result and that unrelated state remains intact.
- Do not force a meaningless case onto an unrelated API. Mark the case inapplicable with its contract
  reason in the test inventory.

---

## Pointer, Identity, and Dereference Safety Law

### Definition:
Pointer-bearing APIs must prove that they validate the pointer properties they claim to validate
before dereferencing or mutating through them.

### The Why:
A pointer can be non-null and still be the wrong object, wrong generation, wrong type, or wrong
address within an allocation. `nullptr` tests alone do not protect the relational object model.

### The Rule:
- For managed handles and objects, test wrong project/type IDs, valid objects of another class, stale
  generations, borrowed versus owned pointers, and interior or misaligned pointers when the API
  promises to reject them.
- A rejected pointer must not be dereferenced, free an unrelated object, or modify the destination.
  Assert the rejection and preserved state.
- Do not pass an arbitrary unmapped address into an ordinary test process and assume it is safe to
  inspect. When a contract promises rejection of such an address, use an isolated child process with
  a bounded timeout and a clear crash expectation; use memory sanitizers for invalid dereference and
  use-after-free defects. A sanitizer finding fails the proof.
- Test wrong type ID and wrong object pointer independently: a forged ID and a genuine pointer to
  another type exercise different failure paths.

---

## Arity and Variadic Dispatch Law

### Definition:
All advertised constructor overloads, macro selectors, and variadic functions must dispatch and
consume arguments according to their declared arity.

### The Why:
Macro counting and variadic forwarding often work for small examples but fail at zero arguments, at
an advertised maximum, or after argument promotion.

### The Rule:
- Compile and run every supported `Class_0`, `Class_1`, and later constructor form, plus the
  corresponding `Class(...)` chooser calls. Assert which form ran and the constructed value.
- Compile-negative tests must reject unsupported argument counts and wrong types where the C23
  interface can enforce them. They must assert failure for the intended diagnostic, not for an
  unrelated build error.
- For an API advertised as variadic without a smaller documented limit, test zero where valid, one,
  several, and **at least ten arguments** in one call. Check order, count, sentinel handling,
  promoted types, and final state.
- An API with a documented finite maximum tests the maximum and its next invalid count. Do not treat
  a ten-argument probe as a reason to read beyond the arguments actually supplied.

---

## Failure Atomicity and Recovery Law

### Definition:
When an operation fails, tests prove what happened to every state it could have touched and whether
a later valid operation can recover.

### The Why:
A returned error can hide a damaged buffer, leaked allocation, advanced generation, lost callback,
half-written file, or partially swapped module.

### The Rule:
- Capture a meaningful pre-operation snapshot. Inject failure at each independently reachable stage:
  invalid input, allocation failure, callback rejection, short read/write, timeout, cancellation, and
  downstream error.
- Assert the documented post-failure state, resource ownership, generation, output buffer, and error
  code. Check that rollback restores both data and behavior, including the old callable
  implementation after a failed hot swap.
- Retry after invalid input and after a transient failure. Prove whether retry succeeds, remains
  safely rejected, or requires an explicit reset, as the contract states. Repeating the same invalid
  call must not corrupt state.
- Test backup and restore with a real change between them. Verify the restored content, identity,
  metadata, and ability to continue operating. A backup file merely existing is insufficient.
- Test error propagation and catching at the boundary that handles it: rejection must reach the
  caller, cleanup must run, and an error must not be swallowed or reported as success.

---

## Failure Observability Law

### Definition:
A rejection the contract promises must be **observable**. The caller receives a defined result
(return code, `false`, sentinel, or safe default), and on the cold path the failure is reported
through `THROW(...)` (the THROW Law) — one `[vex] <file>:<line>: <message>` line to stderr — or
another channel the test can assert (error code, counter). Failure is never silent where the
contract says it rejects.

Vexspoke's `THROW` reports a recoverable cold rejection; Hotcwap's
`FATAL_THROW` is a separate process-terminating operation. The owning tests
must check the safe return or exit **and** the diagnostic, and normal paths
must produce no unexpected rejection report. Debug-only probes may be removed
from Release; bounds and external-input validation needed to avoid undefined
behavior remain. Sanitizer and isolated crash tests catch faults that never
reach a reporter.

### The Why:
A crash, a silent no-op, and a correct rejection are indistinguishable if nothing reports which
happened. A success returned on a 5xx response, or a discarded library-close failure, is exactly this
defect class: the operation failed and nothing said so.

### The Rule:
- Every promised rejection has an asserted observable channel. Assert the return value **and**, on
  cold paths, the reported `THROW` message (`[vex] <file>:<line>: <message>`) or code.
- Log-once determinism is testable: a rejection emitted once per call is asserted once, not raced.
- Hot paths stay quiet per the Cold-Strict, Hot-Minimal Validation Law; observability is a cold-seam
  obligation, not a per-frame cost.
- A swallowed error is a defect. If the disposition is "ignore", the contract must say so and the
  test must assert the caller still observes a correct result.

---

## Determinism and Reproducibility Law

### Definition:
A passing test asserts the **same result for the same input** on every run. No test outcome depends
on clock, scheduler luck, address layout, or an unseeded random source.

### The Why:
Flaky green is worse than red: it hides regressions and silently corrupts the readiness claim. For a
substrate built on predictable, deterministic calculation, nondeterminism in the proof is
self-contradictory.

### The Rule:
- Seed every random source and record the seed in the test output. A run that cannot be reproduced
  is not evidence.
- Never synchronize with a sleep. Use barriers, latches, or explicit state probes to establish
  ordering; vary delays only to explore interleavings, never to make correctness depend on a lucky
  duration.
- Where an exact oracle is impractical, use property or metamorphic tests (invariants that must hold
  for all inputs) and state the property.
- A test that fails intermittently is a defect to diagnose, never a result to rerun until green.

---

## Resource and Lifetime Law

### Definition:
Creation, borrowing, ownership transfer, shutdown, and destruction are observable parts of an API's
behavior.

### The Why:
Successful outputs can still conceal leaks, double frees, dangling references, or shutdown in the
wrong order.

### The Rule:
- Exercise create/use/free, failed construction, repeated initialization and teardown where
  supported, and cleanup after partial initialization.
- Prove borrowed objects remain usable after the borrower releases them, and transferred objects are
  released exactly once by the new owner.
- Run memory checking on tests that exercise manual allocation. An identified leak, invalid access,
  or use-after-free fails the proof.
- For R1–R5 integration, test reverse teardown order and cancellation before freeing resources a
  worker, GPU submission, or callback may still use.

---

## Concurrency and Bounded Progress Law

### Definition:
An API's thread-safety claim requires tests of both shared-state correctness and bounded forward
progress.

### The Why:
A single writer test cannot reveal lost updates, observer calls on the wrong thread, deadlock,
starvation, or teardown races.

### The Rule:
- State whether each public operation is thread-safe, owner-thread-only, or requires external
  synchronization. Test only legal concurrent use as a conformance requirement; test illegal use only
  when rejection is promised.
- For thread-safe operations, use synchronized starts and multiple readers and writers. Assert final
  counts, ordering or linearization guarantees, and the absence of duplicate or lost work. Repeat with
  varied schedules and seeds.
- Test callback thread affinity, cancellation during work, teardown while work is pending, and join
  completion within the Bounded Wait Law's contract.
- For spin locks and atomics, test acquire/release visibility, contention, unlock handoff, and
  progress under load. A test must have an external bounded watchdog; it must never hang the suite.
- Use thread and address sanitizers where supported, with separate runs if their runtimes conflict.
  Report platform skips explicitly.

---

## Adversarial and Hostile-Input Proof Law

### Definition:
Every public seam that consumes **untrusted input** — Bytes, sizes, indices, pointers, paths, URLs,
filenames, binary modules, network frames, or serialized data — has an adversarial battery that feeds
malformed, oversized, truncated, and hostile values at the public cold seam.

### The Why:
Untrusted input is how crashes, integer overflows, out-of-bounds access, path traversal, injection,
server-side request forgery, and decompression bombs enter the system. Kind tests on well-formed input
prove nothing about the hostile case, which is the case an attacker supplies.

### The Rule:
- Enumerate the untrusted seams per file. Every one has an adversarial battery that runs under
  memory, undefined-behavior, and where relevant thread sanitizers.
- Cover size arithmetic: overflow (`len + 1`, `count * sizeof`, capacity doubling, `total + weight`),
  underflow, `UINT*_MAX` boundaries, negative-where-unsigned, and exact-capacity plus one-over.
- Cover encoding: embedded NUL, invalid UTF-8, overlong sequences, and truncation mid-token.
- Cover structure: deep nesting, huge element counts, cyclic or self-referential input, and
  duplicate keys.
- Cover trust boundaries: path traversal, symlink and hardlink, check-then-use races, redirects,
  CRLF and header injection, and binary/ABI mismatch for loaded modules.
- Every adversarial test has an external bounded watchdog and must never hang the suite. A sanitizer
  finding fails the proof.

---

## Hot-Path Minimal Guard Law

### Definition:
A hot-path getter or access validates **nothing beyond a single entry guard** — one `nullptr` test,
or one range test — and returns the Contract's safe default. It never logs, never allocates, and never
re-validates per element. Where it performs a deliberately unchecked, trust-the-handle read, that
hot path is declared `;;HOTCODE`, the hot-path declaration marker.

### The Why:
Re-validating every element on a frame path is how frames die (the Cold-Strict, Hot-Minimal Validation
Law). A numeric or field getter must be a load plus a branch, not a validation pass. The checks belong
at the cold seam that admitted the handle; the hot read trusts it. This law proves the split is real
instead of assumed.

### The Rule:
- A hot-path getter carries exactly one entry guard (nullptr, or a single range test) and returns the
  safe default; assert both the guarded path and the value.
- Assert the absence of cost: no logging and no allocation across a bounded run of calls.
- The hostile matrix (nullptr variants, wrong pointer, overflow, out-of-range) lives at the **cold
  seam** under the Pointer, Identity, and Dereference Safety Law — never on the hot getter.
- Every `;;HOTCODE` marks a hot site; a test proves the cold validator (`;;CHECKER`) rejects the
  hostile input and the marked site is reachable only with a validated handle.
- `grep -rn ';;HOTCODE'` lists every hot site; a new `;;HOTCODE` without a covering hot-path guard
  test is a gap.

---

## Darling Component Narrative and Focus Law

### Definition:
Every UI component test explains the component's purpose and proves its visible interaction contract,
including focus when the component can receive input. This law is stated generally here and made
concrete for the toolkit in the darling Test Laws.

### The Why:
A list of assertions can pass while the component still fails its user-facing job. Focus, dispatch,
and painting are especially easy to test in isolation while their interaction is broken.

### The Rule:
- Open each component test with a short plain-language description: what the component is for, what a
  user does with it, and which behaviors the test proves. Give each scenario a descriptive name and
  its expected outcome.
- For focusable components, test initial focus, acquisition by pointer and keyboard where supported,
  traversal order, focus loss, disabled and hidden behavior, modal capture, destruction of the
  focused component, and focus restoration. Assert which component receives each key event.
- Test pointer capture, inside/outside hit results, callback count, layout and native-pixel
  boundaries, dirty marking, and present-on-demand when relevant.
- Use headless tests for deterministic behavior and a separate integration probe for behavior that
  needs an actual window, compositor, input system, or graphics device. A headless mock must not be
  presented as proof of hardware behavior.

---

## Runtime-Level Seam Law

### Definition:
Each runtime level has local tests and tests at the boundary where it depends on or supervises
another level. This law is stated generally here and instantiated per subsystem in Part II.

### The Why:
Correct classes can fail when ownership, threading, type identity, pixels, or errors cross a runtime
boundary.

### The Rule:
- R2 `vexspoke`: prove CPU computation, math, algorithms, synchronization and
  behavior, plus retained memory/container/type ABI during staged migration.
- R2 `relational-engine`: prove Rust-owned allocation/storage, stable row chunks,
  variable bindings, native C search over borrowed spans and lifetime. No default
  allocator replacement, C/Rust atomic-layout equivalence or automatic schema
  migration is inferred. R1 residency/consumer reload needs separate integration
  proof; GPU shaders/dispatch remain Graphvex R3.
- R3 drivers (`graphvex`, `api-haven`, `language`, `darkbase`): prove protocol parsing, malformed
  input, cancellation, storage durability or GPU resource retirement as applicable, plus their R2
  boundary.
- R1 `hotcwap`: prove boot and teardown, bounded waits, manifest rejection, hot swap rollback, retry,
  and supervised worker or window lifecycle.
- R4 (`darling`, `sesh`): prove input routing, layout, focus, compositing, session ordering, and
  recovery after disconnect as applicable.
- R5 applications: prove representative user workflows, save/restore or undo/redo where offered, and
  integration across the levels they actually use.
- Keep fault injection at the public cold seam and use minimal guards on hot paths, consistent with
  the Cold-Strict, Hot-Minimal Validation Law.

---

## Executable Evidence and Readiness Law

### Definition:
"Battle tested" is a recorded, repeatable result for each production file with declared limits, not a
label attached to a large test file.

### The Why:
Unwired tests, disabled assertions, skipped platforms, and nondeterministic passes create false
confidence.

### The Rule:
- Place tests in `tests/<subsystem>/`, mirroring the production source directory (the Test Tree Mirror
  Law), in the umbrella workspace, or a standalone repository's root `tests/` directory, per the Test
  Segregation Law. Keep generated artifacts out of source control.
- Wire every test into a named build and run target. Compile with `-Wall -Wextra -Werror`; keep
  assertions active in release-like test builds. A suite exits nonzero on any failed check.
- Run deterministic unit and seam tests for each change. Run relevant integration, sanitizer,
  concurrency stress, and platform tests before claiming a changed subsystem battle tested. Record
  command, platform, configuration, seed, skips, and result.
- A skip is neither a pass nor a failure. State which claim remains unproved. Flaky tests are defects
  to diagnose, not results to rerun until green. A test that cannot exercise its contract on this host
  must `return B_TEST_SKIP` (77, from `tests/test_support.h`) after printing the reason to stderr —
  never `return 0`. `b test` reports SKIP separately from PASS; a skip contributes no coverage and
  must never be read as green.
- Each file's readiness claim requires its owner test, public-surface inventory, applicable boundary
  matrix, failure and lifetime proof, and executed test results. A changed contract updates its tests
  and this document in the same development cycle.

---

# Part II — Subsystem Test Laws

Each subsystem section carries its own laws, its own "Seams to Prove" ledger, and the Part I laws as
applied to it. A row on a ledger is one owning test.

---

## hotcwap Test Laws (R1 host)

The supervisor: kernel, registries, manifest, hot reload of dylib modules, window lifecycle,
teardown. Everything here is about **swapping live code safely** and **refusing to trust a binary**.

### Two-Dylib Swap Law
**Proves:** the loader runs two real dylibs and preserves behavior across a live swap.
- Boot with dylib A; drive it; swap to dylib B; assert state preserved through
  `Hot_save`/`Hot_restore`/`Hot_migrate`, the generation advanced exactly once, and A remains mapped
  in the retire ring while B is live.
- Trampoline rows keep a valid `fallback_ptr`; a mid-swap call never dereferences a null trampoline.
- A section present in A and absent in B is kept mapped and never retired.

### Stale and Wrong Binary Law
**Proves:** a bad module is rejected fail-closed, rolled back, and self-heals.
- Missing `VkModuleGetTrampolines`, foreign-magic, wrong-ABI/version, and wrong-table-layout dylibs
  are each rejected with the correct `HOT_ERROR_*`.
- The advertised Dynamic Module ABI Verification Law is either implemented and tested or is a stated,
  documented gap.
- After a rejected swap, the old module still runs; the next poll with a good binary succeeds
  (self-heal). A stale generation stamp is detected as never-installed, not silently adopted.

### Manifest Resilience Law
**Proves:** the manifest is created when absent and never trusted when corrupt.
- A missing manifest is **created**; `Manifest_init` succeeds and the ladder exists.
- Empty, truncated, duplicate-key, and corrupt manifests clear the mount and return failure without
  overwriting the existing file; a partial parse never leaves a half-mounted catalog.
- Section stems and library keys are validated; unvalidated names are rejected.
- `MANIFEST_UPDATE`/`PROMOTE` move the ladder (`bin/new` → `current` → `previous` → `backward`) and
  bump the generation stamp; each stage is asserted.

### Loader Trust Boundary Law (security)
**Proves:** loading a binary is a deliberate, bounded trust decision.
- A symlink swapped between scan and `dlopen`, and between verify and copy, is detected or the TOCTOU
  window is closed (fd-bound open); assert the attack fails.
- `DYLD_*` / search-path hijack and dependency interposition do not divert the loaded module.
- Permissions and ownership of `bin/current` and `manifest.json` are validated before `dlopen`.
- `VEX_MANIFEST` and `HOME` overrides cannot escape the install root (path traversal rejected).
- Unsigned-binary adoption is a **stated** decision with an asserted reason, never silent.

### Retire-Ring Overflow Law
**Proves:** the retirement ring survives more swaps than it has slots.
- 17+ swaps drive the 16-slot ring; the oldest handle is closed only when no thread can still hold
  it.
- A `dlclose` failure is observed and reported, never discarded.
- Concurrent calls into a retiring module are safe until the grace count elapses.

### Teardown and Bounded Wait Law
**Proves:** shutdown joins workers before freeing, in reverse boot order.
- The state-save worker is joined **before** module handles are torn down; a save in flight during
  shutdown completes or cancels safely.
- `HotShutdown`, `Kernel_stop`, and `Kernel_free` obey the Teardown Order Law; `Kernel_free` refuses
  while anything is registered.
- All waits are bounded (the 25 ms save slices and 100 ms console slices); no wait is unbounded.

### Install Ledger Law
**Proves:** the machine remembers an install outside the tree.
- A recorded install reads back; `UNINSTALL` keeps the record (state uninstalled), so a wiped tree is
  not a fresh install; an explicit `forget` purges it.
- The ledger is a plain per-user state file outside the tree — a correctness record, not a secure
  store. The test redirects `HOME` to a scratch dir so it never touches the real state directory.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Two-Dylib Swap | `tests/hotcwap/hot/two_dylib_swap_test.c` | `Hot_poll` / `perform_swap` |
| Stale and Wrong Binary | `tests/hotcwap/hot/wrong_binary_test.c` | `dlopen` / `dlsym` gate |
| Manifest Resilience | `tests/hotcwap/hot/manifest_adversarial_test.c` | `catalog_seed`/`load`/`apply` |
| Loader Trust Boundary | `tests/hotcwap/hot/loader_trust_test.c` | `readdir` → `dlopen` window |
| Retire-Ring Overflow | `tests/hotcwap/hot/retire_ring_overflow_test.c` | `HotRetireRing_*` |
| Teardown and Bounded Wait | `tests/hotcwap/hot/shutdown_order_test.c` | `HotShutdown` |
| Install Ledger | `tests/hotcwap/hot/ledger_test.c` | `Ledger_*`, Keychain backend |

### Owner coverage

Every hotcwap production unit that builds on macOS has an owning test in the mirrored tree
(`tests/hotcwap/<dir>/<unit>_test.c`): the loader family (`hot_test`, `hot_trampoline_test`,
`hot_retire_test`, `hot_behavior_test`, `ledger_test`, `throwable_test`, `manifest_*`), the kernel
(`kernel_function_test`, `kernel_lifecycle_test`, `process_test`, `console_test`, `application_test`),
the spoke bridge (`spoke_test`), and the window subsystem (`window_test`, `window_event_test`,
`bridge_seam_test`, `traffic_light_test`). This is an ownership inventory, not
a current pass report: some listed sources are still unwired or platform-specific.
Record executed target counts and failures from the actual build before claiming
the R1 battery passed.

The window backends are **platform-exclusive** and compiled only on their own host: `window_test` is
the owner test for whichever backend the host builds, so the macOS run proves `window/window_cocoa.m`
and `window/traffic_light_cocoa.m`. `window/window.c`, `window/window_linux.c`,
`window/window_wayland.c`, and `window/window_win32.c` are **explicitly unproved on macOS** — each
requires its own host run before it is claimed battle tested.

---

## vexspoke Test Laws (R2 computation/behavior and retained storage ABI)

Vexspoke owns CPU computation, math, algorithms, synchronization and behavior.
Vexspoke consumes the engine-owned native IO/NIO ABI; those implementation and
owner files have migrated to Relational Engine. Retained container/type/string-pool
behavior stays Vexspoke-owned until its own migration and proof. CPU tests do not
claim that the native arena has been rewritten into Rust.
Everything here is about **predictable calculation** and **pointers that lie**.

### Deterministic Calculation Law
**Proves:** identical input yields identical output, every run.
- Math, calc, hash, sort: property or metamorphic oracles; seeded randomness recorded; no dependence
  on clock, address, or schedule.
- Float boundaries: `-0.0`, NaN, Inf, and precision edges in `StrictMath`, `FastMath`, `Mat4`, and
  `CoordFrame_isValid`.

### Boundary Value Law
**Proves:** zero, negative, empty, and null are first-class cases.
- Every container (`Array`, `List`, `Map`, `Set`, `SparseSet`, `MinHeap`, `Queue`, `Deque`, `Stack`)
  at empty, one, size, size+1, and overflow.
- Values `{}`, `''`, `""`, `0`, and negatives where representable, each with the contract's asserted
  result.
- `nullptr` for every nullable parameter, including optional output pointers.

### Pointer Legitimacy Law
**Proves:** a pointer is validated before it is used.
- `Memory_*`, `BitPool_*`, `StringPool_*`, `SymbolTable_*`, `Cell_*`, `Shelf_*`: wrong pointer,
  misaligned/interior pointer, foreign-arena pointer, freed or stale pointer, forged type/project ID,
  and sugar-tampered header (`ha[-4]`, `ha[-16]`).
- Each rejection preserves unrelated state and does not free or modify another object.
- Where the contract promises rejection of an unmapped address, prove it in an isolated child process
  with a bounded timeout.

### Failure Observability Law (substrate)
**Proves:** what the caller observes when it catches.
- Establish, per function, which of `false` / `0` / `nullptr` / safe-default is returned, and the
  stderr line on cold rejections (`SymbolTable_instant`/`rename` already log).
- A silent no-op is allowed only where the contract says so, and is asserted.

### Overflow Guard Law (security)
**Proves:** size arithmetic rejects rather than wraps.
- `Transient_alloc` carries the `UINT32_MAX` guard that `arena_alloc` has; a huge size is rejected,
  not truncated.
- `StringPool_isSlot` range-checks before dereferencing `(*slot).self`.
- `Url_base64` `4 * ((len + 2) / 3)`, `ProbableObjects_add` `totalWeight + weight`,
  `primitive/string` `len + 1`, and `count * sizeof` in radix sort and BVH each reject at the limit.

### Lifetime and Arena Law
**Proves:** creation and destruction are observable.
- Double-init, use after shutdown, failed-construction cleanup, `MemoryArena_destroy` with blocks
  outstanding, and `MemoryArena_realloc` cross-arena routing.
- Borrowed versus transferred ownership is asserted exactly once per owner.

### Concurrent Substrate Law
**Proves:** the atomics are actually atomic and progress is bounded.
- `atomic/ring` MPMC, `atomic/spin` ticket locks, and `bit` ABA tags under contention.
- Every test has an external bounded watchdog; run under the thread sanitizer.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Deterministic Calculation | `tests/vexspoke/math/determinism_test.c` | `StrictMath`, `Calc_eval`, `Hash_*` |
| Boundary Value | `tests/vexspoke/struct/container_boundary_test.c` | `struct/*` |
| Pointer Legitimacy | `tests/relational-engine/nio/mem_test.c` plus Vexspoke bit/variable owners | Engine memory ABI and retained CPU object seams |
| Failure Observability | `tests/vexspoke/relational/failure_observability_test.c` | cold rejections |
| Failure Observability | `tests/vexspoke/exception/try_value_test.c` | `TryValue`/`TryPtr` value-or-error pair |
| Overflow Guard | `tests/relational-engine/nio/mem_test.c` plus Vexspoke URL/radix owners | Migrated Transient_alloc and retained CPU arithmetic |
| Lifetime and Arena | `tests/relational-engine/nio/mem_test.c`, `transient_lifetime_test.c` | Engine MemoryArena/Transient ABI |
| Concurrent Substrate | `tests/vexspoke/atomic/atomic_contention_test.c` | `atomic/ring`, `atomic/spin`, `bit` |
| Hot-Path Guard | Future `tests/relational-engine/nio/hot_path_guard_test.c` | hot getters, `;;HOTCODE` sites; unproved until registered execution |

---

## relational-engine Test Laws (R2 memory/storage/native C search)

Relational Engine owns migrated production native IO/NIO, Rust allocation/storage,
stable row chunks, named bindings and native C search over Rust-owned spans.
Imported reflection/relational comparison code is not production. All Part I laws
apply; tests distinguish native Memory ABI preservation from future Rust allocator/
type/schema parity. Migration does not confer blanket battle-tested readiness.

### Rust Ownership and Stable Row Proof Law

- Prove creation/use/drop, empty/one/growth cases, overflow rejection and preserved
  state after failure. Stable row addresses remain stable across advertised growth.
- Test the documented row layout and alignment, including 32-byte VariableSlot
  claims, slot exhaustion/growth and append-only index stability when offered.
- Compile-negative borrow, move and privacy cases must fail for the intended
  diagnostic; successful compilation alone is not lifetime proof.
- Only claim sharing, copy-on-write or schema migration if implemented and tested
  for isolation, rollback and preserved identity. Mark unavailable forms as gaps.

### Native Span Boundary Proof Law

- Migrated IO/NIO owners mirror `src/io` and `src/nio` in
  `tests/relational-engine/{io,nio}`. Run `native_run.py` and registered workspace
  owners with assertions, strict C23 and applicable ASan/UBSan. Prove source/header
  provenance, default production linkage and absence of Vexspoke IO/NIO copies.
  Exercise legacy pointer/header ABI, overflow, rejection preservation, arena
  isolation, scratch reset, file/cache round trips and bounded process/frame slots.
- Native storage may borrow Vexspoke CPU-only contracts without duplicating memory
  implementations or a recursive build graph. Keep imported comparison headers
  from shadowing consumer headers. Static linking proves neither live reload nor
  concurrent lifetime exclusion. Clipboard mutation needs explicit lab permission;
  an unavailable test returns 77. HotFileSys is a draft no-op, not a real watcher.

- Compile/run the engine-owned C header as a real C23 client and exercise native
  search over valid Rust-owned spans. Prove empty/not-found/normal/boundary input,
  malformed names, length/stride overflow and output preservation as contracted.
- Borrow lifetime ends before storage destruction. Never reinterpret Rust atomics
  as C `_Atomic`; typed engine ABI operations own cross-language publication.
- Cold lookup resolves once; hot access retains a validated pointer/handle rather
  than performing reflective name search per frame.
- Run applicable C-client sanitizers and bounded concurrency/teardown probes.
  C-client instrumentation does not claim Rust sanitizer instrumentation.

### Resident Backend Proof Law

- Registration/destruction uses documented exclusion; legal concurrent reads and
  writes prove acquire/release visibility, contention, busy rejection, retention
  exhaustion and recovery. Retained string snapshots survive replacement.
- Keep engine code/storage resident across consumer reloads. Prove actual R1
  integration with reload/teardown tests before claiming it; static linking alone
  does not prove the Hot loader seam or automatic record-schema migration.
- Native default Memory implementation ownership is now the engine's; its C ABI
  is preserved. Prove the Rust extern header separately; this migration is not a
  Rust allocator rewrite or full migration of Vexspoke containers.

### Seams to Prove

| Seam | Owner location / runner | Scope |
| :--- | :--- | :--- |
| Rust memory and primitive/string publication | `tests/relational-engine/rust/`; `python3 tests/relational-engine/rust/run.py` | Registered debug/release owners and doctest; exact targets/results recorded after execution |
| Stable chunks and named bindings | Mirrored `tests/relational-engine/rust/struct/` and `variable/` owners | Growth, layout, identity and lifetime when implemented; no readiness from a planned owner |
| Native C ABI/search | `tests/relational-engine/` C clients through the owner runner | Real C23 bridge plus sanitizer scopes; Rust sanitizer gaps explicit |
| Production native IO/NIO | `tests/relational-engine/{io,nio}`; `python3 tests/relational-engine/native_run.py` | Migrated C ownership/ABI and sanitizer scope; exact executed counts/gaps recorded |
| Rust byte/string extern header | `tests/relational-engine/nio/relational_memory_test.c` | Separate C/Rust handshake, not a native allocator rewrite |
| R1 residency/reload/schema migration | Future integration owner | Unproved until executable reload, rollback and reverse teardown evidence exists |

## graphvex Test Laws (R3 GPU)

The graphics driver: device dialects (Vulkan, Raster, Null), images, passes, filters, compositor,
font, shaders. Everything here is about **one contract across every backend** and **retiring GPU
resources without use-after-free**.

### Backend Conformance Law
**Proves:** the same suite passes for every row.
- Vulkan, Raster, and Null each pass the shared device/graphics contract suite.
- A headless mock is never presented as proof of hardware behavior; the Raster row is the reference,
  not a substitute for the Vulkan row.

### Native Pixel and Present-On-Demand Law
**Proves:** pixels are hardware pixels and present happens only on change.
- `Device_resize` and the drawable operate in native hardware pixels; asserted against the reference.
- Exactly one present entry; present fires only when something changed.

### Resource Retirement Law
**Proves:** no in-flight resource is freed early.
- Port and run the retire, swapchain, and sync suites against the active tree: fence-signal versus
  retire-guard versus the two-frame lag, timeout strikes, `VK_ERROR_OUT_OF_DATE`/`SUBOPTIMAL` rebuild,
  and the device-loss latch.
- The generational retire ring closes a slot only when quiescent.

### Safety-Net Law
**Proves:** the guards are present and cheap when off.
- `VkGuard_check`/`checkResource` are ported to the active tree and tested debug-on versus `NDEBUG`
  tree-shake, with deterministic log-once behavior.
- Bindless texture IDs are clamped to the bound maximum.

### Stride Overflow Law (security)
**Proves:** image math rejects rather than wraps.
- `shadowAlloc`, `Image_upload` (`width * height * 4`), and the texture byte checks at `UINT32_MAX`
  dimensions.

### Shader Fallback Law
**Proves:** a missing or broken shader is handled.
- The `.spv` resolution precedence and the compile-failure fallback for missing or corrupt blobs.

### Device Fuzz Law
**Proves:** a hostile device environment is survived.
- No loader, missing global entry points, no graphics queue family, and instance/device creation
  failure each return failure with all partial state freed.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Backend Conformance | `tests/graphvex/device/backend_conformance_test.c` | `Device_*`, `Graphics_*` |
| Native Pixel / Present | `tests/graphvex/device/native_pixel_test.c` | `Device_resize`/`present` |
| Resource Retirement | `tests/graphvex/image/retire_ring_test.c` | retire/swapchain/sync |
| Safety-Net | `tests/graphvex/vulkan/vk_guard_test.c` | `VkGuard_check` |
| Stride Overflow | `tests/graphvex/image/image_overflow_test.c` | `image`, `texture` |
| Shader Fallback | `tests/graphvex/shader/shader_fallback_test.c` | `.spv` resolution |
| Device Fuzz | `tests/graphvex/vulkan/device_fuzz_test.c` | `vk_device` create |

---

## api-haven Test Laws (R3 API)

The API driver: REST client, auth, AI providers, MCP server, SSE, webhooks. Everything here is about
**contacting the network**, **classifying what came back**, and **never trusting an endpoint**.

### Transport Failure-Code Law
**Proves:** each transport failure is a distinct, deterministic outcome.
- DNS failure, connect refused, TLS failure, timeout, reset mid-headers, reset mid-body, and short
  read each produce a defined result — not a single bare false.

### Status Taxonomy Law
**Proves:** every status class is classified.
- 1xx/2xx/3xx/4xx/5xx, 429 with `Retry-After`, and 503 are classified explicitly.
- `Rest_postJson`/`Rest_get` must **not** report success on a 5xx or 4xx (current defect).

### Retry and Recovery Law
**Proves:** retry is a process, not a hope.
- Bounded attempts, capped exponential backoff with jitter, idempotency, and cancellation.
- Each retry test asserts one of: succeeds, stays safely rejected, or needs an explicit reset.

### Dropout Law
**Proves:** a dropped connection is an observable negative across all situations.
- Partial headers, truncated body, stream EOF without the terminator, mid-line flush, and reconnection
  absence, each with an asserted result.
- A dead stream is reported, never silent.

### Volume and Saturation Law
**Proves:** too much of anything has its own test.
- Oversized `Content-Length`, chunked overflow, gzip or expansion bomb, JSON depth limit, exact-cap
  body without a NUL terminator, MCP line limit, and response limit — one test per limit.
- Every limit test has a bounded watchdog.

### Offline-Proof Law
**Proves:** no test needs the live internet.
- A loopback fault-injection server drives DNS, refused, timeout, reset, and short-read
  deterministically.
- A blocked loopback is an explicit, recorded skip, never a pass.

### Endpoint Trust and SSRF Law (security)
**Proves:** an endpoint is constrained before it is contacted.
- A single URL parser (or a shared vector suite over the duplicated copies) enforces scheme and host
  policy; metadata IP (`169.254.169.254`), localhost and LAN ranges, `file://`, IPv6 loopback, and
  userinfo abuse are rejected.
- Redirects are classified; credentials are not forwarded cross-origin.
- The MCP `web_search` base override cannot point at an arbitrary internal host.

### Secret Handling Law (security)
**Proves:** secrets are not exposed.
- TLS verification is asserted, and its current unreachability is documented rather than assumed.
- No cleartext API key in a URL query string; bearer tokens and webhook secrets are redacted in errors
  and logs.

### Framing and Escaping Law
**Proves:** hostile content cannot break the protocol.
- Control Bytes are escaped in emitted JSON; identifier spoofing and truncation are rejected; malformed
  input yields the correct JSON-RPC error code.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Transport Failure-Code | `tests/api-haven/api/transport_fault_test.c` | `Http_perform` |
| Status Taxonomy | `tests/api-haven/api/status_taxonomy_test.c` | `Rest_*` |
| Retry and Recovery | `tests/api-haven/api/retry_test.c` | retry policy |
| Dropout | `tests/api-haven/api/dropout_test.c` | recv loop / SSE |
| Volume and Saturation | `tests/api-haven/api/volume_limit_test.c` | body/line/JSON caps |
| Offline-Proof | `tests/api-haven/api/loopback_fault_server_test.c` | loopback server |
| Endpoint Trust and SSRF | `tests/api-haven/api/ssrf_test.c` | `parseUrl` copies |
| Secret Handling | `tests/api-haven/api/secret_handling_test.c` | auth / TLS |
| Framing and Escaping | `tests/api-haven/mcp/mcp_framing_test.c` | MCP JSON |

---

## darling Test Laws (R4 UI)

The UI toolkit: panels, containers, controls, focus, input, compositor. Everything here runs **inside
a real window**, shows **many variations of the same element**, and proves **focus and input reality**.

### Window-First Law
**Proves:** behavior is shown in a real window, not a mock.
- Every component test mounts and renders inside a window.
- Every widget's openable gallery is an Application with an attached Darling
  Frame, not merely a raw starter Window. One gallery may compare variants in
  one Frame. Keep the Application alive until all registered windows are closed;
  hiding a window is not closing it. Legacy raw-Window test starters do not prove
  this gallery contract and need migration separately.
- A headless probe is used only for deterministic logic and is never presented as proof of hardware
  behavior.

### Robot Input Law
**Proves:** interaction is driven by a scripted robot/human hardware vocabulary, with no real device.
- The vocabulary is synthetic steps: pointer move, enter, hover, press, hold (bounded duration),
  drag begin/to/end, release, click, scroll (wheel and trackpad), and cancel — how a human or a robot
  would tinker the system, replayed through code.
- Every scenario is a named, explicit, deterministic sequence; the same script replays identically
  (the Determinism and Reproducibility Law).
- Assert the component's state after each step: focus, hover/pressed/highlight, callback count
  (exactly once), value change, pointer capture and release, and which component receives each event.
- A gesture that needs real hardware (a real touchpad, a real display) belongs to the IRL probe, never
  the deterministic suite.

### Window Oracle Law (Lab and IRL)
**Proves:** the window itself is the test artifact — it renders, and the render is captured and
compared, like a device tested in a lab and again in the field.
- **Lab (in a vacuum):** render a scenario deterministically into an offscreen target and capture the
  frame(s); compare against a stored reference (golden) or a bounded perceptual delta. One scenario
  yields many named captures — one per element, variant, and state in the Variation Matrix.
- **IRL:** a separate probe runs on the real window, compositor, and hardware. It never stands in for
  the lab oracle and its result is never claimed deterministic.
- A capture mismatch is a defect; a reference is regenerated only with a reviewed, intent-stating
  change — never silently.
- Show the tested element both isolated (in a vacuum) and in composition; keep both captures.

### ScrollPanel Habitat Law
**Proves:** containers live and scroll where users put them.
- Containers are mounted inside a scrollable `ScrollPanel`; scrolling, clipping, and paint-skip are
  asserted.
- Nested `ScrollPanel` chaining consumes what fits and bubbles the remainder (dest-last semantics);
  overscroll, spring, and momentum behave per mode.

### Variation Matrix Law
**Proves:** the same element across its real matrix.
- size: large, small, thin, thick.
- state: focusable, highlightable, hovered, pressed, disabled, checked, selected, open.
- appearance: background, color, foreground, style, opacity, radius mode, z, margin.
- capability: renderable, clickable, logic-based, scrollable, overlay.
- Every row is one asserted case; the matrix is the acceptance surface for the element.

### Focus and Input Reality Law
**Proves:** focus behaves like a user expects.
- Initial focus, keyboard traversal (Tab and arrow keys — added where missing), focus gain/loss, and
  focus restoration after a modal closes or a focused element is destroyed.
- Pointer capture survives re-parenting and is released on destruction; callbacks fire exactly once.
- The global focus store and the per-window focus store do not diverge across windows.

### Present-On-Demand and Dirty Law
**Proves:** dirty tracking and present-on-change are real.
- `Panel_isTreeDirty`/`clearTreeDirty` are implemented (not stubs), so dirty propagation, retained
  targets, and present-on-change are actually proven.

### Layout Robustness Law (security)
**Proves:** geometry math rejects rather than wraps or recurses forever.
- Container growth overflow (`capacity * 2u`, `sizeof(Component) * capacity`).
- Deep-tree recursion in dispatch, paint, and subtree copy is bounded.
- NaN, Inf, and negative geometry into the component and scroll-clamp math is rejected.

### Untrusted Text Law (security)
**Proves:** hostile text cannot corrupt layout.
- Markdown and rich-text scanners, input fields, and character-index lookups against malformed UTF-8,
  embedded NUL, and capacity boundaries.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Window-First | `tests/darling/window/window_host_test.m` | window mount |
| Robot Input | `tests/darling/event/robot_input_test.c` | synthetic gesture vocabulary |
| Window Oracle | `tests/darling/window/window_oracle_test.c` | lab capture vs golden; IRL probe |
| ScrollPanel Habitat | `tests/darling/panel/scrollpanel_habitat_test.c` | `ScrollPanel` |
| Variation Matrix | `tests/darling/panel/variation_matrix_test.c` | widget matrix |
| Focus and Input Reality | `tests/darling/event/focus_reality_test.c` | `dispatch`/`focus` |
| Present-On-Demand and Dirty | `tests/darling/panel/tree_dirty_test.c` | `Panel_isTreeDirty` |
| Layout Robustness | `tests/darling/panel/layout_robustness_test.c` | `container` |
| Untrusted Text | `tests/darling/text/untrusted_text_test.c` | markdown/rich_text/input |

---

## b Build Tool Test Laws (standalone tooling)

### Workspace Assessment and Build Proof Law

`tests/b/adapters/workspace_test.py` owns standalone `b build workspace`, not
Vexgraph's separate `tools/workspace.c` graph. Execute assessment before tools;
prove read-only plans, real recursive C/Python and native-manifest builds,
directory/callback grouping, subtree ownership, growable inventories, literal
paths, progress through failures, aggregate nonzero status and retry recovery.
Prove broad-scope review gates without sudo, excluded caches/VCS, no directory
symlink traversal, source/manifest symlink and control-name rejection. Native
scripts/compiler includes remain trusted; a stable-tree contract is not a
sandbox or TOCTOU claim. No program run, firmware upload or SQL execution is
implied by building. Public C clients require strict C23/assertions and applicable
ASan/UBSan; platform-specific leak-sanitizer gaps remain explicit. Existing
`tests/b/workspace_test.py` is a distinct ecosystem integration owner and must
not inherit a green result from these standalone owners.

### Shader Tool Delegation Proof Law

`tests/b/adapters/glsl_test.py` and `metal_test.py` own build-only shader adapters.
Prove selector/registry/header seams, literal compiler argv, missing tools,
malformed source/include rejection, compiler and linker failure, cleanup,
preservation of previously successful output, retry and rejection of both host
run modes. GLSL stage sources must have real SPIR-V compilation/magic evidence
for each claimed backend; shaderc fixtures are not real glslc proof. Metal needs
real AIR/metallib evidence on an installed Apple toolchain; fixtures alone prove
two-stage invocation/rollback, not compilation. Missing tools are explicit skips,
never automatic downloads. Compiler success is not GPU rendering evidence.
Registered runners use `PYTHONPATH=tests/b python3 -m unittest discover
-s tests/b/adapters -p '*_test.py' -v` with bounded child watchdogs. README,
ADAPTERS.md and TREE.md remain checked by `tests/b/readme_test.py`.

---

# Part III — Blueprint Repos

Language, Darkbase and Samplerate remain blueprints. Sesh now has an explicit
caller-buffer snapshot core; its collaborative sessions and cloud integration
remain future work. These sections govern each first implementation rather than
conferring readiness from the roadmap.

## language Test Laws
Grammar and LSP driver (R3).
- **Tokenizer Boundary Law** — `""`, one byte, over-capacity, embedded NUL, and invalid UTF-8 offset
  skew.
- **Grammar Hot-Swap Law** — grammar dylib ABI/version drift and rollback on reload.
- **LSP Frame Recovery Law** — malformed and partial JSON-RPC frames, and external-process death.

## darkbase Test Laws
Native database store (R3).
- **Persistence Durability Law** — WAL replay, torn-write injection, and backup/restore with a real
  change between them (Failure Atomicity and Recovery Law).
- **Cursor Boundary Law** — zero-copy result overrun, null bitmask, and dest-last lifetime.
- **URI Safety Law** — malformed `DbUri` input and credential redaction.

## sesh Test Laws
Session sync and relay (R4).
- **Session Composition Proof Law** — Seven class owners under
  `tests/sesh/session/`, plus `session/text_test.c`, `lang/arity_test.c` and
  `lang/interop_test.c`, registered by `python3 tests/sesh/session_run.py` and
  the main Sesh runner. Prove all supported constructor forms, next unsupported
  arity rejection, null/zero/max identity boundaries, getter/setter round trips,
  auth revocation and secret-free projections, flat copied-intent/receipt growth,
  missing/changed/replayed operation identities, stale revision/overflow rejection,
  failure preservation/recovery and non-owning close. Queued intent is not an
  applied receipt. Use strict C23/assertions, ASan/UBSan and a bounded TSan probe
  with synchronized starts and one caller-owned serialization domain across four
  clients. API Haven verifier injection is not OAuth proof; local revision checks
  are not remote authorization, distributed CAS, data/SQL execution or persistence.
  Numeric identities are host-assigned; durable uniqueness/provider linking need
  separate integration proof. The C spelling SeshClient must coexist with Graphvex
  Client. Ledger clear/reset requires coordinated baseline/identity-epoch handling.
- **Snapshot Backup Proof Law** — `tests/sesh/snapshot/snapshot_test.c` and the
  API Haven header owner `tests/api-haven/storage/snapshot_io_test.c`, registered
  by `python3 tests/sesh/run.py`, prove copied admission, empty/exact/oversize
  inputs, busy rejection preservation, pending/retry timing, exhaustion, clock
  overflow, cancellation, recovery, getters and bounded projections. Assert exact
  cold diagnostics and quiet normal paths under strict C23/assertions and
  ASan/UBSan with external watchdogs. The ignored `tests/sesh/test.txt` is local
  disposable input; tracked sources remain visible. Offline fake put/get is not
  Drive/iCloud, TLS/OAuth, durable journal, real provider idempotency, overall
  timeout, host integration or concurrent merge proof.
- **File and Directory Workflow Proof Law** — Proposed FileSession and
  DirectorySession do not inherit readiness from the snapshot core. Their owner
  tests must exercise real R2 local file/traversal operations, nested paths,
  create/change/remove snapshots, publication failure preserving the previous
  manifest, clone into new/empty storage, byte/hash verification and hostile
  path/symlink rejection. Rebase requires a baseline, explicit conflicts and
  preserved local/remote work. Real cloud execution is a separately authorized
  smoke test over disposable files, not an offline-suite dependency. No such
  workflow implementation or proof exists yet.
- **Session Ordering and Reconnect Law** — sequence, loss, and bounded-backoff reconciliation.
- **Wire Protocol Fuzz Law** — bad length prefix, CRC, MTU chunking, and varint (Adversarial and
  Hostile-Input Proof Law).
- **Sanitizer Proof Law** — PII and secrets are removed before transmit.

## samplerate Test Laws
Audio engine (R4/R5).
- **Realtime Allocation Law** — long-run soak with an allocation watchdog; zero allocation on the
  audio callback.
- **Ring Concurrency Law** — SPSC overflow/underflow drop-degrade.
- **Audio Parser Law** — malformed WAV and MIDI input.

---

*This lawbook governs proof and readiness. Universal architecture is governed by
the real workspace-root `preferences.md`, published at
https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a.
Cite every law by its Title.*
