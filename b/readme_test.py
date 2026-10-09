"""Offline documentation contracts; no claim of GUI or cross-platform proof."""
from pathlib import Path
import re
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[2] / "personal/b"


class ReadmeTest(unittest.TestCase):
    def test_build_breeze_box_artwork_precedes_title(self):
        text = (ROOT / "README.md").read_text()
        self.assertTrue(text.startswith('<p align="center">\n'))
        self.assertIn('src="https://raw.githubusercontent.com/vex-graph/vex-graph/main/resources/b.png"', text)
        self.assertIn('alt="build, breeze, box!"', text)
        self.assertLess(text.index("resources/b.png"), text.index("# b\n"))

    def test_command_tree_link_names_and_honest_modes(self):
        readme = (ROOT / "README.md").read_text()
        self.assertIn("[command tree and examples](TREE.md)", readme)
        text = (ROOT / "TREE.md").read_text()
        for phrase in ("build <adapter>", "exec <file>", "instance <file>",
                       "upload", "--port <port>", "--fqbn <matching-board>",
                        "PLANNED", "not implemented", "tools/workspace.c",
                       "B_SQL_DATABASE", "no default database", "not a sandbox",
                       "not type-checking", "file suffix", "./tools/b targets",
                       "[Back to README](README.md)", "b run exec ./hello.c"):
            self.assertIn(phrase, text)
        for source in (ROOT / "adapters").glob("*.c"):
            for name in re.findall(r'const Adapter \w+ = \{\s*"([^"]+)"', source.read_text()):
                self.assertIn(f"`{name}`", text, f"Missing adapter name from {source.name}")
        for command in ("build", "run", "test", "check", "coverage", "list", "targets",
                        "ide", "watch", "cc", "clean", "doctor"):
            self.assertRegex(text, rf"(?m)^[├└]── {command}\b")

    def test_readme_identity_usage_and_honest_scope(self):
        text = (ROOT / "README.md").read_text()
        for phrase in ("general-purpose, language-agnostic build system", "Tsoding",
                       "nob", "git clone https://github.com/vex-graph/b.git",
                        "b run <exec|instance>", "b build <adapter>",
                       "b export <manifestmainfile>", "JETBRAINS.md",
                       "No export format or manifest schema is implemented yet",
                       "parse-only", "exactly one", "SDK 10+", "main.rs",
                         "../../tests/b/cli_test.py", "../../preferences.md",
                         "https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a"):
            self.assertIn(phrase, text)
        adapters = (ROOT / "ADAPTERS.md").read_text()
        for language in ("C /", "Java /", "Python /", "Rust /", "C# /", "R /", "Arduino /",
                          "Swift /", "Objective-C /", "JavaScript /", "TypeScript /", "PHP /", "SQL /", "C++ /", "POSIX shell /"):
            self.assertIn(language, adapters)
        self.assertIn("b build cmake", text)
        self.assertIn("orchestrator on top", text)
        self.assertIn(";;OVERVIEW", text)
        for claim in ("Build, breeze, box", "B_SQL_DATABASE", "not a sandbox",
                       "not type-checking", "b build npm", "no default database"):
            self.assertIn(claim.lower(), (text + adapters).lower())

    def test_readme_outline_and_alphabetical_languages(self):
        """Keep the requested overview ordered and its language inventory alphabetical."""
        text = (ROOT / "README.md").read_text()
        headings = re.findall(r"^## (.+)$", text, re.M)
        self.assertEqual(headings, [
            "Disclaimer: CMake is just IDE metadata (not irony)", "Current State",
            "What does it do?", "List of languages", "Tree", "JetBrains IDEs",
            "Adapters", "Actual dogfooding across Vexgraph",
            "Future and b's own build", "Scope and Limitations",
        ])
        table = text.split("## List of languages\n", 1)[1].split("## Tree\n", 1)[0]
        names = re.findall(r"^\| ([^|]+?) \|", table, re.M)[2:]
        self.assertEqual(names, sorted(names, key=str.casefold))
        self.assertEqual(len(names), 20)
        self.assertIn("[ADAPTERS.md](ADAPTERS.md)", text)
        self.assertNotIn("deliberately experimental", text)

    def test_shader_inventory_matches_project_owned_implementation(self):
        """Describe real GLSL generators without inventing standalone shader dispatch."""
        readme = (ROOT / "README.md").read_text()
        adapters = (ROOT / "ADAPTERS.md").read_text()
        workspace = (ROOT.parents[1] / "tools/workspace.c").read_text()
        registry = (ROOT / "adapters/adapter.c").read_text()
        launcher = (ROOT.parents[1] / "tools/b").read_text()
        for text in (readme, adapters):
            for phrase in ("GLSL / SPIR-V", ".vert", ".frag", ".comp", ".glsl", ".spv",
                           "glslangValidator -V", "workspace", "standalone"):
                self.assertIn(phrase, text)
        self.assertIn("not discovered by this graph", adapters)
        self.assertIn("There is no `b build glsl`", adapters)
        self.assertNotIn("GLSL_ADAPTER", registry)
        self.assertIn('strl_push(&g, "glslangValidator")', workspace)
        self.assertIn('strl_push(&g, "-V")', workspace)
        self.assertIn('"quad.vert", "quad.frag"', workspace)
        self.assertIn('run exec "$root/tools/workspace.c"', launcher)

    def test_adapter_reference_covers_registered_names_and_safety(self):
        """Retain native adapter details and explicit deployment/trust boundaries."""
        text = (ROOT / "ADAPTERS.md").read_text()
        table = text.split("| Adapter / CLI name", 1)[1].split("Project backends", 1)[0]
        names = re.findall(r"^\| ([^|]+?) \|", table, re.M)[1:]
        self.assertEqual(names, sorted(names, key=str.casefold))
        for source in (ROOT / "adapters").glob("*.c"):
            for name in re.findall(r'const Adapter \w+ = \{\s*"([^"]+)"', source.read_text()):
                self.assertIn(f"`{name}`", text)
        for phrase in ('// b_build("arduino:avr:uno")', "--port", "--fqbn",
                       "B_SQL_DATABASE", "no default database", "not a sandbox",
                       "--offline", "not type-checking", "parse-only", "SDK 10+",
                       "main.rs", "exactly one", ".csproj"):
            self.assertIn(phrase.lower(), text.lower())

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
