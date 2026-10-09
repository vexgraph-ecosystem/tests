"""Mechanical proof-law/document regression, not backend pressure execution.

This owner checks discoverability, concrete obligations and honest current gaps.
It does not turn the existence of policy text into production readiness.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
TITLE = "Deliberate Exhaustion and Backend Trust Law"


class ExhaustionLawTest(unittest.TestCase):
    def test_law_is_canonical_and_actionable(self):
        laws = (ROOT / "tests/test-preferences.md").read_text()
        self.assertEqual(laws.count("## " + TITLE + "\n"), 1)
        self.assertIn("- " + TITLE + "\n", laws)
        law = laws.split("## " + TITLE + "\n", 1)[1].split("\n## ", 1)[0]
        for requirement in ("first rejection", "inject backing/directory",
                            "refill and exhaust again", "1,024 simultaneously live",
                            "at least four workers", "caller-selectable larger",
                            "live-address non-overlap", "pairwise coverage",
                            "controlled schedules", "external watchdogs",
                            "latency/throughput distributions", "narrower scope",
                            "no finite suite proves every input"):
            self.assertIn(requirement, law)
        constitution = (ROOT / "preferences.md").read_text()
        feature = constitution.split("Feature Implementation and Adversarial Proof Law\n", 1)[1].split(
            "\n## ", 1)[0]
        self.assertIn(TITLE, feature)
        self.assertIn("Operation churn", feature)

    def test_existing_native_proof_declares_the_actual_gap(self):
        engine = ROOT / "ecosystem/repos/relational-engine"
        owner = (ROOT / "tests/relational-engine/nio/mem_exhaustion_test.c").read_text()
        worker = owner.split("static void *worker(", 1)[1].split(
            "static void concurrent(", 1)[0]
        self.assertEqual(worker.count("MemoryArena_alloc("), 1)
        self.assertIn("Memory_free(p)", worker)
        self.assertIn("WORKERS = 4", owner)
        doc = (engine / "docs/native-memory-proof.md").read_text()
        self.assertIn("at most four worker-owned blocks", doc)
        self.assertIn("Neither proves", doc)
        self.assertIn("not\nretroactively promoted", doc)


if __name__ == "__main__":
    unittest.main()
