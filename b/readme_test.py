"""Offline documentation contracts; no claim of GUI or cross-platform proof."""
from pathlib import Path
import re
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[2] / "b"


class ReadmeTest(unittest.TestCase):
    def test_readme_identity_usage_and_honest_scope(self):
        text = (ROOT / "README.md").read_text()
        for phrase in ("general-purpose, language-agnostic build system", "Tsoding",
                       "nob", "git clone https://github.com/vex-graph/b.git",
                       "b run <exec|instance>", "b build <language>",
                       "b export <manifestmainfile>", "JETBRAINS.md",
                       "No export format or manifest schema is implemented yet",
                       "parse-only", "exactly one", "SDK 10+", "main.rs",
                       "../tests/b/cli_test.py", "ecosystem/vexspoke/preferences.md"):
            self.assertIn(phrase, text)
        for language in ("C /", "Java /", "Python /", "Rust /", "C# /", "R /", "Arduino /",
                         "Swift /", "Objective-C /", "JavaScript /", "TypeScript /", "PHP /", "SQL /", "C++ /", "POSIX shell /"):
            self.assertIn(language, text)
        self.assertIn("b build cmake", text)
        self.assertIn("orchestrator on top", text)
        self.assertIn(";;OVERVIEW", text)
        for claim in ("Build, breeze, box", "B_SQL_DATABASE", "not a sandbox",
                      "not type-checking", "b build npm", "no default database"):
            self.assertIn(claim.lower(), text.lower())

    def test_jetbrains_external_tool_instructions(self):
        text = (ROOT / "JETBRAINS.md").read_text()
        for phrase in ("Tools → External Tools", "Program", "Arguments", "Working directory",
                       'run exec "$FilePath$"', "$FileDir$", "$ProjectFileDir$",
                       "Keymap", "Before launch", "debugger", "PATH", "save",
                       "CLion", "IntelliJ IDEA", "Rider", "PyCharm"):
            self.assertIn(phrase, text)
        self.assertNotIn("$USER_HOME$/vexgraph", text)
        self.assertIn('upload arduino "$FilePath$" --port', text)
        self.assertIn('// b_build("arduino:avr:uno")', text)
        self.assertIn("Serial Monitor", text)
        self.assertIn("b does not add those IDE features", text)
        self.assertIn("export is still planned", text)

    def test_jetbrains_two_methods_and_xml_reference_consistency(self):
        text = (ROOT / "JETBRAINS.md").read_text()
        self.assertIn("Method 1: manual UI", text)
        self.assertIn("Method 2: inspectable XML", text)
        self.assertIn(".idea/runConfigurations/", text)
        self.assertIn("IDE-level settings", text)
        blocks = re.findall(r"```xml\n(.*?)\n```", text, re.S)
        self.assertEqual(len(blocks), 2)
        tools, run = map(ET.fromstring, blocks)
        tool = tools.find("tool")
        action = run.find(".//option[@name='ToolBeforeRunTask']")
        self.assertEqual(action.get("actionId"), f"Tool_{tools.get('name')}_{tool.get('name')}")
        options = {item.get("name"): item.get("value") for item in tool.findall("exec/option")}
        self.assertEqual(options["PARAMETERS"], 'upload arduino "$FilePath$" --port /dev/cu.YOUR_BOARD')
        self.assertEqual(options["WORKING_DIRECTORY"], "$FileDir$")
        self.assertEqual(run.find(".//option[@name='SCRIPT_TEXT']").get("value"), ":")


if __name__ == "__main__":
    unittest.main(verbosity=2)
