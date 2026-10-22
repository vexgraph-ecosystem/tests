# vexgraph — Test Preferences (Ecosystem Test Laws)

The ecosystem's proof lawbook: the evidence required before a production file, class, component or subsystem is called tested across R1–R6. Governs **proof and readiness only**; universal architecture is the real workspace `../PREFERENCES.md` (the Living Documentation Law). Cite a test law by its **Title**, never a number (the Law Identity Doctrine).

## How to use this document

1. Pick one subsystem in Part II; each is self-contained.
2. Open its "Seams to Prove" table — one row is one owning test: the target test file and the seam it must exercise. Write the owner test into a named build/run target, compile with `-Wall -Wextra -Werror`, exit nonzero on any failed check, and run it.
3. Record readiness on the Per-File Battle Test Law ladder: **unmapped** → **mapped but unwired** → **failing** → **passing with stated gaps** → **battle tested**. Never promote a whole layer from partial file results; battle-tested requires executed evidence under every applicable law here.
4. Maintain the shared checklist ledger (https://gist.github.com/vex-graph/26f556f1a3a57b09d5c991d5ef9ef5ab, local `../metadata/gists/test-checklist.md`) for every executable test, per the Timestamped Test Checklist Law in workspace `../PREFERENCES.md`: only compilable/runnable test units, runner scripts and the shared harness are inventoried — never markdown, docs, config or production source. Record actual Unix timestamps, subject hashes, command/scope, result, description; never authors or session identifiers. Appearance is user-tested. Unknown/failed/skipped/stale evidence is ❌; a scoped ✅ is not battle-tested readiness or visual acceptance.

Part I binds everything. Part II specializes per subsystem. Part III governs blueprint repos, so their first real commit lands already governed.

## Table of Contents

Part I — Universal Test Laws: Per-File Battle Test, Test Tree Mirror, Contract Before Cases, Public Surface Proof, Value Boundary Matrix, Pointer/Identity/Dereference Safety, Arity and Variadic Dispatch, Failure Atomicity and Recovery, Failure Observability, Determinism and Reproducibility, Resource and Lifetime, Concurrency and Bounded Progress, Adversarial and Hostile-Input Proof, Deliberate Exhaustion and Backend Trust, Hot-Path Minimal Guard, Darling Component Narrative and Focus, Runtime-Level Seam, Executable Evidence and Readiness.

Part II — hotcwap (R1 host), vexspoke (R2 computation/behavior), relational-engine (R2 memory/storage/native C search), graphvex (R3 GPU), api-haven (R3 API), darling (R4 UI), b build tool (standalone tooling).

Part III — Blueprint Repos (stub-stage; rules provisional): language, darkbase, sesh, samplerate, determinant.

---

# Part I — Universal Test Laws

## Per-File Battle Test Law

### Definition:
Smallest readiness unit: one production file at one runtime level — a public class's `.h`/`.c` pair, or a header-only, implementation-only, Objective-C, shader or generated-interface unit — each with its own proof.

### The Why:
A neighbor's green proves nothing about this file's constructors, operations, failure paths and cleanup.

### The Rule:
- Every production file unit owns `<unit>_test.c` in the mirrored `tests/<subsystem>/` tree (the Test Tree Mirror Law) plus a short public-surface and requirements inventory; a file with no callable public API gets an owner test through the nearest stable seam, recorded. The owner test proves normal behavior, value boundaries, invalid inputs, failure state, lifetime and concurrency, independently enough to identify the failing file; shared fixtures allowed, one broad integration test never replaces it.
- Rust units mirror under `tests/relational-engine/rust/` on their registered Cargo owner target, debug and release with warnings denied; native C clients of the public ABI compile/run with assertions and `-Wall -Wextra -Werror` — a Cargo build alone proves no C boundary.
- `.h`/`.c` pair: compile the public header as a client, run the implementation; header-only macros and inline functions need compile and runtime proof; platform-specific implementations need proof on every supported platform or an explicit unproved status.
- Record ladder status; the final rung requires every applicable law here executed. A file on disk is not proof of execution; cross-file and cross-level integration come only after file-level proof and confer no readiness on a file whose own owner test is missing.

## Test Tree Mirror Law

### Definition:
A production unit at `<repo>/<dir>/<unit>.c` owns `tests/<subsystem>/<dir>/<unit>_test.c` — same relative directory, unit name suffixed `_test`; a seam test spanning directories or levels sits in the mirrored directory of its primary subject.

### The Why:
A flat `tests/<subsystem>/` dump is unreadable; mirroring makes ownership a path lookup.

### The Rule:
- Mirror the production directory: `ecosystem/hotcwap/window/window.c` → `tests/hotcwap/window/window_test.c`; one owner test per file unit.
- A repo-root unit (no directory) keeps its owner test at `tests/<subsystem>/<unit>_test.c`.
- A seam test records the seam in its header; the Test Segregation Law still forbids any test file inside a production source tree.

## Contract Before Cases Law

### Definition:
Each production file's public API has a written contract — valid and invalid inputs, ownership, thread affinity, result and error semantics, state after failure — and tests prove that contract.

### The Why:
Without stated expected behavior, a crash, silent no-op, partial write and explicit error all look plausible for the same invalid call.

### The Rule:
- Test documented behavior at the public seam; define a missing contract before claiming the test complete.
- Distinguish invalid input from caller misuse outside the contract; a test may expose an unsafe contract but never silently invents one.
- Record preconditions, observable results, output parameters, callbacks and postconditions for success and failure; a failed operation has an explicit disposition — unchanged, rolled back, partially applied with documented progress, or terminal — asserted exactly.

## Public Surface Proof Law

### Definition:
Every public function, constructor form, macro dispatch, callback entry and destructor declared by a file has test evidence in that file's owner test.

### The Why:
Code that links but is never invoked by a test can fail on its first real use; an unused public function is a coverage gap even when neighbors pass.

### The Rule:
- Keep the per-file public-declaration inventory and its tests; every declaration maps to an executable test or a stated, reviewable reason for a different proof method.
- Exercise every meaningful return path and output parameter; getter/setter round trips assert the trip and the underlying observable state.
- Prove callback registration, delivery, removal and exactly-once behavior where callbacks are part of the API.
- Count only tests built and run by the relevant test target: a source file sitting in `tests/` is not evidence.

## Value Boundary Matrix Law

### Definition:
Every applicable public input domain is tested at its identity, normal, boundary and invalid values.

### The Why:
Most defects hide in transitions between absent, empty, full and invalid states, not at an ordinary example value.

### The Rule:
- Numeric: zero, one, negatives where representable, documented minima and maxima, values just past each boundary; overflow/underflow for arithmetic and size calculations.
- Strings/byte spans: `""`, one element, embedded NUL when length-based, malformed encoding when decoded, exact capacity, one byte over, zero-capacity destination where applicable.
- Collections/objects: empty, one element, growth past initial capacity, removal to empty, duplicates, missing keys, out-of-range indices.
- Floating point: `-0.0`, non-finite values, precision/rounding boundaries when accepted.
- Pointers: `nullptr` separately for each nullable public parameter, including optional outputs; assert the safe result and intact unrelated state.
- Never force a meaningless case onto an unrelated API; mark it inapplicable with its contract reason.

## Pointer, Identity, and Dereference Safety Law

### Definition:
Pointer-bearing APIs prove the pointer properties they claim to validate before dereferencing or mutating through them.

### The Why:
A pointer can be non-null yet wrong object, generation, type or address; `nullptr` tests alone cannot protect the relational object model.

### The Rule:
- Managed handles/objects: wrong project/type IDs, valid objects of another class, stale generations, borrowed versus owned pointers, interior/misaligned pointers when rejection is promised.
- A rejected pointer is never dereferenced and never frees or modifies an unrelated object; assert the rejection and preserved state.
- A forged ID and a genuine pointer to another type exercise different failure paths — test them independently.
- Unmapped-address rejection runs in an isolated child process with a bounded timeout and a clear crash expectation, plus memory sanitizers for invalid dereference and use-after-free; a sanitizer finding fails the proof.

## Arity and Variadic Dispatch Law

### Definition:
All advertised constructor overloads, macro selectors and variadic functions dispatch and consume arguments according to their declared arity.

### The Why:
Macro counting and variadic forwarding work for a small example and fail at zero arguments, the advertised maximum, or after argument promotion.

### The Rule:
- Compile and run every `Class_0`, `Class_1` and later form plus the `Class(...)` chooser; assert which form ran and the constructed value.
- Compile-negative tests reject unsupported argument counts and wrong types where C23 can enforce them, asserting the intended diagnostic — not an unrelated build error.
- Variadic without a smaller documented limit tests zero where valid, one, several and at least ten arguments in one call (order, count, sentinel handling, promoted types, final state); a documented finite maximum tests the maximum and its next invalid count; a ten-argument probe never reads beyond the arguments actually supplied.

## Failure Atomicity and Recovery Law

### Definition:
When an operation fails, tests prove what happened to every state it could have touched and whether a later valid operation recovers.

### The Why:
A returned error can hide a damaged buffer, leaked allocation, advanced generation, lost callback, half-written file or partially swapped module.

### The Rule:
- Capture a pre-operation snapshot; inject failure at each independently reachable stage: invalid input, allocation failure, callback rejection, short read/write, timeout, cancellation, downstream error.
- Assert the documented post-failure state — resources, ownership, generation, output buffer, error code; rollback restores data and behavior, the old callable implementation callable after a failed hot swap.
- Retry after invalid input and after transient failure, proving the contract's exact disposition: succeeds, stays safely rejected, or needs explicit reset; repeating the same invalid call never corrupts state.
- Backup/restore tested with a real change between (content, identity, metadata, ability to continue); where propagation is handled, assert rejection reaches the caller, cleanup runs, errors never swallowed or reported as success.

## Failure Observability Law

### Definition:
A rejection the contract promises is **observable**: a defined result (code, `false`, sentinel, safe default), and on cold paths a `THROW(...)` — exactly `[vex] <file>:<line>: <message>` to stderr — or another assertable channel (error code, counter). Failure is never silent where the contract says it rejects.

### The Why:
A crash, a silent no-op and a correct rejection are indistinguishable if nothing reports which happened.

### The Rule:
- Vexspoke's `THROW` is a recoverable cold rejection; hotcwap's `FATAL_THROW` is a separate process-terminating operation. Owning tests check the safe return/exit **and** the diagnostic; normal paths produce no unexpected rejection report.
- Every promised rejection has an asserted channel: the return value and, on cold paths, the `THROW` message or code.
- Log-once determinism: one rejection per call, asserted once, not raced.
- Hot paths stay quiet (the Cold-Strict, Hot-Minimal Validation Law); observability is a cold-seam obligation, not a per-frame cost.
- A swallowed error is a defect: a documented "ignore" disposition still asserts the caller observes a correct result.

## Determinism and Reproducibility Law

### Definition:
A passing test asserts the same result for the same input on every run — no dependence on clock, scheduler luck, address layout or an unseeded random source.

### The Why:
Flaky green is worse than red: it hides regressions and silently corrupts the readiness claim.

### The Rule:
- Seed every random source and record the seed; an unreproducible run is not evidence.
- Never synchronize with a sleep: barriers, latches or explicit state probes establish ordering; vary delays only to explore interleavings, never to make correctness depend on duration.
- Where an exact oracle is impractical, use property or metamorphic tests and state the property.
- An intermittently failing test is a defect to diagnose, never rerun until green.

## Resource and Lifetime Law

### Definition:
Creation, borrowing, ownership transfer, shutdown and destruction are observable parts of an API's behavior.

### The Why:
Successful outputs can conceal leaks, double frees, dangling references or shutdown in the wrong order.

### The Rule:
- Exercise create/use/free, failed construction, repeated initialization and teardown, cleanup after partial initialization.
- Borrowed objects remain usable after the borrower releases them; transferred objects are released exactly once by the new owner.
- Memory-check tests exercising manual allocation; an identified leak, invalid access or use-after-free fails the proof.
- R1–R6 integration: reverse teardown order and cancellation before freeing resources a worker, GPU submission or callback may still use.

## Concurrency and Bounded Progress Law

### Definition:
An API's thread-safety claim requires tests of both shared-state correctness and bounded forward progress.

### The Why:
A single writer never reveals lost updates, wrong-thread observers, deadlock, starvation or teardown races.

### The Rule:
- State whether each public operation is thread-safe, owner-thread-only, or needs external synchronization; test only legal concurrent use as conformance (illegal use only where rejection is promised).
- Thread-safe operations: synchronized starts, many readers and writers; assert final counts, ordering/linearization guarantees, no duplicate or lost work; repeat with varied schedules and seeds.
- Test callback thread affinity, cancellation during work, teardown while work is pending, and join completion within the Bounded Wait Law's contract.
- Spin locks and atomics: acquire/release visibility, contention, unlock handoff, progress under load; every test carries an external bounded watchdog.
- Thread and address sanitizers (separate runs where runtimes conflict); platform skips reported explicitly.

## Adversarial and Hostile-Input Proof Law

### Definition:
Every public seam consuming untrusted input — Bytes, sizes, indices, pointers, paths, URLs, filenames, binary modules, network frames, serialized data — gets an adversarial battery of malformed, oversized, truncated and hostile values at the public cold seam.

### The Why:
Untrusted input is how crashes, integer overflows, out-of-bounds access, path traversal, injection, SSRF and decompression bombs enter; kind tests on well-formed input prove nothing about the hostile case.

### The Rule:
- Enumerate untrusted seams per file; run each battery under memory, undefined-behavior and (where relevant) thread sanitizers.
- Size arithmetic: overflow (`len + 1`, `count * sizeof`, capacity doubling, `total + weight`), underflow, `UINT*_MAX` boundaries, negative-where-unsigned, exact capacity plus one-over.
- Encoding: embedded NUL, invalid UTF-8, overlong sequences, truncation mid-token.
- Structure: deep nesting, huge element counts, cyclic or self-referential input, duplicate keys.
- Trust boundaries: path traversal, symlink and hardlink, check-then-use races, redirects, CRLF and header injection, binary/ABI mismatch for loaded modules.
- Every adversarial test has an external bounded watchdog; a sanitizer finding fails the proof.

## Deliberate Exhaustion and Backend Trust Law

### Definition:
Tests are an executable basis for trusting a backend under pressure: every resource-owning backend deliberately reaches its documented boundaries, attacks failure and recovery, and verifies the data and ownership invariants consumers depend on.

### The Why:
Allocators, pools, queues and registries fail where capacities, lifetimes and interleavings meet: thousands of successful operations can exercise one recycled slot repeatedly, never exhaust backing, never keep many objects alive together, never reveal cross-thread corruption.

### The Rule:
- **Write the pressure contract first.** Inventory each resource: capacity/growth, ownership, legal concurrency, exhaustion disposition, post-failure state, recovery; per-kind boundaries — fixed arenas, slots, bump regions, retained snapshots, handles/generations, queues; inapplicable cases get source-grounded reasons, never convenience.
- **Actually exhaust each applicable resource.** Reach the last valid admission and first rejection, repeat it, prove data/identity, outputs and ownership stay correct; growable owners cross multiple growth boundaries with injected backing/allocation failures; exhaust test-owned resources, never host-wide RAM/disk; observe any fallback and prove its lifetime and reclamation.
- **Refusal is LOUD, not merely returned.** Per the Exhaustion Loudness Law: assert the owner's queryable refusal count (e.g. `MemoryArena_exhaustionCount`) increments per rejected request, the epoch's single `THROW` names the owner, request size and bound, and `init`/`create`/`reset`/`freeAll`/`destroy` zeroes it for a fresh epoch; a backend whose exhaustion is unobservable is not trusted for R5/R6 use.
- **Prove recovery, not just refusal.** Release or reset under the legal contract, retry, refill, exhaust again; assert exact reclamation, permitted retained capacity, survivor contents, count/identity, continued normal operation; inject independently reachable construction/growth/transfer failure stages.
- **Distinguish occupancy from churn.** Backends claiming legal multithreaded use require a synchronized pressure case holding ≥1,024 simultaneously live allocations across ≥4 workers (caller-selectable larger; explicit smaller-contract reason when inapplicable); record peak live count, workers, operation counts, sizes, seeds; a barrier keeps the live set retained while workers verify it; thousands of serial operations never satisfy occupancy.
- **Test legal pressure and interleavings.** Contention, mixed sizes/types, growth while survivors are used, exhaustion with pending work, handoff visibility, coordinated shutdown/reset; state reader/writer ownership; non-thread-safe APIs use their real serialization; racing reset/free is a conformance case only when safe rejection is promised; synchronize with barriers and state probes, never sleeps; vary recorded seeds and schedules.
- **Verify the working data.** Full retained payloads or independently computed content hashes, alignment, lengths/types, live-address non-overlap, counts, permitted reuse/generation identity, output canaries, state after every failure; exercise public operations under pressure, not merely alloc/free; no leak, race, double release, lost update or unexpected diagnostic; cleanup excludes borrowers before freeing backing.
- **Cover interacting factors deliberately.** Publish a coverage matrix of size, alignment, type/project, occupancy, lifetime, growth, failure stage, thread count and legal schedule — full Cartesian when tractable, pairwise plus targeted higher-order combinations otherwise, seeded state-machine/model histories for large domains; state omitted combinations; random examples are not an exhaustive claim, and an impossible `n^n` sweep is not a substitute for a risk model.
- **Run bounded proof with the right instruments.** Assertions stay active in optimized/release-like owners; applicable ASan/UBSan, separate TSan, injected failures, leak/resource accounting, external watchdogs; unsupported instrumentation/platforms are explicit gaps; report exercised occupancy and pressure, not elapsed time; benchmarks separate (record workload, host, warmup, latency/throughput distributions before speed claims), and a fast corrupt allocator fails.
- **Readiness follows evidence, not green status.** Missing pressure/occupancy/recovery/concurrency proof blocks a battle-tested/production-ready claim; checklist passes keep their narrower scope; no retroactive upgrades — inventory gaps and strengthen first; no finite suite proves every possible input or thread schedule.

## Hot-Path Minimal Guard Law

### Definition:
A hot-path getter validates nothing beyond a single entry guard — one `nullptr` or range test — and returns the contract's safe default; it never logs, allocates or revalidates per element. A deliberately unchecked, trust-the-handle read is declared `;;HOTCODE`.

### The Why:
Revalidating every element on a frame path kills frames; the cold seam owns the checks — this law proves the split instead of assuming it.

### The Rule:
- A hot getter carries exactly one entry guard returning the safe default; assert the guarded path, the observed value, and absence of cost — no logging, no allocation over a bounded run of calls.
- The hostile matrix — `nullptr` variants, wrong pointer, overflow, out-of-range — lives at the cold seam (the Pointer, Identity, and Dereference Safety Law), never on the hot getter.
- Every `;;HOTCODE` marks a hot site; a test proves the cold validator (`;;CHECKER`) rejects hostile input and the marked site is reachable only with a validated handle.
- `grep -rn ';;HOTCODE'` lists every hot site; a new one without a covering hot-path guard test is a gap.

## Darling Component Narrative and Focus Law

### Definition:
Every UI component test explains the component's purpose and proves its visible interaction contract, including focus when the component can receive input. Stated generally here, concrete for the toolkit in the darling Test Laws.

### The Why:
A list of assertions can pass while the component still fails its user-facing job — focus, dispatch and painting are easy to test in isolation while their interaction is broken.

### The Rule:
- Open each component test with a plain-language description: what the component is for, what a user does with it, which behaviors the test proves; each scenario gets a descriptive name and expected outcome.
- Focusable components: initial focus, acquisition by pointer and keyboard where supported, traversal order, focus loss, disabled and hidden behavior, modal capture, destruction of the focused component, focus restoration; assert which component receives each key event.
- Test pointer capture, inside/outside hit results, callback count, layout and native-pixel boundaries, dirty marking, present-on-demand when relevant.
- Headless tests for deterministic behavior; a separate integration probe for behavior needing an actual window, compositor, input system or graphics device — a headless mock is never proof of hardware behavior.

## Runtime-Level Seam Law

### Definition:
Each runtime level has local tests and tests at the boundary where it depends on or supervises another level. Stated generally here, instantiated per subsystem in Part II.

### The Why:
Correct classes can fail when ownership, threading, type identity, pixels or errors cross a runtime boundary.

### The Rule:
- R2 `vexspoke`: prove CPU computation, math, algorithms, synchronization, behavior, plus retained memory/container/type ABI during staged migration.
- R2 `relational-engine`: prove Rust-owned allocation/storage, stable row chunks, variable bindings, native C search over borrowed spans, lifetime; no default allocator replacement, C/Rust atomic-layout equivalence or automatic schema migration is inferred; R1 residency/consumer reload needs separate integration proof; GPU shaders/dispatch remain Graphvex R3.
- R3 drivers (`graphvex`, `api-haven`, `language`, `darkbase`): prove protocol parsing, malformed input, cancellation, storage durability or GPU resource retirement as applicable, plus their R2 boundary.
- R1 `hotcwap`: prove boot and teardown, bounded waits, manifest rejection, hot swap rollback, retry, supervised worker or window lifecycle.
- R4 (`darling`, `sesh`): prove input routing, layout, focus, compositing, session ordering, recovery after disconnect as applicable.
- R5 applications and R6 superapps: prove representative user workflows, save/restore or undo/redo where offered, integration across the levels they actually use (superapps compose R1–R5 workflows).
- Keep fault injection at the public cold seam and minimal guards on hot paths, consistent with the Cold-Strict, Hot-Minimal Validation Law.

## Executable Evidence and Readiness Law

### Definition:
"Battle tested" is a recorded, repeatable result for each production file with declared limits, not a label attached to a large test file.

### The Why:
Unwired tests, disabled assertions, skipped platforms and nondeterministic passes create false confidence.

### The Rule:
- Place tests in `tests/<subsystem>/`, mirroring the production source directory (the Test Tree Mirror Law), the umbrella workspace, or a standalone repository's root `tests/` directory, per the Test Segregation Law; keep generated artifacts out of source control.
- Wire every test into a named build and run target; compile with `-Wall -Wextra -Werror`; assertions stay active in release-like test builds; a suite exits nonzero on any failed check.
- Run deterministic unit and seam tests per change; run relevant integration, sanitizer, concurrency stress and platform tests before claiming a changed subsystem battle tested; record command, platform, configuration, seed, skips and result.
- A skip is neither pass nor failure — state which claim stays unproved; flaky tests are defects to diagnose, never rerun until green. A test that cannot exercise its contract on this host must `return B_TEST_SKIP` (77, from `tests/test_support.h`) after printing the reason to stderr — never `return 0`. `b test` reports SKIP separately from PASS; a skip contributes no coverage and is never read as green.
- Readiness per file requires its owner test, public-surface inventory, applicable boundary matrix, failure and lifetime proof, executed results; a changed contract updates its tests and this document in the same cycle.

---

# Part II — Subsystem Test Laws

Each subsystem section carries its own laws, its own "Seams to Prove" ledger, and the Part I laws as applied to it. A row on a ledger is one owning test.

## hotcwap Test Laws (R1 host)

The supervisor: kernel, registries, manifest, hot reload of dylib modules, window lifecycle, teardown — **swapping live code safely** and **refusing to trust a binary**.

### Two-Dylib Swap Law
**Proves:** the loader runs two real dylibs and preserves behavior across a live swap.
- Boot A, drive it, swap to B; state preserved through `Hot_save`/`Hot_restore`/`Hot_migrate`, generation advanced exactly once, A still tracked in the retire ring while B is live.
- Trampoline rows keep a valid `fallback_ptr`; a mid-swap call never dereferences a null trampoline.
- A section present in A and absent in B stays mapped and is never retired.

### Stale and Wrong Binary Law
**Proves:** a bad module is rejected fail-closed, rolled back, and self-heals.
- Missing `VkModuleGetTrampolines`, foreign-magic, wrong-ABI/version, and wrong-table-layout dylibs are each rejected with the correct `HOT_ERROR_*`; the Dynamic Module ABI Verification Law is either implemented and tested or a stated, documented gap.
- After a rejected swap the old module still runs; the next poll with a good binary succeeds (self-heal). A stale generation stamp is detected as never-installed, not silently adopted.

### Manifest Resilience Law
**Proves:** the manifest is created when absent and never trusted when corrupt.
- A missing manifest is **created**; `Manifest_init` succeeds and the ladder exists.
- Empty, truncated, duplicate-key and corrupt manifests clear the mount and return failure without overwriting the file; a partial parse never leaves a half-mounted catalog; section stems and library keys are validated.
- `MANIFEST_UPDATE`/`PROMOTE` move the ladder (`bin/new` → `current` → `previous` → `backward`) and bump the generation stamp; each stage asserted.

### Loader Trust Boundary Law (security)
**Proves:** loading a binary is a deliberate, bounded trust decision.
- A symlink swapped between scan and `dlopen`, and between verify and copy, is detected or the TOCTOU window is closed (fd-bound open); the attack fails.
- `DYLD_*`/search-path hijack and dependency interposition do not divert the loaded module.
- Permissions and ownership of `bin/current` and `manifest.json` are validated before `dlopen`; `VEX_MANIFEST` and `HOME` overrides cannot escape the install root (path traversal rejected).
- Unsigned-binary adoption is a **stated** decision with an asserted reason, never silent.

### Retire-Ring Overflow Law
**Proves:** the retirement ring survives more swaps than it has slots.
- 17+ swaps drive the 16-slot ring; the oldest handle closes only when no thread can still hold it; a `dlclose` failure is observed and reported, never discarded.
- Concurrent calls into a retiring module are safe until the grace count elapses.

### Teardown and Bounded Wait Law
**Proves:** shutdown joins workers before freeing, in reverse boot order.
- The state-save worker joins **before** module handles are torn down; a save in flight completes or cancels safely; `HotShutdown`, `Kernel_stop` and `Kernel_destroy` obey the Teardown Order Law; `Kernel_free` refuses while anything is registered.
- All waits are bounded (the 25 ms save slices and 100 ms console slices); no wait is unbounded.

### Install Ledger Law
**Proves:** the machine remembers an install outside the tree.
- A recorded install reads back; `UNINSTALL` keeps the record (state uninstalled) so a wiped tree is not a fresh install; an explicit `forget` purges it.
- The ledger is a plain per-user state file outside the tree — a correctness record, not a secure store; the test redirects `HOME` to a scratch dir so it never touches the real state directory.

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

Every hotcwap production unit that builds on macOS has an owning test in the mirrored tree (`tests/hotcwap/<dir>/<unit>_test.c`): the loader family (`hot_test`, `hot_trampoline_test`, `hot_retire_test`, `hot_behavior_test`, `ledger_test`, `throwable_test`, `manifest_*`), the kernel (`kernel_function_test`, `kernel_lifecycle_test`, `process_test`, `console_test`, `application_test`), the spoke bridge (`spoke_test`), and the window subsystem (`window_test`, `window_event_test`, `bridge_seam_test`, `traffic_light_test`). Ownership inventory, not a pass report: some listed sources remain unwired or platform-specific; record executed target counts and failures before claiming the R1 battery passed.

The window backends are **platform-exclusive**: `window_test` is the owner test for whichever backend the host builds, so the macOS run proves `window/window_cocoa.m` and `window/traffic_light_cocoa.m`. `window/window.c`, `window/window_linux.c`, `window/window_wayland.c` and `window/window_win32.c` are **explicitly unproved on macOS** — each needs its own host run before it is claimed battle tested.

## vexspoke Test Laws (R2 computation/behavior and retained storage ABI)

Vexspoke owns CPU computation, math, algorithms, synchronization and behavior; it consumes the engine-owned native IO/NIO ABI (implementation and owner files migrated to Relational Engine), with retained container/type/string-pool behavior Vexspoke-owned until its own migration and proof. CPU tests do not claim the native arena was rewritten into Rust. Everything here is **predictable calculation** and **pointers that lie**.

### Deterministic Calculation Law
**Proves:** identical input yields identical output, every run.
- Math, calc, hash, sort: property or metamorphic oracles; seeded randomness recorded; no dependence on clock, address or schedule.
- Float boundaries: `-0.0`, NaN, Inf, and precision edges in `StrictMath`, `FastMath`, `Mat4`, and `CoordFrame_isValid`.

### Boundary Value Law
**Proves:** zero, negative, empty, and null are first-class cases.
- Every container (`Array`, `List`, `Map`, `Set`, `SparseSet`, `MinHeap`, `Queue`, `Deque`, `Stack`) at empty, one, size, size+1, and overflow.
- Values `{}`, `''`, `""`, `0`, and negatives where representable, each with the contract's asserted result; `nullptr` for every nullable parameter, including optional output pointers.

### Pointer Legitimacy Law
**Proves:** a pointer is validated before it is used.
- `Memory_*`, `BitPool_*`, `StringPool_*`, `SymbolTable_*`, `Cell_*`, `Shelf_*`: wrong pointer, misaligned/interior pointer, foreign-arena pointer, freed or stale pointer, forged type/project ID, and hash/id-tampered header (`ha[-24]`, `ha[-32]`).
- Each rejection preserves unrelated state and never frees or modifies another object; a promised unmapped-address rejection is proved in an isolated child process with a bounded timeout.

### Failure Observability Law (substrate)
**Proves:** what the caller observes when it catches.
- Establish, per function, which of `false` / `0` / `nullptr` / safe-default is returned, and the stderr line on cold rejections (`SymbolTable_instant`/`rename` already log); a silent no-op is allowed only where the contract says so, and is asserted.

### Overflow Guard Law (security)
**Proves:** size arithmetic rejects rather than wraps.
- `Transient_alloc` carries the `UINT32_MAX` guard that `arena_alloc` has; `StringPool_isSlot` range-checks before dereferencing `(*slot).self`; a huge size is rejected, not truncated.
- `Url_base64` `4 * ((len + 2) / 3)`, `ProbableObjects_add` `totalWeight + weight`, `primitive/string` `len + 1`, and `count * sizeof` in radix sort and BVH each reject at the limit.

### Lifetime and Arena Law
**Proves:** creation and destruction are observable.
- Double-init, use after shutdown, failed-construction cleanup, `MemoryArena_destroy` with blocks outstanding, and `MemoryArena_realloc` cross-arena routing.
- Borrowed versus transferred ownership is asserted exactly once per owner.

### Concurrent Substrate Law
**Proves:** the atomics are actually atomic and progress is bounded.
- `atomic/ring` MPMC, `atomic/spin` ticket locks, and `bit` ABA tags under contention; external bounded watchdog per test; run under the thread sanitizer.

### WebSocket Protocol Law
**Proves:** the RFC 6455 client speaks the protocol, and refuses what the RFC makes a refusal.
- `tests/vexspoke/net/websocket_test.c` (registered by `python3 tests/vexspoke/net/run.py`) forks a raw loopback peer speaking Bytes, never the library, so the client is never validated against itself. Handshake shape proved: request path, `Upgrade`, `Connection`, `Sec-WebSocket-Version: 13`, a 16-Byte key, `Sec-WebSocket-Accept` verified against base64(SHA1(key || GUID)) — bad accept hash, non-101 status and missing `Upgrade` each refused.
- Frame codec: client frames are masked, an answered ping is a masked pong, text and binary decode with embedded NUL Bytes intact, three continuation frames reassemble into one message, and a peer close reports its code and reason.
- Fail-closed hostility: masked server frame, control frame past 125 Bytes, continuation with no start frame, unmasked 64-bit length with its high bit set, invalid UTF-8 in a text message, and announced length twice the cap each move the client to FAILED; every case asserts ITS OWN guard's refusal text — one guard answering for another's frame fails. The length case is unmasked on purpose (the mask rule cannot stand in for the length rule); the oversize case sends no payload, proving the cap is refused from the header alone; a failed client never invents a later message.
- A local refusal (invalid outbound text) never fails a healthy transport. `wss://` routes through the TLS seam: an unreachable secure peer fails as a TLS connect (naming TLS), not a URL rejection, and the poll budget is pushed into the session so a TLS read cannot outlive the slice. A completed secure handshake is proved only by the opt-in live smoke; the offline suite's peer is a scripted loopback — no live network, proxy or server concurrency is claimed.

### WebSocket Live Smoke Law
**Proves:** the client agrees with a real server, and an idle session is cheap.
- `tests/vexspoke/net/websocket_smoke.c` (registered by `python3 tests/vexspoke/net/ws_smoke_run.py`) always measures idle cost: a forked loopback peer completes a handshake then goes quiet while the client polls; the owner asserts the process's user+sys time stays under a tenth of the window (a spinning wait would spend it) and the client still wakes on the one late message.
- With `WS_SMOKE_URL` set, the same owner round-trips against that endpoint — the ONLY place a completed TLS handshake, a real `Sec-WebSocket-Accept` and a foreign server's framing are proved; opt-in so the shared suite never depends on the live internet (the Offline-Proof Law).
- Known-good credential-free target: Discord's gateway, sending HELLO (`op 10`, with `heartbeat_interval`) unprompted the moment the socket opens — `WS_SMOKE_URL='wss://gateway.discord.gg/?v=10&encoding=json'`; an executed run decoded a 128-Byte HELLO. A skip proves the offline tier only; an unavailable endpoint is a skip, never a pass.
- No Discord authentication, session, heartbeat, resume or REST call is implied: the smoke reads one unprompted frame and closes.

### WebSocket Seam Binding Law
**Proves:** the R2 client really satisfies the R3 transport shape, and a host can bind them without either repository including the other.
- `tests/api-haven/ws_source_binding_test.c` (registered by `python3 tests/api-haven/ws_binding_run.py`) is the host that sees both headers: it assigns `VexWsSource_connect`/`_poll`/`_send`/`_close` into a `HavenWsSource` table and `_Static_assert`s each signature, so drift fails to compile; each source compiles with its own repository's `src` first (the One Type Registry Law).
- Against a forked loopback peer it proves the chain: the handle attached to `HavenWsFanout`, one message per `pollStep` in order with nothing dropped between steps, the seam's `send` arriving as a masked TEXT frame (decoded by the peer, never the library), and `close`/`detach` releasing the slot.
- `poll` delivering one message per call is the lossless contract, not a limitation: undelivered Bytes stay in the socket, so slow polling cannot drop a message. A null handle, an unconfigured handle and a locally rejected payload are refused without disturbing a healthy transport. No live network, TLS, proxy or remote service is claimed; the peer is scripted — not interop or Discord-gateway proof.

### Seams to Prove

| Law | Target test | Seam |
| :--- | :--- | :--- |
| Deterministic Calculation | `tests/vexspoke/math/determinism_test.c` | `StrictMath`, `Calc_eval`, `Hash_*` |
| Boundary Value | `tests/vexspoke/struct/container_boundary_test.c` | `struct/*` |
| Pointer Legitimacy | `tests/relational-engine/nio/mem_test.c` plus Vexspoke bit/variable owners | Engine memory ABI and retained CPU object seams |
| Failure Observability | `tests/vexspoke/relational/failure_observability_test.c` | cold rejections |
| Failure Observability | `tests/vexspoke/exception/try_value_test.c` | `TryValue`/`TryPtr` value-or-error pair |
| Diagnostic Ownership | `tests/vexspoke/exception/exception_test.c` | `Exception(...)` formatting, setters, and owned-string release |
| Audio Configuration Construction | `tests/vexspoke/audio/audio_hal_test.c` | `AudioHalConfig(...)` defaults, callback binding, and setter validation |
| Function Call Contract | `tests/vexspoke/c23/fn_test.c` | `Function(...)` arities, callback/context setters, binding, and null-safe execution |
| Overflow Guard | `tests/relational-engine/nio/mem_test.c` plus Vexspoke URL/radix owners | Migrated Transient_alloc and retained CPU arithmetic |
| Lifetime and Arena | `tests/relational-engine/nio/mem_test.c`, `transient_lifetime_test.c` | Engine MemoryArena/Transient ABI |
| Concurrent Substrate | `tests/vexspoke/atomic/atomic_contention_test.c` | `atomic/ring`, `atomic/spin`, `bit` |
| WebSocket Protocol | `tests/vexspoke/net/websocket_test.c` through `python3 tests/vexspoke/net/run.py` | `WebSocket_open`/`_poll`/`_write*`; forked loopback peer |
| WebSocket Seam Binding | `tests/api-haven/ws_source_binding_test.c` through `python3 tests/api-haven/ws_binding_run.py` | `VexWsSource_*` bound into `HavenWsSource`/`HavenWsFanout`; compile-time signature asserts |
| Apple TLS Backend | `tests/vexspoke/net/tls_apple_test.c` through `python3 tests/vexspoke/net/tls_apple_run.py` | `Tls_connect`/`_send`/`_recv`/`_close` refusal paths; positive handshake unproved |
| Hot-Path Guard | Future `tests/relational-engine/nio/hot_path_guard_test.c` | hot getters, `;;HOTCODE` sites; unproved until registered execution |

## relational-engine Test Laws (R2 memory/storage/native C search)

Relational Engine owns migrated production native IO/NIO, Rust allocation/storage, stable row chunks, named bindings and native C search over Rust-owned spans. Imported reflection/relational comparison code is not production. All Part I laws apply; tests distinguish native Memory ABI preservation from future Rust allocator/type/schema parity. Migration does not confer blanket battle-tested readiness.

### Rust Ownership and Stable Row Proof Law
- Prove creation/use/drop, empty/one/growth cases, overflow rejection and preserved state after failure; stable row addresses remain stable across advertised growth.
- Test the documented row layout and alignment, including 32-byte VariableSlot claims, slot exhaustion/growth and append-only index stability when offered.
- Compile-negative borrow, move and privacy cases must fail for the intended diagnostic; successful compilation alone is not lifetime proof.
- Only claim sharing, copy-on-write or schema migration if implemented and tested for isolation, rollback and preserved identity; mark unavailable forms as gaps.

### Native Span Boundary Proof Law
- Migrated IO/NIO owners mirror `src/io` and `src/nio` in `tests/relational-engine/{io,nio}`. Run `native_run.py` and registered workspace owners with assertions, strict C23 and applicable ASan/UBSan; prove source/header provenance, default production linkage, absence of Vexspoke IO/NIO copies; exercise legacy pointer/header ABI, overflow, rejection preservation, arena isolation, scratch reset, file/cache round trips and bounded process/frame slots.
- Native storage may borrow Vexspoke CPU-only contracts without duplicating memory implementations or a recursive build graph; keep imported comparison headers from shadowing consumer headers. Static linking proves neither live reload nor concurrent lifetime exclusion. Clipboard mutation needs explicit lab permission; an unavailable test returns 77. HotFileSys is a draft no-op, not a real watcher.
- Compile/run the engine-owned C header as a real C23 client and exercise native search over valid Rust-owned spans: empty/not-found/normal/boundary input, malformed names, length/stride overflow, output preservation as contracted.
- Borrow lifetime ends before storage destruction; never reinterpret Rust atomics as C `_Atomic` — typed engine ABI operations own cross-language publication. Cold lookup resolves once; hot access retains a validated pointer/handle rather than a reflective name search per frame.
- Run applicable C-client sanitizers and bounded concurrency/teardown probes; C-client instrumentation does not claim Rust sanitizer instrumentation.

### Resident Backend Proof Law
- Registration/destruction uses documented exclusion; legal concurrent reads and writes prove acquire/release visibility, contention, busy rejection, retention exhaustion and recovery; retained string snapshots survive replacement.
- Keep engine code/storage resident across consumer reloads. Prove actual R1 integration with reload/teardown tests before claiming it; static linking alone does not prove the Hot loader seam or automatic record-schema migration.
- Native default Memory implementation ownership is now the engine's; its C ABI is preserved. Prove the Rust extern header separately; this migration is not a Rust allocator rewrite or full migration of Vexspoke containers.

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

The graphics driver: device dialects (Vulkan, Raster, Null), images, passes, filters, compositor, font, shaders. Everything here is about **one contract across every backend** and **retiring GPU resources without use-after-free**.

### Backend Conformance Law
**Proves:** the same suite passes for every row.
- Vulkan, Raster, and Null each pass the shared device/graphics contract suite.
- A headless mock is never presented as proof of hardware behavior; the Raster row is the reference, not a substitute for the Vulkan row.

### Native Pixel and Present-On-Demand Law
**Proves:** pixels are hardware pixels and present happens only on change.
- `Device_resize` and the drawable operate in native hardware pixels; asserted against the reference.
- Exactly one present entry; present fires only when something changed.

### Resource Retirement Law
**Proves:** no in-flight resource is freed early.
- Port and run the retire, swapchain, and sync suites against the active tree: fence-signal versus retire-guard versus the two-frame lag, timeout strikes, `VK_ERROR_OUT_OF_DATE`/`SUBOPTIMAL` rebuild, and the device-loss latch.
- The generational retire ring closes a slot only when quiescent.

### Safety-Net Law
**Proves:** the guards are present and cheap when off.
- `VkGuard_check`/`checkResource` are ported to the active tree and tested debug-on versus `NDEBUG` tree-shake, with deterministic log-once behavior.
- Bindless texture IDs are clamped to the bound maximum.

### Stride Overflow Law (security)
**Proves:** image math rejects rather than wraps.
- `shadowAlloc`, `Image_upload` (`width * height * 4`), and the texture byte checks at `UINT32_MAX` dimensions.

### Shader Fallback Law
**Proves:** a missing or broken shader is handled.
- The `.spv` resolution precedence and the compile-failure fallback for missing or corrupt blobs.

### Device Fuzz Law
**Proves:** a hostile device environment is survived.
- No loader, missing global entry points, no graphics queue family, and instance/device creation failure each return failure with all partial state freed.

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

## api-haven Test Laws (R3 API)

The API driver: REST client, auth, AI providers, MCP server, SSE, webhooks. Everything here is about **contacting the network**, **classifying what came back**, and **never trusting an endpoint**.

### Transport Failure-Code Law
**Proves:** each transport failure is a distinct, deterministic outcome.
- DNS failure, connect refused, TLS failure, timeout, reset mid-headers, reset mid-body, and short read each produce a defined result — not a single bare false.

### Status Taxonomy Law
**Proves:** every status class is classified.
- 1xx/2xx/3xx/4xx/5xx, 429 with `Retry-After`, and 503 are classified explicitly.
- `Rest_postJson`/`Rest_get` must **not** report success on a 5xx or 4xx (current defect).

### Retry and Recovery Law
**Proves:** retry is a process, not a hope.
- Bounded attempts, capped exponential backoff with jitter, idempotency, and cancellation; each retry test asserts one of: succeeds, stays safely rejected, or needs an explicit reset.

### Dropout Law
**Proves:** a dropped connection is an observable negative across all situations.
- Partial headers, truncated body, stream EOF without the terminator, mid-line flush, and reconnection absence, each with an asserted result.
- A dead stream is reported, never silent.

### Volume and Saturation Law
**Proves:** too much of anything has its own test.
- Oversized `Content-Length`, chunked overflow, gzip or expansion bomb, JSON depth limit, exact-cap body without a NUL terminator, MCP line limit, and response limit — one test per limit, each with a bounded watchdog.

### Offline-Proof Law
**Proves:** no test needs the live internet.
- A loopback fault-injection server drives DNS, refused, timeout, reset, and short-read deterministically.
- A blocked loopback is an explicit, recorded skip, never a pass.

### Endpoint Trust and SSRF Law (security)
**Proves:** an endpoint is constrained before it is contacted.
- A single URL parser (or a shared vector suite over the duplicated copies) enforces scheme and host policy; metadata IP (`169.254.169.254`), localhost and LAN ranges, `file://`, IPv6 loopback, and userinfo abuse are rejected.
- Redirects are classified; credentials are not forwarded cross-origin.
- The MCP `web_search` base override cannot point at an arbitrary internal host.

### Secret Handling Law (security)
**Proves:** secrets are not exposed.
- TLS verification is asserted, and its current unreachability is documented rather than assumed.
- No cleartext API key in a URL query string; bearer tokens and webhook secrets are redacted in errors and logs.

### Framing and Escaping Law
**Proves:** hostile content cannot break the protocol.
- Control Bytes are escaped in emitted JSON; identifier spoofing and truncation are rejected; malformed input yields the correct JSON-RPC error code.

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

## darling Test Laws (R4 UI)

The UI toolkit: panels, containers, controls, focus, input, compositor. Everything here runs **inside a real window**, shows **many variations of the same element**, and proves **focus and input reality**.

### Window-First Law
**Proves:** behavior is shown in a real window, not a mock.
- Every component test mounts and renders inside a window; a headless probe is used only for deterministic logic and is never proof of hardware behavior.
- Every widget's openable gallery is an Application with an attached Darling Frame, not a raw starter Window; one gallery may compare variants in one Frame. Keep the Application alive until all registered windows are closed; hiding is not closing. Legacy raw-Window test starters do not prove this gallery contract and need migration separately.

### Robot Input Law
**Proves:** interaction is driven by a scripted robot/human hardware vocabulary, with no real device.
- The vocabulary is synthetic steps: pointer move, enter, hover, press, hold (bounded duration), drag begin/to/end, release, click, scroll (wheel and trackpad), cancel — how a human or robot would tinker the system, replayed through code. Every scenario is a named, explicit, deterministic sequence; the same script replays identically (the Determinism and Reproducibility Law).
- Assert component state after each step: focus, hover/pressed/highlight, callback count (exactly once), value change, pointer capture and release, and which component receives each event.
- A gesture that needs real hardware (a real touchpad, a real display) belongs to the IRL probe, never the deterministic suite.

### Window Oracle Law (Lab and IRL)
**Proves:** the window itself is the test artifact — it renders, and the render is captured and compared, like a device tested in a lab and again in the field.
- **Lab (in a vacuum):** render a scenario deterministically into an offscreen target and capture the frame(s); compare against a stored reference (golden) or a bounded perceptual delta. One scenario yields many named captures — one per element, variant, and state in the Variation Matrix.
- **IRL:** a separate probe runs on the real window, compositor, and hardware; it never stands in for the lab oracle and its result is never claimed deterministic.
- A capture mismatch is a defect; a reference is regenerated only with a reviewed, intent-stating change — never silently. Show the tested element both isolated (in a vacuum) and in composition; keep both captures.

### ScrollPanel Habitat Law
**Proves:** containers live and scroll where users put them.
- Containers are mounted inside a scrollable `ScrollPanel`; scrolling, clipping, and paint-skip are asserted.
- Nested `ScrollPanel` chaining consumes what fits and bubbles the remainder; getters return scalar values without output buffers; overscroll, spring, and momentum behave per mode.

### Variation Matrix Law
**Proves:** the same element across its real matrix.
- size: large, small, thin, thick. state: focusable, highlightable, hovered, pressed, disabled, checked, selected, open.
- appearance: background, color, foreground, style, opacity, radius mode, z, margin. capability: renderable, clickable, logic-based, scrollable, overlay.
- Every row is one asserted case; the matrix is the acceptance surface for the element.

### Focus and Input Reality Law
**Proves:** focus behaves like a user expects.
- Initial focus, keyboard traversal (Tab and arrow keys — added where missing), focus gain/loss, and focus restoration after a modal closes or a focused element is destroyed.
- Pointer capture survives re-parenting and is released on destruction; callbacks fire exactly once.
- The global focus store and the per-window focus store do not diverge across windows.

### Present-On-Demand and Dirty Law
**Proves:** dirty tracking and present-on-change are real.
- `Panel_isTreeDirty`/`clearTreeDirty` are implemented (not stubs), so dirty propagation, retained targets, and present-on-change are actually proven.

### Layout Robustness Law (security)
**Proves:** geometry math rejects rather than wraps or recurses forever.
- Container growth overflow (`capacity * 2u`, `sizeof(Component) * capacity`); deep-tree recursion in dispatch, paint, and subtree copy is bounded.
- NaN, Inf, and negative geometry into the component and scroll-clamp math is rejected.

### Untrusted Text Law (security)
**Proves:** hostile text cannot corrupt layout.
- Markdown and rich-text scanners, input fields, and character-index lookups against malformed UTF-8, embedded NUL, and capacity boundaries.

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

## b Build Tool Test Laws (standalone tooling)

### Workspace Assessment and Build Proof Law

`tests/b/adapters/workspace_test.py` owns standalone `b build workspace`, not Vexgraph's separate `tools/workspace.c` graph. Execute assessment before tools; prove read-only plans, real recursive C/Python and native-manifest builds, directory/callback grouping, subtree ownership, growable inventories, literal paths, progress through failures, aggregate nonzero status, retry recovery, broad-scope review gates without sudo, excluded caches/VCS, no directory symlink traversal, source/manifest symlink plus control-name rejection. Native scripts/compiler includes stay trusted; a stable-tree contract is not a sandbox or TOCTOU claim; building implies no program run, firmware upload or SQL execution. Public C clients require strict C23/assertions and applicable ASan/UBSan; on macOS, where ASan cannot detect leaks, the OS `/usr/bin/leaks` tool must prove zero leaks on the CLI seam, and any platform-specific leak-sanitizer gap stays explicit. Existing `tests/b/workspace_test.py` is a distinct ecosystem integration owner and must not inherit a green result from these standalone owners; it must track the current storage-ownership layout.

### Shader Tool Delegation Proof Law

`tests/b/adapters/glsl_test.py` and `metal_test.py` own build-only shader adapters. Prove selector/registry/header seams, literal compiler argv, missing tools, malformed source/include rejection, compiler and linker failure, cleanup, preservation of previously successful output, retry, and rejection of both host run modes. GLSL stage sources need real SPIR-V compilation/magic evidence per claimed backend; shaderc fixtures are not real glslc proof. Metal needs real AIR/metallib evidence on an installed Apple toolchain; fixtures alone prove two-stage invocation/rollback, not compilation. Missing tools are explicit skips, never automatic downloads; compiler success is not GPU rendering evidence. Registered runners use `PYTHONPATH=tests/b python3 -m unittest discover
-s tests/b/adapters -p '*_test.py' -v` with bounded child watchdogs. README, ADAPTERS.md and TREE.md remain checked by `tests/b/readme_test.py`.

### Box Manifest and Portable Export Proof Law

`tests/b/box_test.c` (run by `tests/b/box_test.py` under strict C23, assertions and ASan/UBSan) owns the `BoxManifest` class, and `tests/b/cli_test.py` owns the `b create manifest` / `b box` CLI seam. Prove template exclusivity (a second `create manifest` refuses without modifying the file), the template's parsed schema-1 fields, and a cold rejection matrix covering malformed JSON, unknown and duplicate keys, unsupported `\u` escapes, control characters, schema version, unknown/empty format sets, absolute paths, `.`/`..` components, a non-`clean` state policy, a non-empty `targets` array, duplicate export destinations, an oversized manifest, trailing content. The C owner captures its own stderr, asserts the exact diagnostic count and one expected message fragment per case, and fails on a silent rejection, a duplicate report or stray output. Prove owned-string lifetime (`BoxManifest_free` including a partially parsed record), null-input safety, bounded, truncation-flagged, null-safe `toString`/`toStringStruct`.

For the export, prove the payload layout: the entry under its own name plus each declared file at its declared destination (file name by default), nested destinations, preserved permission bits, undeclared sources not copied, and no private staging directory surviving. Refusal preserves state: an existing destination is never replaced; an unavailable entry/source, symlink payload, unsupported format and non-directory parent each reject without creating anything; a mid-copy failure (a FIFO in the payload tree) removes staging and leaves the destination absent. Only `portable` is implemented — every other format name rejects loudly; per-target exports, signing, notarization, installer lifecycle and exported machine/user state remain unimplemented, must not be implied. Export is not a sandbox: requiring a stable payload tree is not hostile-writer confinement.

`tests/b/blueprint_test.py` requires `box.h`/`box.c` to carry the `;;DEFINITION`/`;;OVERVIEW` blueprints naming every function, and `tests/b/readme_test.py` keeps README, TREE.md, ADAPTERS.md and JETBRAINS.md honest about what box does and does not implement. The recursive workspace owner compiles the CLI including `box.c`, so a new b source file that breaks the launcher or sanitized build is caught there.

---

# Part III — Blueprint Repos

Language, Darkbase and Samplerate remain blueprints. Sesh now has an explicit caller-buffer snapshot core; its collaborative sessions and cloud integration remain future work. These sections govern each first implementation rather than conferring readiness from the roadmap.

## language Test Laws
Grammar and LSP driver (R3).
- **C23 Lexical Span Producer Law** — `tests/language/adapters/c/c23_lexer_test.c` and `c23_lex_step_test.c`, run by `tests/language/adapters/c/c23_lexer_run.py`, own the borrowed-snapshot C23 lexical subset and public step value: keyword and punctuator spans, literals, preprocessing numbers, byte offsets, document identity, strict/annotation profiles, identifiers without semantic heuristics, malformed UTF-8 and NUL, literal/comment recovery, full non-whitespace coverage, projections, constructors. The same runner owns `tests/language/syntax/role_test.c` for the appended IDENTIFIER role and its core `text` style key. `tests/language/adapters/c/c23_inlay_hints_test.c` proves byte anchors, nested direct calls, duplicate-declaration exclusion, member-call exclusion, conservative omission of complex/unnamed declarators, empty and zero-parameter input, zero constructors, source immutability, rejection preservation, safe getters, flagged projection truncation. Registered owners run strict C23, ASan/UBSan, constructor-arity negatives; no parsing, preprocessing, semantic resolution or consumer UI proof.
- **Tokenizer Boundary Law** — `""`, one byte, over-capacity, embedded NUL, and invalid UTF-8 offset skew.
- **Grammar Hot-Swap Law** — grammar dylib ABI/version drift and rollback on reload.
- **LSP Frame Recovery Law** — malformed and partial JSON-RPC frames, and external-process death.

## darkbase Test Laws
Native database store (R3).
- **Structured Equality Query Law** — `tests/darkbase-engine/database/database_test.c` proves exact-byte matching across zero/one/many rows, reflected field-span and value-length admission, absent entity/field, null value, preserved rows after rejection. `DatabaseResult` owns the temporary pointer-list container but borrows row values; its owner proves freeing that container does not free a row. A cold linear scan — not numeric coercion, projection, ordering, or index proof. `Database_queryInt32Range` proves inclusive signed bounds (`INT32_MIN`/`INT32_MAX`), empty and reversed ranges, wrong-width and out-of-span fields, preserved state; it interprets a four-byte span as native `int32_t` without validating reflected type identity. The same owner proves the declared in-memory int32 index (`REFLECT_FIELD_INDEXED` admission, identical matches on indexed and scan paths, refresh after an insert, rejection of a null argument, unknown entity/field, and an undeclared field without disturbing an existing index), the persisted form (a save carries the index, reopen adopts it; a same-stride schema whose indexed field moved is refused, not silently rebuilt — whole-image index only, no page-level incremental index, ordering or full projection), named field resolution (`Database_fieldSpan` returns a copied `{code, offset, size}` for a folded field name, reads a row's Bytes through it, and explicitly refuses null, unknown or out-of-row spans instead of returning one a reader could not safely use), and copied persisted catalog discovery by index, record metadata, out-of-range rejection, independence from Store-owned bytes. This does not reconstruct schemas or prove indexes.
- **Database Page-Chain Bridge Law** — `tests/darkbase-engine/database/database_test.c`, registered by `python3 tests/darkbase-engine/database/run.py`, proves a reflected schema and owned rows save through the page Store, close/reopen, and load into a separately registered Database; wrong-stride schema admission rejects while preserving the target's pre-existing owned row. It also exhausts a deliberately small journal on replacement and proves the prior root and rows survive reopen. A child exits after the Database root's redo marker sync; reopen replays that commit and the public Database loader recovers the row. C owners run strict optimized C23 with ASan/UBSan; the linked Rust archive is not sanitizer-instrumented. This whole-image adapter retains old data pages/WAL; it is not multi-page transactions, schema migration, incremental row addressing, checkpoint/recycling, or the blocked workspace graph's full integration proof.
- **Row Borrow and Owned Copy Law** — `tests/darkbase-engine/database/database_test.c` proves exact-stride copy-in, caller-buffer independence, per-entity mode exclusion, loaded-row extension and rejection preservation. Schemas and borrowed rows outlive Database; Database frees only its owned copies.
- **Atomic Snapshot Compatibility Law** — the same owner proves complete payload validation and staged row publication; checksum/schema rejection leaves every entity unchanged. Allocation-failure injection during staging remains unproved. The flat M2 snapshot is compatibility-only, not the pages + WAL persistence path.
- **CRC32 Owner Law** — `tests/darkbase-engine/util/crc32_test.c` proves the standard check vector, incremental equivalence and zero-length identity; the database and WAL owners cover stored-format call sites.
- **Persistence Durability Law** — WAL replay, torn-write injection, and backup/restore with a real change between them (the Failure Atomicity and Recovery Law).
- **Cursor Boundary Law** — zero-copy result overrun, null bitmask, and borrowed-output lifetime.
- **URI Safety Law** — malformed `DbUri` input and credential redaction.

## sesh Test Laws
Session sync and relay (R4).
- **Session Composition Proof Law** — Seven class owners under `tests/sesh/session/`, plus `session/text_test.c`, `lang/arity_test.c` and `lang/interop_test.c`, registered by `python3 tests/sesh/session_run.py` and the main Sesh runner. Prove all supported constructor forms, next unsupported arity rejection, null/zero/max identity boundaries, getter/setter round trips, auth revocation and secret-free projections, flat copied-intent/receipt growth, missing/changed/replayed operation identities, stale revision/overflow rejection, failure preservation/recovery, non-owning close. Queued intent is not an applied receipt. Use strict C23/assertions, ASan/UBSan, a bounded TSan probe with synchronized starts and one caller-owned serialization domain across four clients. API Haven verifier injection is not OAuth proof; local revision checks are not remote authorization, distributed CAS, data/SQL execution or persistence. Numeric identities are host-assigned; durable uniqueness/provider linking need separate integration proof. C spelling SeshClient must coexist with Graphvex Client. Ledger clear/reset requires coordinated baseline/identity-epoch handling.
- **Snapshot Backup Proof Law** — `tests/sesh/snapshot/snapshot_test.c` and the API Haven header owner `tests/api-haven/storage/snapshot_io_test.c`, registered by `python3 tests/sesh/run.py`, prove copied admission, empty/exact/oversize inputs, busy rejection preservation, pending/retry timing, exhaustion, clock overflow, cancellation, recovery, getters, bounded projections. Assert exact cold diagnostics and quiet normal paths under strict C23/assertions and ASan/UBSan with external watchdogs. The ignored `tests/sesh/test.txt` is local disposable input; tracked sources remain visible. Offline fake put/get is not Drive/iCloud, TLS/OAuth, durable journal, real provider idempotency, overall timeout, host integration or concurrent merge proof.
- **File and Directory Workflow Proof Law** — Proposed FileSession and DirectorySession do not inherit readiness from the snapshot core. Their owner tests must exercise real R2 local file/traversal operations, nested paths, create/change/remove snapshots, publication failure preserving the previous manifest, clone into new/empty storage, byte/hash verification, hostile path/symlink rejection. Rebase requires a baseline, explicit conflicts and preserved local/remote work. Real cloud execution is a separately authorized smoke test over disposable files, not an offline-suite dependency. No such workflow implementation or proof exists yet.
- **Session Ordering and Reconnect Law** — sequence, loss, and bounded-backoff reconciliation.
- **Wire Protocol Fuzz Law** — bad length prefix, CRC, MTU chunking, and varint (the Adversarial and Hostile-Input Proof Law).
- **Sanitizer Proof Law** — PII and secrets are removed before transmit.

## samplerate Test Laws
Native audio driver (R3).
- **Realtime Allocation Law** — long-run soak with an allocation watchdog; zero allocation on the audio callback.
- **Ring Concurrency Law** — SPSC overflow/underflow drop-degrade.
- **Audio Parser Law** — malformed WAV and MIDI input.

## determinant Test Laws (R3 terminal toolkit)

Determinant owns terminal-cell rendering, reusable widgets, CLI parsing and caller-fed input. Vexspoke retains bare one-line output; R2 never calls this R3 toolkit. Part I laws apply to each extracted and new file independently.

### Terminal Admission and Presentation Proof Law
- Registered mirrored owners under `tests/determinant/{cli,terminal}` prove public construction/arity, bounded projections, empty/normal/boundary inputs, overflow/truncation, counted capacity refusal, growth, preserved state after failure, reset/recovery. Input is fed by the caller; tests cannot depend on blocking stdin or synthetic sleeps.
- Render into caller-owned terminal-cell backing and an injected output seam; assert cells and widget/input behavior, no raw application stdout, quiet admitted hot rendering. A fake sink proves routing, not a physical terminal.
- Explicit host serialization excludes active callbacks before backing relocation or module replacement. Configurable state has validated symmetric APIs; derived geometry/count/ownership/generation can only change through the documented lifecycle operations, not setters that forge lifetime truth.
- `TerminalCodeView` owner proof admits ordered, non-overlapping UTF-8 byte spans and source-anchored virtual hints, rejects invalid UTF-8 boundaries while preserving the prior line, and renders foreground/style planes under cell-based clipping and horizontal scrolling. Proves presentation only: no lexer, semantic C23 analysis, hint generation, theme registry, Semicolon integration, physical terminal, or visual acceptance is implied.

### Resident Terminal Reload Proof Law
- Preserve terminal ownership and admitted state across view/command replacement; test busy exclusion, incompatible candidates, retained old callbacks/state, successful replacement, teardown after users detach. Real loaded modules require an externally supervised owner; callback substitution alone is narrower evidence and must be labelled as such.
- R1 owns trusted loading and code residency; toolkit extraction is not proof of Primeagen hot reload. The scheduled Primeagen consumer cycle must reproduce the reported restart-required defect, then change a view/command in the same production terminal process without reacquiring the terminal or losing conversation, draft, consent, pipe or host state. Candidate rejection preserves old code/state; unresolved rollback blocks resumption. No live model call or visual acceptance is implicitly authorized.
- Vexspoke's independent bare-output owner proves one-line behavior and does not grant managed-screen ownership. Hotcwap's Console remains an R1 lifecycle object. Other hosts, physical terminal behavior, Unicode shaping and production Primeagen integration retain explicit proof gaps until their owners execute.

*This lawbook governs proof and readiness. Universal architecture is governed by the real workspace `../PREFERENCES.md`, published at https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a. Cite every law by its Title.*
