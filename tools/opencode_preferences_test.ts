import { afterEach, expect, test } from "bun:test"
import { execFile } from "node:child_process"
import { mkdtemp, mkdir, readFile, rm, writeFile } from "node:fs/promises"
import { tmpdir } from "node:os"
import { join, resolve } from "node:path"
import { promisify } from "node:util"
import plugin, { buildPreferencesPrompt } from "../../.opencode/plugins/preferences/index"

const ROOT = resolve(import.meta.dir, "../..")
const BUDGET = 32 * 1024
const run = promisify(execFile)
const scratch: string[] = []

afterEach(async () => {
  for (const path of scratch.splice(0)) await rm(path, { recursive: true, force: true })
})

async function prefs(...args: string[]): Promise<string> {
  const { stdout } = await run(join(ROOT, "tools", "prefs"), args, {
    cwd: ROOT, encoding: "utf8", maxBuffer: 8 * 1024 * 1024, timeout: 30_000,
  })
  return stdout
}

// A workspace where the command is absent, so only the fallback path can run.
async function fallbackFixture(gist: string | null): Promise<string> {
  const root = await mkdtemp(join(tmpdir(), "opencode-preferences-"))
  scratch.push(root)
  await mkdir(join(root, "metadata/gists/preferences"), { recursive: true })
  await mkdir(join(root, "tools/scripts"), { recursive: true })
  await writeFile(join(root, "tools/scripts/agents.sh"), "")
  await writeFile(join(root, "SETUP.md"), "SESSION_SETUP_ONCE")
  if (gist !== null) await writeFile(join(root, "metadata/gists/preferences/gist.md"), gist)
  return root
}

test("the injected block is the compressed constitution plus indexes, never the lawbooks", async () => {
  const prompt = await buildPreferencesPrompt(ROOT)
  const gist = await readFile(join(ROOT, "metadata/gists/preferences/gist.md"), "utf8")
  expect(prompt).toContain(gist.trimEnd())
  expect(prompt).toContain("Per-File Battle Test Law")

  for (const lawbook of ["suite/ecosystem/graphvex/graphvex-preferences.md",
    "suite/ecosystem/determinant/determinant-preferences.md",
    "suite/personal/primeagen/primeagen-preferences.md"]) {
    expect(prompt).toContain(lawbook)
    expect(prompt).not.toContain(await readFile(join(ROOT, lawbook), "utf8"))
  }
  expect(prompt).not.toContain(await readFile(join(ROOT, "tests/test-preferences.md"), "utf8"))
})

test("the block fits the per-request budget", async () => {
  const bytes = Buffer.byteLength(await buildPreferencesPrompt(ROOT))
  expect(bytes).toBeLessThan(BUDGET)
  expect(bytes).toBeGreaterThan(8 * 1024) // the constitution itself, not an empty block
})

test("the plugin and the command return the same live text", async () => {
  expect(await buildPreferencesPrompt(ROOT)).toBe(await prefs("prompt"))
})

test("missing command falls back to the compressed constitution, not to silence", async () => {
  const root = await fallbackFixture("COMPRESSED_CONSTITUTION")
  const prompt = await buildPreferencesPrompt(root)
  expect(prompt).toContain("COMPRESSED_CONSTITUTION")
  expect(prompt).toContain("tools/prefs prompt")
  expect(prompt).toContain("`./tools/prefs repo <name>`")
})

test("no constitution at all fails loudly instead of omitting the laws", async () => {
  const root = await fallbackFixture(null)
  await expect(buildPreferencesPrompt(root)).rejects.toThrow()
})

test("context hook prepends setup once per session and reloads the block every request", async () => {
  const root = await fallbackFixture("COMPRESSED_CONSTITUTION")
  let hook: ((event: unknown) => Promise<void>) | undefined
  await plugin.setup({
    location: { directory: join(root, "nested/leaf") },
    session: { hook: async (name, callback) => { expect(name).toBe("context"); hook = callback } },
  })
  const first = { sessionID: "session-a", system: [{ type: "text", text: "existing" }] }
  await hook!(first)
  expect(first.system).toHaveLength(3)
  expect(first.system[0].text).toBe("SESSION_SETUP_ONCE")
  expect(first.system[1].text).toContain("COMPRESSED_CONSTITUTION")
  expect(first.system[2].text).toBe("existing")

  await writeFile(join(root, "metadata/gists/preferences/gist.md"), "UPDATED_CONSTITUTION")
  const second = { sessionID: "session-a", system: [] as Array<{ type: string; text: string }> }
  await hook!(second)
  expect(second.system).toHaveLength(1)
  expect(second.system[0].text).toContain("UPDATED_CONSTITUTION")
  expect(second.system[0].text).not.toContain("COMPRESSED_CONSTITUTION")

  const other = { sessionID: "session-b", system: [] as Array<{ type: string; text: string }> }
  await hook!(other)
  expect(other.system[0].text).toBe("SESSION_SETUP_ONCE")
  expect(other.system[1].text).toContain("UPDATED_CONSTITUTION")
})

test("laws resolve by Title, and an unknown law is refused", async () => {
  expect(await prefs("law", "vertical")).toContain("## 6. Vertical Integration Law")
  expect(await prefs("section", "compose down")).toContain("### 6.3 Compose down, never rebuild")
  expect(await prefs("section", "compose down", "--full")).toContain("Compose down, never rebuild")
  expect(await prefs("law", "relational memory")).toContain("No malloc, No free")
  expect(await prefs("law", "relational memory")).toContain("Memory_alloc")
  expect(await prefs("law", "relational memory")).toContain("malloc")
  await expect(prefs("law", "99")).rejects.toThrow()
})

test("proof laws and repository laws are served complete on demand", async () => {
  expect(await prefs("test")).toContain("Per-File Battle Test Law")
  expect(await prefs("test", "Two-Dylib Swap")).toContain("### Two-Dylib Swap Law")
  expect(await prefs("repo", "func")).toContain("func — Repo-Local Living Preferences")
  expect(await prefs("repo", "func", "Standalone Opcode")).toContain("### Standalone Opcode Tool Law")
  expect(await prefs("repo", "func", "--path")).toContain("suite/personal/func/func-preferences.md")
  await expect(prefs("repo", "no-such-repo")).rejects.toThrow()
})

test("check holds the shape and the injection budget", async () => {
  const report = await prefs("check")
  expect(report).toContain("PASS")
  expect(report).toContain("injected:")
  expect(report).toContain("38 compressed / 38 full")
})

test("project config declares constitution and AGENTS points at the command", async () => {
  const config = JSON.parse(await readFile(join(ROOT, ".opencode/opencode.json"), "utf8"))
  expect(config.instructions).toEqual([])
  expect(config.permissions).toContainEqual({ action: "execute", resource: "*", effect: "deny" })
  const agents = await readFile(join(ROOT, "AGENTS.md"), "utf8")
  expect(agents).toContain("prefs repo <name>")
  expect(agents).toContain("./tools/prefs law 6 --full")
  const readme = await readFile(join(ROOT, ".opencode/README.md"), "utf8")
  expect(readme).toContain("`instructions` array is empty")
  expect(readme).toContain("prepends the preferences block")
})
