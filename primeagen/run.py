#!/usr/bin/env python3
"""Registered Primeagen offline owners; each file has its own bounded child."""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
failed = False
for test in sorted(ROOT.rglob("*_test.py")):
    result = subprocess.run([sys.executable, "-B", str(test)], timeout=240)
    failed |= result.returncode not in (0, 77)  # explicit live/platform skips are not passes
raise SystemExit(1 if failed else 0)
