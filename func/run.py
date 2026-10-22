#!/usr/bin/env python3
"""Registered bounded owner runner for the initial standalone Func slice."""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
failed = False
for test in sorted(ROOT.rglob("*_test.py")):
    result = subprocess.run([sys.executable, "-B", str(test)], timeout=180)
    failed |= result.returncode != 0
raise SystemExit(1 if failed else 0)
