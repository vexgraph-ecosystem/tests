# tests

Independent, tracked test-source repository for the ecosystem. Presence of a
source file is **not** evidence that its target was built or executed.

## Run

```sh
tests/run.sh              # list the friendly names
tests/run.sh <name>       # build + run one target through the umbrella `b`
```

Any other name is handed straight to `b run` (partial names ok). This script
replaces the former C runner (`main.c`). `b` compiles on demand; see the
umbrella `README.md`.

From the umbrella build, run `ctest --test-dir <build> -N` to inventory
registered tests, then `ctest --test-dir <build> --output-on-failure` to run
the configured set. `diagnostic_smoke_tests` separately builds and runs the
recoverable Vexspoke and fatal Hotcwap diagnostic tests. `run_all_tests` is
currently a Graphvex renovation subset, **not** a release-wide proof gate.

The registered diagnostic/container slice checks safe rejection and recovery
in both Debug and Release. It does not yet cover every test source or all
sanitizer, concurrency, GPU, and platform failure paths in
`test-preferences.md`. Never mark those gaps as passing merely because a
subset is green. Keep test assertions enabled in Release test targets; an
`assert` erased by `NDEBUG` is not proof.
