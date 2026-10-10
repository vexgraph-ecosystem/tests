"""Owner: Func decoder + launcher. Native artifacts, hostile inputs and recovery.

This is literal arithmetic compiler proof, not general orchestration, arbitrary
opcode registry, concurrency or all-platform readiness. ASan/UBSan covers decoder
growth/parse/free, not the Python runtime or compiler internals.
"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import runpy
import unittest

ROOT = Path(__file__).resolve().parents[3]
FUNC = ROOT / "personal/func"


class FuncTest(unittest.TestCase):
    def setUp(self):
        self.home = tempfile.TemporaryDirectory(prefix="func-owner-")
        self.addCleanup(self.home.cleanup)
        self.path = Path(self.home.name)
        self.source = self.path / "input with spaces.func"
        self.output = self.path / "artifact ; literal"

    def build(self, text, syntax="surface", extra=()):
        self.source.write_bytes(text if isinstance(text, bytes) else text.encode())
        return subprocess.run([sys.executable, str(FUNC / "func"), "build", str(self.source),
                               "--syntax", syntax, "-o", str(self.output), *extra],
                              capture_output=True, timeout=30)

    def artifact(self):
        return subprocess.run([str(self.output)], capture_output=True, timeout=5)

    def test_both_forms_native_literals_order_boundaries_and_no_implicit_run(self):
        for syntax, text in [("surface", "add(34,7); mul(-2,3); print(-9223372036854775808); print(9223372036854775807)"),
                             ("numeric", "1,34,7,2,-2,3,3,-9223372036854775808,3,9223372036854775807")]:
            result = self.build(text, syntax)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, b"")
            self.assertEqual(result.stdout, str(self.output).encode() + b"\n")
            native = self.artifact()
            self.assertEqual(native.returncode, 0)
            self.assertEqual(native.stdout, b"41\n-6\n-9223372036854775808\n9223372036854775807\n")
            self.assertEqual(native.stderr, b"")

    def test_malformed_inputs_preserve_previous_artifact_and_recover(self):
        self.assertEqual(self.build("add(34,7)").returncode, 0)
        prior = self.output.read_bytes()
        cases = [("surface", text) for text in ["", "add()", "add(1)", "add(1,2,3)", "mul(1,)", "unknown(1)",
            "print(9223372036854775808)", "print(-9223372036854775809)", "print(0x12)", "print(1) garbage", "add(1,2);;", b"print(1)\0print(2)"]]
        cases += [("numeric", text) for text in ["0,1", "4,1", "1,1", "1,1,2,", "1,1,2, \n", "1,1,2,3", "3", "3,1,1", "1.5,1,2"]]
        for syntax, text in cases:
            with self.subTest(syntax=syntax, text=text):
                result = self.build(text, syntax)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(b"[vex]", result.stderr)
                self.assertEqual(self.output.read_bytes(), prior)
        self.assertEqual(self.build("mul(6,7)").returncode, 0)
        self.assertEqual(self.artifact().stdout, b"42\n")
        self.assertEqual(list(self.path.glob(".func-*")), [])

    def test_growth_native_overflow_and_compiler_failure(self):
        result = self.build(";".join(f"add({i},1)" for i in range(257)))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.artifact().stdout.decode().splitlines(), [str(i + 1) for i in range(257)])
        for source in ["add(9223372036854775807,1)", "mul(-9223372036854775808,-1)"]:
            self.assertEqual(self.build(source).returncode, 0)
            result = self.artifact()
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(b"overflow", result.stderr)
        prior = self.output.read_bytes()
        result = self.build("print(0)", extra=["--compiler", "/usr/bin/false"])
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(self.output.read_bytes(), prior)
        for value in ("0", "nan", "inf", "-1"):
            self.assertNotEqual(self.build("print(0)", extra=["--timeout", value]).returncode, 0)

    def test_source_limit_symlink_rejection_and_decoder_sanitizers(self):
        result = self.build(b" " * (1024 * 1024 + 1))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn(b"safety", result.stderr)
        victim = self.path / "victim"
        victim.write_bytes(b"unchanged")
        self.output.symlink_to(victim)
        self.assertNotEqual(self.build("print(1)").returncode, 0)
        self.assertEqual(victim.read_bytes(), b"unchanged")
        decoder = self.path / "decoder"
        command = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2",
                   "-fsanitize=address,undefined", "-fno-omit-frame-pointer", str(FUNC / "src/func.c"), "-o", str(decoder)]
        subprocess.run(command, check=True, capture_output=True, timeout=30)
        for syntax, text, success in [("surface", ";".join("add(1,2)" for _ in range(1025)).encode(), True),
                                     ("numeric", b"1,", False), ("surface", b"add(1,2,3)", False),
                                     ("surface", b"print(0)\0", False), ("numeric", b"1,1,2, ", False)]:
            result = subprocess.run([str(decoder), syntax], input=text, capture_output=True, timeout=5,
                                    env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
            self.assertEqual(result.returncode == 0, success, result.stderr)
            self.assertNotIn(b"AddressSanitizer", result.stderr)
            self.assertNotIn(b"runtime error:", result.stderr)

    def test_launcher_whole_stage_timeout_output_cap_and_recovery(self):
        child = runpy.run_path(str(FUNC / "func"))["child"]
        for source, kwargs, diagnostic in [("import time; time.sleep(30)", {"timeout": 0.1}, "deadline"),
                                           ("import sys; sys.stdout.write('x'*1000)", {"timeout": 5, "cap": 100}, "truncated"),
                                           ("import sys; sys.stderr.write('bad')", {"timeout": 5}, "diagnostics")]:
            with self.assertRaisesRegex(ValueError, diagnostic):
                child([sys.executable, "-c", source], **kwargs)
            self.assertEqual(child([sys.executable, "-c", "print('recovered')"], 5), b"recovered\n")


if __name__ == "__main__":
    unittest.main()
