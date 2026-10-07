"""Profile README markup/link and PNG integrity proof, not visual acceptance.

Uses the supplied artwork unchanged. No network or browser is launched; the
user owns GitHub appearance approval and external destination availability.
"""
from html.parser import HTMLParser
from pathlib import Path
import struct
import unittest
import zlib

ROOT = Path(__file__).resolve().parents[3] / "personal/vex-graph"
CARDS = [
    ("personal-projects.png", "personal projects", "https://github.com/vex-graph?tab=repositories"),
    ("ecosystem.png", "ecosystem", "https://github.com/vexgraph-ecosystem"),
    ("b.png", "b — build, breeze, box!", "https://github.com/vex-graph/b"),
    ("preferences-dot-md.png", "preferences.md", "https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a"),
]


class ProfileMarkup(HTMLParser):
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
            raise AssertionError(f"Unbalanced profile HTML: {tag}")
        self.stack.pop()


class ReadmeTest(unittest.TestCase):
    def test_centered_header_intro_and_four_linked_cards_in_order(self):
        text = (ROOT / "README.md").read_text()
        markup = ProfileMarkup()
        markup.feed(text)
        markup.close()
        self.assertEqual(markup.stack, [])
        self.assertEqual(len(markup.images), 5)
        header, parents = markup.images[0]
        self.assertEqual(header, {"src": "resources/vexgraph.png", "alt": "vexgraph", "width": "800"})
        self.assertEqual(parents, [("p", {"align": "center"})])
        self.assertIn("\n# hey! vex here! 🦊\n", text)
        self.assertIn("\n### i make stuff unseriously in java and c23, and others too in my spare time\n", text)
        self.assertLess(text.index("vexgraph.png"), text.index("hey! vex here!"))
        self.assertLess(text.index("spare time"), text.index("personal-projects.png"))
        for (image, parents), (filename, alt, href) in zip(markup.images[1:], CARDS):
            self.assertEqual(image, {"src": f"resources/{filename}", "alt": alt, "width": "23%"})
            self.assertEqual(parents, [("p", {"align": "center"}), ("a", {"href": href})])
        self.assertNotIn("<script", text.lower())
        self.assertNotIn("<style", text.lower())

    def test_all_referenced_pngs_have_valid_chunks_and_decodable_pixels(self):
        for filename in ["vexgraph.png", *(card[0] for card in CARDS)]:
            with self.subTest(image=filename):
                data = (ROOT / "resources" / filename).read_bytes()
                self.assertEqual(data[:8], b"\x89PNG\r\n\x1a\n")
                position = 8
                compressed = bytearray()
                dimensions = None
                ended = False
                while position < len(data):
                    self.assertGreaterEqual(len(data) - position, 12)
                    length = struct.unpack_from(">I", data, position)[0]
                    self.assertLessEqual(position + length + 12, len(data))
                    kind = data[position + 4:position + 8]
                    payload = data[position + 8:position + 8 + length]
                    crc = struct.unpack_from(">I", data, position + 8 + length)[0]
                    self.assertEqual(zlib.crc32(kind + payload), crc)
                    if kind == b"IHDR":
                        self.assertIsNone(dimensions)
                        dimensions = struct.unpack(">IIBBBBB", payload)
                    elif kind == b"IDAT":
                        compressed.extend(payload)
                    elif kind == b"IEND":
                        self.assertEqual(length, 0)
                        ended = True
                    position += length + 12
                    if ended:
                        break
                self.assertTrue(ended)
                self.assertEqual(position, len(data))
                self.assertIsNotNone(dimensions)
                width, height, depth, color, compression, filtering, interlace = dimensions
                self.assertEqual((width, height), (1920, 1080))
                self.assertEqual((depth, color, compression, filtering, interlace), (8, 6, 0, 0, 0))
                pixels = zlib.decompress(compressed)
                stride = width * 4 + 1
                self.assertEqual(len(pixels), height * stride)
                self.assertTrue(all(pixels[row * stride] <= 4 for row in range(height)))


if __name__ == "__main__":
    unittest.main(verbosity=2)
