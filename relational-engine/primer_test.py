"""Offline primer owner: checks documented boundaries, not runtime readiness.

Run independently: python3 -B tests/relational-engine/primer_test.py.
Source assertions pin the documented identity warnings to the reviewed code;
if behavior changes, review the warnings rather than blindly updating this test.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
ENGINE = ROOT / "ecosystem/repos/relational-engine"


class PrimerTest(unittest.TestCase):
    def setUp(self):
        self.doc = (ENGINE / "docs/relational-engine-primer.md").read_text()

    def test_readiness_and_roadmap_are_scoped(self):
        for note in (
            "no whole-engine production-readiness claim",
            "Repair typed identity",
            "Choose the first consumer and its contract",
            "Expose only the required C surface",
            "Mapping is a choice, not a readiness badge",
            "Rust is here (it does not have to be)",
            "No runtime\nsuite was rerun",
            "C-client ASan/UBSan does not instrument Rust",
        ):
            self.assertIn(note, self.doc)
        self.assertEqual(self.doc.count("```") % 2, 0)

    def test_identity_warnings_match_reviewed_source(self):
        chunk = (ENGINE / "rust/src/nio/typed_chunk.rs").read_text()
        pool = (ENGINE / "rust/src/struct/typed_pool.rs").read_text()
        handle = (ENGINE / "rust/src/nio/handle.rs").read_text()
        self.assertIn("generations.resize(capacity, 0u32)", chunk)
        self.assertIn("wrapping_add(1)", chunk)
        self.assertIn("Self { index: 0, generation: 0 }", handle)
        self.assertIn("*entry = None", pool)
        self.assertIn("let mut chunk = TypedChunk::new(self.rows_per_chunk)?", pool)
        for note in ("first insertion", "wraps back to one",
                     "discards generation metadata", "no owner identity"):
            self.assertIn(note, self.doc)

    def test_c_example_and_ownership_limits(self):
        header = (ENGINE / "rust/include/relational_engine/memory.h").read_text()
        for name in ("re_memory_new", "re_memory_copy", "re_memory_read"):
            self.assertIn(name, header)
            self.assertIn(name, self.doc)
        self.assertIn("new may abort on allocator OOM", header)
        for note in ("can abort on OOM", "do **not** implement",
                     "does not own, type-check or keep", "Integers still have byte order"):
            self.assertIn(note, " ".join(self.doc.split()))


if __name__ == "__main__":
    unittest.main()
