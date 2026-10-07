"""Owner contract checks for the new local lawbook and Rust file organization."""
from pathlib import Path
import re

WORKSPACE = Path(__file__).resolve().parents[2]
ROOT = WORKSPACE / "personal/relational-engine"
prefs = (ROOT / "relational-engine-preferences.md").read_text()
for title in ("Resident Storage Boundary Law", "Cross-Language Atomic Access Law",
              "One Rust Class Per File Law", "CamelCase Rust Constructor Macro Law"):
    assert f"### {title}" in prefs
assert "4132a6c45cb6d3797c3e8eff2e94035a" in prefs
assert "record-schema migration" in prefs and "not" in prefs
constitution = (WORKSPACE / "preferences.md").read_text()
assert "personal/relational-engine/relational-engine-preferences.md" in constitution
assert "No C/Rust atomic-layout compatibility is assumed" in constitution
for path in (ROOT / "rust/src").rglob("*.rs"):
    if path.name == "helloworld.rs":
        continue  # Existing unrelated user scratch file, not engine module closure.
    declarations = re.findall(r"^\s*(?:pub(?:\([^)]*\))?\s+)?(?:struct|enum)\s+\w+", path.read_text(), re.M)
    assert len(declarations) <= 1, (path, declarations)
assert "get_atomic_byte" in (ROOT / "rust/README.md").read_text()
assert "atomic_string_owner" in (WORKSPACE / "tests/relational-engine/README.md").read_text()
vex_prefs = (WORKSPACE / "ecosystem/repos/vexspoke/vexspoke-preferences.md").read_text()
assert "relational_engine/memory.h" in vex_prefs
assert "actual Hotcwap reload integration remains unproved" in vex_prefs
assert "relational-engine-preferences.md" in (ROOT / "README.md").read_text()
lib = (ROOT / "rust/src/lib.rs").read_text()
assert "macro_rules! Memory" in lib and "macro_rules! Bytes" in lib
print("PASS: resident/atomic contracts, reading map and one named Rust type per file")
