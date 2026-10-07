"""Offline cross-document header contracts; no GitHub rendering approval.

Header URLs refer to the existing profile artwork; only the vexgraph portion
of the compound ecosystem banner links to the author's profile. Remote asset
availability and subjective appearance remain outside this automated proof.
"""
from html.parser import HTMLParser
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
ARTWORK = "https://raw.githubusercontent.com/vex-graph/vex-graph/main/resources/"


class Header(HTMLParser):
    def __init__(self):
        super().__init__()
        self.stack = []
        self.images = []

    def handle_starttag(self, tag, attributes):
        attributes = dict(attributes)
        if tag == "img":
            self.images.append((attributes, list(self.stack)))
        else:
            self.stack.append((tag, attributes))

    def handle_endtag(self, tag):
        if not self.stack or self.stack[-1][0] != tag:
            raise AssertionError(f"Unbalanced header: {tag}")
        self.stack.pop()


def header(path):
    text = (ROOT / path).read_text()
    block, body = text.split("</p>\n\n", 1)
    parser = Header()
    parser.feed(block + "</p>")
    parser.close()
    if parser.stack:
        raise AssertionError("Header tags were not closed")
    return parser.images, body


class ArtworkHeadersTest(unittest.TestCase):
    def test_single_artwork_headers_preserve_document_titles(self):
        for path, filename, alt, title in (
            ("personal/b/README.md", "b.png", "build, breeze, box!", "# b\n"),
            ("preferences.md", "preferences-dot-md.png", "preferences.md",
             "# vexgraph — C23/Rust Ecosystem & Multi-Repo Preferences\n"),
        ):
            with self.subTest(document=path):
                images, body = header(path)
                self.assertEqual(images, [({"src": ARTWORK + filename, "alt": alt, "width": "800"},
                                           [("p", {"align": "center"})])])
                self.assertTrue(body.startswith(title))
                self.assertTrue((ROOT / "personal/vex-graph/resources" / filename).is_file())

    def test_ecosystem_combined_row_links_only_vexgraph(self):
        images, body = header("ecosystem/.github/profile/README.md")
        self.assertEqual(images, [
            ({"src": ARTWORK + "vexgraph.png", "alt": "vexgraph", "width": "48%"},
             [("p", {"align": "center"}), ("a", {"href": "https://github.com/vex-graph"})]),
            ({"src": ARTWORK + "ecosystem.png", "alt": "ecosystem", "width": "48%"},
             [("p", {"align": "center"})]),
        ])
        self.assertTrue(body.startswith("# vexgraph-ecosystem\n"))

    def test_profile_heading_and_subheading_still_precede_cards(self):
        text = (ROOT / "personal/vex-graph/README.md").read_text()
        self.assertIn("\n# hey! vex here! 🦊\n", text)
        self.assertIn("\n### i make stuff unseriously in java and c23, and others too in my spare time\n", text)
        self.assertLess(text.index("spare time"), text.index("personal-projects.png"))


if __name__ == "__main__":
    unittest.main(verbosity=2)
