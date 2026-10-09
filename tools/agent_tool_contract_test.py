"""Source-grounded documentation owner for func/Harness boundaries and backlog prose.

Checks actual source absence and metadata, then forward-contract consistency.
This is not compiler/provider/MCP execution or production-readiness evidence.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
CONSTITUTION = "https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a"
SOURCE_SUFFIXES = {".c", ".h", ".m", ".mm", ".rs", ".cpp", ".cc"}


class AgentToolContractTest(unittest.TestCase):
    def test_actual_blueprints_and_discovery(self):
        for repo, relative in (("func", "personal/func"),
                               ("harness", "ecosystem/repos/harness")):
            owner = ROOT / relative
            sources = [p for p in owner.rglob("*") if p.is_file()
                       and ".git" not in p.relative_to(owner).parts
                       and p.suffix in SOURCE_SUFFIXES]
            self.assertEqual(sources, [], repo)
            cmake = (owner / "CMakeLists.txt").read_text()
            self.assertIn("LANGUAGES NONE", cmake)
            readme = (owner / "README.md").read_text()
            for scope in ("## Current State", "## Scope and Limitations",
                          "Platforms proven:** none", "nothing"):
                self.assertIn(scope, readme)
            prefs = owner / f"{repo}-preferences.md"
            self.assertTrue(prefs.is_file())
            self.assertIn(CONSTITUTION, prefs.read_text())
            self.assertIn(CONSTITUTION, (owner / "CONTRIBUTING.md").read_text())
            constitution = (ROOT / "preferences.md").read_text()
            self.assertIn(f"]({relative}/{repo}-preferences.md)", constitution)

    def test_func_numeric_native_and_bounded_contracts(self):
        owner = ROOT / "personal/func"
        prefs = (owner / "func-preferences.md").read_text()
        inventory = (owner / "CLASSES.md").read_text()
        for concept in ("numeric", "arity", "FuncProgram", "native", "interpreted"):
            self.assertIn(concept, inventory)
            self.assertIn(concept, prefs)
        for obligation in ("not an R1–R5", "no ecosystem", "not ecosystem object type IDs",
                           "literal argument vectors", "not implicit run",
                           "Deliberate Exhaustion and Backend Trust Law", "no implementation"):
            self.assertIn(obligation, prefs)
        self.assertIn("personal/func/func-preferences.md", (ROOT / "preferences.md").read_text())

    def test_harness_objects_do_not_duplicate_api_drivers(self):
        owner = ROOT / "ecosystem/repos/harness"
        prefs = (owner / "harness-preferences.md").read_text()
        readme = (owner / "README.md").read_text()
        for concept in ("Model", "Prompt", "Conversation", "Answer", "Question",
                        "ToolRegistry", "HarnessMcp"):
            self.assertIn(concept, readme)
            self.assertIn(concept, prefs)
        for boundary in ("never a parallel R4", "not", "bounded", "planned",
                         "Deliberate Exhaustion and Backend Trust Law"):
            self.assertIn(boundary, prefs)
        constitution = (ROOT / "preferences.md").read_text()
        self.assertIn("no competing R4 JSON-RPC or transport driver", constitution)
        self.assertIn("remain unimplemented contracts", constitution)

    def test_blockers_introduction_is_impersonal_and_non_authorizing(self):
        intro = (ROOT / "_notes/blockers.md").read_text().split("## How to use", 1)[0]
        self.assertNotRegex(intro.lower(), r"\byou(?:r)?\b")
        self.assertIn("does not imply oversight or forgotten", intro)
        self.assertIn("does not authorize implementation", intro)
        self.assertIn("repository pushes", intro)
        self.assertIn("Reviewed: 2026-10-08", intro) # Wording-only change, not a fresh audit.


if __name__ == "__main__":
    unittest.main()
