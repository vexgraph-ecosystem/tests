"""Shared native owner fixture. Disposable files, strict C23 + ASan/UBSan.
Each mirrored owner exercises its named tool through the real private CLI seam.
No live provider, user files, remote agent or production-readiness claim.
"""
from pathlib import Path
import json
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
VEX = ROOT / "host-behavior/vexspoke/src"
SOURCES = ("main", "fs", "read", "list", "write", "create", "edit", "delete", "bash", "websearch")


def build(dest, sanitized):
    flags = ["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1", "-mcpu=apple-m1",
             "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{VEX}"]
    if sanitized:
        flags += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    subprocess.run([*flags, *(str(APP / f"tools/{name}.c") for name in SOURCES),
                    str(VEX / "net/json.c"), "-o", str(dest)], check=True, capture_output=True, timeout=60)


class NativeOwner(unittest.TestCase):
    tag = None

    def test_named_native_contract(self):
        for sanitized in (False, True):
            with self.subTest(sanitized=sanitized), tempfile.TemporaryDirectory(prefix="primeagen-tool-") as directory:
                base = Path(directory).resolve()
                root = base / "root with spaces"
                root.mkdir()
                binary = base / "tools"
                build(binary, sanitized)
                target = root / "literal ; file.txt"
                target.write_text("hello café")
                outside = base / "outside"
                outside.write_text("preserve")
                (root / "alias").symlink_to(outside)

                def call(*args, data=b"", cap=128 * 1024, success=True):
                    result = subprocess.run([str(binary), str(root), str(cap), self.tag, *map(str, args)],
                                            input=data, capture_output=True, timeout=5,
                                            env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})
                    if success:
                        self.assertEqual(result.returncode, 0, result.stderr)
                        self.assertEqual(result.stderr, b"")
                    else:
                        self.assertNotEqual(result.returncode, 0)
                        self.assertEqual(result.stdout, b"")
                        if self.tag != "bash":
                            self.assertTrue(result.stderr.startswith(b"[vex]"), result.stderr)
                    return result.stdout

                if self.tag == "read":
                    self.assertEqual(call(target.name), "hello café".encode())
                    self.assertEqual(call(target.name, cap=len(target.read_bytes())), target.read_bytes())
                    call(target.name, cap=len(target.read_bytes()) - 1, success=False)
                    for path in ("alias", "../outside", str(outside), "missing", "", "a/../b", "./literal ; file.txt"):
                        call(path, success=False)
                    os.link(outside, root / "hardlink")
                    call("hardlink", success=False)
                    fifo = root / "fifo"
                    os.mkfifo(fifo)
                    call("fifo", success=False)  # cannot block on FIFO admission
                elif self.tag == "list":
                    self.assertEqual(json.loads(call(".")), ["alias", target.name])
                    folder = root / "dir"
                    folder.mkdir()
                    self.assertEqual(call("dir"), b"[]")
                    for i in range(65):
                        (folder / f"name-{i:03}").touch()
                    self.assertEqual(len(json.loads(call("dir"))), 65)
                    (folder / 'quoted"\nname').touch()
                    self.assertIn('quoted"\nname', json.loads(call("dir")))
                    call("dir", cap=10, success=False)
                    for path in ("../outside", "alias", target.name):
                        call(path, success=False)
                elif self.tag in {"write", "create"}:
                    path = target.name if self.tag == "write" else "new.txt"
                    self.assertEqual(call(path, data="new 👋 bytes".encode()), b"completed")
                    self.assertEqual((root / path).read_text(), "new 👋 bytes")
                    if self.tag == "create":
                        call(path, data=b"overwrite", success=False)
                        self.assertEqual((root / path).read_text(), "new 👋 bytes")
                        path = "empty.txt"
                    self.assertEqual(call(path, data=b""), b"completed")
                    self.assertEqual((root / path).read_bytes(), b"")
                    for path in ("alias", "../outside", "missing/child"):
                        call(path, data=b"changed", success=False)
                    call("oversized.txt" if self.tag == "create" else target.name, data=b"x" * 9, cap=8, success=False)
                elif self.tag == "edit":
                    self.assertEqual(call(target.name, "café", "👋"), b"completed")
                    self.assertEqual(target.read_text(), "hello 👋")
                    call(target.name, "absent", "x", success=False)
                    call(target.name, "", "x", success=False)
                    self.assertEqual(target.read_text(), "hello 👋")
                    target.write_text("aaaa")
                    call(target.name, "aa", "x", success=False)  # overlap ambiguity
                    self.assertEqual(target.read_text(), "aaaa")
                    target.write_bytes(b"a\0b")
                    call(target.name, "a", "x", success=False)
                    self.assertEqual(target.read_bytes(), b"a\0b")
                    call("alias", "preserve", "x", success=False)
                elif self.tag == "delete":
                    self.assertEqual(call(target.name), b"completed")
                    self.assertFalse(target.exists())
                    for path in (target.name, "alias", "../outside", "."):
                        call(path, success=False)
                elif self.tag == "bash":
                    self.assertEqual(call("printf 'native bash'"), b"native bash")
                    self.assertEqual(call("pwd").decode().strip(), str(root))
                    call("exit 7", success=False)
                    call("", success=False)
                elif self.tag == "websearch":
                    provider = base / "fake search"
                    provider.write_text('#!/bin/sh\nprintf "query=%s" "$1"\n')
                    provider.chmod(0o700)
                    self.assertEqual(call(provider, "words ; $(touch nope)"), b"query=words ; $(touch nope)")
                    self.assertFalse((root / "nope").exists())
                    call(base / "missing", "query", success=False)
                    call("relative", "query", success=False)
                    call(provider, "", success=False)
                elif self.tag == "invalid":
                    call("anything", success=False)
                else:
                    self.fail("missing owner tag")
                self.assertEqual(outside.read_text(), "preserve")
                self.assertFalse(any(root.glob(".primeagen-*")))
