# tests/darkbase

Owner-test partition for `../../drivers/darkbase-engine`, mirroring its production
source tree (the Test Tree Mirror Law). A production unit at
`../../drivers/darkbase-engine/src/<dir>/<unit>.c` owns
`tests/darkbase-engine/<dir>/<unit>_test.c`.

Current owner tests:
- `database/database_test.c` — L2 `Database` entity registry and live-row binding.
- `database/database_result_test.c` — the dest-last `DatabaseResult` cursor.

Run through the workspace `b` graph: `./tools/b test database`.

No persistence, transactions or reactive-program owner tests exist yet; those
milestones have no source to own.
