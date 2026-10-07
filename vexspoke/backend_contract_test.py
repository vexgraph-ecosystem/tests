"""Documentation and unchanged-copy proof for the temporary C backend.

This is not allocator, I/O, reflection or relational behavioral proof; registered
C owner tests establish those separately. No Rust delegation is implemented.
"""
from pathlib import Path
import hashlib
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[2]
VEX = ROOT / "ecosystem/repos/vexspoke"
ENGINE = ROOT / "ecosystem/repos/relational-engine"


class BackendContractTest(unittest.TestCase):
    def test_restored_reference_bytes_are_unchanged(self):
        count = 0
        for part in ("nio", "io", "relational", "reflection"):
            for reference in sorted((ENGINE / "src" / part).iterdir()):
                if reference.suffix not in (".c", ".h"):
                    continue
                production = VEX / "src" / part / reference.name
                self.assertTrue(production.is_file(), str(production))
                self.assertEqual(hashlib.sha256(reference.read_bytes()).digest(),
                                 hashlib.sha256(production.read_bytes()).digest(), str(production))
                count += 1
        self.assertEqual(count, 49)

    def test_documented_backend_is_not_a_rust_wrapper_claim(self):
        text = (VEX / "BACKEND.md").read_text()
        for phrase in ("restored unchanged", "no allocator", "not a default",
                        "without a dependency", "./tools/b build"):
            self.assertIn(phrase, text)
        result = subprocess.run(["git", "check-ignore", "--no-index", "BACKEND.md"],
                                cwd=VEX, capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 1, result.stderr.decode())

    def test_optional_engine_boundary_is_not_default_migration(self):
        content = (VEX / "BACKEND.md").read_text()
        header = (VEX / "src/nio/relational_memory.h").read_text()
        self.assertIn("partial storage C ABI", content)
        self.assertIn("staged", content)
        self.assertIn("R1 owns", content)
        self.assertIn("automatic schema migration", content)
        self.assertIn('relational_engine/memory.h', header)
        self.assertIn("NOT the default", header)


if __name__ == "__main__":
    unittest.main()
