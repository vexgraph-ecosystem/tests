#!/usr/bin/env python3
# tests/tools/vexspoke_coverage_ratchet.py
#
# The Per-File Battle Test Law, enforced: every production file unit compiled
# into libvexspoke must have a dedicated owner test (tests/vexspoke/<dir>/
# <unit>_test.c). The baseline records the units that are knowingly still
# uncovered; it may only shrink. A unit that loses its owner, or a new unit
# shipped without one, fails this check.
#
# Read-only: parses the owning repo's CMakeLists and the test tree; writes
# nothing. Exit 0 = no new gaps, 1 = a regression, 2 = setup error.

import os
import re
import sys
import glob

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))          # tests/
REPO = os.path.dirname(ROOT)                                                # vexgraph/
CMAKE = os.path.join(REPO, "ecosystem/vexspoke/CMakeLists.txt")
TESTS = os.path.join(ROOT, "vexspoke")
BASELINE = os.path.join(TESTS, "coverage_baseline.txt")

def compiled_units():
    """Every src/**/*.c named in the owning repo's CMakeLists that exists on disk."""
    text = open(CMAKE, encoding="utf-8").read()
    units = set(re.findall(r"(src/[A-Za-z0-9_./]+\.c)", text))
    base = os.path.join(REPO, "ecosystem/vexspoke")
    return sorted(u for u in units if os.path.exists(os.path.join(base, u)))

def owner_test_exists(unit):
    name = os.path.basename(unit)[:-2]              # e.g. mem
    hits = glob.glob(os.path.join(TESTS, "**", name + "_test.c"), recursive=True)
    return bool(hits)

def baseline():
    if not os.path.exists(BASELINE):
        return None
    known = set()
    for line in open(BASELINE, encoding="utf-8"):
        line = line.strip()
        if line and not line.startswith("#"):
            known.add(line)
    return known

def main():
    if not os.path.exists(CMAKE):
        print("coverage_ratchet: cannot find", CMAKE)
        return 2
    units = compiled_units()
    uncovered = [u for u in units if not owner_test_exists(u)]
    known = baseline()
    if known is None:
        print("coverage_ratchet: no baseline at", BASELINE)
        return 2

    new_gaps = [u for u in uncovered if u not in known]
    covered_now = [u for u in units if owner_test_exists(u)]

    print("vexspoke coverage ratchet")
    print("  compiled units :", len(units))
    print("  owned          :", len(covered_now))
    print("  uncovered      :", len(uncovered), "(baseline", len(known), ")")

    if new_gaps:
        print("\nREGRESSION — unit(s) shipped without an owner test:")
        for u in new_gaps:
            print("  MISSING", u)
        return 1

    stale = sorted(known - set(uncovered))
    if stale:
        print("\nnote: baseline lists now-covered units (shrink the baseline):")
        for u in stale:
            print("  COVERED", u)
    print("\nOK — no new coverage gaps")
    return 0

if __name__ == "__main__":
    sys.exit(main())
