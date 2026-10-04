"""CLI owner proof: public grammar, process status, source builds and rejection.

Temporary projects use real compilers; every child has an external timeout.
Java needs an installed JDK; other platforms and instance/export remain gaps.
"""

import os
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2] / "b"


class CliTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.scratch = tempfile.TemporaryDirectory(prefix="b-cli-")
        cls.addClassCleanup(cls.scratch.cleanup)
        cls.home = Path(cls.scratch.name)
        cls.environment = dict(os.environ, B_HOME=str(cls.home / "state"))
        subprocess.run([str(ROOT / "b"), "--help"], env=cls.environment,
                       check=True, capture_output=True, timeout=60)

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="project ", dir=self.home)
        self.addCleanup(self.temp.cleanup)
        self.project = Path(self.temp.name)

    def invoke(self, *arguments, expected=0, cwd=None, environment=None):
        result = subprocess.run([str(ROOT / "b"), *map(str, arguments)],
                                cwd=cwd or self.project,
                                env=environment or self.environment,
                                capture_output=True, text=True, timeout=60)
        self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
        return result

    def source(self, name="hello.c", text='int main(void) { return 0; }\n'):
        path = self.project / name
        path.write_text(text)
        return path

    def require_java(self):
        try:
            result = subprocess.run(["javac", "-version"], capture_output=True,
                                    text=True, timeout=15)
        except (OSError, subprocess.TimeoutExpired):
            self.skipTest("installed JDK unavailable")
        if result.returncode != 0:
            self.skipTest("installed JDK unavailable")

    def test_help_and_invalid_grammar(self):
        help_text = self.invoke("--help").stdout
        for command in ("b run", "b build", "b export", "b java"):
            self.assertIn(command, help_text)
            self.assertIn(command, (ROOT / "README.md").read_text())
        for arguments in (("nonsense",), ("build",), ("run", "exec"),
                          ("build", "c", ".", "extra"), ("java", "hello.c")):
            self.assertIn("[vex]", self.invoke(*arguments, expected=1).stderr)

    def test_c_source_run_arguments_and_exit_status(self):
        self.source("hello with spaces.c", '#include <stdio.h>\n'
                    'int main(int argc, char **argv) {\n'
                    '    for (int i = 1; i < argc; ++i) puts(argv[i]);\n'
                    '    return 7;\n}\n')
        arguments = ["a space", "$(touch NEVER)", ";", *map(str, range(10))]
        result = self.invoke("run", "exec", "hello with spaces.c", "--", *arguments, expected=7)
        self.assertEqual(result.stdout.splitlines(), arguments)
        self.assertFalse((self.project / "NEVER").exists())
        self.assertEqual(result.stderr, "")

    def test_build_current_directory_and_existing_exec(self):
        self.source(text='int main(void) { return 9; }\n')
        output = Path(self.invoke("build", "c").stdout.strip())
        self.assertTrue(output.is_file())
        self.assertTrue(output.is_relative_to(self.home / "state"))
        self.invoke("run", "exec", output, expected=9)
        self.assertEqual(list(self.project.iterdir()), [self.project / "hello.c"])

    def test_build_multiple_sources_and_include_directory(self):
        self.source("main.c", '#include "value.h"\nint main(void) { return value(); }\n')
        self.source("value.c", 'int value(void) { return 11; }\n')
        self.source("value.h", 'int value(void);\n')
        output = Path(self.invoke("build", "c", self.project).stdout.strip())
        self.invoke("run", "exec", output, expected=11)

    def test_directory_glob_characters_are_literal(self):
        unusual = self.project / "literal [project]"
        unusual.mkdir()
        (unusual / "main.c").write_text('int main(void) { return 0; }\n')
        output = Path(self.invoke("build", "c", unusual).stdout.strip())
        self.invoke("run", "exec", output)

    def test_empty_missing_and_non_executable_inputs(self):
        self.invoke("build", "c", expected=1)
        self.invoke("build", "c", "missing", expected=1)
        self.invoke("build", "rust", expected=1)
        self.invoke("run", "exec", "missing.c", expected=1)
        text = self.source("plain.txt", "not executable\n")
        self.invoke("run", "exec", text, expected=1)
        self.invoke("run", "exec", self.project, expected=1)

    def test_compiler_failure_never_runs_previous_result(self):
        source = self.source(text='int main(void) { return 23; }\n')
        self.invoke("run", "exec", source, expected=23)
        source.write_text("not C code\n")
        self.invoke("run", "exec", source, expected=1)

    def test_missing_tool_is_reported(self):
        self.source()
        environment = dict(self.environment, CC="missing-b-test-compiler")
        result = self.invoke("build", "c", environment=environment, expected=1)
        self.assertIn("cannot launch", result.stderr)

    def test_instance_and_export_reject_without_side_effects(self):
        source = self.source(text='int main(void) { return 23; }\n')
        self.invoke("run", "instance", source, expected=1)
        manifest = self.project / "manifest.json"
        manifest.write_text("{}\n")
        for kind in ("exe", "app", "msi", "iso", "zip", "invalid"):
            destination = self.project / f"export.{kind}"
            self.invoke("export", manifest, destination, kind, expected=1)
            self.assertFalse(destination.exists())

    def test_java_source_alias_and_explicit_run(self):
        self.require_java()
        self.source("Hello.java", 'class Hello { public static void main(String[] args) {'
                    'System.out.println(args[0]); System.exit(6); } }\n')
        for arguments in (("java", "Hello.java"), ("run", "exec", "Hello.java")):
            result = self.invoke(*arguments, "--", "hello world", expected=6)
            self.assertEqual(result.stdout.strip(), "hello world")

    def test_java_directory_build(self):
        self.require_java()
        self.source("Hello.java", 'class Hello { public static void main(String[] args) {} }\n')
        output = Path(self.invoke("build", "java").stdout.strip())
        self.assertTrue((output / "Hello.class").is_file())


if __name__ == "__main__":
    unittest.main(verbosity=2)
