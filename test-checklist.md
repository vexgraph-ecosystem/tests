# Ecosystem test checklist

Generated test report maintained by `python3 tools/test_checklist.py`.
Governed by the **Timestamped Test Checklist Law** in `preferences.md`.

- ✅ = an automated lab check passed for this exact file hash, in its stated scope.
- ❌ = untested, failed, skipped, or stale; see evidence. This is not a battle-tested claim.
- Last checked is actual Unix time: integer seconds since 1970-01-01T00:00:00Z.
  `—` means never checked. Estimates must never be recorded as executed evidence.
- The hash identifies the tested content. A file change invalidates a previous ✅;
  the old timestamp/evidence remains visible until the next executed check.
- All non-ignored files are inventoried, including headers, shaders, documentation,
  build/configuration files, test files, and tools. This report alone is excluded
  to avoid a self-referential hash. Empty/blueprint frameworks remain visible.
- An integration pass only applies to explicitly named subjects and scope.
  Neither test-file presence nor a passing build proves every framework file.
- Platform gaps and omitted cases must be stated in the evidence/scope column.
- Visual appearance, interactive demos, and subjective approval are not tested
  by this ledger. The user performs visual checks and reports what is broken.
  Automated assertions (including numeric/pixel oracles) are lab evidence only,
  never visual approval. Do not launch galleries or manual demos to earn ✅.
- Descriptions explain the check; no author, agent name, or session ID is stored.

## Commands

```sh
python3 tools/test_checklist.py sync   # inventory files; invalidate stale greens
python3 tools/test_checklist.py check  # reject missing rows or stale results
# Execute first, then record only the explicitly named subjects:
python3 tools/test_checklist.py run --file tools/agents.sh -- bash -n tools/agents.sh
```

`run` propagates failures; exit 77 is recorded as skipped, never green.
Add repeated `--file` arguments only for files actually exercised by the command.
Use `--scope` to describe proof limits, platform and gaps. A syntax check is only
syntax evidence, not runtime or full contract verification. Agents must read this
report before working and update affected rows in the same work cycle.
Use `--description` for a short explanation. Only `--kind lab` is recordable;
`--kind visual` is rejected before execution. Visual reports belong to the user.
Do not hand-edit generated tables; add files/tests and use `sync` or `run`.

## ecosystem/.github

### `ecosystem/.github/profile`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/.github/profile/README.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/ecosystem

### `ecosystem/ecosystem`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/ecosystem/Home.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/_Sidebar.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/anti.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/api-haven.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/darkbase.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/darling-editor.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/darling.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/drawling.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/graphvex.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/hotcwap.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/impedance.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/language.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/samplerate.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/semicolon.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/sesh.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/ecosystem/vexspoke.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/ecosystem/tools`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/ecosystem/tools/validate_tables.py` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/projects/anti

### `ecosystem/projects/anti`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/projects/anti/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/anti/CMakeLists.txt` | ✅ | 1791337104 | 0a856534d56695173486149417ea2d5068bd86d352bb555d3c96a36b806fa187 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/anti/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/anti/README.md` | ✅ | 1791337104 | 8db311525d4acbfbc270b9724da511b5ea4d3c9177f6d49d4bafd12f44a7e7b6 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |

## ecosystem/projects/drawling

### `ecosystem/projects/drawling`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/projects/drawling/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/drawling/CMakeLists.txt` | ✅ | 1791337104 | 05c40afb34e0cbf27b8cf6818efac3ff1802e214c81fdd3ade8060e20f317c01 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/drawling/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/drawling/README.md` | ✅ | 1791337104 | fb39295086566a14822e40c95d0ecf5c47fd98c92ff4babfed41271bee804d9e | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |

## ecosystem/projects/impedance

### `ecosystem/projects/impedance`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/projects/impedance/.gitignore` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/impedance/CMakeLists.txt` | ✅ | 1791337104 | ce4f97902d02afb21afdc71d8036524831381b4ebe0c00fd881768fd6fec9bd1 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/impedance/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/impedance/README.md` | ✅ | 1791337104 | 8ecb1a8a6daab755898ccd35586b3e238180ae7bbb67599cf9761571033e3aad | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |

### `ecosystem/projects/impedance/src`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/projects/impedance/src/impedance.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/impedance/src/impedance.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/impedance/src/main.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/projects/semicolon

### `ecosystem/projects/semicolon`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/projects/semicolon/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/semicolon/CMakeLists.txt` | ✅ | 1791337104 | 0919d872153cf7fd12b2255c47fb46ba6bd1a7f1a2baa18231f1151165825f44 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/projects/semicolon/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/projects/semicolon/README.md` | ✅ | 1791337104 | 73a55b712fe9507d802d35a57e83a394edcdc53e5dc6c62350ac123e56008efa | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |

## ecosystem/repos/api-haven

### `ecosystem/repos/api-haven`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/.gitignore` | ✅ | 1791337104 | 78d7956890e6e431ebb31fc969c273a8411301b18d35b93baa0a8401c45a3a61 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/api-haven/CMakeLists.txt` | ✅ | 1791337104 | 8a7763b0d01c4ba2ec0a8af6121ca1305cda52382785da8fb7ccef3f68878fc2 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/api-haven/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/README.md` | ✅ | 1791337104 | 1c3dbe4218670ddbd23cedf827386a5f43d9367d15ac4b383e506e0100d1d97d | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/api-haven/api-haven-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/ai`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/ai/ai_chat.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_chat.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_chat_anthropic.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_chat_anthropic.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_chat_gemini.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_chat_gemini.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_sse.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/ai_sse.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/ai/data`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/ai/data/providers_china.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/data/providers_europe.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/ai/data/providers_global.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/api`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/api/auth.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/auth.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/client.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/client.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/discord.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/haven_ws_fanout.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/haven_ws_fanout.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/rest.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/api/rest.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/app`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/app/app_broker.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/app/app_broker.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/app/app_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/app/app_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/asset`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/asset/asset_broker.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/asset/asset_broker.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/asset/asset_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/asset/asset_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/com/discord`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/com/discord/discord.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/com/discord/discord.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/com/discord/discord_webhook.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/com/slack`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/com/slack/slack.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/com/slack/slack.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/database`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/database/db_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/database/db_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/database/db_sqlite_file.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/database/db_sqlite_file.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/database/data`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/database/data/db_providers.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/harness`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/harness/engine_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/harness/engine_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/harness/harness.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/harness/harness.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/main`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/main/mcp_main.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/mcp`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/mcp/mcp_server.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/mcp/mcp_server.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/src/search`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/src/search/search_provider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/src/search/search_provider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/api-haven/tools`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/api-haven/tools/gen_db_providers.py` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/api-haven/tools/gen_providers.py` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/darkbase

### `ecosystem/repos/darkbase`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darkbase/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darkbase/CMakeLists.txt` | ✅ | 1791337104 | 41f721b88d7a4e39504a074d4608ab50f4bd0632d587ee5260efb4aff5b1a9de | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darkbase/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darkbase/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darkbase/README.md` | ✅ | 1791337104 | c7a3cc2ffd78aec66c68328feee02e1b401bd66c330981e425add7125fbe6c34 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darkbase/darkbase-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/darling-framework

### `ecosystem/repos/darling-framework`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darling-framework/APPLICATION_LIFECYCLE.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/CMakeLists.txt` | ✅ | 1791337104 | d074cb94902f028b89caf28d87deb085cfb805a8f033922d5fac89a5cea7ae9c | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darling-framework/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/CURSORS.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/README.md` | ✅ | 1791337104 | c01d71d1abc57353e627e52fe28fd7da62e38de763893bd0a194b2af0b3ef2a1 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/darling-framework/SCAFFOLDS.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/STATUS.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/scaffolds.json` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/anim`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/anim/anim.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/anim/anim.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/bridge`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/bridge/clipboard.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/clipboard.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/font_bridge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/font_bridge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/panel_bridge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/panel_bridge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/text_bridge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/text_bridge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/window_bridge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/bridge/window_bridge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/button`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/button/button.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/button/button.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/button/checkbox.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/button/checkbox.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/button/switch.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/button/switch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/c23`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/c23/darling-type.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/c23/darling-type.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/c23/event_invoke.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/c23/event_invoke.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/c23/overload.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/c23/overload.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/canvas`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/canvas/canvas.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/canvas/canvas.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/code`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/code/code_field.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/code/code_field.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/color`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/color/color.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/color/color.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/combo`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/combo/select.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/combo/select.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/compositor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/compositor/compositor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/compositor/compositor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/cursor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/cursor/cursor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/cursor/cursor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/dialog`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/dialog/alert_dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/alert_dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/color_dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/color_dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/file_dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/file_dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/input_dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/input_dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/option_dialog.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/dialog/option_dialog.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/drawable`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/drawable/object_3d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/drawable/object_3d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/drawable/picture.c` | ✅ | 1791343774 | 3e1f27379199ff7b0df98ec9694f1d4172fcf7a94368c7496390c71a94051c60 | ['tools/b', 'test', 'picture_test']; macOS headless widget admission/lifetime only; no real Frame/presentation or appearance proof. Actual texture pixels proven separately by sampled_image_test and filter_gallery_fixture_test. Missing Darling repo-local lawbook remains a reading gap. | Strict registered Picture owner: CPU and GPU-only borrowed Image admission, symmetric widget operations, rejection diagnostics, detach and Image ownership | passed |
| `ecosystem/repos/darling-framework/src/drawable/picture.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/drawable/viewer_3d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/drawable/viewer_3d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/emoji`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/emoji/emoji.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/emoji/emoji.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/event`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/event/action.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/action.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/bridge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/bridge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/document.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/document.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/focus.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/focus.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/gesture.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/gesture.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/hit.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/hit.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/key.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/key.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/tree.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/tree.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/value.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/event/value.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/export`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/export/html_exporter.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/export/html_exporter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/feedback`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/feedback/focus_ring.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/focus_ring.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/progress_bar.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/progress_bar.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/skeleton.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/skeleton.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/spinner.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/feedback/spinner.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/frame`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/frame/frame.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/frame/frame.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/frame/frame_internal.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/game`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/game/cooldown_button.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/cooldown_button.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/damage_numbers.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/damage_numbers.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/dialog_box.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/dialog_box.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/gamepad_nav.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/gamepad_nav.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/hud_bar.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/hud_bar.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/inventory_grid.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/inventory_grid.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/minimap.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/game/minimap.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/graph`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/graph/node_editor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/graph/node_editor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/graph/plot.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/graph/plot.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/history`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/history/history.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/history/history.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/input`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/input/input.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/input.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/input_otp.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/input_otp.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/knob.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/knob.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/pointer.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/pointer.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/scroll_bar.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/scroll_bar.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/search_field.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/search_field.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/slider.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/slider.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/textarea.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/input/textarea.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/kit`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/kit/avatar.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/avatar.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/badge.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/badge.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/breadcrumb.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/breadcrumb.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/chip.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/chip.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/pagination.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/pagination.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/pill.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/pill.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/stat_card.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/kit/stat_card.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/label`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/label/kbd.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/label/kbd.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/label/label.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/label/label.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/label/rich_label.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/label/rich_label.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/layout`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/layout/container.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/layout/container.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/list`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/list/list_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/list/list_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/overlay`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/overlay/command_palette.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/command_palette.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/context_menu.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/context_menu.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/menu.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/menu.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/overlay_root.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/overlay_root.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/popover.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/popover.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/toast.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/toast.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/toast_stack.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/toast_stack.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/tooltip.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/overlay/tooltip.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/panel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/panel/accordion.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/accordion.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/card_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/card_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/dock_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/dock_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/flex_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/flex_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/grid_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/grid_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/layered_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/layered_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/markdown_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/markdown_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/material_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/material_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/panel_internal.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/rich_text_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/rich_text_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/scroll_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/scroll_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/section_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/section_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/split_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/split_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/svg_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/svg_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/tab_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/tab_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/table_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/table_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/tree_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/tree_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/video_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/video_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/web_panel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/panel/web_panel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/picker`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/picker/color_picker.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/picker/color_picker.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/picker/color_swatch.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/picker/color_swatch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/picker/date_picker.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/picker/date_picker.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/properties`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/properties/add.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/add.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/remove.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/remove.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/revalidate.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/revalidate.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_alignment.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_alignment.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_anchor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_anchor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_background_color.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_background_color.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_blur.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_blur.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_border.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_border.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_corner_radius.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_corner_radius.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_cursor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_cursor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_enabled.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_enabled.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_focus.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_focus.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_font.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_font.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_gap.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_gap.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_location.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_location.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_margin.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_margin.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_maximum_size.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_maximum_size.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_minimum_size.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_minimum_size.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_padding.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_padding.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_pivot.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_pivot.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_scroll_offset.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_scroll_offset.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_shadow.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_shadow.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_size.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_size.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_sizing_mode.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_sizing_mode.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_text.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_text.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_theme.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_theme.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_value.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_value.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_visible.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/properties/set_visible.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/radio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/radio/radio_group.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/radio/radio_group.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/scene`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/scene/scene.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/scene/scene.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/scene/scene_2d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/scene/scene_2d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/scene/scene_3d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/scene/scene_3d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/shape`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/shape/icon_image.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/shape/icon_image.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/spatial`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/spatial/property_inspector.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/spatial/property_inspector.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/spatial/spatial_canvas.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/spatial/spatial_canvas.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/spatial/transform_box.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/spatial/transform_box.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/text`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/text/text_core.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/text/text_core.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/text/typography.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/text/typography.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/darling-framework/src/theme`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/darling-framework/src/theme/theme.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/darling-framework/src/theme/theme.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/graphvex

### `ecosystem/repos/graphvex`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/.gitignore` | ✅ | 1791337104 | f7f50f25b0fc4442391fe39905395e28f59f0eb16358327b3119c8a1e52d0c0a | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/graphvex/CMakeLists.txt` | ✅ | 1791337104 | fd86d1b693b8417deec081fc1589c3a264e685130793aec43abdf81d570739d8 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/graphvex/COMPOSITOR.md` | ✅ | 1791343963 | e2965a387d1e3057387b4f4feeea08224f962ebf00f8029111a2bb48e9530bf2 | ['python3', '-c', 'import subprocess; [subprocess.run(["tools/b","test",name],check=True,timeout=120) for name in ("sampled_image_test","filter_gallery_fixture_test","gpu_scope_test","image_test","picture_test","vk_renderer_test")]; subprocess.run(["python3","-B","tests/tools/compositor_contract_test.py","CompositorContractTest.test_gallery_uses_gpu_scope_not_cpu_fixture","CompositorContractTest.test_sampled_image_constructor_dispatch","CompositorContractTest.test_documented_reference_client_compiles","CompositorContractTest.test_filter_constructor_arity_is_rejected_for_intended_reason","CompositorContractTest.test_gpu_color_pass_public_arity_and_no_cpu_extension"],check=True,timeout=60); subprocess.run(["python3","-B","tests/tools/filter_gallery_resources_test.py"],check=True,timeout=180)']; macOS strict registered image/sample/scope/Picture/renderer tests; gallery app build/bundle only, no window/presentation/appearance or memory profiling. Five selected docs checks, not full legacy-path/wiki suite; readiness wiki and Darling lawbook unavailable. Numeric/ASan/UBSan proof recorded separately. | Final strict registered owner runs, five selected documentation/constructor checks and gallery resource bundle build/refresh/signature, no interactive launch | passed |
| `ecosystem/repos/graphvex/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/FILTERS.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/README.md` | ✅ | 1791337104 | 94db97b8b3b6f003f9ab45c86e7aff1be09299bb88f3c93eb8bbbb1ccade8de4 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/graphvex/graphvex-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/board.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/board.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/image.c` | ✅ | 1791343936 | 4c552ff156b28fc3a62c571844c80b61322b0af5baa82a2e6f208c7fc8e4d5ab | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `ecosystem/repos/graphvex/src/image.h` | ✅ | 1791343936 | 429a758b9893f55c7f95adb40ce97a816e957579aae1dbdfc35cd433d1692235 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

### `ecosystem/repos/graphvex/src/compositor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/compositor/color_pass.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/color_pass.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_image.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_image.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_scope.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_scope.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_submit.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/compositor_submit.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/filter_pool.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/filter_pool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/compositor/gpu_scope.c` | ✅ | 1791343936 | 34764ba10949b8cbc12106844d3466fe6156c74d703bc8c23ba715160e807feb | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `ecosystem/repos/graphvex/src/compositor/gpu_scope.h` | ✅ | 1791343936 | d0a91c589e742000c1089760634efddab90bd6b6745a2c1643f067d3520213ea | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

### `ecosystem/repos/graphvex/src/filter`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/filter/filter_functions.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/filter/filter_type.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/graphics`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/graphics/graphics.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/graphics.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/image_runs.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/image_runs.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/render_loop.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/render_loop.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/viewport.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/graphics/viewport.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/lang`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/lang/filter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/nio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/nio/pool.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/nio/pool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/nio/property_pool.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/nio/property_pool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/shaders/compositor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/shaders/compositor/color.frag` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/shaders/compositor/resolve.frag` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/shaders/compositor/resolve.vert` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/shaders/compositor/scatter.frag` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/shaders/compositor/scatter.vert` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/shaders/compositor/scope.frag` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/shaders/frag`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/shaders/frag/quad.frag` | ✅ | 1791343936 | 69727e5d0f1dbaa5557c319a2f5b9ca42295b650bacca5c5c05a2d2d8deba376 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

### `ecosystem/repos/graphvex/src/shaders/vert`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/shaders/vert/quad.vert` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/ui`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/ui/element.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/ui/element.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/ui/property.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/ui/property.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/graphvex/src/vulkan`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/graphvex/src/vulkan/device.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/device.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/pipeline.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/pipeline.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/sampled_image.c` | ✅ | 1791343936 | 00dfc396f5eccfc136a6d0b5e94452b7e06ecd5b2cecab11d2cf70614cf4315e | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `ecosystem/repos/graphvex/src/vulkan/sampled_image.h` | ✅ | 1791343936 | 3b5fb3677085f96a436cac981b15dd7fe80cef223d2606c4e45b5e1ea80fa9ac | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `ecosystem/repos/graphvex/src/vulkan/surface.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/surface.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/vk_batch.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/vk_batch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/graphvex/src/vulkan/vk_renderer.c` | ✅ | 1791343936 | a4793142c35732cde23f078d4ef8e858ec719bf89a367c24d4c91d0c15ecfa42 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `ecosystem/repos/graphvex/src/vulkan/vulkan_backend.h` | ✅ | 1791343936 | ce849f9ddb4821f25a5eb209b60a79a2ce6d095ca42647021489491cf0daafad | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

## ecosystem/repos/hotcwap

### `ecosystem/repos/hotcwap`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/.gitignore` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/CMakeLists.txt` | ✅ | 1791337104 | 3cd01dd1183debe5547af9fcbe07b9e739f3ba9fb9b5590e12223ae97274eb33 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/hotcwap/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/README.md` | ✅ | 1791337104 | 53727415f2bcaa26ef41861853765d6fc45903cd0a4b1b33928652aaa81f6d66 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/hotcwap/hotcwap-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/capability`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/capability/capability.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/capability/capability.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/docs`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/docs/bridging.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/docs/install.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/hot`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/hot/hot.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot_behavior.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot_retire.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot_retire.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot_trampoline.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/hot_trampoline.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/ledger.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/ledger.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/manifest.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/manifest.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/throwable.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/hot/throwable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/kernel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/kernel/application.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/application.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/console.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/console.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/kernel.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/kernel.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/process.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/kernel/process.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/permission`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/permission/permission.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/permission/permission.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/permission/permission_backend.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/permission/objc`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/permission/objc/permission_cocoa.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/spoke`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/spoke/lifetime.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/spoke/lifetime.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/hotcwap/window`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/hotcwap/window/traffic_light.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/traffic_light_cocoa.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_cocoa.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_event.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_event.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_linux.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_wayland.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/hotcwap/window/window_win32.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/language

### `ecosystem/repos/language`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/language/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/language/CMakeLists.txt` | ✅ | 1791337104 | 1b6723bd6c25fed2f06091c3dc161bc1862e78e014df048b5bc8f2fc8bdfa52a | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/language/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/language/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/language/README.md` | ✅ | 1791337104 | 53ea0b829a4059bb6a71ae22682d3feb63851870746f31ebcdebbe3bdf2a2ba3 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/language/language-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/samplerate

### `ecosystem/repos/samplerate`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/samplerate/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/samplerate/CMakeLists.txt` | ✅ | 1791337104 | a3d3c6221e996952a1d4e08762f95e22729da437176c63dae4b92d353cafe4c0 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/samplerate/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/samplerate/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/samplerate/README.md` | ✅ | 1791337104 | 767d640a5b1713802af90c27f24ed1037ec37fc594a577a5fd7d8b3a3355b95f | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/samplerate/samplerate-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/sesh

### `ecosystem/repos/sesh`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/sesh/.gitignore` | ✅ | 1791337104 | 3bad14d6030a64ac54c9b43679525632bcb355daeb590cd262303083fe7678fc | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/sesh/CMakeLists.txt` | ✅ | 1791337104 | 468c1e0be0cbf9db967ff1d643a8c5ccf9fedf58aca5a3cf8a80b81cbbe10b62 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/sesh/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/sesh/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/sesh/README.md` | ✅ | 1791337104 | 94ba60db737b73e75253d6832e0f5a2f87e8e881bd152533625ec17e45c46e8c | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/sesh/sesh-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## ecosystem/repos/vexspoke

### `ecosystem/repos/vexspoke`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/.gitignore` | ✅ | 1791275083 | 8cf46984457865da19f07cd60bd58b400a4467fa12a5b835382bb78d2b192ad4 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["./tools/b","build"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["python3","-B","tests/vexspoke/backend_contract_test.py"],check=True)']; macOS b build plus eight workspace regressions and two backend-document/copy checks. 610 registered compilation units; imported reference bytes preserved. Source-empty repos are not builds; no Rust backend, standalone per-repo, Windows or visual approval. | Integrated build repair and unchanged C-backend restoration | passed |
| `ecosystem/repos/vexspoke/BACKEND.md` | ✅ | 1791275083 | 7d8691ed8013a3d8dea0d4d6a7c24f42fe9bec79bbe279d614247d17447473c9 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["./tools/b","build"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["python3","-B","tests/vexspoke/backend_contract_test.py"],check=True)']; macOS b build plus eight workspace regressions and two backend-document/copy checks. 610 registered compilation units; imported reference bytes preserved. Source-empty repos are not builds; no Rust backend, standalone per-repo, Windows or visual approval. | Integrated build repair and unchanged C-backend restoration | passed |
| `ecosystem/repos/vexspoke/CMakeLists.txt` | ✅ | 1791337104 | 10f27fc4037fb6c30a9153d4a6061acc1e6ac76eeb3d7e0232bc723a5815431e | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/vexspoke/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/README.md` | ✅ | 1791337104 | b4d123720e3bdc66cd132c5a2b4b90d2abcc56c8224a8a5e17890b79a198b35d | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `ecosystem/repos/vexspoke/vexspoke-preferences.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/algo`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/algo/bvh.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/bvh.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/dijkstra.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/dijkstra.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/draft_sort.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/draft_sort.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/kd_tree.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/kd_tree.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/radix_sort.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/radix_sort.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/segment_index.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/segment_index.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/tree_sit.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/algo/tree_sit.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/annotation`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/annotation/checker.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/definition.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/draft.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/getter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/hotcode.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/incomplete.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/inherits.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/intention.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/overview.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/platform_exclusive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/setter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/annotation/what.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/atomic`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/atomic/registry.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/atomic/registry.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/atomic/ring.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/atomic/ring.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/atomic/spin.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/atomic/spin.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/audio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/audio/audio.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/audio/audio_hal.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/audio/audio_hal_stub.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/audio/audio_stub.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/bit`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/bit/bit.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/bit/bit.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/c23`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/c23/constructor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/equals.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/equals.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/fn.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/free.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/free.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/overload.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/c23/zero.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/cli`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/cli/command.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/command.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/commandparser.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/commandparser.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/commandregistry.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/commandregistry.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/console.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/console.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/logcommands.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/logcommands.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/scanner.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/cli/scanner.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/deferred`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/deferred/dispatch.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/deferred/dispatch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/engine`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/engine/loop.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/engine/loop.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/event`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/event/keyhandler.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/event/mousehandler.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/event/touchhandler.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/exception`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/exception/exception.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/exception.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/throw.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_code.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_code.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_ptr.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_ptr.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_value.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/exception/try_value.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/input`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/input/focus.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/focus.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/gamepad.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/gamepad.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/gesture.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/gesture.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/hardware_event.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/hardware_event.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/key.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/key.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/key_map.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/key_map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/mouse.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/mouse.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/piano_key.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/piano_key.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/touch.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/touch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/turntable.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/input/turntable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/io`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/io/cache.c` | ❌ | 1791275016 | 6435274eb70c4f86dc539e2558b7786d7d89e63f14e30c65ea9d53cd1a6f5030 | ['./tools/b', 'test', 'cache_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cache registered regression | stale — content changed; rerun required |
| `ecosystem/repos/vexspoke/src/io/cache.h` | ✅ | 1791275016 | 9e62f519040ca6eb9a7e1ec1e915c538ecd9c16ba176db3841629b4d37ee3ae8 | ['./tools/b', 'test', 'cache_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cache registered regression | passed |
| `ecosystem/repos/vexspoke/src/io/clipboard.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/clipboard_stub.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/file.c` | ✅ | 1791275014 | 42b964f3786f06b0e9f292e1f9fb0a4891a7319799794f5294bf1d77638acce4 | ['./tools/b', 'test', 'file_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend file registered regression | passed |
| `ecosystem/repos/vexspoke/src/io/file.h` | ✅ | 1791275014 | 7542231ad45ff2df96fe2b12f8d876846baa0ade2df1fbb6f3e6d97d8caf90d1 | ['./tools/b', 'test', 'file_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend file registered regression | passed |
| `ecosystem/repos/vexspoke/src/io/filewriter.c` | ✅ | 1791275015 | b117c58fdd1539053e3ceedaeb7e63b79c555e71b7c63f5bbdf960479d576792 | ['./tools/b', 'test', 'filewriter_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend filewriter registered regression | passed |
| `ecosystem/repos/vexspoke/src/io/filewriter.h` | ✅ | 1791275015 | af126ec31e9098426470c85c0e8f5dcb096aa0a9609bc1463297085899b2a6c8 | ['./tools/b', 'test', 'filewriter_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend filewriter registered regression | passed |
| `ecosystem/repos/vexspoke/src/io/hot_file.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/hot_file.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/log.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/log.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/logkind.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/logparser.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/logparser.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/process_spawn.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/process_spawn.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/vexhome.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/vexhome.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/ws_client.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/io/ws_client.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/mat4.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/mat4.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/str.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/str.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec2.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec2.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec4.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec4.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang/point`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/point/point.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/point/point.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang/rect`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/rect/rectangle.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/rect/rectangle.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang/vec2`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/vec2/vec2.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec2/vec2.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec2/vec2d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec2/vec2d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang/vec3`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3_int_float.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3_int_float.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3_long_double.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3_long_double.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec3/vec3d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/lang/vec4`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/lang/vec4/vec4.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec4/vec4.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec4/vec4d.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/lang/vec4/vec4d.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/math`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/math/coord_frame.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/coord_frame.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/fast_math.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/fast_math.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/math.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/math.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/strict_math.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/math/strict_math.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/net`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/net/download.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/download.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/http.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/http.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/json.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/json.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/net.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/netfacade.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/tls.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/tls_curl.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/url.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/net/url.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/nio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/nio/mem.c` | ❌ | 1791274974 | 96a7f1249f8d3ae61a7dd2dd288be6b95d542c67ba21567abeb487161135c194 | ['./tools/b', 'test', 'mem_test']; macOS registered mem_test, built with C23 warnings denied and assertions active; no Rust delegation, other-platform or complete concurrency proof | Restored C allocator compatibility backend owner regression | stale — content changed; rerun required |
| `ecosystem/repos/vexspoke/src/nio/mem.h` | ✅ | 1791274974 | 7f31e649b4a28b6767e523a141503dce7bd900320ba41a1f6d35ea8e80b45d53 | ['./tools/b', 'test', 'mem_test']; macOS registered mem_test, built with C23 warnings denied and assertions active; no Rust delegation, other-platform or complete concurrency proof | Restored C allocator compatibility backend owner regression | passed |

### `ecosystem/repos/vexspoke/src/objc`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/objc/audio_cocoa.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objc/audio_hal_mac.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objc/clipboard_mac.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objc/discovery.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objc/tls_apple.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objc/touchid_cocoa.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/objects`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/objects/choice.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/choice.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/future.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/future.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/global.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/global.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/local.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/local.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/passive.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/passive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/probable.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/probable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/probable_objects.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/probable_objects.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/objects/reactive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/oop`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/oop/stride.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/oop/stride.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/oop/type.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/oop/type.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/primitive`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/primitive/bool.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/bool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/brain.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/brain.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/byte.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/byte.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/double.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/double.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/fixed32.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/fixed32.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/fixed64.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/fixed64.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/float.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/float.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int_double.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int_double.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int_float.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/int_float.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long_double.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long_double.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long_float.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/long_float.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/pack.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/pack.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/short.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/short.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/string.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/primitive/string.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/reactive`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/reactive/dispatch.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/generic.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_object.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_object.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_primitive.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_primitive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_probable.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_probable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_probable_tmpl.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_probable_tmpl.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_tmpl.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reactive/reactive_tmpl.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/reflection`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/reflection/class.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/class.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/field.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/field.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/method.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/method.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/struct.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/struct.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/variable.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/reflection/variable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/relational`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/relational/cell.c` | ✅ | 1791275020 | 4e0eca5567fa500993d8e4cd76385629ff11623578eb54981afbab04dbc5eef0 | ['./tools/b', 'test', 'cell_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cell registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/cell.h` | ✅ | 1791275020 | 4f9910e159a65d0a6f814b1547d3c15e284c31600ed392d5fe2451c738ac0974 | ['./tools/b', 'test', 'cell_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cell registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/relational.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/relational.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/shelf.c` | ✅ | 1791275021 | 7e45d2bd5b870a6d1d449ba2e3ac2ef2aa41af08c3116d34b08980610e32369d | ['./tools/b', 'test', 'shelf_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend shelf registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/shelf.h` | ✅ | 1791275021 | 06f13f980f4e7e831a474917b55809d82006ad02a723be8ce65c57a24f0119ba | ['./tools/b', 'test', 'shelf_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend shelf registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/symbol_table.c` | ✅ | 1791275017 | 4f5c3ac5cb41c09a3c6382c9a8243e5b7c3518b42ad576c4f78385a8768030ec | ['./tools/b', 'test', 'symbol_table_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend symbol_table registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/symbol_table.h` | ✅ | 1791275017 | d9cae7a8e4d66afa848e6d80f756dbaebbb66307d01036ad94b1f2a8b4e754f4 | ['./tools/b', 'test', 'symbol_table_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend symbol_table registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/variable_hash_map.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/variable_hash_map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/variable_mini_map.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/variable_mini_map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/variable_pool.c` | ✅ | 1791275018 | 2ede5385fdfc0617a5aba2f8957dd3a394497359f8fe31bf797a38babbbc58de | ['./tools/b', 'test', 'variable_pool_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend variable_pool registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/variable_pool.h` | ✅ | 1791275018 | 526c4afff14e94e4b146191d2b3d458147b5ee846efbee90b87e7944fffea7d7 | ['./tools/b', 'test', 'variable_pool_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend variable_pool registered regression | passed |
| `ecosystem/repos/vexspoke/src/relational/variable_slot.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/relational/variable_slot.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/search`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/search/calc.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/search/calc.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/search/find.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/search/find.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/search/spotlight.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/search/spotlight.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/security`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/security/crypto.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/security/crypto.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/security/secure_random.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/security/secure_random.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/security/touchid.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/security/touchid.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/spoke`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/spoke/bespoke.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/spoke/bespoke.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/struct`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/struct/array.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/array.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/chunked_list.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/chunked_list.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/circle_array.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/circle_array.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/collection.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/collection.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/deque.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/deque.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/list.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/list.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/map.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/minheap.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/minheap.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/octree.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/octree.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/queue.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/queue.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/set.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/set.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/sparseset.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/sparseset.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/sphere_array.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/sphere_array.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/stack.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/struct/stack.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/system`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/system/app_detect.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/app_detect.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/capture_tool.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/capture_tool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/discovery.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/display_info.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/display_info.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/display_monitor.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/display_monitor.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/graphics_info.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/graphics_info.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/hardware_info.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/hardware_info.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/image_mac.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/image_mac.m` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/process_probe.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/process_probe.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/system.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/system.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/system/data`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/system/data/apps.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/system/data/capture_tools.inc` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/thread`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/thread/compute.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/compute.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/draw.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/draw.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/event.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/event.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/networking.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/networking.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/reactive.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/reactive.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/scripting.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/scripting.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/thread.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/thread.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/ui.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/thread/ui.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/time`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/time/calendar.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/calendar.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/clock.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/clock.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/datetime.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/datetime.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/nanotime.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/time/nanotime.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `ecosystem/repos/vexspoke/src/util`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/repos/vexspoke/src/util/arrays.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/util/arrays.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/util/hash.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/util/hash.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/util/random.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `ecosystem/repos/vexspoke/src/util/random.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## personal/b

### `personal/b`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/b/.gitignore` | ✅ | 1791337104 | b25f8f35c89deb4cd88c31b279946095df9d24b400f45818825c8e5b204412e9 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `personal/b/CMakeLists.txt` | ✅ | 1791337104 | 51fde6ea9245597ef9ac7ddb4a8ea7064d37724f1fd39a6eff6ab139d184d8cb | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `personal/b/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/JETBRAINS.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/README.md` | ✅ | 1791337104 | 46c2386233507392b6ac6bfc8e67b7fd3145f6a5fd08901f3c6263cb9645599c | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `personal/b/TREE.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/annotation.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/b` | ✅ | 1791275595 | a7c96f72cfedcd3eff3c5f4be271bd892767650717c3d3c78144976ea9e9ef2c | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/run_current_test.py"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["./tools/b","build"],check=True)']; macOS headless C/Rust routing, arguments/status/recovery, XML wiring, workspace regression and complete registered build; IDE UI and interactive appearance remain user-owned. b-local lawbook unavailable. | Project-owned graph and repaired CLion active-file routing | passed |
| `personal/b/b.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/b.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/inspect.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/inspect.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/util.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/b/adapters`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/b/adapters/adapter.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/adapter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/arduino.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/arduino.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/c.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/c.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cargo.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cargo.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cmake.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cmake.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cpp.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/cpp.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/csharp.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/csharp.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/go.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/go.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/html.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/html.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/java.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/java.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/javascript.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/javascript.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/lua.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/lua.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/npm.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/npm.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/objc.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/objc.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/php.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/php.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/python.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/python.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/r.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/r.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/rust.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/rust.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/shell.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/shell.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/sql.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/sql.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/swift.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/swift.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/typescript.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/typescript.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/zig.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/b/adapters/zig.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## personal/relational-engine

### `personal/relational-engine`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/.gitignore` | ✅ | 1791274619 | e18058efa35ba8d308f02988f59d7ccf8e1bf2f1f282dacfb83db5d09b4118c7 | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS: 24 C indexing entries, mixed ignores, documentation assertions and warnings-denied locked Cargo check. Not runtime or ABI proof. | Recheck scaffold after local nio addition | passed |
| `personal/relational-engine/CMakeLists.txt` | ✅ | 1791337104 | f2e0cd9c312d9ecc5059a05946fd58e8f59b292b2855c6c633b8113932590a5f | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `personal/relational-engine/CONTRIBUTING.md` | ✅ | 1791274133 | bd56eed92076b1851885c4fbc01e3fadc1477f5002319f131d74eba31d56429b | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS: 23 generated C compile entries, mixed ignore behavior, license labels, explicit docs status, locked offline warnings-denied Rust scaffold check. C implementation, IDE appearance, ABI, concurrency and Windows unproved. | Parallel C/Rust scratchpads, preserved license notices and CLion-only metadata | passed |
| `personal/relational-engine/LICENSE` | ✅ | 1791274133 | ab1089aa14409c87ffb49bb8677e58124bd6246d86f80fd0e4966a8320ead3cd | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS: 23 generated C compile entries, mixed ignore behavior, license labels, explicit docs status, locked offline warnings-denied Rust scaffold check. C implementation, IDE appearance, ABI, concurrency and Windows unproved. | Parallel C/Rust scratchpads, preserved license notices and CLion-only metadata | passed |
| `personal/relational-engine/README.md` | ✅ | 1791358975 | c11497fe7d0f5063099f565aaccb5110e49ace430aca6f73434b71c0e9edc765 | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS scaffold metadata/README assertions and warnings-denied Cargo check; docs scope only, not allocator equivalence or visual acceptance | Refresh scaffold contracts for runnable Rust learning backend | passed |

### `personal/relational-engine/rust`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/rust/Cargo.lock` | ✅ | 1791274619 | d898b4063c85191298152e3fc567a2bc9a4087373a89d7af81f883037e9939f9 | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS: 24 C indexing entries, mixed ignores, documentation assertions and warnings-denied locked Cargo check. Not runtime or ABI proof. | Recheck scaffold after local nio addition | passed |
| `personal/relational-engine/rust/Cargo.toml` | ✅ | 1791358966 | 33f1000fc827e58ee25f72b87a300eeb88ab51475b47734b87d59f79c845dce8 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/rust/README.md` | ✅ | 1791358975 | c6272c4f445e861ce5a71fab8c442d5a959a53eaec5719842cd6d137814ce8ce | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS scaffold metadata/README assertions and warnings-denied Cargo check; docs scope only, not allocator equivalence or visual acceptance | Refresh scaffold contracts for runnable Rust learning backend | passed |

### `personal/relational-engine/rust/include`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/rust/include/relational_memory.h` | ✅ | 1791358966 | c01c0b8d7fce2b32a1ae3a804412e80d2698e605aae7bfdc28a030d4da974cf1 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |

### `personal/relational-engine/rust/src`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/rust/src/ffi.rs` | ✅ | 1791358966 | 859d7e66e6657ae9946d1b66a3a96f7e65aa2bee5330df55dea4692d46be0a4e | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/rust/src/helloworld.rs` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/rust/src/lib.rs` | ✅ | 1791358966 | ea0910a8f994ab7f58cd87233c36c1cacaaedbd8df42a7e7de04e69063d22555 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/rust/src/mem.rs` | ✅ | 1791358966 | 80a5568cb7e2d0031924ccfdba5464192ec5bf77d5a5d18efd18cf92b848ad02 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/rust/src/string.rs` | ✅ | 1791358966 | df32022ab5a79b46eff5f2af63352861c08bd02bd68766e0dd38c5ca4e265196 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |

### `personal/relational-engine/src`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/src/LICENSE` | ✅ | 1791274133 | c9bff75738922193e67fa726fa225535870d2aa1059f91452c411736284ad566 | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS: 23 generated C compile entries, mixed ignore behavior, license labels, explicit docs status, locked offline warnings-denied Rust scaffold check. C implementation, IDE appearance, ABI, concurrency and Windows unproved. | Parallel C/Rust scratchpads, preserved license notices and CLion-only metadata | passed |

### `personal/relational-engine/src/io`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/src/io/cache.c` | ❌ | 1791274608 | 6435274eb70c4f86dc539e2558b7786d7d89e63f14e30c65ea9d53cd1a6f5030 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | stale — content changed; rerun required |
| `personal/relational-engine/src/io/cache.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/clipboard.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/clipboard_stub.c` | ✅ | 1791274608 | 9f88659f7432ad49f8713a17c17d3a90551703dcd20def0ce712e8c83a1101e5 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/file.c` | ✅ | 1791274608 | 42b964f3786f06b0e9f292e1f9fb0a4891a7319799794f5294bf1d77638acce4 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/file.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/filewriter.c` | ✅ | 1791274608 | b117c58fdd1539053e3ceedaeb7e63b79c555e71b7c63f5bbdf960479d576792 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/filewriter.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/hot_file.c` | ✅ | 1791274608 | bebbc0e02c564be6dd22de6bdbe3388a18c22e03c27cb0f8c3cea55350b978d4 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/hot_file.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/log.c` | ✅ | 1791274608 | 0dc44ca0f18864f29ed23e649b614aa1e899b7a585e014a53c72f4dec9e4811d | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/log.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/logkind.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/logparser.c` | ✅ | 1791274608 | 1c13a5a728738d0057f4b93d321af06635f6008800e1cd541d962a99d287a209 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/logparser.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/process_spawn.c` | ✅ | 1791274608 | 6e81ae47214f535a9f29fbd1e3d561bf4a0994a2190e1aa36d755bae47f26c1d | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/process_spawn.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/vexhome.c` | ❌ | 1791274608 | f5e8ed49de517bec71911a4f6a456d8295b5fa0c860ce9590b467fc32fccd7b6 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | stale — content changed; rerun required |
| `personal/relational-engine/src/io/vexhome.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/io/ws_client.c` | ✅ | 1791274608 | d20f9f579b7c9fde73d68ca94fcffd9bd890cc7af17ecfc095e27625d9101339 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/io/ws_client.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/relational-engine/src/nio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/src/nio/mem.c` | ❌ | 1791274608 | 96a7f1249f8d3ae61a7dd2dd288be6b95d542c67ba21567abeb487161135c194 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | stale — content changed; rerun required |
| `personal/relational-engine/src/nio/mem.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/relational-engine/src/reflection`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/src/reflection/class.c` | ✅ | 1791274608 | b22796fcbb9d7b961451095b295489a752b14a24e743b90629561301b845b34a | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/reflection/class.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/reflection/field.c` | ✅ | 1791274608 | 608e031972e5e8f432f4d5c34ce0007cf31b7ea2a0e72c077d2dca49138dae61 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/reflection/field.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/reflection/method.c` | ✅ | 1791274608 | e54281a6bd50d7a3dcc73f80f5c5c34693fed6806b85757eedc6b76cba21a312 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/reflection/method.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/reflection/struct.c` | ✅ | 1791274608 | ab9d124b0debc286455d7caeb0312cc607583839546a07af2380daf70d616cef | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/reflection/struct.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/reflection/variable.c` | ✅ | 1791274608 | 87b7ba7971865871375c025722362874d5894a160cd1d0aa02c8371a286f06fc | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/reflection/variable.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/relational-engine/src/relational`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/src/relational/cell.c` | ✅ | 1791274608 | 4e0eca5567fa500993d8e4cd76385629ff11623578eb54981afbab04dbc5eef0 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/cell.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/relational.c` | ✅ | 1791274608 | f65e05619573139013692284ea9d5ff3e28ca647f152be4aaa16e6c36a06e637 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/relational.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/shelf.c` | ✅ | 1791274608 | 7e45d2bd5b870a6d1d449ba2e3ac2ef2aa41af08c3116d34b08980610e32369d | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/shelf.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/symbol_table.c` | ✅ | 1791274608 | 4f5c3ac5cb41c09a3c6382c9a8243e5b7c3518b42ad576c4f78385a8768030ec | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/symbol_table.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/variable_hash_map.c` | ✅ | 1791274608 | 53824ab080f0b53a9e97c1356e4a647f6611d55a2574fa9d04eb020d7479bc3d | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/variable_hash_map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/variable_mini_map.c` | ✅ | 1791274608 | 5527988edf64abc51220c63b838c0ebad3e01efc64b34010fbdb6bfa005df8df | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/variable_mini_map.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/variable_pool.c` | ✅ | 1791274608 | 2ede5385fdfc0617a5aba2f8957dd3a394497359f8fe31bf797a38babbbc58de | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/variable_pool.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/relational-engine/src/relational/variable_slot.c` | ✅ | 1791274608 | 19c39337b9274d9e266fadeb2225e774698c0e35c462c0b3eeb62a04b3f55942 | ['/opt/homebrew/opt/python@3.14/bin/python3.14', '-c', 'import subprocess; subprocess.run(["cmake","-S",\'/Users/vexgraph/vexgraph/personal/relational-engine\',"-B",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"-DVEXSPOKE_SOURCE_DIR="+\'/Users/vexgraph/vexgraph/ecosystem/repos/vexspoke/src\'],check=True); subprocess.run(["cmake","--build",\'/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode/relational-build-audit\',"--clean-first","--target","reference_nio","reference_io","reference_relational","reference_reflection","-j","4"],check=True)']; macOS AppleClang C23 -Wall -Wextra -Werror: object compilation only, using current Vexspoke headers. No link, runtime tests, standalone closure, Rust ABI or Windows proof. | Compile all four imported C reference directories including newly moved nio | passed |
| `personal/relational-engine/src/relational/variable_slot.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/relational-engine/tests`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/tests/scaffold_test.py` | ✅ | 1791358975 | e806f3a2f31019637ceb47d03b42ef8772cea106b4d81a39e40ba4b58c40ea6f | ['python3', 'personal/relational-engine/tests/scaffold_test.py']; macOS scaffold metadata/README assertions and warnings-denied Cargo check; docs scope only, not allocator equivalence or visual acceptance | Refresh scaffold contracts for runnable Rust learning backend | passed |

### `personal/relational-engine/tests/rust`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/relational-engine/tests/rust/borrow_rejection.rs` | ✅ | 1791358966 | 12a9003370b2987f3b75df77f7cfbea8e68df38cf71f770f04f2da28bbb17068 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/tests/rust/ffi_test.c` | ✅ | 1791358966 | 795d3774f582c3a417e61eb32d501c616bb611a3ead6810ca83643529a5dccfa | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/tests/rust/mem_test.rs` | ✅ | 1791358966 | 0012fc005cb349bd234df603755e52fce38a318877c22a6c561739bf8e4dc8bd | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |
| `personal/relational-engine/tests/rust/run.py` | ✅ | 1791358966 | 81fea96d520c762f726b4c5d5a537eb5561afc4e1ffd5344958ce7a50ea97286 | ['python3', 'personal/relational-engine/tests/rust/run.py']; macOS debug/release Rust owner tests, doctest, E0502 compile rejection, strict C23 executable ABI and C-client ASan/UBSan; Rust internals not sanitizer-instrumented, OOM injection/concurrency/Windows/Vexspoke parity unproved | Learning byte ownership and standalone C copy ABI | passed |

## personal/vex-graph

### `personal/vex-graph`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/vex-graph/.DS_Store` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/README.md` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `personal/vex-graph/resources`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `personal/vex-graph/resources/.DS_Store` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/resources/b.png` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/resources/ecosystem.png` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/resources/personal-projects.png` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/resources/preferences-dot-md.png` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `personal/vex-graph/resources/vexgraph.png` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## tests

### `tests`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/.gitignore` | ✅ | 1791170944 | bb20c3cdc190ad90baebf66e1e01e2c0661eafd1664e2fe802342b83e5ff9795 | ['python3', '-c', "import subprocess; assert subprocess.run(['git','-C','tests','check-ignore','b/__pycache__/example.pyc'],capture_output=True).returncode == 0; assert subprocess.run(['git','-C','tests','check-ignore','--no-index','b/cli_test.py'],capture_output=True).returncode == 1"]; Git ignore-rule assertions only. | Generated Python bytecode is ignored and owning Python test source is not ignored. | passed |
| `tests/CMakeLists.txt` | ✅ | 1791277610 | 39a8bb4f0569319cae83d48acda191ab2af7d8a5ff8b56475158ceca1e5678a7 | ['python3', '-B', 'tests/tools/clion_adapter_test.py', '--generator', 'Ninja', '--ninja', '/Applications/CLion.app/Contents/bin/ninja/mac/aarch64/ninja']; macOS eight adapter checks plus two headless CTest runs; no interactive or CLion UI acceptance | Root and tests-only C23 models with explicit compiler resource headers | passed |
| `tests/LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/README.md` | ✅ | 1791337104 | 6ff1aac9118c36c48c8394724e3a5216ba9f08e911fdc2035191223969135dc8 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `tests/run.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/test-preferences.md` | ✅ | 1791124536 | 4e6f55113a128c731d9bc6dccf82c609f650645b3938c50943f34ad38cb3e4c8 | ['python3', 'tests/tools/compositor_contract_test.py']; macOS document/schema consistency and C23 client compilation only; not full law enforcement or appearance approval. | Documentation contract checks: Picture promotion counts, Application/Frame gallery lifecycle law, reference limitations, ownership clauses and compiled documented C client. | passed |
| `tests/test_support.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/api-haven`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/api-haven/ai_chat_sisters_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/ai_provider_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/ai_sse_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/app_broker_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/asset_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/db_provider_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/db_sqlite_file_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/harness_run_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/haven_ws_fanout_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/json_flex_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/mcp_server_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/probe_consumer_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/api-haven/webhook_loopback_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/b`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/b/adapter_support.py` | ❌ | 1791173599 | c113e2e10ea8982ad50738bd5921e905162dcce745f128362aee0528560619b1 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | stale — content changed; rerun required |
| `tests/b/arduino_test.py` | ❌ | 1791170808 | 874e24773cf5179e44c4e25d0e44312bc5f2c9d0c2db80d0886c763affa87faa | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 warnings, Rust 1.99/.NET SDK 10.0.401/R 4.6.1/Arduino CLI 1.5.1 AVR 1.8.8/CMake; scoped adapter and documentation evidence only. No flashing by this command, GUI automation, Windows, sanitizer, full workspace GPU or every public boundary claim. | Orchestrator regression: real C/JDK/Python/Rust/.NET/R execution, Arduino header validation and compile-before-upload mocks plus real Uno compile-only, CMake configure/build/rebuild, full source blueprint registries, JetBrains UI/XML docs and preserved workspace headless seam. | stale — content changed; rerun required |
| `tests/b/blueprint_test.py` | ❌ | 1791189490 | a4d288f54d1595c54f78d0cc9afe4b8a11cecfc548c592c94d9bb757eafe95a4 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | stale — content changed; rerun required |
| `tests/b/cli_test.py` | ❌ | 1791170808 | 1195d94a5f4ee600d4d9005b2bedb650fe21d9943f83a19735e153d9f2c5a412 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 warnings, Rust 1.99/.NET SDK 10.0.401/R 4.6.1/Arduino CLI 1.5.1 AVR 1.8.8/CMake; scoped adapter and documentation evidence only. No flashing by this command, GUI automation, Windows, sanitizer, full workspace GPU or every public boundary claim. | Orchestrator regression: real C/JDK/Python/Rust/.NET/R execution, Arduino header validation and compile-before-upload mocks plus real Uno compile-only, CMake configure/build/rebuild, full source blueprint registries, JetBrains UI/XML docs and preserved workspace headless seam. | stale — content changed; rerun required |
| `tests/b/cmake_test.py` | ❌ | 1791170808 | e22146c3e1cb947479c68fa2ff0b76c0badf3770ecbedcf4d25487f18c2d3c20 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 warnings, Rust 1.99/.NET SDK 10.0.401/R 4.6.1/Arduino CLI 1.5.1 AVR 1.8.8/CMake; scoped adapter and documentation evidence only. No flashing by this command, GUI automation, Windows, sanitizer, full workspace GPU or every public boundary claim. | Orchestrator regression: real C/JDK/Python/Rust/.NET/R execution, Arduino header validation and compile-before-upload mocks plus real Uno compile-only, CMake configure/build/rebuild, full source blueprint registries, JetBrains UI/XML docs and preserved workspace headless seam. | stale — content changed; rerun required |
| `tests/b/cpp_test.py` | ✅ | 1791173599 | 0aaa5779fd5590f89cb02c14bd587324435f13b2a691c2fe6b2478e101cb7e73 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/html_test.py` | ✅ | 1791173599 | 7c31bba3d1381acbb2bdc7a045809b25af9b301948dcbcc9e6cc0d8e67217619 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/inspect_test.py` | ✅ | 1791189490 | 412edc45650067a20befbdd7c575e630f3ac0c4809c21264d783a75b0d413688 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/javascript_test.py` | ✅ | 1791173599 | 291786163bf6ca2b69bf5e91ce932ccfbd8b5d3a53cf7bc953c07412fc4df8a9 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/npm_test.py` | ✅ | 1791189490 | 3102c6f5dff2a9802e6d8d8effc48b56301d5681f526df43697f4b4235fd5a61 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/objc_test.py` | ✅ | 1791173599 | 04cfe03a7b238742fabc47e01137a3fdc549d85dc3195ce46b358569b1e614e4 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/php_test.py` | ✅ | 1791173599 | 842511e42aa56942ec65350058cfb6e9db55fbc479f59e1c1c6e57752497f836 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/preferences_test.py` | ❌ | 1791177866 | 9b12ddf4fbaf24f3a2fed6fd856236f640d07d8eaa50f7b698a9d3194919128e | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 build and automated source checks; workspace doctor/IDE metadata/headless compositor target executed. No blanket preference compliance, GUI rendering, Windows or whole-engine public-surface proof. | Reference-form correction and regression gate: all b C sources/headers reject pointer-member arrows; comments/output preserved; full 89-test adapter and workspace headless regression. | stale — content changed; rerun required |
| `tests/b/readme_test.py` | ❌ | 1791189490 | 1e9906ca035a83a1324f438272322d647c6c671ab0ec92fb8529320d197430ed | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | stale — content changed; rerun required |
| `tests/b/shell_test.py` | ✅ | 1791173599 | 8751acd5bac01b9fc46d2b6f1f32e457a46a590d368ab66d9d39754a49de980a | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/sql_test.py` | ✅ | 1791173599 | 1b0dd303653219e1fff4558f4b5c71f32d89890d551eb6a9ce781a664978ed75 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/swift_test.py` | ✅ | 1791173599 | c1e5d9ca08b2f6e1d6dfca0f8156951fd68a60e72f12b2a9e54d5243d6603fbe | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/typescript_test.py` | ✅ | 1791173599 | 470778b9bab585279038f11e971c80eb329feb46a4f46c20f9609b19d438d85b | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/util_test.c` | ✅ | 1791173599 | 13f3569b866a91265f5c8cc3bf292406e4278b9f5f15b31319ed8647ef3581fd | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/util_test.py` | ✅ | 1791173599 | ae1e1b14354257064a27129ea89fcbec24a6c577cdca9072b88e6197e05ab52b | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64; strict C23 compilation; new shared helper boundaries checked with ASan/UBSan (leak detection disabled). SQL uses a temporary socket-only cluster; HTML uses a mock opener. No flashing, browser rendering, Windows proof, full API battle-test coverage or packaging/export claim. | 85-test orchestrator regression: real native and script adapters, literal argv, syntax failures and recovery, npm script delegation, isolated PostgreSQL transaction rollback, headless HTML opener, source blueprints and JetBrains documentation. | passed |
| `tests/b/workspace_test.py` | ✅ | 1791278081 | c5536bcf61f5395593fed7df5e3e0b525e89e88fe5b86c34987c602f48267ea5 | ['python3', '-B', 'tests/b/workspace_test.py']; macOS eight headless workspace regressions using isolated B_HOME to avoid concurrent user gallery build; CPU compositor scope only, no interactive launch | Isolated workspace regressions after complete non-test IDE export | passed |

### `tests/b/adapters`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/b/adapters/__init__.py` | ✅ | 1791189490 | 1fd07c1bb7f6b76360cc51f901a9c89458165241b4bd24cd540ee8c932e34cb5 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/adapters/adapter_test.py` | ✅ | 1791189490 | fafd501ee7185e01e1c7da693675187fe3f1724dd8cf257150f0a378be85f3a8 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/adapters/cargo_test.py` | ✅ | 1791189490 | cd1f537645ed73bdc565e63e7e3275ccbbbe86d1abf1d40109f639c9b443e963 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/adapters/go_test.py` | ✅ | 1791189490 | 166927222156523639bd11fa4d6c26f233804d3f491c90a8d0c104b907fa7b88 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/adapters/lua_test.py` | ✅ | 1791189490 | 44b1ff93e483724dc6f8eb88841da78f15b6280d59976d04e7b5a46611f6667e | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |
| `tests/b/adapters/zig_test.py` | ✅ | 1791189490 | 0b2cccfa55ea07b77df8da443584bbfa4089aec33d8e4d8b98d3e4ac2e3116d9 | ['python3', '-m', 'unittest', 'discover', '-s', 'tests/b', '-p', '*_test.py', '-v']; macOS arm64 strict C23 -Wall -Wextra -Werror; Go 1.27.1/Lua 5.5.1/Zig 0.17.0/Cargo 1.99. Registry/shared helper ASan+UBSan; leak detection disabled. SQL isolated socket-only cluster, browser and Zig argv launch fixtures; Uno compile-only, no flashing. No Windows/Linux, GUI appearance, full public API battle-test, packaging or arbitrary-pointer validation claim. | Adapter migration and standalone discovery: real Go/Cargo/Lua/Zig execution and recovery, all existing adapters, exact manifest selectors, no-execution doctor, registry header client, blueprints and workspace regressions. | passed |

### `tests/darling`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/BATTLE_TESTS.md` | ✅ | 1791038820 | 46881e2ccfb859a39db083403200e9e21ecadff0bffe1709e510853f58a4f614 | ['python3', '-B', '-c', 'from pathlib import Path; p=Path("tests/darling/BATTLE_TESTS.md"); s=p.read_text(); assert "All 15" in s; assert len(list(Path("tests/darling").rglob("ui_*test.c"))) == 15; assert Path("_notes/darling/ui-battle-testing.md").is_file(); assert "207 passed" in s and "timed out" in s; print("UI battle documentation inventory and references consistent")']; Documentation-only inventory/link/count consistency: confirms 15 ui_ source targets and referenced local notes exist; not independent execution evidence or proof of every prose claim. | Automated lab evidence only; visual approval not recorded | passed |
| `tests/darling/darling_tests.c` | ✅ | 1791048317 | 03128d839a736fcf302440a27f8887c521f52ab58dca50fd6a752a9e9a734a12 | ['./tools/b', 'build', 'darling']; Incremental C23 -Wall -Wextra -Werror Darling targets, including interactive branches and hello/gallery app bundles; no manual viewer/app launched, no runtime or visual acceptance for those demos | Umbrella Darling compile/link and app packaging with shared Application starter include paths | passed |
| `tests/darling/test_application.h` | ❌ | 1791048098 | 614f2113af2943db02254c043211541a68541c5405af88baf6c79f2d5f5a2dd9 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["./tools/b","run","tests/darling/frame/frame_application_lifecycle_test.c"],check=True); subprocess.run(["./tools/b","run","tests/darling/frame/frame_fps_focus_test.c"],check=True)']; macOS Apple Silicon: worker callback/return, owner invoke-close, hidden vs closed two-window lifetime, close-all/join, Frame attach/detach; native focus requests + deterministic R3 -1/1/120 submission ceilings/coalescing with independent scene worker. Header client/API evidence only; no whole-contract, Linux/Windows, physical display-Hz or visual approval | Application/Frame lifecycle and R3 presentation-cap integration owner tests | stale — content changed; rerun required |

### `tests/darling/capture`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/capture/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/capture/ui_clip_restore_battle_test.c` | ✅ | 1791047986 | 992496b66b1aa69422721bf9c9f9f138d32741f0f5829f8374965b1f0e9669a8 | ['./tools/b', 'run', 'tests/darling/capture/ui_clip_restore_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/capture/ui_clip_scope_battle_test.c` | ❌ | 1791047988 | df188636b4a3a4e15ca44de8e1da51c50b0a2cb18cb30ea16794ff2a7fa07149 | ['./tools/b', 'run', 'tests/darling/capture/ui_clip_scope_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/capture/ui_multiple_masks_pixels_test.c` | ✅ | 1791047989 | 9ce1ca323a9e7154067813af9195c7c275272a522e1d21e1bb423a4144b1b0b7 | ['./tools/b', 'run', 'tests/darling/capture/ui_multiple_masks_pixels_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/capture/ui_nested_clip_battle_test.c` | ✅ | 1791047991 | 19ed297ff79efd8ca9dc9546b954971503871c7cf2deb3d255c376f1a440ce94 | ['./tools/b', 'run', 'tests/darling/capture/ui_nested_clip_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/capture/ui_rounded_clip_battle_test.c` | ✅ | 1791047992 | 11c2d74a470913f88bfa5d69801045a5ad5319a9447abfb09ba24504bc7b950e | ['./tools/b', 'run', 'tests/darling/capture/ui_rounded_clip_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/capture/ui_rounded_hit_battle_test.c` | ✅ | 1791047994 | e0c0307d4677aa18709fba25f814c410c7080bc4233080f011998aff2eea1ac3 | ['./tools/b', 'run', 'tests/darling/capture/ui_rounded_hit_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/codefield/languages`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/codefield/languages/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/color`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/color/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/color/ui_border_pixels_battle_test.c` | ✅ | 1791047995 | 8eebce211e4907880e28a08dc65d5c4473f2ee2220bfef3e8550fa3f932844d2 | ['./tools/b', 'run', 'tests/darling/color/ui_border_pixels_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/color/ui_visual_state_pixels_test.c` | ✅ | 1791047996 | 1f065345ed706859fb8bfd597601b740abb860c41f460b0c347f17a3109d81f5 | ['./tools/b', 'run', 'tests/darling/color/ui_visual_state_pixels_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/compositor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/compositor/filter_gallery.c` | ✅ | 1791343963 | e39eb8be7c8bc0251ad7fe7947eb55be9a250fa72e2bdb35c10f04b8821eb871 | ['python3', '-c', 'import subprocess; [subprocess.run(["tools/b","test",name],check=True,timeout=120) for name in ("sampled_image_test","filter_gallery_fixture_test","gpu_scope_test","image_test","picture_test","vk_renderer_test")]; subprocess.run(["python3","-B","tests/tools/compositor_contract_test.py","CompositorContractTest.test_gallery_uses_gpu_scope_not_cpu_fixture","CompositorContractTest.test_sampled_image_constructor_dispatch","CompositorContractTest.test_documented_reference_client_compiles","CompositorContractTest.test_filter_constructor_arity_is_rejected_for_intended_reason","CompositorContractTest.test_gpu_color_pass_public_arity_and_no_cpu_extension"],check=True,timeout=60); subprocess.run(["python3","-B","tests/tools/filter_gallery_resources_test.py"],check=True,timeout=180)']; macOS strict registered image/sample/scope/Picture/renderer tests; gallery app build/bundle only, no window/presentation/appearance or memory profiling. Five selected docs checks, not full legacy-path/wiki suite; readiness wiki and Darling lawbook unavailable. Numeric/ASan/UBSan proof recorded separately. | Final strict registered owner runs, five selected documentation/constructor checks and gallery resource bundle build/refresh/signature, no interactive launch | passed |
| `tests/darling/compositor/filter_gallery_fixture.h` | ✅ | 1791343936 | 678267f77a1ec6e3e6d011a9f810ef3fc16e8d3fd9baa00dd95b3fd4d59a22ab | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `tests/darling/compositor/gallery_photo.c` | ✅ | 1791342045 | cbf97fb14ed43f85639769ad0159c63652339a6da0aefac3d83a2081bb3b2efb | ['python3', '-B', 'tests/tools/filter_gallery_resources_test.py']; macOS noninteractive build and ImageIO runtime; missing-resource child intentionally fails its assertion. No interactive/gallery appearance, other-host, macOS14 runtime, injected allocation failure or oversized source metadata proof | Final strict gallery bundle build/incremental resource refresh/signature; optimized ASan/UBSan decoder owner and relocated bundle/no-fallback proof | passed |
| `tests/darling/compositor/gallery_photo.h` | ✅ | 1791342045 | 85e11585756e4e6fa1129ef8d2197ac32e5123530610f3f8968da99862005413 | ['python3', '-B', 'tests/tools/filter_gallery_resources_test.py']; macOS noninteractive build and ImageIO runtime; missing-resource child intentionally fails its assertion. No interactive/gallery appearance, other-host, macOS14 runtime, injected allocation failure or oversized source metadata proof | Final strict gallery bundle build/incremental resource refresh/signature; optimized ASan/UBSan decoder owner and relocated bundle/no-fallback proof | passed |
| `tests/darling/compositor/gallery_photo_test.c` | ✅ | 1791342064 | c2b816df4932f6973940faf13e36eea90d5c0d753de3359def8f8d0c340ebf7e | ['tools/b', 'test', 'gallery_photo_test']; macOS strict C23 owner target, not GUI appearance or other-platform proof | Registered owner target executes final photo decoding/orientation/negative/recovery assertions | passed |

### `tests/darling/cursor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/cursor/set_cursor_test.c` | ❌ | 1791047998 | b90cd72acfe8919dcafa62ebf7a258d5fd3406be6004dc4fb5c861487449e98e | ['./tools/b', 'run', 'tests/darling/cursor/set_cursor_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |

### `tests/darling/dialog`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/dialog/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/drawable`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/drawable/picture_test.c` | ✅ | 1791343774 | d644b41c52697fae707f7ac1bd9d9704bc378ee72d84c3bbea2de3637d558146 | ['tools/b', 'test', 'picture_test']; macOS headless widget admission/lifetime only; no real Frame/presentation or appearance proof. Actual texture pixels proven separately by sampled_image_test and filter_gallery_fixture_test. Missing Darling repo-local lawbook remains a reading gap. | Strict registered Picture owner: CPU and GPU-only borrowed Image admission, symmetric widget operations, rejection diagnostics, detach and Image ownership | passed |

### `tests/darling/event`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/event_invoke_test.c` | ✅ | 1791047999 | 8328c76ac6ef903c86ec4946e934baf577416e169460770ff0d5b54c2df4d570 | ['./tools/b', 'run', 'tests/darling/event/event_invoke_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/event/event_kinds_test.c` | ✅ | 1791048001 | 5c7fd313ff21019ef55b3b6b558e8102c0a1bb1147934ea8a4a79118688e1efa | ['./tools/b', 'run', 'tests/darling/event/event_kinds_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/event/document`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/document/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/event/focus`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/focus/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/event/key`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/key/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/event/mouse`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/mouse/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/frame`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/frame/README.md` | ✅ | 1791048306 | b89448c5f8f27f8e8365e8e4a39d333745b588978efffcadd42057eec1a596e1 | ['python3', '-B', 'tests/tools/darling_lifecycle_test.py']; Three structural/documentation checks: all C starters use Application, no test-written pump loop, sample compiles freshly with C23 warnings-as-errors, links/README command homes and amended lifecycle-law phrases. No full documentation correctness, runtime or visual proof | Darling starter migration inventory and documented lifecycle API compilation | passed |
| `tests/darling/frame/borderless_frame_test.c` | ✅ | 1791048002 | 6b15a1ecaf644e377f58a235e83c6e6de11e12e4c30c0a0158177e736f6abf7b | ['./tools/b', 'run', 'tests/darling/frame/borderless_frame_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/frame/frame_application_lifecycle_test.c` | ❌ | 1791048004 | 105b67b02f60084f1b62082b28059f86d7df9767411d3f9b37a5c8a0f6fc8112 | ['./tools/b', 'run', 'tests/darling/frame/frame_application_lifecycle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/frame/frame_chrome_lab.h` | ❌ | 1791046167 | fdb31bc8d15e1d7e4f6189d12a12c2923786e744e783d4455b89aa71c5e78075 | ['python3', '-B', '-c', 'import subprocess; targets=("naked_frame_test", "borderless_frame_test", "traffic_light_frame_test"); [subprocess.run(["./tools/b", "test", t], check=True) for t in targets]']; macOS; C23 -Wall -Wextra -Werror. Empty-content pixel samples, capture dimensions, resize, paint/transparency independence, reversible mode changes; macOS traffic-light visibility preferences/header position/floating state. Interactive branches built but not manually executed. No native chrome pixel oracle, focus/input automation, Window Server appearance or visual approval; existing Liquid Glass harness not exercised. | Three live Frame chrome tests: naked, borderless and traffic-light modes. | stale — content changed; rerun required |
| `tests/darling/frame/frame_drag_lab.h` | ✅ | 1791046178 | e8233e3abea9bc1920fac2def301e22c1e2a03071eb1560645f685d41b79cb63 | ['./tools/b', 'test', 'ui_matryoshka_battle_test']; macOS live Frame; 96 captured rings, mutation/hit/resize, scripted down/drag/up through Element_dispatchEvent, selected subtree offset and before/after pixel assertions, release off selected panel inside Frame, PropertyPool reclamation. Interactive mode compiled, not manually viewed. No outside-window pointer capture, native physical-device automation, keyboard focus or visual acceptance. | Matryoshka nested-panel capture and dispatcher-driven drag regression. | passed |
| `tests/darling/frame/frame_fps_focus_test.c` | ❌ | 1791125554 | 107792d27ba1b4409254538289761668a2953a0e277ff37b82cef8eb08ad291f | ['python3', '-c', 'import subprocess; targets=("frame_fps_focus_test","ui_revalidate_cascade_battle_test","frame_resize_test"); [subprocess.run(["tools/b","test",target],check=True) for target in targets]']; macOS numeric native owner assertions; no appearance or hardware drag timing approval. | Resize fix regressions: normal focus/FPS pacing, ordered revalidation and native target size rebuilds. | stale — content changed; rerun required |
| `tests/darling/frame/frame_live_resize_test.c` | ✅ | 1791125522 | 0bc7b44896d7134965339234d2ed38c67992b92a6c3114265c2d988d3a2c0291 | ['tools/b', 'test', 'frame_live_resize_test']; macOS Apple Silicon IOSurface programmatic geometry callback and C23 header client; CA live-flush branch, actual display timing, manual drag smoothness, fractional geometry and other platforms remain unproved. | Frozen-clock native resize: changed bounds publish immediately despite 30 Hz content cap, restore cap, update native pixels; ordinary demand remains deferred and invalid/same extents do not publish. | passed |
| `tests/darling/frame/frame_resize_test.c` | ❌ | 1791125554 | 4f0ad626469f36dab01699cf82f552da29a7a31db078d4c7aaf2f44a9fe61843 | ['python3', '-c', 'import subprocess; targets=("frame_fps_focus_test","ui_revalidate_cascade_battle_test","frame_resize_test"); [subprocess.run(["tools/b","test",target],check=True) for target in targets]']; macOS numeric native owner assertions; no appearance or hardware drag timing approval. | Resize fix regressions: normal focus/FPS pacing, ordered revalidation and native target size rebuilds. | stale — content changed; rerun required |
| `tests/darling/frame/frame_test.c` | ❌ | 1791048008 | b7a5699cfe72ca151c5e26d4c4cc60d5b74f56c136f4521b351dd9011de69b0e | ['./tools/b', 'run', 'tests/darling/frame/frame_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/frame/hello_window.c` | ✅ | 1791048317 | b4e1f7117e65ec72a57962693b82685f8a184cc8e238f0b40c69eaa39db4062b | ['./tools/b', 'build', 'darling']; Incremental C23 -Wall -Wextra -Werror Darling targets, including interactive branches and hello/gallery app bundles; no manual viewer/app launched, no runtime or visual acceptance for those demos | Umbrella Darling compile/link and app packaging with shared Application starter include paths | passed |
| `tests/darling/frame/liquid_glass_frame_test.c` | ❌ | 1791048211 | a94f2dfd543048b9199c68f6c736fb58da883d26e265c168c7dc7b32964e1bd1 | ['./tools/b', 'run', 'tests/darling/frame/liquid_glass_frame_test.c']; macOS; descriptor/tree/capture assertions only, not Liquid Glass/anchor appearance or manual visual approval | Application-owned bounded default mode; interactive lifetime compiled but not entered | stale — content changed; rerun required |
| `tests/darling/frame/naked_frame_test.c` | ✅ | 1791048010 | a91abcb1b760f4ad3c779f7f4c7091485b64718c1a775bb793916e191189342b | ['./tools/b', 'run', 'tests/darling/frame/naked_frame_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/frame/traffic_light_frame_test.c` | ✅ | 1791048011 | 0b367bfd95f36acfa7f9c09ff6c0770e643e6d4f6657e98a7ca13ed7da5729c8 | ['./tools/b', 'run', 'tests/darling/frame/traffic_light_frame_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/frame/ui_matryoshka_battle_test.c` | ✅ | 1791048013 | 5622df499b1479e2b718e4b77773a39af32db1e781190fe889165c82a7a7eb8a | ['./tools/b', 'run', 'tests/darling/frame/ui_matryoshka_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/frame/ui_revalidate_cascade_battle_test.c` | ❌ | 1791125554 | 176437dc9b3fc459cb2b92dae3b9b134ffe21cc7c58cb961ff4c807640d78b57 | ['python3', '-c', 'import subprocess; targets=("frame_fps_focus_test","ui_revalidate_cascade_battle_test","frame_resize_test"); [subprocess.run(["tools/b","test",target],check=True) for target in targets]']; macOS numeric native owner assertions; no appearance or hardware drag timing approval. | Resize fix regressions: normal focus/FPS pacing, ordered revalidation and native target size rebuilds. | stale — content changed; rerun required |
| `tests/darling/frame/ui_wide_tree_battle_test.c` | ❌ | 1791048017 | 6717152c4a997a2558529fd77c6ee10904c940c38bb19528b938291887206dd4 | ['./tools/b', 'run', 'tests/darling/frame/ui_wide_tree_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |

### `tests/darling/graph`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/graph/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/kinematics/anchor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/kinematics/anchor/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/kinematics/anchor/ui_anchor_pivot_pixels_test.c` | ✅ | 1791048019 | 07bf7604580ff6f728c4d04fb6eaff0d90e96057afce088c777fd53eff433da3 | ['./tools/b', 'run', 'tests/darling/kinematics/anchor/ui_anchor_pivot_pixels_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/kinematics/pivot`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/kinematics/pivot/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/kinematics/pivot/ui_size_limits_pixels_test.c` | ❌ | 1791048021 | a9af8e221919a4a9d211f0252db904892e46946c1e196b45fcc31e8b02593890 | ['./tools/b', 'run', 'tests/darling/kinematics/pivot/ui_size_limits_pixels_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |

### `tests/darling/panel/flexpanel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/flexpanel/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/panel/panel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/panel/panel_add_test.c` | ❌ | 1791048022 | 22fa8c15736614376bd2138314e91e16439345be0813977db57829c1a03060a6 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_add_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/panel/panel/panel_anchor_corner_radius_test.c` | ❌ | 1791048213 | 0eb175d1177d4ede685e484d788e5b1f1fc280007e67e6adf89ba4b352b713e4 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_anchor_corner_radius_test.c']; macOS; descriptor/tree/capture assertions only, not Liquid Glass/anchor appearance or manual visual approval | Application-owned bounded default mode; interactive lifetime compiled but not entered | stale — content changed; rerun required |
| `tests/darling/panel/panel/panel_corner_radius_test.c` | ❌ | 1791048023 | 3e85c1e4697226a1dce7fe78fe7c707b4498da2dbaa42f115ceb44e9c6c61722 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_corner_radius_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/panel/panel/panel_events_test.c` | ✅ | 1791048025 | 05acb1b99d47153b72e421fcac6fdf661ea5295cff1f4fd53d00128215c956aa | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_events_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/panel/panel/panel_matryoshka_test.c` | ✅ | 1791048026 | 981ffb6a615e87306374f1ef49efd172cc44ef424d36eea141af956fa15005a4 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_matryoshka_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/panel/panel/panel_shadow_test.c` | ✅ | 1791048028 | 55a2128e3840d59f32c363589fa99ddd0b8c3ed7216bd564883edc15437174c0 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_shadow_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |
| `tests/darling/panel/panel/panel_test.c` | ❌ | 1791048029 | dec46d6886cfb8160cb9722e6c147f34206d2c004e793ff9a3945a984608fa33 | ['./tools/b', 'run', 'tests/darling/panel/panel/panel_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |

### `tests/darling/panel/scrollpanel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/scrollpanel/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/panel/scrollpanel/scroll_panel_test.c` | ❌ | 1791048031 | d0efce8ae3735df3c67292d48fe2d2dbc67b45cba10d9073dee8624e97a94e16 | ['./tools/b', 'run', 'tests/darling/panel/scrollpanel/scroll_panel_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |
| `tests/darling/panel/scrollpanel/ui_scroll_capture_battle_test.c` | ✅ | 1791048032 | 17688bce9ceed67bcda741043efbba2ba40c6e2c8cac8979b536311979804220 | ['./tools/b', 'run', 'tests/darling/panel/scrollpanel/ui_scroll_capture_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/panel/shared`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/shared/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/darling/panel/shared/ui_element_contract_battle_test.c` | ✅ | 1791048034 | 4566fbf3f110a5e94d008ec702bd2aca53d0568aaf506e9a72ca9a580312aba9 | ['./tools/b', 'run', 'tests/darling/panel/shared/ui_element_contract_battle_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | passed |

### `tests/darling/picker`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/picker/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/properties`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/properties/properties_test.c` | ❌ | 1791048035 | b3fa38d7ec52b9a678ab3da44920bdf439da5e8595e789fdf63f3954e2fe344a | ['./tools/b', 'run', 'tests/darling/properties/properties_test.c']; macOS Apple Silicon; only this test assertions, no physical display-Hz, other-platform or visual approval | Darling owner assertions under blocking Application lifetime; native windows, no manual viewing | stale — content changed; rerun required |

### `tests/darling/scaffold`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scaffold/scaffold_lab_test.py` | ✅ | 1791040802 | adb6d18b955550b1d29ac8891b048fc2a140c271822238e78a340507e5124a0c | ['python3', '-B', 'tests/darling/scaffold/scaffold_lab_test.py']; 126 draft/incomplete pairs: manifest/blueprint/path/include-guard checks, no callable stub APIs/storage/event opt-ins, aggregate header coexistence and fresh syntax compilation of each .c with -std=gnu23 -Wall -Wextra -Werror. Documentation evidence checks source rows only, not historical prose or ratings. No runtime/layout/input/render/lifetime/performance or visual acceptance proof. | Draft scaffold structure, current source-map consistency and fresh C23 compilation; no component implementation. | passed |

### `tests/darling/scene/2d`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/2d/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/scene/3d`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/3d/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/scene/properties`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/properties/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/text/emoji`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/emoji/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/text/label`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/label/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/text/markdown`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/markdown/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/darling/text/richtext`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/richtext/.gitkeep` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/graphvex`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/board_test.c` | ❌ | 1791038758 | bc172367bdeb85f0783ffe8887072c13897bc7df247d7e5b36679b6f51095290 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | Automated lab evidence only; visual approval not recorded | stale — content changed; rerun required |
| `tests/graphvex/element_property_test.c` | ✅ | 1791038758 | d01c6a47a5abb7322bea757780519e8d253443527e2340e3f6c05693bbfa8743 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | Automated lab evidence only; visual approval not recorded | passed |
| `tests/graphvex/element_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/image_test.c` | ✅ | 1791343936 | 5f203b1adc5dfa8955b6b6c727f6337d163fcadd8c5342c308a735429525a643 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

### `tests/graphvex/compositor`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/compositor/color_pass_test.c` | ✅ | 1791191596 | b59553242f494adef027190aac0484972b46943bd11d086dd2434d982eb25921 | ['python3', '-c', 'import pathlib,subprocess,tempfile,os; directory=tempfile.TemporaryDirectory(prefix="gpu-color-isolated-",dir="/private/var/folders/z7/x7y_f3fd7vnchzxx7gmwkhp40000gn/T/opencode"); base=pathlib.Path(directory.name); shaders=base/"out/debug/shader/compositor"; shaders.mkdir(parents=True); root=pathlib.Path("ecosystem/drivers/graphvex/src"); [subprocess.run(["glslangValidator","-V","-I"+str(root),str(root/"shaders/compositor"/name),"-o",str(shaders/(name+".spv"))],check=True,timeout=30) for name in ("resolve.vert","color.frag")]; binary=str(base/"color_pass_test"); command=["cc","-std=gnu23","-Wall","-Wextra","-Werror","-O2","-g","-mcpu=apple-m1","-mmacosx-version-min=14.0","-fsanitize=address,undefined","-fno-omit-frame-pointer","-Iecosystem/drivers/graphvex/src","-Iecosystem/vexspoke/src","-Itests","-I/opt/homebrew/include","tests/graphvex/compositor/color_pass_test.c","ecosystem/drivers/graphvex/src/compositor/color_pass.c","ecosystem/drivers/graphvex/src/vulkan/device.c","-L/opt/homebrew/lib","-lvulkan","-Wl,-rpath,/opt/homebrew/lib","-o",binary]; subprocess.run(command,check=True,timeout=60); subprocess.run([binary],env=dict(os.environ,B_HOME=str(base)),check=True,timeout=30); directory.cleanup()']; Apple A18 Pro macOS -O2 strict C23 assertions retained, ASan/UBSan host code plus real Vulkan shader texture draws/readback/recovery. Fresh isolated SPIR-V from current sources with canonical include root; 60s compile/30s runtime watchdog and 100ms fences. Local loader dylib built macOS26, macOS14 runtime floor unproved. No shader sanitizer, validation layers, injected OOM, denormal parity, automatic scope/tree wiring or Windows proof; LeakSanitizer unavailable. | Optimized sanitized Vulkan color owner using freshly compiled isolated shaders, not stale b outputs. | passed |
| `tests/graphvex/compositor/compositor_image_test.c` | ❌ | 1791125693 | f55540c2d5027222496ac03c20ca8cd02abc2ea087dcf48180cb82e12e5c3aa2 | ['tools/b', 'test', 'compositor']; macOS CPU/raster reference foundation; no automatic widget stacks, GPU filters or production readiness inference. | Final CPU compositor feature regression: ordered scatter, origin-aware bounds, RGBA image adapters, display-list placement and failure contracts. | stale — content changed; rerun required |
| `tests/graphvex/compositor/compositor_scope_test.c` | ✅ | 1791124223 | 8eab1294eb85ed8233605716c968d93ecee81b88a88d7674964b5869b01615f3 | ['tools/b', 'test', 'compositor_scope_test']; macOS CPU reference; opaque prior scene, rectangular masks, inline tokens only; no automatic tree integration or GPU filter execution. | Explicit three-scope CPU compositor: prefix replacement, foreground clipping, whole-group spill, crop origins and rejection/recovery. | passed |
| `tests/graphvex/compositor/compositor_submit_test.c` | ✅ | 1791125693 | 0caf47600abc5bd90896934963443db971586616020638902b0a653681291347 | ['tools/b', 'test', 'compositor']; macOS CPU/raster reference foundation; no automatic widget stacks, GPU filters or production readiness inference. | Final CPU compositor feature regression: ordered scatter, origin-aware bounds, RGBA image adapters, display-list placement and failure contracts. | passed |
| `tests/graphvex/compositor/compositor_test.c` | ✅ | 1791125693 | 4410bb558ea2cb8415f20ec4c2908f489562e339f6fa8d5dcea9fef1a67d4bae | ['tools/b', 'test', 'compositor']; macOS CPU/raster reference foundation; no automatic widget stacks, GPU filters or production readiness inference. | Final CPU compositor feature regression: ordered scatter, origin-aware bounds, RGBA image adapters, display-list placement and failure contracts. | passed |
| `tests/graphvex/compositor/filter_gallery_fixture_test.c` | ✅ | 1791343936 | 781c6b3911bfd8e90c5d1e4d09a904f852f2b6bba6405beb76b8a78a824e9da4 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `tests/graphvex/compositor/filter_pool_test.c` | ✅ | 1791125706 | 26855d6b413ece20f1c2a7a3761b227ff391c357cf905a869e1bed3e37343495 | ['tools/b', 'test', 'filter']; macOS CPU filter recipes only; no automatic complex GPU parameter schema or shader execution. | Final compact filter-token/pool regression: inline decoding, ordered immutable recipes, retain/release and stale generation rejection. | passed |
| `tests/graphvex/compositor/gpu_scope_test.c` | ✅ | 1791343936 | 7c00b629dcd15db5116e428131d0e60f2baf7d1a68126c066759e30c875ae477 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |

### `tests/graphvex/filter`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/filter/filter_functions_test.c` | ❌ | 1791191159 | c4369d379cbc3ad93e472f30b6c41d1288fe31979a9a8f684898a62fd990e643 | ['tools/b', 'test', 'filter']; macOS strict C23 debug assertions active; canonical constructor bits and unsupported CPU submission restored. GPU token admission is proved separately by ColorPass, not this reference seam. | Five filter regressions after CPU color rollback and Vulkan-only constructor documentation. | stale — content changed; rerun required |
| `tests/graphvex/filter/filter_type_test.c` | ✅ | 1791181278 | 6edda4eedcdcc785a0cfd73d8adf3e043f619f542420f73f360ba92b28d6cfc5 | ['tools/b', 'test', 'filter']; Apple Silicon macOS debug C23 -Wall -Wextra -Werror assertions active; exact hexadecimal FILTERNAME_ID registry and compatibility aliases, pure constructors and unsupported rejection. No new effect execution, typed pool/COW/migration or GPU runtime proof. | Registered filter constructor/registry/legacy/pool regressions after FILTERNAME_ID naming correction. | passed |

### `tests/graphvex/graphics`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/graphics/capture_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/graphics/graphics_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/graphics/image_runs_test.c` | ✅ | 1791124215 | f429ba6d9f98539daeb9087455fd094af5abafbaef69a31813a6c5196e3088f5 | ['tools/b', 'test', 'image_runs_test']; macOS raster/reference image path only; other graphics APIs and GPU texture pipeline not re-proven. | Nearest CPU-shadow color runs, retained stride, clipping, invalid geometry, callback failure and actual raster image pixels. | passed |
| `tests/graphvex/graphics/render_loop_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/graphics/viewport_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/graphvex/lang`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/lang/filter_test.c` | ✅ | 1791125706 | ee3c17edbe408a9bffbc24fa9b493fa5651da4b85bec3e25d19bff4593e0a60f | ['tools/b', 'test', 'filter']; macOS CPU filter recipes only; no automatic complex GPU parameter schema or shader execution. | Final compact filter-token/pool regression: inline decoding, ordered immutable recipes, retain/release and stale generation rejection. | passed |

### `tests/graphvex/nio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/nio/pool_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/nio/property_pool_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/graphvex/ui`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/ui/element_bounds_test.c` | ❌ | 1791122851 | afb2b6e117cffab6809ab907c0c978f18697fe630f11164d8a2a5c7ef9301fe3 | ['tools/b', 'test', 'element']; macOS arm64 debug four Element suites: event/absolute geometry, all anchor/pivot combinations, descendant support, default rounded child clipping, own halos, hit independence and existing Property/Element contracts. No automatic filter attachment, GPU pixels, OOM/concurrency or hostile cyclic-tree proof. | Executed two-bound geometry seam and existing Element regressions. | stale — content changed; rerun required |
| `tests/graphvex/ui/element_image_test.c` | ❌ | 1791124241 | 6c4f27a3097ea4f9ff3e9c2f82a874081f9c1464e4c66c6af4e7216170f45ba2 | ['tools/b', 'test', 'element']; macOS CPU/raster Element tests; no GPU filter attachment or visual approval. | Element image borrowing, raster colors, parent and rounded clipping plus existing bounds/property regressions. | stale — content changed; rerun required |

### `tests/graphvex/vulkan`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/vulkan/clip_rounded_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/gpu_render_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/iosurface_host.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/iosurface_host.h` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/pipeline_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/resize_clip_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/sampled_image_test.c` | ✅ | 1791343936 | 951768895900a9c236ee79717584826dbc811cacc08f511291ff8961a5acdc59 | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `tests/graphvex/vulkan/surface_gpu_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/surface_test.c` | ✅ | 1791048396 | a7f59df29ec8509e8a6bae4ddbf794c3178ece49cb117b3d6b9c5c3e56b3f7b3 | ['./tools/b', 'run', 'tests/graphvex/vulkan/surface_test.c']; Surface original APIs/borrowed boards and host seam only; no native FPS or appearance proof | Existing uncapped Surface contract regression check | passed |
| `tests/graphvex/vulkan/vk_batch_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/graphvex/vulkan/vk_renderer_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/hotcwap/capability`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/capability/capability_test.c` | ✅ | 1791039324 | c8be61831e72eea78183bb911ef820a4c6aafcbdc63ca5caec98d4e4efbf6bbc | ['./tools/b', 'run', 'capability_test']; Apple Silicon macOS; only assertions in tests/hotcwap/capability/capability_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | capability_test: automated headless/prompt-free contract assertions; no visual approval | passed |

### `tests/hotcwap/hot`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/hot/hot_behavior_test.c` | ✅ | 1791039325 | 594ca42f4a5a76dafb0ceb6cb03df5178affbf220c66aa06cfc07b32a26ec482 | ['./tools/b', 'run', 'hot_behavior_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/hot_behavior_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | hot_behavior_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/hot/hot_retire_test.c` | ✅ | 1791039325 | 820e5f6ea2b666c6c4f18051498655cfba85c68a2e90cbef7ee9a778d21ed843 | ['./tools/b', 'run', 'hot_retire_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/hot_retire_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | hot_retire_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/hot/hot_test.c` | ✅ | 1791039326 | 7927c3519648c6d0f0aafd8d2ffd377ba31d141fe8c24cd6a6e93e95cbb3ee8b | ['./tools/b', 'run', 'hot_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/hot_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | hot_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/hot/hot_trampoline_test.c` | ✅ | 1791039327 | d038bfb6a0a426e665b0375152b74ac6d6474dba40612e80d5e0546adc272acb | ['./tools/b', 'run', 'hot_trampoline_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/hot_trampoline_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | hot_trampoline_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/hot/ledger_test.c` | ❌ | 1791039327 | 0d383d949c3bf66716c5a9c2cd25674a276f855ec278fc1073c894e723957a54 | ['./tools/b', 'run', 'ledger_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/ledger_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | ledger_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/loader_trust_test.c` | ❌ | 1791039328 | 5e56bd77a89fbfdb32cfe1178bb23558d304a26d24a917397b6ca27512aa981e | ['./tools/b', 'run', 'loader_trust_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/loader_trust_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | loader_trust_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/manifest_adversarial_test.c` | ❌ | 1791039328 | 3760d1890e8dc5dcd199df435093c3d618feb068d704149b29c1fc332cf265e1 | ['./tools/b', 'run', 'manifest_adversarial_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/manifest_adversarial_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | manifest_adversarial_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/manifest_hot_test.c` | ❌ | 1791039329 | cc397107ed06ea98d163a605c7399144bdaed866d4250970c71b49d38af523e0 | ['./tools/b', 'run', 'manifest_hot_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/manifest_hot_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | manifest_hot_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/manifest_rollback_test.c` | ❌ | 1791039330 | 170c571bb704dc01b465ec76ba7673413f86e6cd5df3ac5e8eed96f7531d6253 | ['./tools/b', 'run', 'manifest_rollback_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/manifest_rollback_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | manifest_rollback_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/manifest_uninstall_test.c` | ❌ | 1791039331 | 986cbd770ed660fdfd95e6dfd87f34934f36d4b1cfb7bd9cfd9eff5d5959d920 | ['./tools/b', 'run', 'manifest_uninstall_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/manifest_uninstall_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | manifest_uninstall_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/retire_ring_overflow_test.c` | ❌ | 1791039334 | 92ef1d0bb7b5ade5c93e9770c68be9413bf1f9e8709037942c67dfccba6423d4 | ['./tools/b', 'run', 'retire_ring_overflow_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/retire_ring_overflow_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | retire_ring_overflow_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/shutdown_order_test.c` | ❌ | 1791039335 | a3165d8ed12038888e77ccfef713aefbb7f50532c296ba9e140e05ac3d86c017 | ['./tools/b', 'run', 'shutdown_order_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/shutdown_order_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | shutdown_order_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/throwable_test.c` | ❌ | 1791039335 | 9d172a7db4b4e2dedb48b99830403634b1b06ae3c32eee0def03c1d08dd33237 | ['./tools/b', 'run', 'throwable_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/throwable_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | throwable_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/two_dylib_swap_test.c` | ❌ | 1791039336 | c2f0c7cac03b0eda5e9cf22f8dc43eebbc975846599d501616460ba60d359924 | ['./tools/b', 'run', 'two_dylib_swap_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/two_dylib_swap_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | two_dylib_swap_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/uninstall_guard_test.c` | ❌ | 1791039337 | badec767f4afde39760c144061541bf6b9daa56998ad1fd1bfdc01113552fac3 | ['./tools/b', 'run', 'uninstall_guard_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/uninstall_guard_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | uninstall_guard_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |
| `tests/hotcwap/hot/wrong_binary_test.c` | ❌ | 1791039337 | 6288d13c1fb8d24bfdb2c52e8928176f8bb32d669af76b574f1a7d42ca2a01ea | ['./tools/b', 'run', 'wrong_binary_test']; Apple Silicon macOS; only assertions in tests/hotcwap/hot/wrong_binary_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | wrong_binary_test: automated headless/prompt-free contract assertions; no visual approval | stale — content changed; rerun required |

### `tests/hotcwap/kernel`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/kernel/application_test.c` | ✅ | 1791047639 | d95ed03de723da19526a18de87d1dd6b80c53116ee141842de53caa27912733d | ['./tools/b', 'run', 'tests/hotcwap/kernel/application_test.c']; macOS Apple Silicon; only executed assertion branches, no full-contract/platform/visual proof | Lifecycle compatibility/owner assertions after blocking-start migration | passed |
| `tests/hotcwap/kernel/console_test.c` | ✅ | 1791039338 | 205ecabaf8b32e9bd797b94a456bc65f5a43428dc2011c1037ddc5ffb8b127ae | ['./tools/b', 'run', 'console_test']; Apple Silicon macOS; only assertions in tests/hotcwap/kernel/console_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | console_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/kernel/kernel_function_test.c` | ❌ | 1791047640 | e59b07f2e16150ead878bb9aaa1d8daae1548a9e8905db83ee8f7dcc4f78f89d | ['./tools/b', 'run', 'tests/hotcwap/kernel/kernel_function_test.c']; macOS Apple Silicon; only executed assertion branches, no full-contract/platform/visual proof | Lifecycle compatibility/owner assertions after blocking-start migration | stale — content changed; rerun required |
| `tests/hotcwap/kernel/kernel_lifecycle_test.c` | ✅ | 1791047641 | eaf8bbe5bbbf1dde56ad082216539c7a903c9ba8a81e066bcbd97104b8babb2e | ['./tools/b', 'run', 'tests/hotcwap/kernel/kernel_lifecycle_test.c']; macOS Apple Silicon; only executed assertion branches, no full-contract/platform/visual proof | Lifecycle compatibility/owner assertions after blocking-start migration | passed |
| `tests/hotcwap/kernel/process_test.c` | ✅ | 1791039340 | 3ad6c0677b3eb2069b6ab1f48c0ce93b96f5183feda13e439fe52426e04d09a8 | ['./tools/b', 'run', 'process_test']; Apple Silicon macOS; only assertions in tests/hotcwap/kernel/process_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | process_test: automated headless/prompt-free contract assertions; no visual approval | passed |

### `tests/hotcwap/permission`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/permission/permission_test.c` | ✅ | 1791039341 | 6ef5e3e42f5ab7d47241291a164ada1919cc57f19ff9ca3b408c7e890108ad6a | ['./tools/b', 'run', 'permission_test']; Apple Silicon macOS; only assertions in tests/hotcwap/permission/permission_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | permission_test: automated headless/prompt-free contract assertions; no visual approval | passed |

### `tests/hotcwap/spoke`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/spoke/spoke_test.c` | ✅ | 1791039341 | 6d7e7a491a226d7e4e254add46903129f0369eb5ae267d8eb9ecc17d4d2a6d98 | ['./tools/b', 'run', 'spoke_test']; Apple Silicon macOS; only assertions in tests/hotcwap/spoke/spoke_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | spoke_test: automated headless/prompt-free contract assertions; no visual approval | passed |

### `tests/hotcwap/window`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/window/bridge_seam_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/hotcwap/window/present_surface_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/hotcwap/window/traffic_light_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/hotcwap/window/window_event_test.c` | ✅ | 1791039342 | 81a0118c2b6887d7f05a36a03fbb15c940f673cc0d9253294b4aab8417cd9f12 | ['./tools/b', 'run', 'window_event_test']; Apple Silicon macOS; only assertions in tests/hotcwap/window/window_event_test.c; .h evidence is client compile/API use; integration scope where multiple subjects are named; no visual tour, OS prompts, sanitizer, full public-surface, or other-platform proof | window_event_test: automated headless/prompt-free contract assertions; no visual approval | passed |
| `tests/hotcwap/window/window_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/repos`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/repos/artwork_headers_test.py` | ✅ | 1791187304 | 6dd106442d9cf1ec7f1fcb5d0f67fafea36fb0fd23192804d99f2fd56abc0ed9 | ['python3', '-c', 'import subprocess; subprocess.run(["python3", "tests/repos/vex-graph/readme_test.py"], check=True); subprocess.run(["python3", "tests/repos/artwork_headers_test.py"], check=True)']; Offline PNG CRC/decompression and link-markup assertions only; original supplied pixels unchanged; GitHub appearance approval remains user-owned. | Renamed replacement vexgraph artwork: PNG integrity and updated personal/ecosystem header references; five offline document checks. | passed |

### `tests/repos/vex-graph`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/repos/vex-graph/readme_test.py` | ✅ | 1791187304 | c6c30fb6372509ccb34cd44035bac3c9908597681e51d90f47e493c26ffcf824 | ['python3', '-c', 'import subprocess; subprocess.run(["python3", "tests/repos/vex-graph/readme_test.py"], check=True); subprocess.run(["python3", "tests/repos/artwork_headers_test.py"], check=True)']; Offline PNG CRC/decompression and link-markup assertions only; original supplied pixels unchanged; GitHub appearance approval remains user-owned. | Renamed replacement vexgraph artwork: PNG integrity and updated personal/ecosystem header references; five offline document checks. | passed |

### `tests/resources`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/resources/README.md` | ✅ | 1791343963 | d7195f49be59d3687250622dcb68670f04d24a50456f6a138286553024853a50 | ['python3', '-c', 'import subprocess; [subprocess.run(["tools/b","test",name],check=True,timeout=120) for name in ("sampled_image_test","filter_gallery_fixture_test","gpu_scope_test","image_test","picture_test","vk_renderer_test")]; subprocess.run(["python3","-B","tests/tools/compositor_contract_test.py","CompositorContractTest.test_gallery_uses_gpu_scope_not_cpu_fixture","CompositorContractTest.test_sampled_image_constructor_dispatch","CompositorContractTest.test_documented_reference_client_compiles","CompositorContractTest.test_filter_constructor_arity_is_rejected_for_intended_reason","CompositorContractTest.test_gpu_color_pass_public_arity_and_no_cpu_extension"],check=True,timeout=60); subprocess.run(["python3","-B","tests/tools/filter_gallery_resources_test.py"],check=True,timeout=180)']; macOS strict registered image/sample/scope/Picture/renderer tests; gallery app build/bundle only, no window/presentation/appearance or memory profiling. Five selected docs checks, not full legacy-path/wiki suite; readiness wiki and Darling lawbook unavailable. Numeric/ASan/UBSan proof recorded separately. | Final strict registered owner runs, five selected documentation/constructor checks and gallery resource bundle build/refresh/signature, no interactive launch | passed |
| `tests/resources/other-sunflower.png` | ✅ | 1791342045 | 97fb52258c18e91102d36acce83590d0ecf8790f2a13fd37baa8f81becaaccf5 | ['python3', '-B', 'tests/tools/filter_gallery_resources_test.py']; macOS noninteractive build and ImageIO runtime; missing-resource child intentionally fails its assertion. No interactive/gallery appearance, other-host, macOS14 runtime, injected allocation failure or oversized source metadata proof | Final strict gallery bundle build/incremental resource refresh/signature; optimized ASan/UBSan decoder owner and relocated bundle/no-fallback proof | passed |

### `tests/tools`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/tools/agents_test.py` | ✅ | 1791048984 | 4fad586bf1ac5311304b3835c4a19da804056bb39c3ddf329e71583a794363a3 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/agents_test.py"],check=True); subprocess.run(["python3","-B","tests/tools/test_checklist_test.py"],check=True)']; Five agent bus tests and eleven checklist tests, including invalidation, privacy, bounded execution and rejection of visual evidence; no live remote messaging correctness claim | Offline agent signatures and timestamped checklist regression tests before publication | passed |
| `tests/tools/c23_migration_test.py` | ✅ | 1791277954 | 79c8ccc7dfde97cfdbdb9ba267e9f04ba59e5d2ffea8830f31555d18db0f1456 | ['python3', '-B', 'tests/tools/c23_migration_test.py']; macOS five checks: zero parser/diagnostic errors for BVH, algo suite, filter_gallery.c and darling_tests.c. Refactoring tweaks restricted to ExpandAutoType because SwapBinaryOperands self-test fails on valid color macros; no GUI approval | Bundled clangd syntax proof for production, tests and gallery mains | passed |
| `tests/tools/clion_adapter_test.py` | ❌ | 1791277908 | e383beca0cf31573fc2d5671968a1362d57b033dfce569c3cb306bd57b6ef6f4 | ['python3', '-B', 'tests/tools/clion_adapter_test.py', '--generator', 'Ninja', '--ninja', '/Applications/CLion.app/Contents/bin/ninja/mac/aarch64/ninja']; macOS ten adapter checks: every exported source has root/tests-only compile commands, actual strict gallery compilation with header-only fixture, two headless CTest tests; no gallery execution or GUI approval | Complete registered source contexts including gallery mains and header helper | stale — content changed; rerun required |
| `tests/tools/compositor_contract_test.py` | ✅ | 1791343963 | 5176049a8445f51b6064aa8526bf7174d26ef9ca60113b8c51b284e7380a7c93 | ['python3', '-c', 'import subprocess; [subprocess.run(["tools/b","test",name],check=True,timeout=120) for name in ("sampled_image_test","filter_gallery_fixture_test","gpu_scope_test","image_test","picture_test","vk_renderer_test")]; subprocess.run(["python3","-B","tests/tools/compositor_contract_test.py","CompositorContractTest.test_gallery_uses_gpu_scope_not_cpu_fixture","CompositorContractTest.test_sampled_image_constructor_dispatch","CompositorContractTest.test_documented_reference_client_compiles","CompositorContractTest.test_filter_constructor_arity_is_rejected_for_intended_reason","CompositorContractTest.test_gpu_color_pass_public_arity_and_no_cpu_extension"],check=True,timeout=60); subprocess.run(["python3","-B","tests/tools/filter_gallery_resources_test.py"],check=True,timeout=180)']; macOS strict registered image/sample/scope/Picture/renderer tests; gallery app build/bundle only, no window/presentation/appearance or memory profiling. Five selected docs checks, not full legacy-path/wiki suite; readiness wiki and Darling lawbook unavailable. Numeric/ASan/UBSan proof recorded separately. | Final strict registered owner runs, five selected documentation/constructor checks and gallery resource bundle build/refresh/signature, no interactive launch | passed |
| `tests/tools/compositor_shader_test.py` | ✅ | 1791193925 | 023becab5328d4823cc8aa9eb990ea9016e8e7bc9908376a82aec58a47435a0a | ['python3', 'tests/tools/compositor_shader_test.py']; macOS glslang/SPIR-V syntax validation for six modules including scope/scatter ABI extension; actual GPU execution is separately recorded by GpuScope owner, not inferred from compiler success. | Six compositor shader modules compile and pass spirv-val. | passed |
| `tests/tools/darling_lifecycle_test.py` | ✅ | 1791048306 | 6914811cb9717a764a7d5412c265f3dd0c4667fef854055fa157de3271bbe6eb | ['python3', '-B', 'tests/tools/darling_lifecycle_test.py']; Three structural/documentation checks: all C starters use Application, no test-written pump loop, sample compiles freshly with C23 warnings-as-errors, links/README command homes and amended lifecycle-law phrases. No full documentation correctness, runtime or visual proof | Darling starter migration inventory and documented lifecycle API compilation | passed |
| `tests/tools/filter_gallery_resources_test.py` | ✅ | 1791342045 | c53c09ffacc1fa478d73b396a6dd85ce7c99705f13be7273a162ae58b1b0c68c | ['python3', '-B', 'tests/tools/filter_gallery_resources_test.py']; macOS noninteractive build and ImageIO runtime; missing-resource child intentionally fails its assertion. No interactive/gallery appearance, other-host, macOS14 runtime, injected allocation failure or oversized source metadata proof | Final strict gallery bundle build/incremental resource refresh/signature; optimized ASan/UBSan decoder owner and relocated bundle/no-fallback proof | passed |
| `tests/tools/opencode_preferences_test.ts` | ✅ | 1791178702 | ad131990bb8eaa5f83c38ab96cf6dd3a70a8960183dee931e6e3c4ecb0d9fb2b | ['bun', 'test', 'tests/tools/opencode_preferences_test.ts']; macOS Bun offline documentation and context-hook regression; discovers all repo lawbooks and validates taxonomy link/path equality, nonempty targets, inventory completeness and mandatory reading clauses. No live agent compliance or production runtime proof. | Mandatory repo-preferences reading map and constitution-first policy: eight tests including all existing lawbook links. | passed |
| `tests/tools/per_repo_ide_test.py` | ✅ | 1791337104 | c7bd5ce917a259d30ca5ca5c99ef38f61a2812aea0cc5025565cc78c0deac3da | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `tests/tools/run_current_test.py` | ✅ | 1791275828 | f71046d45cee86086300efbdf64bec71afa72f1b0276e09634b35828c606d95f | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/run_current_test.py"],check=True); subprocess.run(["./tools/run-current","tests/vexspoke/nio/mem_test.c"],check=True)']; macOS eight headless runner tests, then run active mem_test.c through the real Vexgraph graph. No GUI/gallery launches. Existing standalone Rust sample exits 5 by design. | CLion runner unit and actual workspace C route proof | passed |
| `tests/tools/sampled_texture_test.py` | ✅ | 1791343936 | 55e9c38dbbff6921ca0007b9ac2c13385d3f71cba9927c578adb3d5d06b0b88c | ['python3', '-B', 'tests/tools/sampled_texture_test.py']; macOS Apple Silicon actual Vulkan -O2 -Wall -Wextra -Werror ASan/UBSan assertions, 30s runtime watchdogs. Every sunflower sampled output pixel equals readback reference; GPU-only Images, 576 vertex bytes/image. No gallery/window/visual approval or process-footprint/drag profiling. macOS14 runtime (local loader built macOS26), other hosts, validation layers, real device loss/OOM and generic all-public renderer coverage remain gaps; sanitizer covers host, not GPU shaders. | Final optimized sanitized sampled-texture and actual GPU three-scope pixel proof: growth, stable VBO, alpha/clip/order, CPU edit invalidation, independent frame lifetime, scope teardown and injected upload/frame/scope timeout recovery | passed |
| `tests/tools/test_checklist_test.py` | ✅ | 1791274047 | 41cad4c0bb8c0e6eee4601434d9b434c9480d1650d08723f8eb77d1e8b77ba4c | ['python3', '-B', 'tests/tools/test_checklist_test.py']; Offline inventory, own-ignore, timestamps, rejection and stale-evidence regression tests; no engine behavior | Discover personal repositories after workspace reorganization | passed |

### `tests/vexspoke`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/backend_contract_test.py` | ✅ | 1791275083 | 9de3bdf4f9866726d168bff199435c404a9ff71828f10a5cc62400e994a031bf | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["./tools/b","build"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["python3","-B","tests/vexspoke/backend_contract_test.py"],check=True)']; macOS b build plus eight workspace regressions and two backend-document/copy checks. 610 registered compilation units; imported reference bytes preserved. Source-empty repos are not builds; no Rust backend, standalone per-repo, Windows or visual approval. | Integrated build repair and unchanged C-backend restoration | passed |
| `tests/vexspoke/coverage_baseline.txt` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/function_baseline.txt` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/mirror_exceptions.txt` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/surface_baseline.txt` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/algo`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/algo/algo_suite_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/bvh_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/dijkstra_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/draft_sort_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/kd_tree_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/radix_sort_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/segment_index_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/algo/tree_sit_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/atomic`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/atomic/registry_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/atomic/ring_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/atomic/spin_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/bit`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/bit/bit_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/c23`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/c23/equals_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/c23/free_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/deferred`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/deferred/dispatch_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/demo`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/demo/touchid_demo.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/demo/touchid_demo.mm` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/exception`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/exception/throw_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/exception/try_code_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/exception/try_ptr_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/exception/try_value_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/input`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/input/hardware_input_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/input/input_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/input/key_map_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/io`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/io/cache_test.c` | ✅ | 1791275016 | b873af094441e395c5dd2b6528e98dc8858dd23ed6beab951ab5e0f14d09620a | ['./tools/b', 'test', 'cache_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cache registered regression | passed |
| `tests/vexspoke/io/clipboard_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/io/file_test.c` | ✅ | 1791275014 | 91f358d7227a2ba7caa67ea6dcaeee643fb741deb23bf772ab0e7b4c4f7e7c94 | ['./tools/b', 'test', 'file_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend file registered regression | passed |
| `tests/vexspoke/io/filewriter_test.c` | ✅ | 1791275015 | 4efceacf8451c6a6fc6ccf662f65a5ff59e9e1039aee8788f98ba6c3c61af273 | ['./tools/b', 'test', 'filewriter_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend filewriter registered regression | passed |
| `tests/vexspoke/io/log_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/io/logparser_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/io/vexhome_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/lang`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/lang/mat4_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/lang/vec2_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/lang/vec3_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/lang/vec4_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/math`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/math/coord_frame_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/math/fast_math_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/math/math_coord_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/math/math_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/math/strict_math_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/misc`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/misc/header_veto_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/net`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/net/net_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/net/url_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/nio`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/nio/mem_test.c` | ✅ | 1791274974 | 0edbebc58c35a75ef1b3d4de88e602bbf07704699890c762ff60a2139ac8a442 | ['./tools/b', 'test', 'mem_test']; macOS registered mem_test, built with C23 warnings denied and assertions active; no Rust delegation, other-platform or complete concurrency proof | Restored C allocator compatibility backend owner regression | passed |
| `tests/vexspoke/nio/transient_lifetime_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/objects`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/objects/choice_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/future_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/global_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/local_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/passive_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/probable_objects_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/objects/probable_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/oop`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/oop/stride_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/oop/type_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/primitive`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/primitive/bool_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/brain_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/byte_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/double_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/fixed32_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/fixed64_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/float_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/int_double_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/int_float_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/int_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/long_double_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/long_float_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/long_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/pack_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/short_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/primitive/string_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/reactive`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/reactive/reactive_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/reactive/reactive_typed_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/reflection`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/reflection/reflection_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/relational`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/relational/cell_test.c` | ✅ | 1791275020 | 15521aea46e2bcc2e87f5e19233905b82c2b4204bf976a2d3fd41a999cd6fc72 | ['./tools/b', 'test', 'cell_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend cell registered regression | passed |
| `tests/vexspoke/relational/class_relational_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/relational/shelf_test.c` | ✅ | 1791275021 | 110caf6ea25223d73ccaee239f7a109aee506f70e4070e9df634675da5722c07 | ['./tools/b', 'test', 'shelf_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend shelf registered regression | passed |
| `tests/vexspoke/relational/symbol_table_test.c` | ✅ | 1791275017 | 3ffb297771a74dae500e0089fb2759e0dba256ddb8f5c82a04ff4e5b06799901 | ['./tools/b', 'test', 'symbol_table_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend symbol_table registered regression | passed |
| `tests/vexspoke/relational/variable_hash_map_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/relational/variable_mini_map_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/relational/variable_pool_test.c` | ✅ | 1791275018 | c90b851ea422b625f525664c6fc1afb6eb85e40a3ae93b4e5dfe78ed683aa760 | ['./tools/b', 'test', 'variable_pool_test']; macOS headless registered owner test through b; warnings denied and assertions active. Scoped regression only, not full adversarial/concurrency/other-platform readiness. | Restored C backend variable_pool registered regression | passed |
| `tests/vexspoke/relational/variable_slot_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/relational/variable_strict_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/search`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/search/find_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/search/search_calc_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/search/spotlight_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/security`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/security/crypto_security_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/security/crypto_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/security/secure_random_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/struct`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/struct/array_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/chunked_list_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/chunked_paged_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/circle_array_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/collection_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/deque_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/list_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/map_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/minheap_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/octree_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/queue_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/set_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/sparseset_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/spatial_struct_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/sphere_array_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/struct/stack_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/system`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/system/app_detect_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/capture_tool_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/display_info_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/display_monitor_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/graphics_info_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/hardware_info_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/process_probe_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/system/system_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/thread`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/thread/compute_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/thread/thread_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/time`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/time/calendar_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/time/clock_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/time/datetime_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/time/nanotime_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/time/time_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tests/vexspoke/util`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/util/arrays_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/util/hash_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tests/vexspoke/util/random_test.c` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

## workspace

### `.`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `.clangd` | ✅ | 1791277954 | bc2650e4099856ab05fdc1ad82560bdc1f2b6c60502b253597d0a7ba70e2319b | ['python3', '-B', 'tests/tools/c23_migration_test.py']; macOS five checks: zero parser/diagnostic errors for BVH, algo suite, filter_gallery.c and darling_tests.c. Refactoring tweaks restricted to ExpandAutoType because SwapBinaryOperands self-test fails on valid color macros; no GUI approval | Bundled clangd syntax proof for production, tests and gallery mains | passed |
| `.gitignore` | ✅ | 1791271846 | ec122bf93d95bf088908ffc01e3d4e8d4a95e1b8a41e7dffe638a16b2cd5423b | ['python3', '-c', 'from pathlib import Path; import subprocess; p=Path("preferences.md"); assert p.is_file() and not p.is_symlink(); assert "/preferences.md" in Path(".gitignore").read_text().splitlines(); assert not Path("ecosystem/repos/vexspoke/preferences.md").exists(); assert not subprocess.check_output(["git","ls-files","--","preferences.md"]); assert not subprocess.check_output(["git","-C","ecosystem/repos/vexspoke","ls-files","--","preferences.md"]); subprocess.run(["git","check-ignore","preferences.md"],check=True); remote=subprocess.check_output(["gh","gist","view","4132a6c45cb6d3797c3e8eff2e94035a","--raw","--filename","preferences.md"],timeout=60); assert remote==p.read_bytes(); assert b"Whenever this file changes, upload the complete current file" in remote; print("PASS: migration, publication rule and Gist byte equality")']; macOS filesystem/Git and authenticated existing-Gist byte comparison only; no production or visual proof. | Local constitution migration: real root file, removed Git tracking, root ignore and exact Gist round-trip. | passed |
| `AGENTS.md` | ✅ | 1791178702 | 052bf77e71faed22d8098bc28d508b70fb295f1a95711bc3cca6f302adac0009 | ['bun', 'test', 'tests/tools/opencode_preferences_test.ts']; macOS Bun offline documentation and context-hook regression; discovers all repo lawbooks and validates taxonomy link/path equality, nonempty targets, inventory completeness and mandatory reading clauses. No live agent compliance or production runtime proof. | Mandatory repo-preferences reading map and constitution-first policy: eight tests including all existing lawbook links. | passed |
| `CMakeLists.txt` | ✅ | 1791277908 | 89655948a6120188526046aa7d1c6073ea7eb8f35f65144ddf2d0d5d11395c19 | ['python3', '-B', 'tests/tools/clion_adapter_test.py', '--generator', 'Ninja', '--ninja', '/Applications/CLion.app/Contents/bin/ninja/mac/aarch64/ninja']; macOS ten adapter checks: every exported source has root/tests-only compile commands, actual strict gallery compilation with header-only fixture, two headless CTest tests; no gallery execution or GUI approval | Complete registered source contexts including gallery mains and header helper | passed |
| `LICENSE` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `README.md` | ✅ | 1791337104 | 616a36c4cb39b8132f0e3e48c89ee6751f5ec6660799a88bf2dfdc0298db6b04 | ['python3', '-B', 'tests/tools/per_repo_ide_test.py']; macOS four owner checks across 15 repos: configure, default-no-op targets, C23 commands, representative syntax, missing-header rejection, ignored build outputs, blueprint and docs contracts. No release linking, dependencies downloaded, apps, Rust/C ABI or other-host runtime proof. | Independent per-repo C23 IDE entries, ignored outputs and b-build links | passed |
| `b.json` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |

### `tools`

| Filename | Lab tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Description | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tools/BUILD.md` | ✅ | 1791277908 | c5e7e7f6aa99c0c9d08f22c33fb9faec123b3af2a7e50551869b767a04d5339a | ['python3', '-B', 'tests/tools/clion_adapter_test.py', '--generator', 'Ninja', '--ninja', '/Applications/CLion.app/Contents/bin/ninja/mac/aarch64/ninja']; macOS ten adapter checks: every exported source has root/tests-only compile commands, actual strict gallery compilation with header-only fixture, two headless CTest tests; no gallery execution or GUI approval | Complete registered source contexts including gallery mains and header helper | passed |
| `tools/agents.sh` | ✅ | 1791048984 | 7561e1a0ada9781376bae821ae2cf68c008d76382fce9214e605d6c81edb9e83 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/agents_test.py"],check=True); subprocess.run(["python3","-B","tests/tools/test_checklist_test.py"],check=True)']; Five agent bus tests and eleven checklist tests, including invalidation, privacy, bounded execution and rejection of visual evidence; no live remote messaging correctness claim | Offline agent signatures and timestamped checklist regression tests before publication | passed |
| `tools/b` | ✅ | 1791275595 | 21f29523a9ef9b0648f05ac94ebc2f1c3ab4d5b0fe17ef55c520901141714315 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/run_current_test.py"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["./tools/b","build"],check=True)']; macOS headless C/Rust routing, arguments/status/recovery, XML wiring, workspace regression and complete registered build; IDE UI and interactive appearance remain user-owned. b-local lawbook unavailable. | Project-owned graph and repaired CLion active-file routing | passed |
| `tools/build_annotation.h` | ✅ | 1791275595 | 0837cbce75e4f3a1e426aa1988a161c6309347c41a04a94fc6a38dfd6041f2a2 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/run_current_test.py"],check=True); subprocess.run(["python3","-B","tests/b/workspace_test.py"],check=True); subprocess.run(["./tools/b","build"],check=True)']; macOS headless C/Rust routing, arguments/status/recovery, XML wiring, workspace regression and complete registered build; IDE UI and interactive appearance remain user-owned. b-local lawbook unavailable. | Project-owned graph and repaired CLion active-file routing | passed |
| `tools/darling-gallery.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/debug_compile.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/linter.py` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/make_code_txt.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/make_vkapp.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/run-current` | ✅ | 1791275828 | b76b7917708ae8e59221aa9d29cbe0fa06eb27e591c6c97d1c840039526f9416 | ['python3', '-B', '-c', 'import subprocess; subprocess.run(["python3","-B","tests/tools/run_current_test.py"],check=True); subprocess.run(["./tools/run-current","tests/vexspoke/nio/mem_test.c"],check=True)']; macOS eight headless runner tests, then run active mem_test.c through the real Vexgraph graph. No GUI/gallery launches. Existing standalone Rust sample exits 5 by design. | CLion runner unit and actual workspace C route proof | passed |
| `tools/run.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/run_debug.sh` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/spv_header.py` | ❌ | — | — | No executed evidence | Awaiting automated lab check | untested |
| `tools/test_checklist.py` | ✅ | 1791274047 | 403cc80671679bcbb339ea961be762d8e525f0fc5d9d1e93256b4f791636a40f | ['python3', '-B', 'tests/tools/test_checklist_test.py']; Offline inventory, own-ignore, timestamps, rejection and stale-evidence regression tests; no engine behavior | Discover personal repositories after workspace reorganization | passed |
| `tools/workspace.c` | ✅ | 1791343963 | 0842e59cea0e0f17be3ad4f68c668dc5e264c8d37bd4b390153b85603060d71e | ['python3', '-c', 'import subprocess; [subprocess.run(["tools/b","test",name],check=True,timeout=120) for name in ("sampled_image_test","filter_gallery_fixture_test","gpu_scope_test","image_test","picture_test","vk_renderer_test")]; subprocess.run(["python3","-B","tests/tools/compositor_contract_test.py","CompositorContractTest.test_gallery_uses_gpu_scope_not_cpu_fixture","CompositorContractTest.test_sampled_image_constructor_dispatch","CompositorContractTest.test_documented_reference_client_compiles","CompositorContractTest.test_filter_constructor_arity_is_rejected_for_intended_reason","CompositorContractTest.test_gpu_color_pass_public_arity_and_no_cpu_extension"],check=True,timeout=60); subprocess.run(["python3","-B","tests/tools/filter_gallery_resources_test.py"],check=True,timeout=180)']; macOS strict registered image/sample/scope/Picture/renderer tests; gallery app build/bundle only, no window/presentation/appearance or memory profiling. Five selected docs checks, not full legacy-path/wiki suite; readiness wiki and Darling lawbook unavailable. Numeric/ASan/UBSan proof recorded separately. | Final strict registered owner runs, five selected documentation/constructor checks and gallery resource bundle build/refresh/signature, no interactive launch | passed |
