# Ecosystem test checklist

Generated test report maintained by `python3 tools/test_checklist.py`.
Governed by the **Timestamped Test Checklist Law** in `preferences.md`.

- ✅ = the recorded command passed for this exact file hash, in its stated scope.
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
Do not hand-edit generated tables; add files/tests and use `sync` or `run`.

## ecosystem/drivers/api-haven

### `ecosystem/drivers/api-haven`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/api-haven-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/ai`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/ai/ai_chat.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_chat.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_chat_anthropic.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_chat_anthropic.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_chat_gemini.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_chat_gemini.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_provider.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_sse.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/ai_sse.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/ai/data`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/ai/data/providers_china.inc` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/data/providers_europe.inc` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/ai/data/providers_global.inc` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/api`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/api/auth.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/auth.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/client.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/client.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/discord.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/haven_ws_fanout.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/haven_ws_fanout.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/rest.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/api/rest.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/app`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/app/app_broker.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/app/app_broker.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/app/app_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/app/app_provider.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/asset`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/asset/asset_broker.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/asset/asset_broker.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/asset/asset_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/asset/asset_provider.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/com/discord`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/com/discord/discord.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/com/discord/discord.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/com/discord/discord_webhook.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/com/slack`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/com/slack/slack.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/com/slack/slack.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/database`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/database/db_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/database/db_provider.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/database/db_sqlite_file.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/database/db_sqlite_file.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/database/data`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/database/data/db_providers.inc` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/harness`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/harness/engine_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/harness/engine_provider.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/harness/harness.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/harness/harness.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/main`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/main/mcp_main.c` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/mcp`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/mcp/mcp_server.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/mcp/mcp_server.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/src/search`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/src/search/search_provider.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/src/search/search_provider.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/api-haven/tools`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/api-haven/tools/gen_db_providers.py` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/api-haven/tools/gen_providers.py` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/drivers/darkbase

### `ecosystem/drivers/darkbase`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/darkbase/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/darkbase/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/darkbase/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/darkbase/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/darkbase/darkbase-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/drivers/graphvex

### `ecosystem/drivers/graphvex`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/graphvex-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/graphvex/src`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/board.c` | ✅ | 1791038758 | c8ef94d95e5ed17c3a333d1fd5ccee67376bcb3deb24ccb35ea1e869fff1e3cf | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/board.h` | ✅ | 1791038758 | 8f4a385b0579f16924194f2b63d9d662a9d1e3a4000593fe128bc91e84b712cd | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/image.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/image.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/graphvex/src/graphics`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/graphics/graphics.c` | ✅ | 1791038696 | 51ca60a8cf957694ffa63e5f0138f58cd2b1cf6e81a1ce999474e65344a40745 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `ecosystem/drivers/graphvex/src/graphics/graphics.h` | ✅ | 1791038696 | d08229cbd4ba1f4fb6443b75bd697d880d6641ad3bed17881e58cf8d4c2e7eb9 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `ecosystem/drivers/graphvex/src/graphics/render_loop.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/graphics/render_loop.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/graphics/viewport.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/graphics/viewport.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/graphvex/src/nio`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/nio/pool.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/nio/pool.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/nio/property_pool.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/nio/property_pool.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/graphvex/src/shaders/frag`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/shaders/frag/quad.frag` | ✅ | 1791038696 | 5d6888eb30439ea37305ba2905d174eedf37e0a319811992e37a1cb20f4a2d7a | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `ecosystem/drivers/graphvex/src/shaders/vert`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/shaders/vert/quad.vert` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/drivers/graphvex/src/ui`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/ui/element.c` | ✅ | 1791038758 | 482c0952827972c0a60f42a8a43ec818b819011e5742f9d0b6bba10c3d4017a4 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/ui/element.h` | ✅ | 1791038758 | a0709f54e18cf17ad61b3e5f25c9b930fa0eb397cef1498c75006ef74a02dd90 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/ui/property.c` | ✅ | 1791038758 | 92f3e5972d511a5473839121e549b984847ca328293bbee9b45c58c3a4f35780 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/ui/property.h` | ✅ | 1791038758 | e4619e214957d8c1d78b078767d2526222e25763f138da6737f9bd8d0ac457a6 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

### `ecosystem/drivers/graphvex/src/vulkan`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/graphvex/src/vulkan/device.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/device.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/pipeline.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/pipeline.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/surface.c` | ✅ | 1791038758 | 9cfa255f25f8bc5727c60430bd08ef0910d030b911d40e15451385ea5c09c756 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/vulkan/surface.h` | ✅ | 1791038758 | dc3755ba3add758ba0f46c8ee549136047120ef57d2d0bd8fbf266473ee4d5e9 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/drivers/graphvex/src/vulkan/vk_batch.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/vk_batch.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/vk_renderer.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/graphvex/src/vulkan/vulkan_backend.h` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/drivers/language

### `ecosystem/drivers/language`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/language/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/language/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/language/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/language/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/language/language-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/drivers/samplerate

### `ecosystem/drivers/samplerate`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/drivers/samplerate/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/samplerate/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/samplerate/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/samplerate/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/drivers/samplerate/samplerate-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/hotcwap

### `ecosystem/hotcwap`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hotcwap-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/capability`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/capability/capability.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/capability/capability.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/docs`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/docs/bridging.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/docs/install.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/hot`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/hot/hot.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot_behavior.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot_retire.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot_retire.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot_trampoline.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/hot_trampoline.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/ledger.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/ledger.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/manifest.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/manifest.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/throwable.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/hot/throwable.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/kernel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/kernel/application.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/application.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/console.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/console.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/kernel.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/kernel.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/process.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/kernel/process.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/permission`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/permission/permission.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/permission/permission.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/permission/permission_backend.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/permission/objc`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/permission/objc/permission_cocoa.m` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/spoke`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/spoke/lifetime.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/spoke/lifetime.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/hotcwap/window`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/hotcwap/window/traffic_light.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/traffic_light_cocoa.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_cocoa.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_event.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_event.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_linux.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_wayland.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/hotcwap/window/window_win32.c` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/interface/darling-framework

### `ecosystem/interface/darling-framework`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/darling-framework/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/darling-framework/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/darling-framework/README.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/interface/darling-framework/src/c23`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/c23/event_invoke.c` | ✅ | 1791038758 | e1c731d2d23b6580d66bd4b5ca0f8a068a06c7afbc8e3c6d0fbe06db47509fed | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/c23/event_invoke.h` | ✅ | 1791038758 | 892755d8bbce1a0ed2f64a4a391bf3b8dc9bfb05c5ca10e527b19e72d2d1b6dd | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/c23/overload.c` | ✅ | 1791038758 | e8d495364c3fc3c2316d8e37535a970a264d4d93d4e8ac932c2b4b215b36d4f3 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/c23/overload.h` | ✅ | 1791038758 | 4be54031bb5412426f9c577a67d5ec80cd123257063abc3e59c0fa77440c5e03 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

### `ecosystem/interface/darling-framework/src/event`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/event/hit.c` | ✅ | 1791038758 | 126583a4958d36b272f30870cab313f7293a3fc3a2a64bacf15bf18b9d4bbb6b | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/event/hit.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/interface/darling-framework/src/frame`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/frame/frame.c` | ✅ | 1791038758 | 5085bfbe96576a2b4b8f2fde3660bf07e06b75bbbb5aa323e7c64de01ff1c837 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/frame/frame.h` | ✅ | 1791038758 | 467654b1db4cdc63e8d3df3ecde17b9ca7839ed4e3bcdc6f34f16b690a85f158 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/frame/frame_internal.h` | ✅ | 1791038758 | 2a7150705375c3d042416581ae8a119cc20871ac13350805e73ce4da243a0e4b | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

### `ecosystem/interface/darling-framework/src/input`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/input/pointer.c` | ✅ | 1791038758 | da0e13f50af945cc02e3a08c505bd196875bdf601fc4e842c15ef698511aa658 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/input/pointer.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/interface/darling-framework/src/panel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/panel/panel.c` | ✅ | 1791038758 | 13c0ac469632d12a31bb51795ff3024ab524b1a878d7f49a07dfdfc5f9ed363b | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/panel/panel.h` | ✅ | 1791038758 | 2ebff0e65f1c4eef296b75c7b25be3a7f5205e3c6afb92a8c6ba9b16d78506f8 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/panel/panel_internal.h` | ✅ | 1791038758 | d563eed5819ff51b21ca3d46096db6852274b5ce124e7fb08e68166964816f01 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/panel/scroll_panel.c` | ✅ | 1791038758 | 4c9cc2d5c3db5058fd303591d19201a4b401472e251de7aebbe4ce0f6078a765 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/panel/scroll_panel.h` | ✅ | 1791038758 | a193bda638a32b3b32c3dc763fff59077608dc09d478e1653b9c167f7c7f4a25 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

### `ecosystem/interface/darling-framework/src/properties`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/darling-framework/src/properties/add.c` | ✅ | 1791038758 | 182878b0f6ee3b3aa405b6d6bc2764697eecfb01291d2557570c78256e275c35 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/add.h` | ✅ | 1791038758 | eca0bf588c7454acacff34772674ec82a6cb23d0259d87d1836a77f13f7b2c3a | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/remove.c` | ✅ | 1791038758 | b4683ae1b4cdfd203d6055ae9e114693cd4d0443238c375dcf2ab938f1561355 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/remove.h` | ✅ | 1791038758 | 5b925969bce1302e98dd713eebfb13bfcd29b6ab4284d391cdd848d13019f99a | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/revalidate.c` | ✅ | 1791038758 | 30a3759bab9fbb18a340b1c29c463d9072fe56b08c3ff91e24da33f66a0d336a | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/revalidate.h` | ✅ | 1791038758 | 1af11f7e77d7f9bd0954e6fa1307bdb25508390435a38c88472ab11dd632f415 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_corner_radius.c` | ✅ | 1791038758 | 24cd17f12e6f969528a0dccbec550868c72ddf9eec1bd28b144d9ae7673f7d46 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_corner_radius.h` | ✅ | 1791038758 | 1bf14edb8c99cff8ffe1bf526f716b80c39e59f0d0089b111ed19e2aa8edfe72 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_location.c` | ✅ | 1791038758 | 82284bc16f22ad1ff93a016d7cd97b6c88a2cf2fe39116880bcc0684fbae2106 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_location.h` | ✅ | 1791038758 | 3401a3c6e4375e635ee7d50b0eb25a65059e1f80259700120df3a418edea97a3 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_maximum_size.c` | ✅ | 1791038758 | 0f2db3792011315ea6ce2ef280aa48739e56e9db1e2ba4c4c252e09dfca8b592 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_maximum_size.h` | ✅ | 1791038758 | 382731b9a44fb370438670952137a1f42ec0887883bc0e0a640631dc03adaa88 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_minimum_size.c` | ✅ | 1791038758 | 19ede377c6a8f50a05980fc6794ddbd744a681f96dec9e13acbc46f175ec5355 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_minimum_size.h` | ✅ | 1791038758 | b590e20cd1000f507a75b1572ee3f38e389774d53e70ae993e5a799530afbec1 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_size.c` | ✅ | 1791038758 | 9fa3e78464bd4b0b800bef4f06211cbe2da5cb771c570f9c3e4f316f19bb20ed | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `ecosystem/interface/darling-framework/src/properties/set_size.h` | ✅ | 1791038758 | a551b66952d7d2ced333c1ceb4ed2808eaed9155650c56e764426d8fb49a3cb4 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

## ecosystem/interface/sesh

### `ecosystem/interface/sesh`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/interface/sesh/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/sesh/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/sesh/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/sesh/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/interface/sesh/sesh-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

## ecosystem/vexspoke

### `ecosystem/vexspoke`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/CONTRIBUTING.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/preferences.md` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/vexspoke-preferences.md` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/algo`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/algo/bvh.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/bvh.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/dijkstra.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/dijkstra.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/draft_sort.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/draft_sort.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/kd_tree.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/kd_tree.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/radix_sort.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/radix_sort.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/segment_index.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/segment_index.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/tree_sit.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/algo/tree_sit.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/annotation`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/annotation/checker.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/definition.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/draft.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/getter.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/hotcode.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/incomplete.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/inherits.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/intention.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/overview.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/platform_exclusive.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/setter.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/annotation/what.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/atomic`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/atomic/registry.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/atomic/registry.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/atomic/ring.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/atomic/ring.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/atomic/spin.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/atomic/spin.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/audio`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/audio/audio.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/audio/audio_hal.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/audio/audio_hal_stub.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/audio/audio_stub.c` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/bit`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/bit/bit.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/bit/bit.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/c23`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/c23/constructor.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/equals.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/equals.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/fn.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/free.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/free.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/overload.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/c23/zero.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/cli`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/cli/command.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/command.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/commandparser.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/commandparser.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/commandregistry.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/commandregistry.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/console.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/console.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/logcommands.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/logcommands.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/scanner.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/cli/scanner.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/deferred`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/deferred/dispatch.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/deferred/dispatch.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/engine`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/engine/loop.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/engine/loop.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/event`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/event/keyhandler.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/event/mousehandler.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/event/touchhandler.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/exception`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/exception/exception.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/exception.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/throw.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_code.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_code.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_ptr.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_ptr.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_value.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/exception/try_value.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/input`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/input/focus.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/focus.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/gamepad.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/gamepad.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/gesture.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/gesture.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/hardware_event.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/hardware_event.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/key.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/key.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/key_map.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/key_map.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/mouse.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/mouse.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/piano_key.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/piano_key.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/touch.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/touch.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/turntable.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/input/turntable.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/io`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/io/cache.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/cache.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/clipboard.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/clipboard_stub.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/file.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/file.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/filewriter.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/filewriter.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/hot_file.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/hot_file.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/log.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/log.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/logkind.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/logparser.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/logparser.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/process_spawn.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/process_spawn.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/vexhome.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/vexhome.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/ws_client.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/io/ws_client.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/mat4.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/mat4.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/str.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/str.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec2.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec2.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec4.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec4.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang/point`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/point/point.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/point/point.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang/rect`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/rect/rectangle.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/rect/rectangle.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang/vec2`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/vec2/vec2.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec2/vec2.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec2/vec2d.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec2/vec2d.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang/vec3`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/vec3/vec3.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3_int_float.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3_int_float.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3_long_double.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3_long_double.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3d.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec3/vec3d.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/lang/vec4`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/lang/vec4/vec4.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec4/vec4.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec4/vec4d.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/lang/vec4/vec4d.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/math`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/math/coord_frame.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/coord_frame.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/fast_math.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/fast_math.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/math.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/math.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/strict_math.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/math/strict_math.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/net`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/net/download.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/download.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/http.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/http.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/json.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/json.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/net.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/netfacade.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/tls.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/tls_curl.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/url.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/net/url.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/nio`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/nio/mem.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/nio/mem.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/objc`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/objc/audio_cocoa.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objc/audio_hal_mac.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objc/clipboard_mac.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objc/discovery.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objc/tls_apple.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objc/touchid_cocoa.m` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/objects`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/objects/choice.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/choice.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/future.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/future.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/global.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/global.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/local.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/local.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/passive.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/passive.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/probable.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/probable.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/probable_objects.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/probable_objects.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/objects/reactive.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/oop`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/oop/stride.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/oop/stride.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/oop/type.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/oop/type.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/primitive`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/primitive/bool.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/bool.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/brain.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/brain.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/byte.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/byte.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/double.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/double.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/fixed32.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/fixed32.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/fixed64.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/fixed64.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/float.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/float.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int_double.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int_double.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int_float.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/int_float.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long_double.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long_double.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long_float.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/long_float.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/pack.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/pack.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/short.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/short.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/string.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/primitive/string.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/reactive`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/reactive/dispatch.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/generic.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_object.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_object.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_primitive.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_primitive.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_probable.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_probable.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_probable_tmpl.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_probable_tmpl.inc` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_tmpl.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reactive/reactive_tmpl.inc` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/reflection`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/reflection/class.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/class.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/field.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/field.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/method.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/method.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/struct.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/struct.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/variable.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/reflection/variable.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/relational`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/relational/cell.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/cell.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/relational.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/relational.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/shelf.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/shelf.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/symbol_table.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/symbol_table.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_hash_map.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_hash_map.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_mini_map.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_mini_map.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_pool.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_pool.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_slot.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/relational/variable_slot.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/search`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/search/calc.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/search/calc.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/search/find.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/search/find.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/search/spotlight.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/search/spotlight.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/security`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/security/crypto.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/security/crypto.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/security/secure_random.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/security/secure_random.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/security/touchid.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/security/touchid.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/spoke`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/spoke/bespoke.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/spoke/bespoke.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/struct`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/struct/array.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/array.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/chunked_list.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/chunked_list.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/circle_array.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/circle_array.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/collection.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/collection.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/deque.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/deque.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/list.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/list.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/map.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/map.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/minheap.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/minheap.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/octree.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/octree.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/queue.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/queue.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/set.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/set.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/sparseset.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/sparseset.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/sphere_array.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/sphere_array.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/stack.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/struct/stack.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/system`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/system/app_detect.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/app_detect.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/capture_tool.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/capture_tool.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/discovery.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/display_info.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/display_info.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/display_monitor.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/display_monitor.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/graphics_info.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/graphics_info.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/hardware_info.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/hardware_info.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/image_mac.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/image_mac.m` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/process_probe.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/process_probe.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/system.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/system.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/system/data`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/system/data/apps.inc` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/system/data/capture_tools.inc` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/thread`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/thread/compute.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/compute.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/draw.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/draw.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/event.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/event.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/networking.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/networking.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/reactive.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/reactive.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/scripting.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/scripting.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/thread.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/thread.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/ui.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/thread/ui.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/time`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/time/calendar.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/calendar.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/clock.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/clock.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/datetime.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/datetime.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/nanotime.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/time/nanotime.h` | ❌ | — | — | No executed evidence | — | untested |

### `ecosystem/vexspoke/src/util`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `ecosystem/vexspoke/src/util/arrays.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/util/arrays.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/util/hash.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/util/hash.h` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/util/random.c` | ❌ | — | — | No executed evidence | — | untested |
| `ecosystem/vexspoke/src/util/random.h` | ❌ | — | — | No executed evidence | — | untested |

## tests

### `tests`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `tests/LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `tests/README.md` | ❌ | — | — | No executed evidence | — | untested |
| `tests/run.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tests/test-preferences.md` | ❌ | — | — | No executed evidence | — | untested |
| `tests/test_support.h` | ❌ | — | — | No executed evidence | — | untested |

### `tests/api-haven`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/api-haven/ai_chat_sisters_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/ai_provider_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/ai_sse_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/app_broker_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/asset_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/db_provider_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/db_sqlite_file_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/harness_run_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/haven_ws_fanout_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/json_flex_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/mcp_server_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/probe_consumer_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/api-haven/webhook_loopback_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/BATTLE_TESTS.md` | ✅ | 1791038820 | 46881e2ccfb859a39db083403200e9e21ecadff0bffe1709e510853f58a4f614 | ['python3', '-B', '-c', 'from pathlib import Path; p=Path("tests/darling/BATTLE_TESTS.md"); s=p.read_text(); assert "All 15" in s; assert len(list(Path("tests/darling").rglob("ui_*test.c"))) == 15; assert Path("_notes/darling/ui-battle-testing.md").is_file(); assert "207 passed" in s and "timed out" in s; print("UI battle documentation inventory and references consistent")']; Documentation-only inventory/link/count consistency: confirms 15 ui_ source targets and referenced local notes exist; not independent execution evidence or proof of every prose claim. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/darling_tests.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/capture`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/capture/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/capture/ui_clip_restore_battle_test.c` | ✅ | 1791038696 | db360f79b144b3a1bb1e759605ba7efa48414692efa5e748890f94d97822ae0a | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/capture/ui_clip_scope_battle_test.c` | ✅ | 1791038696 | c9c4de40ec7aaeee6996f97536dae1de0269b9e5bd9e96cb8cbec944a13fe7df | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/capture/ui_multiple_masks_pixels_test.c` | ✅ | 1791038696 | deafbea36103aee8a715343d60375f7e16b18fcf426136577a6f4ebf3b9c2f39 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/capture/ui_nested_clip_battle_test.c` | ✅ | 1791038696 | 26f47bf7f4f1b1394417fda4000aae0d97c297f61b4a172ae3d0578bbc37cabc | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/capture/ui_rounded_clip_battle_test.c` | ✅ | 1791038696 | 8e3955c94bc46882add60d026f84965c8555cc44caebfd26ec11be8f9d62cfe0 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/capture/ui_rounded_hit_battle_test.c` | ✅ | 1791038696 | 6e505a963ad39f6f604beda618642b8b89bb5a764c61e517421390592a925ce6 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/codefield/languages`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/codefield/languages/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/color`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/color/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/color/ui_border_pixels_battle_test.c` | ✅ | 1791038696 | 09c6e98da0d726ef9f189af600b3fb883422952b6f4a03d75cf92b9ef78fa899 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/color/ui_visual_state_pixels_test.c` | ✅ | 1791038696 | f15dd97baa018daeb51fd91d5814115ab9154b828e1dc9e6964a4ad8502451b1 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/dialog`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/dialog/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/event`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/event_invoke_test.c` | ✅ | 1791038758 | 605a6ed13b91119dfd5fa67652b7bdc465e76f3b7503d297f8b88dd086e3c05f | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/darling/event/event_kinds_test.c` | ✅ | 1791038758 | d25280c9b95cb7ea98998a13c224493392113f753f9a981af7208a10d8874854 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |

### `tests/darling/event/document`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/document/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/event/focus`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/focus/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/event/key`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/key/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/event/mouse`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/event/mouse/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/frame`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/frame/frame_resize_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/frame/frame_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/frame/hello_window.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/frame/liquid_glass_frame_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/frame/ui_matryoshka_battle_test.c` | ✅ | 1791038696 | c5665f2d4c3adf7212660e24f6b6dc4a75a5ca7f6bc04310536e95a6bad841c6 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/frame/ui_revalidate_cascade_battle_test.c` | ✅ | 1791038696 | 6d6fb190cd96b9bb57b84ffa2f8535164286c3aec1031461cabd3b739529ff76 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |
| `tests/darling/frame/ui_wide_tree_battle_test.c` | ✅ | 1791038696 | fe45e8d7b8cb0293bceddd46f4c6ddddbb156c8983c04075a6898286ab30fb67 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/graph`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/graph/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/kinematics/anchor`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/kinematics/anchor/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/kinematics/anchor/ui_anchor_pivot_pixels_test.c` | ✅ | 1791038696 | 311c31167cc46f0e5742de18bc9d865a7946ed3fd225f781fc04fbe557ff2704 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/kinematics/pivot`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/kinematics/pivot/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/kinematics/pivot/ui_size_limits_pixels_test.c` | ✅ | 1791038696 | 37750dabb8a32e4526373d619e2bca1d4805effacdcb3abbca88ed27f5c3c0b9 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/panel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/panel_add_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/panel_anchor_corner_radius_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/panel_corner_radius_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/panel_events_test.c` | ✅ | 1791038758 | 8b6bed7389434d859ed0f14a33ec4c140b925aa922a6e29f9845697a52cefaa1 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/darling/panel/panel_matryoshka_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/panel_shadow_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/panel_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/properties_test.c` | ✅ | 1791038758 | 33274bf82cc9c48d607a1bdcdf50d8b160d6c8fd2aacfd0a5c452c044c1571d0 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/darling/panel/scroll_panel_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/panel/flexpanel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/flexpanel/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/panel/panel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/panel/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/panel/scrollpanel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/scrollpanel/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/scrollpanel/ui_scroll_capture_battle_test.c` | ✅ | 1791038696 | e910e4ab3b5be2e4fafc26ee12f9d4c8e2307926ebe0154284c9b71e75ac4164 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/panel/shared`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/panel/shared/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |
| `tests/darling/panel/shared/ui_element_contract_battle_test.c` | ✅ | 1791038696 | 99a50df8819881c96c1b5e2ed2143c9f821662fceaa1df14ea77169c3db9fd77 | ['./tools/b', 'test', 'ui_']; Apple Silicon macOS, raster + Vulkan/MoltenVK: execute 15 UI battle targets; public Element operations, shared size clamps, independent captured-pixel mask/border/geometry oracles, caller clip recovery, 50k visible children, 96-level tree, Frame cascade, scroll capture and pool reclamation. Header evidence is client compilation/API invocation. Shader evidence is rendered GPU pixels, not all shader branches. No sanitizer/OOM/concurrency/Windows/Linux/performance proof; not blanket battle-tested readiness. | ui-battle-tests (agent ses_f02e90dd8ffeWregsJGm76hQM1) | passed |

### `tests/darling/picker`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/picker/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/scene/2d`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/2d/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/scene/3d`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/3d/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/scene/properties`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/scene/properties/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/text/emoji`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/emoji/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/text/label`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/label/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/text/markdown`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/markdown/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/darling/text/richtext`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/darling/text/richtext/.gitkeep` | ❌ | — | — | No executed evidence | — | untested |

### `tests/graphvex`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/board_test.c` | ✅ | 1791038758 | bc172367bdeb85f0783ffe8887072c13897bc7df247d7e5b36679b6f51095290 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/graphvex/element_property_test.c` | ✅ | 1791038758 | d01c6a47a5abb7322bea757780519e8d253443527e2340e3f6c05693bbfa8743 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/graphvex/element_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/image_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/graphvex/graphics`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/graphics/capture_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/graphics/graphics_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/graphics/render_loop_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/graphics/viewport_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/graphvex/nio`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/nio/pool_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/nio/property_pool_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/graphvex/vulkan`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/graphvex/vulkan/clip_rounded_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/gpu_render_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/iosurface_host.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/iosurface_host.h` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/pipeline_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/resize_clip_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/surface_gpu_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/surface_test.c` | ✅ | 1791038758 | a7f59df29ec8509e8a6bae4ddbf794c3178ece49cb117b3d6b9c5c3e56b3f7b3 | ['./tools/b', 'test']; full suite via ./tools/b test (209 pass/0 fail/0 timeout at record time); integration pass, not a per-file contract proof | agent (agent ses_f049e2312ffeDf099dn5rnqQjG) | passed |
| `tests/graphvex/vulkan/vk_batch_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/graphvex/vulkan/vk_renderer_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/capability`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/capability/capability_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/hot`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/hot/hot_behavior_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/hot_retire_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/hot_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/hot_trampoline_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/ledger_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/loader_trust_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/manifest_adversarial_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/manifest_hot_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/manifest_rollback_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/manifest_uninstall_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/retire_ring_overflow_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/shutdown_order_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/throwable_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/two_dylib_swap_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/uninstall_guard_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/hot/wrong_binary_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/kernel`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/kernel/application_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/kernel/console_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/kernel/kernel_function_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/kernel/kernel_lifecycle_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/kernel/process_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/permission`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/permission/permission_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/spoke`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/spoke/spoke_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/hotcwap/window`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/hotcwap/window/bridge_seam_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/window/present_surface_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/window/traffic_light_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/window/window_event_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/hotcwap/window/window_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/tools`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/tools/agents_test.py` | ✅ | 1791038534 | 4fad586bf1ac5311304b3835c4a19da804056bb39c3ddf329e71583a794363a3 | ['python3', '-B', 'tests/tools/agents_test.py']; macOS; 5 offline fake-API tests: signed say/tell, exclude self, raw opt-out, persisted name/env override, bus attribution; live API delivery not proved | agent (agent ses_efdd52f7affe7Nv6yn5GrSHEk9) | passed |
| `tests/tools/test_checklist_test.py` | ✅ | 1791038528 | 7d4638afa52ffa2e933982e5d6a6d5ec8989ad76c2b6465f07f95028b743f274 | ['python3', '-B', 'tests/tools/test_checklist_test.py']; macOS; 6 offline tests: inventory, unknown status, Unix timestamp, command failure/skip, stale content, missing rows, Markdown escaping, mutation during test; no framework runtime proof | agent (agent ses_efdd52f7affe7Nv6yn5GrSHEk9) | passed |

### `tests/vexspoke`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/coverage_baseline.txt` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/function_baseline.txt` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/mirror_exceptions.txt` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/surface_baseline.txt` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/algo`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/algo/algo_suite_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/bvh_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/dijkstra_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/draft_sort_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/kd_tree_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/radix_sort_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/segment_index_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/algo/tree_sit_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/atomic`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/atomic/registry_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/atomic/ring_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/atomic/spin_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/bit`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/bit/bit_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/c23`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/c23/equals_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/c23/free_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/deferred`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/deferred/dispatch_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/demo`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/demo/touchid_demo.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/demo/touchid_demo.mm` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/exception`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/exception/throw_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/exception/try_code_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/exception/try_ptr_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/exception/try_value_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/input`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/input/hardware_input_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/input/input_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/input/key_map_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/io`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/io/cache_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/clipboard_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/file_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/filewriter_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/log_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/logparser_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/io/vexhome_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/lang`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/lang/mat4_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/lang/vec2_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/lang/vec3_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/lang/vec4_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/math`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/math/coord_frame_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/math/fast_math_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/math/math_coord_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/math/math_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/math/strict_math_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/misc`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/misc/header_veto_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/net`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/net/net_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/net/url_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/nio`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/nio/mem_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/nio/transient_lifetime_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/objects`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/objects/choice_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/future_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/global_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/local_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/passive_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/probable_objects_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/objects/probable_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/oop`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/oop/stride_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/oop/type_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/primitive`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/primitive/bool_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/brain_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/byte_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/double_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/fixed32_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/fixed64_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/float_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/int_double_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/int_float_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/int_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/long_double_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/long_float_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/long_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/pack_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/short_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/primitive/string_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/reactive`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/reactive/reactive_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/reactive/reactive_typed_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/reflection`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/reflection/reflection_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/relational`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/relational/cell_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/class_relational_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/shelf_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/symbol_table_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/variable_hash_map_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/variable_mini_map_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/variable_pool_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/variable_slot_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/relational/variable_strict_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/search`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/search/find_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/search/search_calc_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/search/spotlight_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/security`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/security/crypto_security_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/security/crypto_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/security/secure_random_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/struct`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/struct/array_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/chunked_list_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/chunked_paged_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/circle_array_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/collection_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/deque_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/list_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/map_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/minheap_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/octree_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/queue_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/set_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/sparseset_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/spatial_struct_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/sphere_array_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/struct/stack_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/system`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/system/app_detect_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/capture_tool_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/display_info_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/display_monitor_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/graphics_info_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/hardware_info_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/process_probe_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/system/system_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/thread`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/thread/compute_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/thread/thread_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/time`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/time/calendar_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/time/clock_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/time/datetime_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/time/nanotime_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/time/time_test.c` | ❌ | — | — | No executed evidence | — | untested |

### `tests/vexspoke/util`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tests/vexspoke/util/arrays_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/util/hash_test.c` | ❌ | — | — | No executed evidence | — | untested |
| `tests/vexspoke/util/random_test.c` | ❌ | — | — | No executed evidence | — | untested |

## workspace

### `.`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `.gitignore` | ❌ | — | — | No executed evidence | — | untested |
| `AGENTS.md` | ❌ | — | — | No executed evidence | — | untested |
| `LICENSE` | ❌ | — | — | No executed evidence | — | untested |
| `README.md` | ❌ | — | — | No executed evidence | — | untested |
| `darling-test-thought` | ❌ | — | — | No executed evidence | — | untested |
| `deferred system.txt` | ❌ | — | — | No executed evidence | — | untested |
| `functions-for-darling.txt` | ❌ | — | — | No executed evidence | — | untested |
| `preferences.md` | ❌ | — | — | No executed evidence | — | untested |
| `prune-opencode-projects.sh` | ❌ | — | — | No executed evidence | — | untested |

### `.opencode`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `.opencode/opencode.json` | ❌ | — | — | No executed evidence | — | untested |

### `.opencode/plugins/agent-bus`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `.opencode/plugins/agent-bus/index.ts` | ❌ | — | — | No executed evidence | — | untested |

### `run`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `run/hello.c` | ❌ | — | — | No executed evidence | — | untested |

### `tools`

| Filename | Tested? | Last checked (Unix seconds) | SHA-256 at check | Evidence / scope | Written by | Result |
| :--- | :---: | ---: | :--- | :--- | :--- | :--- |
| `tools/agents.sh` | ✅ | 1791038534 | 7561e1a0ada9781376bae821ae2cf68c008d76382fce9214e605d6c81edb9e83 | ['python3', '-B', 'tests/tools/agents_test.py']; macOS; 5 offline fake-API tests: signed say/tell, exclude self, raw opt-out, persisted name/env override, bus attribution; live API delivery not proved | agent (agent ses_efdd52f7affe7Nv6yn5GrSHEk9) | passed |
| `tools/b` | ❌ | — | — | No executed evidence | — | untested |
| `tools/b.c` | ❌ | — | — | No executed evidence | — | untested |
| `tools/darling-gallery.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/debug_compile.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/linter.py` | ❌ | — | — | No executed evidence | — | untested |
| `tools/make_code_txt.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/make_vkapp.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/run.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/run_debug.sh` | ❌ | — | — | No executed evidence | — | untested |
| `tools/spv_header.py` | ❌ | — | — | No executed evidence | — | untested |
| `tools/test_checklist.py` | ✅ | 1791038528 | a1ec94fd34c45842c77881119389ef8a9603cf0cb69d7d042fc598818937fffa | ['python3', '-B', 'tests/tools/test_checklist_test.py']; macOS; 6 offline tests: inventory, unknown status, Unix timestamp, command failure/skip, stale content, missing rows, Markdown escaping, mutation during test; no framework runtime proof | agent (agent ses_efdd52f7affe7Nv6yn5GrSHEk9) | passed |
