"""Documentation proof for b's general build-system identity and honest scope."""

from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[2]


class ReadmeTest(unittest.TestCase):
    def test_identity_command_contract_and_adapter_gaps(self):
        text = (ROOT / "b/README.md").read_text()
        self.assertIn("general-purpose, language-agnostic build system", text)
        self.assertIn("C is the implementation language, not a restriction", text)
        for command in ("b run <exec|instance>", "b build <language>", "b export <manifestmainfile>"):
            self.assertIn(command, text)
        for language in ("C++", "Rust", "C#", "Java", "JavaScript", "TypeScript", "HTML"):
            self.assertIn(language, text)
        for comparison in ("nob", "IntelliJ", "Maven", "Gradle", "vexgraph"):
            self.assertNotIn(comparison, text)
        self.assertIn("No export format or manifest schema is implemented yet", text)
        self.assertIn("serving and web builds are **not implemented yet**", text)
        self.assertIn("../tests/b/cli_test.py", text)
        self.assertIn("compatibility adapter, not b's general project model", text)


if __name__ == "__main__":
    unittest.main(verbosity=2)
