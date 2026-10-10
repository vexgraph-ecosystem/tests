"""Independent mechanical Func competency review, grounded in current sources."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
FUNC = ROOT / "personal/func"


class ReadmeTest(unittest.TestCase):
    def test_documented_initial_slice_matches_native_and_launcher_sources(self):
        readme = (FUNC / "README.md").read_text()
        source = (FUNC / "src/func.c").read_text()
        launcher = (FUNC / "func").read_text()
        for claim in ("## Current State", "## Scope and Limitations", "not production-ready", "1=add", "2=mul", "3=print", "literal", "never runs", "Python"):
            self.assertIn(claim, readme)
        for token in ("#define ADD_ID 1", "#define MUL_ID 2", "#define PRINT_ID 3", "__builtin_%s_overflow", "MODULE: Func", "PRIVATE HELPERS", "FUNCTION REGISTRY:"):
            self.assertIn(token, source)
        self.assertIn("os.replace(artifact, output)", launcher)
        self.assertNotIn("[str(artifact)]", launcher)
        self.assertNotIn("shell=True", launcher)
        self.assertIn("selector", launcher)
        self.assertFalse((FUNC / "CMakeLists.txt").exists())
        self.assertIn("forward", (FUNC / "CLASSES.md").read_text().lower())


if __name__ == "__main__":
    unittest.main()
