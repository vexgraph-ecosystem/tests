# tests/darkbase

Owner-test partition for `ecosystem/repos/darkbase`, mirroring its production
source tree (the Test Tree Mirror Law). A production unit at
`ecosystem/repos/darkbase/src/<dir>/<unit>.c` owns
`tests/darkbase/<dir>/<unit>_test.c`.

No owner test exists yet: darkbase currently ships only the registry header
`src/darkbase/type.h` and the workspace build entry `setup_darkbase` in
`tools/workspace.c`. Tests land with the first class in the next milestone.

Run through the workspace `b` graph: `./tools/b test <name>`.
