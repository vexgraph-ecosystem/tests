#!/usr/bin/env python3
"""Owner: Semicolon's opt-in per-user command installer.

Disposable bin directories prove both command names, idempotence, and refusal
before mutation. This test does not edit a shell profile, build, or launch the UI.
"""
import subprocess
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
APP = ROOT / "suite/projects/semicolon"
INSTALLER = APP / "install-command.sh"
LAUNCHER = APP / "semicolon"


def install(bin_dir):
    return subprocess.run(["sh", str(INSTALLER), str(bin_dir)],
                          capture_output=True, text=True, timeout=10)


def main():
    syntax = subprocess.run(["sh", "-n", str(INSTALLER)],
                            capture_output=True, text=True, timeout=10)
    assert syntax.returncode == 0, syntax.stderr
    with tempfile.TemporaryDirectory(prefix="semicolon-command-") as directory:
        scratch = Path(directory)
        bin_dir = scratch / "bin with spaces"
        first = install(bin_dir)
        assert first.returncode == 0, first.stderr
        for name in ("semicolon", "smc"):
            link = bin_dir / name
            assert link.is_symlink() and link.readlink() == LAUNCHER, name
        second = install(bin_dir)
        assert second.returncode == 0 and "Already installed" in second.stdout
        assert all((bin_dir / name).readlink() == LAUNCHER
                   for name in ("semicolon", "smc")), "idempotence lost a link"

        foreign_dir = scratch / "foreign"
        foreign_dir.mkdir()
        foreign = foreign_dir / "smc"
        foreign.write_text("unrelated command\n")
        refused = install(foreign_dir)
        assert refused.returncode != 0 and "Refusing to replace" in refused.stderr
        assert foreign.read_text() == "unrelated command\n"
        assert not (foreign_dir / "semicolon").exists(), \
            "installer partially created semicolon before refusing smc"

        dangling_dir = scratch / "dangling"
        dangling_dir.mkdir()
        (dangling_dir / "smc").symlink_to(scratch / "nonexistent")
        refused = install(dangling_dir)
        assert refused.returncode != 0 and not (dangling_dir / "semicolon").exists()
        assert (dangling_dir / "smc").is_symlink(), "foreign symlink was replaced"

        legacy_dir = scratch / "existing"
        legacy_dir.mkdir()
        (legacy_dir / "semicolon").symlink_to(LAUNCHER)
        upgraded = install(legacy_dir)
        assert upgraded.returncode == 0 and (legacy_dir / "smc").readlink() == LAUNCHER

    print("Semicolon installer owner passed")


if __name__ == "__main__":
    main()
