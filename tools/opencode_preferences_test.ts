import { afterEach, expect, test } from "bun:test"
import { mkdtemp, mkdir, readFile, readdir, rm, symlink, writeFile } from "node:fs/promises"
import { tmpdir } from "node:os"
import { join, resolve } from "node:path"
import plugin, { buildPreferencesPrompt } from "../../.opencode/plugins/preferences/index"

const scratch: string[] = []
afterEach(async () => {
  for (const path of scratch.splice(0)) await rm(path, { recursive: true, force: true })
})

async function fixture() {
  const root = await mkdtemp(join(tmpdir(), "opencode-preferences-"))
  scratch.push(root)
  await mkdir(join(root, "metadata/gists"), { recursive: true })
  await mkdir(join(root, "host-behavior/leaf/.git"), { recursive: true })
  await mkdir(join(root, "tests"))
  await mkdir(join(root, "tools"))
  await writeFile(join(root, "tools/agents.sh"), "")
  await writeFile(join(root, "PREFERENCES.md"), "UNIVERSAL_COMPLETE")
  await symlink(join(root, "PREFERENCES.md"), join(root, "metadata/gists/preferences.md"))
  await writeFile(join(root, "tests/test-preferences.md"), "PROOF_COMPLETE")
  await writeFile(join(root, "host-behavior/leaf/leaf-preferences.md"), "LOCAL_COMPLETE")
  await writeFile(join(root, "host-behavior/leaf/.git/preferences.md"), "EXCLUDED")
  await symlink(join(root, "metadata/gists/preferences.md"), join(root, "host-behavior/leaf/preferences.md"))
  return root
}

test("one complete prompt, scoped sources, alias deduplication and metadata exclusion", async () => {
  const root = await fixture()
  const prompt = await buildPreferencesPrompt(root)
  for (const text of ["UNIVERSAL_COMPLETE", "PROOF_COMPLETE", "LOCAL_COMPLETE"])
    expect(prompt.split(text).length - 1).toBe(1)
  expect(prompt).not.toContain("EXCLUDED")
  expect(prompt).toContain("apply to their owning repositories")
})

test("context hook resolves ancestors, preserves existing prompt and reloads changes", async () => {
  const root = await fixture()
  let hook: ((event: unknown) => Promise<void>) | undefined
  await plugin.setup({
    location: { directory: join(root, "host-behavior/leaf") },
    session: { hook: async (name, callback) => { expect(name).toBe("context"); hook = callback } },
  })
  const first = { system: [{ type: "text", text: "existing" }] }
  await hook!(first)
  expect(first.system).toHaveLength(2)
  expect(first.system[0].text).toContain("UNIVERSAL_COMPLETE")
  expect(first.system[1].text).toBe("existing")
  await writeFile(join(root, "PREFERENCES.md"), "UPDATED_COMPLETE")
  const second = { system: [] as Array<{ type: string; text: string }> }
  await hook!(second)
  expect(second.system).toHaveLength(1)
  expect(second.system[0].text).toContain("UPDATED_COMPLETE")
  expect(second.system[0].text).not.toContain("UNIVERSAL_COMPLETE")
})

test("missing required instructions fail rather than silently omit laws", async () => {
  const root = await fixture()
  await rm(join(root, "tests/test-preferences.md"))
  await expect(buildPreferencesPrompt(root)).rejects.toThrow()
})

test("actual workspace constitution and test lawbook are included verbatim", async () => {
  const root = resolve(import.meta.dir, "../..")
  const prompt = await buildPreferencesPrompt(root)
  expect(prompt).toContain(await readFile(join(root, "metadata/gists/preferences.md"), "utf8"))
  expect(prompt).toContain(await readFile(join(root, "tests/test-preferences.md"), "utf8"))
  expect(prompt).toContain(await readFile(join(root, "suite/ecosystem/graphvex/graphvex-preferences.md"), "utf8"))
  expect(prompt).toContain(await readFile(join(root, "suite/ecosystem/determinant/determinant-preferences.md"), "utf8"))
})

test("Gist sources use the metadata alias and keep the checklist outside tests", async () => {
  const root = resolve(import.meta.dir, "../..")
  const constitution = await readFile(join(root, "metadata/gists/preferences.md"), "utf8")
  const checklist = await readFile(join(root, "metadata/gists/test-checklist.md"), "utf8")
  expect(constitution).toContain("R1 > R2 > R3 > R4 > R5 > R6")
  expect(constitution).toContain("Primeagen remains standalone R5")
  expect(checklist).toContain("Timestamped Test Checklist Law")
  expect(await readFile(join(root, "PREFERENCES.md"), "utf8")).toBe(constitution)
  expect(await readFile(join(root, "metadata/gists/preferences.md"), "utf8")).toBe(constitution)
  await expect(readFile(join(root, "tests/test-checklist.md"))).rejects.toThrow()
})

test("constitution precedes proof and repo laws, with mandatory read-first guidance", async () => {
  const root = await fixture()
  const prompt = await buildPreferencesPrompt(root)
  expect(prompt.indexOf("UNIVERSAL_COMPLETE")).toBeLessThan(prompt.indexOf("PROOF_COMPLETE"))
  expect(prompt.indexOf("PROOF_COMPLETE")).toBeLessThan(prompt.indexOf("LOCAL_COMPLETE"))
  expect(prompt).toContain("Read the complete constitution first")
  expect(prompt).toContain("Inspect existing implementation")
})

test("project config declares constitution and AGENTS supplies V2 fallback", async () => {
  const root = resolve(import.meta.dir, "../..")
  const config = JSON.parse(await readFile(join(root, ".opencode/opencode.json"), "utf8"))
  expect(config.instructions[0]).toBe("metadata/gists/preferences.md")
  expect(config.permissions).toContainEqual({ action: "execute", resource: "*", effect: "deny" })
  const agents = await readFile(join(root, "AGENTS.md"), "utf8")
  expect(agents).toContain("Every primary agent and subagent")
  expect(agents).toContain("explicitly\nread `metadata/gists/preferences.md` before proceeding")
  const readme = await readFile(join(root, ".opencode/README.md"), "utf8")
  expect(readme).toContain("does not resolve the `instructions` config field")
  expect(readme).toContain("prepends its block")
  expect(readme).toContain("outside this workspace")
})

test("canonical constitution requires implementation, adversarial proof and per-class records", async () => {
  const root = resolve(import.meta.dir, "../..")
  const constitution = await readFile(join(root, "metadata/gists/preferences.md"), "utf8")
  expect(constitution).toContain("| 30 | Feature Implementation and Adversarial Proof Law |")
  for (const clause of ["Implement accepted intent", "Attack the implemented contract",
    "Execute, repair, repeat", "Close with evidence", "Commit per class in its own repository",
    "not required to form an independently buildable, bisectable stream"])
    expect(constitution).toContain(clause)
  expect(constitution).not.toContain("Keep broken intermediate states uncommitted")
  expect(constitution).not.toContain("Related class pairs may land together")
})

test("taxonomy registers every workspace repo lawbook and requires chained reading", async () => {
  const root = resolve(import.meta.dir, "../..")
  const constitution = await readFile(join(root, "metadata/gists/preferences.md"), "utf8")
  const map = constitution.split("### Repository preferences: mandatory reading chain")[1]?.split("## 1. Semantic Consistency Law")[0]
  expect(map).toBeDefined()
  expect(map).toContain("complete constitution first")
  expect(map).toContain("owning repository's complete")
  expect(map).toContain("every repository being changed")
  expect(map).toContain("missing local")
  expect(constitution).toContain("**Mandatory chained reading:**")
  const found: string[] = []
  async function scan(directory: string) {
    for (const entry of await readdir(join(root, directory), { withFileTypes: true })) {
      if (entry.name.startsWith(".") || entry.name === "node_modules" ||
        entry.name === "_trash" || entry.name === "_notes") continue
      const path = join(directory, entry.name)
      if (entry.isDirectory()) await scan(path)
      else if (entry.name.endsWith("-preferences.md") && entry.name !== "test-preferences.md") found.push(path)
    }
  }
  await scan(".")
  const linked: string[] = []
  for (const match of map!.matchAll(/\[([^\]]+-preferences\.md)\]\(([^)]+)\) \| `([^`]+)`/g)) {
    linked.push(match[3])
  }
  expect(linked.length).toBeGreaterThan(0)
  expect(found).toContain("suite/ecosystem/graphvex/graphvex-preferences.md")
  expect(found).toContain("suite/ecosystem/determinant/determinant-preferences.md")
  const prompt = await buildPreferencesPrompt(root)
  for (const preference of found)
    expect(prompt).toContain(await readFile(join(root, preference), "utf8"))
  expect(new Set(linked).size).toBe(linked.length)
  const agents = await readFile(join(root, "AGENTS.md"), "utf8")
  expect(agents).toContain("Follow the repository reading map")
})
