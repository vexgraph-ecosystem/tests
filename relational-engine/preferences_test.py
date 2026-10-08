"""Owner contract checks for the new local lawbook and Rust file organization."""
from pathlib import Path
import re

WORKSPACE = Path(__file__).resolve().parents[2]
ROOT = WORKSPACE / "ecosystem/repos/relational-engine"
prefs = (ROOT / "relational-engine-preferences.md").read_text()
for title in ("Resident Storage Boundary Law", "Cross-Language Atomic Access Law",
              "One Rust Class Per File Law", "CamelCase Rust Constructor Macro Law",
              "R2 Responsibility Layout Law"):
    assert f"### {title}" in prefs
assert "4132a6c45cb6d3797c3e8eff2e94035a" in prefs
assert "record-schema migration" in prefs and "not" in prefs
constitution = (WORKSPACE / "preferences.md").read_text()
assert "ecosystem/repos/relational-engine/relational-engine-preferences.md" in constitution
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
for module in ("nio", "io", "primitives", "variable", "struct", "compress", "virtual", "ffi"):
    assert (ROOT / "rust/src" / module / "mod.rs").is_file()
    spelling = f"r#{module}" if module in ("struct", "virtual") else module
    assert f"pub mod {spelling};" in lib
assert "mmap" in prefs and "R2 storage backend alongside Vexspoke" in prefs
assert "Graphvex" in (ROOT / "rust/src/virtual/mod.rs").read_text()
assert "No codecs are implemented" in (ROOT / "rust/src/compress/mod.rs").read_text()
for part in ("search", "search/primitives"):
    assert "no" in (ROOT / "src" / part / "README.md").read_text().lower()
assert not (ROOT / "rust/src/compute").exists()
assert "### Stable Row and Variable Binding Law" in prefs
assert "borrowed VALUE" in prefs
assert "self" in prefs and "not" in prefs
for module, unit in (("nio", "chunk"), ("struct", "chunked_list"),
                     ("variable", "variable_slot"), ("variable", "variable_registry")):
    assert (ROOT / "rust/src" / module / f"{unit}.rs").is_file()
    assert (WORKSPACE / "tests/relational-engine/rust" / module / f"{unit}_test.rs").is_file()
assert "VariableRegistry" in (ROOT / "rust/README.md").read_text()
assert "eleven independent owner targets" in (WORKSPACE / "tests/relational-engine/README.md").read_text()
for module, unit in (("nio", "typed_chunk"), ("struct", "typed_pool")):
    assert (ROOT / "rust/src" / module / f"{unit}.rs").is_file()
    assert (WORKSPACE / "tests/relational-engine/rust" / module / f"{unit}_test.rs").is_file()
assert "not generation-tagged handles" in prefs
assert "Reusable typed storage" in (ROOT / "rust/README.md").read_text()
wiki = (WORKSPACE / "ecosystem/ecosystem/relational-engine.md").read_text()
assert "TypedChunk / TypedPool" in wiki and "sixteen cases" in wiki
assert "typed_chunk_test.rs" in wiki and "typed_pool_test.rs" in wiki
assert "metadata encoding unresolved" in wiki
assert "buffered readers/writers" in prefs
assert "Manifest-backed persistence remains proposed" in prefs
assert "No runtime file API" in (ROOT / "rust/src/io/mod.rs").read_text()
assert "re_name_search" in (ROOT / "src/search/primitives/README.md").read_text()
assert "BORROW" in (ROOT / "rust/include/relational_engine/variable_registry.h").read_text().upper()
print("PASS: resident/atomic contracts, reading map and one named Rust type per file")
