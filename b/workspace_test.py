"""Migration seam: compatibility launcher, workspace metadata and CPU target.

No interactive gallery or visual approval; the builder is exercised through its
existing metadata and registered headless compositor target, not blanket proof.
"""

import json
import os
import re
import shutil
import struct
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]


class WorkspaceTest(unittest.TestCase):
    def invoke(self, *arguments):
        result = subprocess.run([str(ROOT / "tools/b"), *arguments], cwd=ROOT / "b",
                                capture_output=True, text=True, timeout=120)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_doctor_resolves_worktree_not_checkout(self):
        doctor = self.invoke("doctor")
        self.assertIn(str(ROOT), doctor)
        self.assertIn("-Werror", doctor)
        self.assertIn("target vexspoke", doctor)
        self.assertFalse((ROOT / "tools/b.c").exists())

    def test_metadata_and_preserved_shader_wiring(self):
        metadata = json.loads(self.invoke("ide"))
        self.assertIsInstance(metadata, dict)
        self.assertIn("vexspoke", json.dumps(metadata))
        engine = (ROOT / "b/workspace.c").read_text()
        entries = re.search(r"const char \*compositorShaders\[\] = \{(.*?)\};", engine, re.S)
        self.assertIsNotNone(entries)
        self.assertEqual(re.findall(r'"([^"]+)"', entries.group(1)),
                         ["scatter.vert", "scatter.frag", "resolve.vert", "resolve.frag", "color.frag"])
        self.assertIn('if (!strcmp(compositorShaders[i], "color.frag"))\n'
                      '            add_gen(strf("%s/src/filter/filter_type.h", base), out, g);', engine)
        readme = (ROOT / "README.md").read_text()
        self.assertIn("`tools/b`", readme)
        self.assertIn("forwarding launcher", readme)

    def test_registered_cpu_target_still_builds_and_runs(self):
        self.assertIn("PASS", self.invoke("test", "compositor_scope_test"))

    def test_compositor_generators_compile_and_header_changes_invalidate(self):
        """Execute setup_graphvex/run_gens only, without building adjacent passes.

        Input copies and fixed mtimes isolate generator decisions from other
        agents. Real GLSL compilation proves wiring, not GPU rendering.
        """
        if shutil.which("glslangValidator") is None:
            self.skipTest("glslangValidator unavailable")
        with tempfile.TemporaryDirectory(prefix="b shader wiring ") as scratch:
            home = Path(scratch)
            base = home / "ecosystem/drivers/graphvex/src"
            shaders = base / "shaders/compositor"
            shaders.mkdir(parents=True)
            (base / "filter").mkdir()
            owner = ROOT / "ecosystem/drivers/graphvex/src"
            names = ["scatter.vert", "scatter.frag", "resolve.vert", "resolve.frag", "color.frag"]
            for name in names:
                shutil.copyfile(owner / "shaders/compositor" / name, shaders / name)
            header = base / "filter/filter_type.h"
            shutil.copyfile(owner / "filter/filter_type.h", header)
            for path in [header, *(shaders / name for name in names)]:
                os.utime(path, ns=(10**18, 10**18))
            client = home / "generators.c"
            client.write_text(r'''
#define main workspaceEntry
#include "workspace.c"
#undef main
#include <assert.h>
int main(int argc, char **argv) {
    assert(argc == 3);
    g_root = argv[1];
    g_out = argv[2];
    g_verbose = true;
    TargetList targets = {0};
    setup_graphvex(&targets);
    int count = 0;
    int headers = 0;
    for (int i = 0; i < g_genCount; ++i) {
        if (strstr(g_gens[i].out, "/shader/compositor/") == nullptr)
            continue;
        if (strstr(g_gens[i].src, "/filter/filter_type.h") != nullptr)
            ++headers;
        g_gens[count++] = g_gens[i];
    }
    assert(count == 6 && headers == 1);
    g_genCount = count;
    run_gens();
    return 0;
}
''')
            binary = home / "generators"
            subprocess.run([os.environ.get("CC", "cc"), "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                            "-I", str(ROOT / "b"), str(client), "-o", str(binary)],
                           capture_output=True, check=True, timeout=120)
            out = home / "out"

            def generate(expected=0):
                result = subprocess.run([str(binary), str(home), str(out)],
                                        capture_output=True, text=True, timeout=120)
                self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
                return result

            generate()
            outputs = [out / "shader/compositor" / (name + ".spv") for name in names]
            for path in outputs:
                self.assertEqual(struct.unpack("<I", path.read_bytes()[:4])[0], 0x07230203)
                os.utime(path, ns=(12 * 10**17, 12 * 10**17))
            self.assertEqual(generate().stdout, "")
            header.write_text(header.read_text() + "\n// generator invalidation fixture\n")
            os.utime(header, ns=(13 * 10**17, 13 * 10**17))
            rebuilt = generate().stdout
            self.assertIn("color.frag.spv", rebuilt)
            for name in names[:-1]:
                self.assertNotIn(name + ".spv", rebuilt)
            self.assertEqual(generate().stdout, "")
            color = shaders / "color.frag"
            original = color.read_text()
            color.write_text(original + "\ninvalid GLSL syntax !!!\n")
            os.utime(color, ns=(14 * 10**17, 14 * 10**17))
            os.utime(outputs[-1], ns=(12 * 10**17, 12 * 10**17))
            self.assertIn("command failed", generate(expected=1).stderr)
            color.write_text(original)
            os.utime(color, ns=(14 * 10**17, 14 * 10**17))
            generate()


if __name__ == "__main__":
    unittest.main(verbosity=2)
