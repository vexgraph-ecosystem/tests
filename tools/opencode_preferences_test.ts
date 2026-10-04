import { afterEach, expect, test } from "bun:test"
import { mkdtemp, mkdir, readFile, rm, symlink, writeFile } from "node:fs/promises"
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
  await mkdir(join(root, "ecosystem/leaf/.git"), { recursive: true })
  await mkdir(join(root, "tests"))
  await mkdir(join(root, "tools"))
  await writeFile(join(root, "tools/agents.sh"), "")
  await writeFile(join(root, "preferences.md"), "UNIVERSAL_COMPLETE")
  await writeFile(join(root, "tests/test-preferences.md"), "PROOF_COMPLETE")
  await writeFile(join(root, "ecosystem/leaf/leaf-preferences.md"), "LOCAL_COMPLETE")
  await writeFile(join(root, "ecosystem/leaf/.git/preferences.md"), "EXCLUDED")
  await symlink(join(root, "preferences.md"), join(root, "ecosystem/leaf/preferences.md"))
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
    location: { directory: join(root, "ecosystem/leaf") },
    session: { hook: async (name, callback) => { expect(name).toBe("context"); hook = callback } },
  })
  const first = { system: [{ type: "text", text: "existing" }] }
  await hook!(first)
  expect(first.system).toHaveLength(2)
  expect(first.system[0].text).toBe("existing")
  await writeFile(join(root, "preferences.md"), "UPDATED_COMPLETE")
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
  expect(prompt).toContain(await readFile(join(root, "preferences.md"), "utf8"))
  expect(prompt).toContain(await readFile(join(root, "tests/test-preferences.md"), "utf8"))
  expect(prompt).toContain(await readFile(join(root, "ecosystem/drivers/graphvex/graphvex-preferences.md"), "utf8"))
})
