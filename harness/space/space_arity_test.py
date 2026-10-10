"""Arity and Variadic Dispatch Law owner for the harness space values.

Compiles real clients against the four class headers and proves:
  - every advertised constructor form (0-arg and the named N-arg) compiles;
  - the ``Class(...)`` chooser selects the right arity;
  - an unsupported argument count names an undeclared ``Class_N`` and FAILS to
    compile for that intended diagnostic (not for an unrelated error);
  - wrong argument types are rejected.
No runtime, network or interactive launch is involved.
"""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "ecosystem/repos/harness"
VEX = ROOT / "ecosystem/repos/vexspoke/src"
REL = ROOT / "ecosystem/repos/relational-engine/src"
CC = "clang"
INCLUDES = [f"-I{APP / 'src'}", f"-I{VEX}", f"-I{ROOT / 'tests'}", f"-I{REL}"]


def syntax_check(body):
    """Return (ok, diagnostics) for a translation unit compiled with -fsyntax-only."""
    source = '#include "space/model_user.h"\n#include "space/channel.h"\n' \
             '#include "space/message.h"\n#include "space/task.h"\n' + body
    with tempfile.TemporaryDirectory() as d:
        path = Path(d) / "probe.c"
        path.write_text(source)
        result = subprocess.run(
            [CC, "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-fsyntax-only", *INCLUDES, str(path)],
            capture_output=True, text=True, timeout=60)
        return result.returncode == 0, result.stderr


class ArityCompileTest(unittest.TestCase):
    def test_supported_constructor_forms_compile(self):
        positive = (
            "int main(void){"
            " ModelUser a = ModelUser();"
            " ModelUser b = ModelUser(1,\"vex\",2);"
            " ModelUser c = ModelUser_3(1,\"vex\",2);"
            " Channel d = Channel(); Channel e = Channel(1,2); Channel f = Channel_2(1,2);"
            " Message g = Message(); Message h = Message(1,2,0,0,\"x\",1);"
            " Message i = Message_6(1,2,0,0,\"x\",1);"
            " Task j = Task(); Task k = Task(1,2,0,3,1); Task l = Task_5(1,2,0,3,1);"
            " return (int)(a.id+b.id+c.id+d.id+e.id+f.id+g.id+h.id+i.id+j.id+k.id+l.id); }"
        )
        ok, diag = syntax_check(positive)
        self.assertTrue(ok, diag)

    def test_unsupported_arity_names_the_declared_symbol(self):
        # (call, the undeclared Class_N the chooser must select)
        cases = [
            ("(void) ModelUser(1,2);", "ModelUser_2"),
            ("(void) ModelUser(1,2,3,4);", "ModelUser_4"),
            ("(void) Channel(1);", "Channel_1"),
            ("(void) Channel(1,2,3);", "Channel_3"),
            ("(void) Message(1,2,3);", "Message_3"),
            ("(void) Message(1,2,3,4,5);", "Message_5"),
            ("(void) Task(1,2,3);", "Task_3"),
            ("(void) Task(1,2,3,4,5,6);", "Task_6"),
        ]
        for expr, symbol in cases:
            with self.subTest(expr=expr):
                ok, diag = syntax_check(f"int main(void){{ {expr} return 0; }}")
                self.assertFalse(ok, f"{expr} unexpectedly compiled")
                self.assertIn(symbol, diag, diag)

    def test_wrong_argument_types_are_rejected(self):
        for expr in ("(void) ModelUser(1, 2, 3);",          # int where const char* is required
                     "(void) Channel(\"x\", 2);"):          # string where uint64_t is required
            with self.subTest(expr=expr):
                ok, _ = syntax_check(f"int main(void){{ {expr} return 0; }}")
                self.assertFalse(ok, f"{expr} unexpectedly compiled")


if __name__ == "__main__":
    unittest.main()
