"""Offline architecture-documentation owner, not runtime or visual readiness.

Proves the two-owner R2 split, staged migration, unfinished R5 warnings,
canonical constitution links and actual checkout/lawbook discovery paths.
No web request, application launch or allocator/FFI execution is performed.
Run independently: python3 -B tests/tools/ecosystem_docs_test.py.
"""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
GIST = "https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a"
READINESS_GIST = "https://gist.github.com/vex-graph/6943f92acb931b25dad1073c46da6ce7"
BLOCKERS_GIST = "https://gist.github.com/vex-graph/e921fa188eebbd0c68c4e59646109887"
REPOS = ("relational-engine", "vexspoke", "graphvex", "api-haven", "hotcwap",
         "language", "darkbase", "darling-framework", "sesh", "samplerate")
APPS = ("anti", "drawling", "semicolon", "impedance")
WIKI = (*APPS, "darling-editor", "samplerate", "vexspoke", "relational-engine",
        "graphvex", "api-haven", "hotcwap", "language", "darkbase", "darling", "sesh")
SUBJECTS = (
    "README.md", "preferences.md", "personal/README.md",
    "personal/b/README.md", "personal/b/CONTRIBUTING.md",
    "personal/vex-graph/README.md", "ecosystem/.github/profile/README.md",
    "tests/README.md", "tests/test-preferences.md", "ecosystem/repos/vexspoke/BACKEND.md",
    "ecosystem/ecosystem/Home.md", "ecosystem/ecosystem/_Sidebar.md",
    *(f"ecosystem/repos/{repo}/{file}" for repo in REPOS
      for file in ("README.md", "CONTRIBUTING.md")),
    *(f"ecosystem/repos/{repo}/{repo}-preferences.md" for repo in REPOS
      if repo != "darling-framework"),
    *(f"ecosystem/projects/{app}/{file}" for app in APPS
      for file in ("README.md", "CONTRIBUTING.md")),
    *(f"ecosystem/ecosystem/{page}.md" for page in WIKI),
)


def text(path):
    return (ROOT / path).read_text(encoding="utf-8")


def normalized(path):
    return " ".join(text(path).lower().split())


class EcosystemDocsTest(unittest.TestCase):
    def test_subject_inventory_and_retired_constitution_links(self):
        for path in SUBJECTS:
            with self.subTest(path=path):
                content = text(path)
                self.assertTrue(content.strip())
                self.assertNotIn("vexspoke/blob/main/preferences.md", content)
                self.assertNotIn("_repositories/.ecosystem", content)
                self.assertNotIn("personal/relational-engine", content)
                self.assertNotRegex(content, r"(?:path is|checkout remains)\s+``",
                                    "empty path from a broken replacement")

    def test_repo_readmes_define_staged_r2_contract(self):
        for path in [*(f"ecosystem/repos/{repo}/README.md" for repo in REPOS),
                     *(f"ecosystem/projects/{app}/README.md" for app in APPS),
                     "README.md", "ecosystem/ecosystem/Home.md",
                     "ecosystem/.github/profile/README.md"]:
            with self.subTest(path=path):
                content = normalized(path)
                for expected in ("vexspoke", "relational engine", "r2",
                                 "r1"):
                    self.assertIn(expected, content)
                self.assertRegex(content, r"gpu|graphics")
                self.assertRegex(content, r"comput(ation|e)")
                self.assertIn("behavior", content)

    def test_contributing_and_local_lawbook_links(self):
        for path in [*(f"ecosystem/repos/{repo}/CONTRIBUTING.md" for repo in REPOS),
                     *(f"ecosystem/projects/{app}/CONTRIBUTING.md" for app in APPS),
                     "personal/b/CONTRIBUTING.md", "personal/vex-graph/README.md"]:
            with self.subTest(path=path):
                self.assertIn(GIST, text(path))
        for repo in REPOS:
            if repo == "darling-framework":
                self.assertIn("no local", normalized(f"ecosystem/repos/{repo}/CONTRIBUTING.md"))
                continue
            path = f"ecosystem/repos/{repo}/{repo}-preferences.md"
            content = text(path)
            self.assertIn(GIST, content)
            self.assertIn("relational", content.lower())
            for target in re.findall(r"\]\(([^)]+\.md)\)", content):
                if target.startswith(("http:", "https:")):
                    continue
                self.assertTrue(((ROOT / path).parent / target).resolve().exists(), target)

    def test_constitution_map_matches_real_owner_checkouts(self):
        content = text("preferences.md")
        for repo in REPOS:
            if repo == "darling-framework":
                continue
            path = f"ecosystem/repos/{repo}/{repo}-preferences.md"
            self.assertIn(f"]({path})", content)
            self.assertTrue((ROOT / path).is_file())
        self.assertFalse((ROOT / "preferences.md").is_symlink())
        for expected in ("R2 — COMPUTATION + STORAGE", "R2 `relational-engine`",
                         "R2 `vexspoke`", "and/or `relational-engine` public contracts",
                          "default production build", "No C/Rust atomic-layout",
                         "R5 applications remain unfinished"):
            self.assertIn(expected, " ".join(content.split()))

    def test_unfinished_ecosystem_and_all_r5_pages(self):
        for path in ("README.md", "ecosystem/.github/profile/README.md",
                     "ecosystem/ecosystem/Home.md"):
            content = normalized(path)
            self.assertIn("unfinished", content)
            self.assertIn("r5", content)
        for page in (*APPS, "darling-editor", "samplerate"):
            self.assertIn("unfinished", normalized(f"ecosystem/ecosystem/{page}.md"))
        for app in APPS:
            self.assertIn("unfinished", normalized(f"ecosystem/projects/{app}/README.md"))
        home = text("ecosystem/ecosystem/Home.md")
        sidebar = text("ecosystem/ecosystem/_Sidebar.md")
        self.assertIn("[[relational-engine]]", home)
        self.assertIn("|relational-engine]]", sidebar)
        engine = text("ecosystem/ecosystem/relational-engine.md")
        self.assertNotIn("🟩", engine)
        self.assertNotIn("💚", engine)
        self.assertIn("Manifest-backed persistent objects", engine)

    def test_test_lawbook_and_engine_implementation_boundaries(self):
        laws = text("tests/test-preferences.md")
        for expected in ("Rust Ownership and Stable Row Proof Law",
                         "Native Span Boundary Proof Law", "Resident Backend Proof Law",
                         "32-byte VariableSlot", "debug and release", "schema migration"):
            self.assertIn(expected, laws)
        self.assertIn(GIST, laws)
        engine = normalized("ecosystem/repos/relational-engine/README.md")
        for expected in ("chunk<t>", "chunkedlist<t>", "variableslot", "variableregistry",
                         "re_name_search", "append-only", "borrowed", "planned",
                           "manifest-backed", "discussion", "native io/nio", "native c search"):
            self.assertIn(expected, engine)

    def test_readiness_matrix_and_blockers_are_gist_canonical(self):
        """The readiness matrix and open-work backlog live in Gists, not the wiki."""
        constitution = text("preferences.md")
        self.assertIn(READINESS_GIST, constitution)
        self.assertIn(BLOCKERS_GIST, constitution)
        self.assertIn("no longer canonical", normalized("preferences.md"))
        for repo in REPOS:
            if repo == "darling-framework":
                continue
            law = text(f"ecosystem/repos/{repo}/{repo}-preferences.md")
            self.assertIn(READINESS_GIST, law)
            self.assertIn(BLOCKERS_GIST, law)

    def test_wiki_pages_link_the_new_storage_owner_without_automatic_migration(self):
        for page in WIKI:
            if page == "relational-engine":
                continue
            with self.subTest(page=page):
                content = normalized(f"ecosystem/ecosystem/{page}.md")
                self.assertIn("[[relational-engine]]", content)
                self.assertIn("staged", content)


if __name__ == "__main__":
    unittest.main(verbosity=2)
