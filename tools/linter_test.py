"""Proof for the architectural linter and its ratchet baseline.

Covers the Semantic Consistency Law (reference form, cast spelling, access
depth, `nullptr`), the Canonical Include Paths Law, the Living Documentation
Law, the No Section Sign Law, comment/string immunity, and the ratchet
semantics that let CI gate on new debt without rewriting 1,400 existing lines.

Fixtures are real `.c` files in a throwaway tree so the scanner walks the same
code path CI uses. No build, no network.
"""

import importlib.util
import json
import tempfile
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3] if len(Path(__file__).resolve().parents) > 3 else Path(__file__).resolve().parents[2]
_CANDIDATES = [ROOT / "tools/python/linter.py", ROOT / "tools/linter.py"]
SCRIPT = next((p for p in _CANDIDATES if p.exists()), _CANDIDATES[0])
SPEC = importlib.util.spec_from_file_location("linter", SCRIPT)
LINTER = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(LINTER)


class LinterTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def write(self, rel, text):
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def write_source(self, rel, body):
        """A production source file that already satisfies the file headers."""
        return self.write(rel, ";;DEFINITION\n;;OVERVIEW\n" + body)

    def scan(self):
        return LINTER.Linter([str(self.root)]).collect()

    def rules(self, violations):
        return Counter(v["rule"] for v in violations)

    def test_arrow_is_a_violation_and_rewrites_to_deref_field(self):
        self.write_source("src/subject.c", "void f(Node *n) { (*n).x = 1; }\n")
        clean = self.scan()
        self.assertEqual(clean, [])

        self.write_source("src/subject.c", "void f(Node *n) { n->x = 1; }\n")
        found = self.scan()
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0]["rule"],
                         "Semantic Consistency Law (Reference form)")
        self.assertTrue(found[0]["fixable"])

        LINTER.Linter([str(self.root)]).collect(fix=True)
        self.assertIn("(*n).x = 1", (self.root / "src/subject.c").read_text())
        self.assertEqual(self.scan(), [])

    def test_chained_arrow_rewrites_to_nested_deref(self):
        self.write_source("src/subject.c", "void f(A *a) { (*a)->b->c = 1; }\n")
        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertNotIn("->", text)

    def test_arrow_fix_preserves_leading_whitespace(self):
        self.write_source("src/subject.c", "void f(Node *n) { n->x = 1; }\n")
        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertIn("{ (*n).x = 1; }", text)

    def test_arrow_fix_never_trades_into_access_depth(self):
        self.write_source("src/subject.c",
                          "void f(Element *e) { (*e).property->w = 1; }\n")
        found = self.scan()
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0]["rule"],
                         "Semantic Consistency Law (Reference form)")
        self.assertFalse(found[0]["fixable"])

        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertIn("(*e).property->w = 1;", text)
        self.assertEqual([v["rule"] for v in self.scan()],
                         ["Semantic Consistency Law (Reference form)"])

    def test_multi_arrow_chain_keeps_the_hop_that_needs_hoisting(self):
        self.write_source("src/subject.c", "void f(Node *a) { a->b->c = 1; }\n")
        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertIn("(*a).b->c", text)
        self.assertNotIn("(*(*a).b)", text)

    def test_null_becomes_nullptr_only_in_code(self):
        self.write_source("src/subject.c",
                          'void f(void) { void *p = NULL; char *s = "NULL"; }\n'
                          "/* NULL in a comment stays */\n")
        found = self.scan()
        self.assertEqual(len(found), 1)
        self.assertIn("NULL", found[0]["msg"])

        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertIn("void *p = nullptr;", text)
        self.assertIn('"NULL"', text)
        self.assertIn("/* NULL in a comment stays */", text)

    def test_cast_spelling(self):
        self.write_source("src/subject.c",
                          "void f(void *p) { int *q = (int *) p; (void*)q; }\n")
        found = self.scan()
        self.assertEqual(self.rules(found)[
            "Semantic Consistency Law (Cast and pointer spelling)"], 2)

        LINTER.Linter([str(self.root)]).collect(fix=True)
        text = (self.root / "src/subject.c").read_text()
        self.assertIn("(int*) p", text)
        self.assertIn("(void*) q", text)

    def test_access_depth_flags_three_layer_chains(self):
        self.write_source("src/subject.c",
                          "void f(A *a) { (*a).b.c.d = 1; }\n")
        found = self.scan()
        self.assertTrue(any(v["rule"] == "Semantic Consistency Law (Access depth)"
                            for v in found))

    def test_parent_include_traversal_is_rejected(self):
        self.write_source("src/subject.c", '#include "../other/thing.h"\n')
        found = self.scan()
        self.assertTrue(any(v["rule"] == "Canonical Include Paths Law"
                            for v in found))

    def test_implementation_needs_overview_and_definition(self):
        self.write("src/subject.c", "int f(void) { return 0; }\n")
        found = self.scan()
        messages = [v["msg"] for v in found
                    if v["rule"] == "Living Documentation Law"]
        self.assertEqual(len(messages), 2)
        self.assertTrue(any(";;OVERVIEW" in m for m in messages))
        self.assertTrue(any(";;DEFINITION" in m for m in messages))

        self.write("src/subject.c",
                   ";;DEFINITION\n;;OVERVIEW\nint f(void) { return 0; }\n")
        self.assertEqual(
            [v for v in self.scan() if v["rule"] == "Living Documentation Law"],
            [])

    def test_tests_do_not_require_file_headers(self):
        self.write("tests/subject_test.c", "int main(void) { return 0; }\n")
        self.assertEqual(
            [v for v in self.scan() if v["rule"] == "Living Documentation Law"],
            [])

    def test_section_sign_is_rejected_in_text(self):
        self.write("docs/note.md", "see \u00a7 4\n")
        found = self.scan()
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0]["rule"], "No Section Sign Law")
        self.assertFalse(found[0]["fixable"])

    def test_strings_and_comments_are_not_arrow_or_null_hits(self):
        self.write_source("src/subject.c",
                          'const char *s = "a->b NULL";\n'
                          "// c->d NULL\n"
                          "/* e->f NULL */\n")
        self.assertEqual(self.scan(), [])

    def test_baseline_ratchet_only_fails_on_new_debt(self):
        path = self.write_source("src/subject.c", "void f(Node *n) { n->x = 1; }\n")
        baseline_path = self.root / "baseline.json"
        LINTER.save_baseline(str(baseline_path), self.scan())
        baseline = LINTER.load_baseline(str(baseline_path))

        new, retired, new_total, retired_total = LINTER.ratchet_diff(
            self.scan(), baseline)
        self.assertEqual((new_total, retired_total), (0, 0))

        path.write_text(";;DEFINITION\n;;OVERVIEW\n"
                        "void f(Node *n) { n->x = 1; }\n"
                        "void g(Node *n) { n->y = 2; }\n")
        new, retired, new_total, retired_total = LINTER.ratchet_diff(
            self.scan(), baseline)
        self.assertEqual(new_total, 1)
        self.assertEqual(retired_total, 0)

        path.write_text(";;DEFINITION\n;;OVERVIEW\n"
                        "void f(Node *n) { (*n).x = 1; }\n")
        new, retired, new_total, retired_total = LINTER.ratchet_diff(
            self.scan(), baseline)
        self.assertEqual(new_total, 0)
        self.assertGreaterEqual(retired_total, 1)

    def test_baseline_file_is_deterministic(self):
        self.write_source("src/a.c", "void f(Node *n) { n->x = 1; }\n")
        first = self.root / "one.json"
        second = self.root / "two.json"
        LINTER.save_baseline(str(first), self.scan())
        LINTER.save_baseline(str(second), self.scan())
        self.assertEqual(first.read_text(), second.read_text())
        data = json.loads(first.read_text())
        self.assertEqual(data["version"], 1)
        self.assertEqual(data["total"], 1)

    def test_baseline_keys_use_forward_slashes(self):
        self.write_source("src/subject.c", "void f(Node *n) { n->x = 1; }\n")
        path = self.root / "baseline.json"
        LINTER.save_baseline(str(path), self.scan())
        data = json.loads(path.read_text())
        self.assertTrue(data["counts"])
        for key in data["counts"]:
            self.assertNotIn("\\", key)
            self.assertIn("/", key)

    def test_json_report_shape(self):
        import contextlib
        import io

        self.write_source("src/subject.c", "void f(Node *n) { n->x = 1; }\n")
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = LINTER.main(["--json", str(self.root)])
        self.assertEqual(rc, 1)
        report = json.loads(out.getvalue())
        self.assertEqual(report["total"], 1)
        self.assertEqual(report["files_scanned"], 1)
        self.assertTrue(report["violations"][0]["file"].endswith("src/subject.c"))


if __name__ == "__main__":
    unittest.main()
