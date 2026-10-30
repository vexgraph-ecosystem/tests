"""Proof for the structural layout arranger (Semantic Consistency Law: Control flow).

Every fixture is a real `.c` file in a throwaway tree, so the same walk and
rewrite path the workspace uses is exercised. The tests pin both what the law
requires (unbraced single bodies, header space, statement separation) and what
the arranger deliberately refuses to touch (multi-statement bodies, if/else
chains, struct members, file-scope declarations, comments, strings, macros).

No build, no network.
"""

import contextlib
import importlib.util
import io
import json
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3] if len(Path(__file__).resolve().parents) > 3 else Path(__file__).resolve().parents[2]
_CANDIDATES = [ROOT / "tools/python/arrange.py", ROOT / "tools/arrange.py"]
SCRIPT = next((p for p in _CANDIDATES if p.exists()), _CANDIDATES[0])
SPEC = importlib.util.spec_from_file_location("arrange", SCRIPT)
ARRANGE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(ARRANGE)


class ArrangeTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def write(self, rel, text):
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def analyze(self, text, rel="src/subject.c"):
        violations, _replacements, _deletions, _insertions = (
            ARRANGE.analyze(str(self.root / rel), text))
        return violations

    def messages(self, text):
        return [v["msg"] for v in self.analyze(text)]

    def fixed(self, text):
        return ARRANGE.fixpoint(str(self.root / "src/subject.c"), text)

    def run_cli(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = ARRANGE.main(list(argv))
        return code, out.getvalue(), err.getvalue()

    # ── single-statement bodies ────────────────────────────────────────────

    def test_single_statement_braces_are_removed(self):
        source = ("void f(int x) {\n"
                  "    if (x) {\n"
                  "        g();\n"
                  "    }\n"
                  "}\n")
        self.assertIn("single-statement braces", self.messages(source))
        self.assertEqual(self.fixed(source),
                         "void f(int x) {\n"
                         "    if (x) \n"
                         "        g();\n"
                         "}\n")

    def test_multi_statement_body_keeps_braces(self):
        source = ("void f(int x) {\n"
                  "    if (x) {\n"
                  "        g();\n"
                  "        h();\n"
                  "    }\n"
                  "}\n")
        self.assertNotIn("single-statement braces", self.messages(source))
        self.assertIn("if (x) {", self.fixed(source))

    def test_body_on_header_line_moves_to_the_next_line(self):
        source = ("void f(int x) {\n"
                  "    if (x) return;\n"
                  "}\n")
        self.assertIn("body on the header line", self.messages(source))
        self.assertEqual(self.fixed(source),
                         "void f(int x) {\n"
                         "    if (x) \n"
                         "       return;\n"
                         "}\n")

    def test_braced_body_with_a_comment_is_left_alone(self):
        source = ("void f(int x) {\n"
                  "    if (x) {\n"
                  "        /* keep this note */\n"
                  "        g();\n"
                  "    }\n"
                  "}\n")
        self.assertNotIn("single-statement braces", self.messages(source))

    def test_if_else_chain_is_left_alone(self):
        source = ("void f(int x) {\n"
                  "    if (x) {\n"
                  "        a();\n"
                  "    } else {\n"
                  "        b();\n"
                  "    }\n"
                  "}\n")
        self.assertNotIn("single-statement braces", self.messages(source))

    def test_do_while_is_left_alone(self):
        source = ("void f(int x) {\n"
                  "    do {\n"
                  "        a();\n"
                  "    } while (x);\n"
                  "}\n")
        self.assertNotIn("single-statement braces", self.messages(source))

    # ── header shape ───────────────────────────────────────────────────────

    def test_header_keeps_a_space_before_the_newline(self):
        source = ("void f(int x) {\n"
                  "    if (x)\n"
                  "        g();\n"
                  "}\n")
        self.assertIn("header space", self.messages(source))
        fixed = self.fixed(source)
        self.assertIn("    if (x) \n", fixed)
        self.assertIn("        g();\n", fixed)

    def test_blank_line_between_header_and_body_is_removed(self):
        source = ("void f(int x) {\n"
                  "    if (x)\n"
                  "\n"
                  "        g();\n"
                  "}\n")
        self.assertIn("header body gap", self.messages(source))
        self.assertIn("    if (x) \n        g();\n", self.fixed(source))

    def test_multi_line_condition_is_not_misread(self):
        source = ("void f(int a, int b) {\n"
                  "    if (a &&\n"
                  "        b) {\n"
                  "        g();\n"
                  "    }\n"
                  "}\n")
        self.assertNotIn("single-statement braces", self.messages(source))

    # ── statement separation ───────────────────────────────────────────────

    def test_sequential_statements_get_one_blank_line(self):
        source = ("void f(void) {\n"
                  "    int a = 1;\n"
                  "    int b = 2;\n"
                  "    return a + b;\n"
                  "}\n")
        self.assertEqual(self.messages(source).count("statement separation"), 2)
        self.assertEqual(self.fixed(source),
                         "void f(void) {\n"
                         "    int a = 1;\n"
                         "\n"
                         "    int b = 2;\n"
                         "\n"
                         "    return a + b;\n"
                         "}\n")

    def test_existing_blank_lines_are_preserved(self):
        source = ("void f(void) {\n"
                  "    int a = 1;\n"
                  "\n"
                  "    int b = 2;\n"
                  "}\n")
        self.assertEqual(self.messages(source), [])
        self.assertEqual(self.fixed(source), source)

    def test_a_closed_block_needs_a_blank_line_after_it(self):
        source = ("void f(int x) {\n"
                  "    if (x) {\n"
                  "        g();\n"
                  "        h();\n"
                  "    }\n"
                  "    return;\n"
                  "}\n")
        self.assertIn("statement separation", self.messages(source))
        self.assertIn("    }\n\n    return;\n", self.fixed(source))

    def test_struct_members_are_not_statements(self):
        source = ("struct S {\n"
                  "    int a;\n"
                  "    int b;\n"
                  "};\n")
        self.assertNotIn("statement separation", self.messages(source))

    def test_file_scope_declarations_are_not_statements(self):
        source = "static int a;\nstatic int b;\n"
        self.assertNotIn("statement separation", self.messages(source))

    def test_comments_and_strings_do_not_create_gaps(self):
        source = ("void f(void) {\n"
                  "    char *s = \"a;b\";\n"
                  "    /* c;d */\n"
                  "    g(s);\n"
                  "}\n")
        self.assertEqual(self.messages(source), [])

    def test_macro_continuation_is_not_a_statement(self):
        source = ("#define M(x) do { \\\n"
                  "    g(x); \\\n"
                  "} while (0)\n"
                  "void f(void) {\n"
                  "    int a = 1;\n"
                  "}\n")
        self.assertNotIn("statement separation", self.messages(source))

    # ── CLI: check, diff, write, baseline ──────────────────────────────────

    def test_check_diff_write_cycle_is_idempotent(self):
        source = ("void f(int x) {\n"
                  "    if (x) return;\n"
                  "}\n")
        path = self.write("src/subject.c", source)

        code, out, _err = self.run_cli(str(path))
        self.assertEqual(code, 1)
        self.assertIn("Arrangement violations: 1", out)
        self.assertEqual(path.read_text(), source)

        code, diff, _err = self.run_cli("--diff", str(path))
        self.assertEqual(code, 1)
        self.assertIn("--- a/", diff)
        self.assertEqual(path.read_text(), source)

        code, _out, _err = self.run_cli("--write", str(path))
        self.assertEqual(code, 0)
        written = path.read_text()
        self.assertIn("if (x) \n       return;", written)

        code, out, _err = self.run_cli(str(path))
        self.assertEqual(code, 0)
        self.assertIn("Arrangement violations: 0", out)

        code, _out, _err = self.run_cli("--write", str(path))
        self.assertEqual(code, 0)
        self.assertEqual(path.read_text(), written)

    def test_baseline_ratchet_only_fails_on_new_debt(self):
        path = self.write("src/subject.c",
                          "void f(void) {\n"
                          "    int a = 1;\n"
                          "    int b = 2;\n"
                          "}\n")
        baseline_path = self.root / "arrange-baseline.json"
        code, _out, _err = self.run_cli(
            "--write-baseline", str(baseline_path), str(self.root))
        self.assertEqual(code, 0)
        data = json.loads(baseline_path.read_text())
        self.assertEqual(data["total"], 1)

        code, out, _err = self.run_cli(
            "--baseline", str(baseline_path), str(self.root))
        self.assertEqual(code, 0)
        self.assertIn("0 new", out)

        path.write_text("void f(void) {\n"
                        "    int a = 1;\n"
                        "    int b = 2;\n"
                        "    int c = 3;\n"
                        "}\n")
        code, out, _err = self.run_cli(
            "--baseline", str(baseline_path), str(self.root))
        self.assertEqual(code, 1)
        self.assertIn("1 new", out)

    def test_baseline_keys_use_forward_slashes(self):
        self.write("src/subject.c",
                   "void f(void) {\n"
                   "    int a = 1;\n"
                   "    int b = 2;\n"
                   "}\n")
        baseline_path = self.root / "arrange-baseline.json"
        code, _out, _err = self.run_cli(
            "--write-baseline", str(baseline_path), str(self.root))
        self.assertEqual(code, 0)
        data = json.loads(baseline_path.read_text())
        self.assertTrue(data["counts"])
        for key in data["counts"]:
            self.assertNotIn("\\", key)
            self.assertIn("/", key)

    def test_json_report_shape(self):
        self.write("src/subject.c",
                   "void f(int x) {\n"
                   "    if (x) return;\n"
                   "}\n")
        code, out, _err = self.run_cli("--json", str(self.root))
        self.assertEqual(code, 1)
        report = json.loads(out)
        self.assertEqual(report["total"], 1)
        self.assertEqual(report["violations"][0]["msg"],
                         "body on the header line")


if __name__ == "__main__":
    unittest.main()
