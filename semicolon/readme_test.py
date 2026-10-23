#!/usr/bin/env python3
"""Mechanically review the workspace/Semicolon dogfooding claims.

This checks documentation against current source wiring, not UI appearance,
application integration, or the truth of every roadmap row.
"""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[2]
APP = ROOT / "suite/projects/semicolon"


class ReadmeTest(unittest.TestCase):
    def test_dogfooding_entry_and_proof_boundary(self):
        workspace = (ROOT / "README.md").read_text()
        semicolon = (APP / "README.md").read_text()
        launcher = (ROOT / "tools/b").read_text()
        source = (APP / "src/main/tui-main.c").read_text()
        build = (APP / "build.py").read_text()
        bridge = (APP / "src/editor/semicolon_syntax.c").read_text()
        language = (ROOT / "suite/ecosystem/language/adapters/c23/c23_adapter.c").read_text()

        self.assertIn("### Dogfooding with Semicolon", workspace)
        self.assertIn("### Vertically integrated across vexgraph's stack", workspace)
        self.assertIn("## Vertically integrated across vexgraph's stack", semicolon)
        self.assertIn("suite/projects/semicolon/README.md", workspace)
        self.assertIn("suite/projects/semicolon", workspace)
        self.assertIn("`b` is orchestration, not another R tier", workspace)
        self.assertIn("`b` is\nthe standalone **orchestration layer**, not an R tier", semicolon)
        self.assertIn("does not prove a windowed app's behavior", semicolon)
        self.assertIn("Language's C23 lexer", workspace)
        self.assertIn("Language's conservative `C23Highlights` pass", semicolon)
        self.assertIn("`#include` and `#define` share that macro pen", semicolon)
        self.assertIn("parameter inlays, diagnostics, completion", semicolon)
        self.assertIn("not exact truecolor", semicolon)
        self.assertGreater(workspace.index("## Scope and Limitations"),
                           workspace.index("## The repositories"))
        self.assertGreater(semicolon.index("## Scope and Limitations"),
                           semicolon.index("## Layout"))

        self.assertIn('exec "$binary" run exec "$root/tools/workspace.c"', launcher)
        self.assertIn('workspaceRunSource(const char *path)', source)
        self.assertIn('"/tools/b" : "/suite/personal/b/b"', source)
        self.assertIn('determinant.glob("terminal/*.c")', build)
        self.assertIn('engine / "nio/mem.c"', build)
        self.assertIn('vexspoke / "primitive/string.c"', build)
        self.assertIn('.languageId = "c23"', language)
        self.assertIn('"adapters/c/c23_lexer.c"', build)
        self.assertIn('"adapters/c/c23_highlights.c"', build)
        self.assertIn('SEMICOLON_LANGUAGE_SOURCE_DIR', build)
        self.assertIn('#include "editor/semicolon_syntax.h"', source)
        self.assertIn('SemicolonSyntax_setLines', source)
        self.assertIn('paintEditorSyntax(&back', source)
        self.assertIn('C23Highlights(1u, source, length)', bridge)
        self.assertIn('C23Highlights_roleAt', bridge)
        self.assertNotIn('#include "lang/', source)
        self.assertNotIn('#include "adapters/c23/', source)

        for relative in ("suite/projects/semicolon/README.md",
                         "suite/ecosystem/language/README.md"):
            self.assertTrue((ROOT / relative).is_file(), relative)
        self.assertTrue((APP / "../../../README.md").resolve().is_file())


if __name__ == "__main__":
    unittest.main(verbosity=2)
