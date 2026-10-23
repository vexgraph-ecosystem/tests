#!/usr/bin/env python3
"""Registered strict/sanitized Language-to-Semicolon consumer owner.

Injects bridge allocation failures only, not Language's own allocator. Exact
stderr counts are checked; this proves offline byte/pen behavior, not appearance.
"""
from pathlib import Path
import importlib.util
import os
import re
import subprocess
import tempfile
import sys

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "suite/projects/semicolon"
LANGUAGE = ROOT / "suite/ecosystem/language"
DETERMINANT = ROOT / "suite/ecosystem/determinant/src"
VEXSPOKE = ROOT / "suite/ecosystem/vexspoke/src"
ENGINE = ROOT / "suite/ecosystem/relational-engine/src"


def run(command):
    result = subprocess.run(command, capture_output=True, text=True, timeout=180)
    if result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {command}\n{result.stdout}{result.stderr}")
    return result


def main():
    spec = importlib.util.spec_from_file_location("semicolon_build", APP / "build.py")
    build = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(build)
    flags = ["-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O2", "-UNDEBUG"]
    if sys.platform == "darwin":
        flags += ["-arch", "arm64", "-mcpu=apple-m1", "-mmacosx-version-min=14.0"]
    includes = [f"-I{path}" for path in (APP, APP / "src", DETERMINANT, VEXSPOKE, ENGINE, LANGUAGE)]
    dependencies = [LANGUAGE / name for name in (
        "adapters/c/c23_highlights.c", "adapters/c/c23_lexer.c",
        "adapters/c/c23_lex_step.c", "syntax/role.c",
        "analysis/syntax_span.c", "document/source_range.c", "text/bounded_text.c")]
    dependencies += [DETERMINANT / "terminal/terminal_canvas.c"]
    with tempfile.TemporaryDirectory(prefix="semicolon-language-") as directory:
        directory = Path(directory)
        source = directory / "main.c"
        source.write_bytes(("int main(void) { return 42; } /*" + " " * 160 + "*/\n"
                            "/* start\nstill */ return 42;\n\"界\" name\n").encode()
                           + b"x\xff int k;\n")
        plain = directory / "notes.txt"
        plain.write_text("int is plain text here;\n")
        for mode, sanitizer in (("strict", []), ("asan-ubsan", ["-fsanitize=address,undefined",
                                                               "-fno-omit-frame-pointer"])):
            obj = directory / f"{mode}.o"
            run(["clang", *flags, *sanitizer, *includes,
                 "-Dmalloc=syntax_test_malloc", "-Dcalloc=syntax_test_calloc",
                 "-Drealloc=syntax_test_realloc", "-Dfree=syntax_test_free", "-c",
                 str(APP / "src/editor/semicolon_syntax.c"), "-o", str(obj)])
            binary = directory / mode
            run(["clang", *flags, *sanitizer, *includes,
                 str(ROOT / "tests/semicolon/editor/semicolon_syntax_test.c"),
                 str(obj), *(str(path) for path in dependencies), "-o", str(binary)])
            result = run([str(binary)])
            # 3 row-admission + 3 palette refusals + 2 bridge allocation stages
            # (source and line directory) + one failed owner construction.
            # Language's own allocations are exercised by its owner, not this
            # bridge's allocator hooks.
            messages = result.stderr.splitlines()
            assert all(line.startswith("[vex] semicolon_syntax.c:") for line in messages), messages
            assert sum("rejected missing" in line for line in messages) == 3, messages
            assert sum("rejected palette" in line for line in messages) == 3, messages
            assert sum("storage refused" in line for line in messages) == 2, messages
            assert sum("owner allocation refused" in line for line in messages) == 1, messages
            assert len(messages) == 9, messages
            assert not result.stdout, result.stdout
            print(f"PASS {mode} SemicolonSyntax: 2560 spans, staged failures, recovery, leak accounting")
            host = directory / f"{mode}-host"
            curses = "-lncurses" if sys.platform == "darwin" else "-lncursesw"
            run(["clang", *flags, *sanitizer, *includes,
                 str(ROOT / "tests/semicolon/editor/language_bridge_test.c"),
                 *(str(path) for path in build.sources(DETERMINANT, VEXSPOKE, ENGINE, LANGUAGE)),
                 curses, "-lm", "-o", str(host)])
            result = run([str(host), str(source), str(plain)])
            messages = result.stderr.splitlines()
            assert len(messages) == 1 and "SemicolonSyntax storage refused" in messages[0], messages
            assert not result.stdout, result.stdout
            print(f"PASS {mode} live editor code-path: ink/weight, wide cells, controls, caret, edits, scroll")
        # Real consumer build, not just a fixture that happens to include headers.
        output = directory / "semicolon"
        build.build(output)
        print("PASS strict Semicolon production link with real Language sources")
        original = output.read_bytes()
        previous = os.environ.get("SEMICOLON_LANGUAGE_SOURCE_DIR")
        try:
            os.environ["SEMICOLON_LANGUAGE_SOURCE_DIR"] = str(directory / "missing-language")
            try:
                build.build(output)
            except ValueError as error:
                assert "c23_lexer" in str(error) or "role.h" in str(error), error
            else:
                raise AssertionError("missing Language source root was accepted")
            assert output.read_bytes() == original, "failed link replaced the previous binary"
        finally:
            if previous is None:
                del os.environ["SEMICOLON_LANGUAGE_SOURCE_DIR"]
            else:
                os.environ["SEMICOLON_LANGUAGE_SOURCE_DIR"] = previous
        negative = subprocess.run(
            ["clang", *flags, *includes, "-x", "c", "-fsyntax-only", "-"],
            input='#include "editor/semicolon_syntax.h"\nvoid f(void) { (void) SemicolonSyntax(1u); }\n',
            capture_output=True, text=True, timeout=30)
        assert negative.returncode != 0 and "too many arguments" in negative.stderr, negative.stderr
        print("PASS missing Language dependency preserves artifact; unsupported constructor arity rejects")
        # Scripted PTY output exercises the production loop and actual ncurses
        # quantization, rather than treating a canvas-only oracle as device proof.
        spec = importlib.util.spec_from_file_location("semicolon_pty_helpers",
                                                       ROOT / "tests/semicolon/tui_main_test.py")
        helpers = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(helpers)
        helpers.STATE_DIR = str(directory / "state")
        editable = directory / "color.c"
        editable.write_text("/** doc */\nint value;\n")
        shell = helpers.Pty(22, 112,
                            argv=[str(output), "--root", str(directory), "--file", str(editable),
                                  "--icons", "plain"],
                            env={"TERM": "xterm-256color", "SEMICOLON_BG": "#202020"})
        try:
            assert shell.wait_for(b"color.c"), "C23 file did not open"
            payload = shell.repaint()
            text = re.sub(rb"\x1b\[[0-9;]*m|\x1b\(B", b"", payload)
            assert b"\x1b[38;5;34m" in payload and b"int" in payload, payload
            assert b"\x1b[38;5;108m" in payload and b"/** doc */" in text, payload
            shell.send(b"\x1b[B")
            shell.send(b"\x1b[H")
            shell.send(b"//")
            payload = shell.repaint()
            # The real caret reverses one glyph, inserting SGR/charset changes
            # inside the source run. Inspect text without those attributes while
            # independently asserting the emitted regular-comment foreground.
            text = re.sub(rb"\x1b\[[0-9;]*m|\x1b\(B", b"", payload)
            assert b"\x1b[38;5;145m" in payload and b"//int value;" in text, payload
            import time
            deadline = time.monotonic() + 5.0
            while (editable.read_text() != "/** doc */\n//int value;\n"
                   and time.monotonic() < deadline):
                shell.drain(0.1)
            assert editable.read_text() == "/** doc */\n//int value;\n", "coloring changed saved source bytes"
        finally:
            assert shell.finish()[0] == 0
        print("PASS scripted PTY C23 keyword/doc/regular-comment foreground and edit recoloring; exact autosaved bytes")


if __name__ == "__main__":
    main()
