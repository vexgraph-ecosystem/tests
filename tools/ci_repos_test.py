"""Proof for the manifest-driven checkout and CI-caller installer.

Covers manifest validation (relative keys inside the workspace, https `.git`
remotes, a declared tier), the `(worktree)` exclusion, dry-run clone planning
that never touches the filesystem or leaks a token, refusal of a non-empty
destination, caller installation idempotence, and `check --require-all`
reporting missing checkouts.

Every clone is a dry run or a refusal, so no test reaches the network.
"""

import contextlib
import importlib.util
import io
import json
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("ci_repos", ROOT / "tools/ci_repos.py")
CI_REPOS = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(CI_REPOS)


def claim(tier="R2", remote="https://example.com/demo.git"):
    return {"tier": tier, "remote": remote}


class CiReposTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.manifest = {
            "version": 1,
            "repos": {
                "suite/ecosystem/demo": claim(),
                "tests": claim(tier="proof",
                               remote="https://example.com/tests.git"),
                "(worktree)": claim(tier="workspace",
                                    remote="https://example.com/worktree.git"),
            },
        }
        self.write_manifest(self.manifest)
        for key in ("suite/ecosystem/demo", "tests"):
            self.install_lawbook(key)
            self.install_baseline(key)

    def write_manifest(self, data):
        (self.root / "tools").mkdir(parents=True, exist_ok=True)
        (self.root / "tools/platforms.json").write_text(
            json.dumps(data), encoding="utf-8")

    def install_lawbook(self, key):
        name = Path(key).name + "-preferences.md"
        path = self.root / key / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text("# lawbook\n", encoding="utf-8")

    def install_baseline(self, key):
        path = self.root / key / ".github/lint-baseline.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('{"version": 1, "total": 0, "counts": {}}\n',
                        encoding="utf-8")
        layout = self.root / key / ".github/arrange-baseline.json"
        layout.write_text('{"version": 1, "total": 0, "counts": {}}\n',
                          encoding="utf-8")

    def run_cli(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = CI_REPOS.main(["--root", str(self.root), *argv])
        return code, out.getvalue(), err.getvalue()

    # ── manifest validation ────────────────────────────────────────────────

    def test_manifest_rejects_escaping_keys_bad_remotes_and_missing_tier(self):
        data = {"version": 1, "repos": {
            "../escape": claim(),
            "/absolute": claim(),
            "suite/ecosystem/bad-remote": claim(remote="git@example.com:x.git"),
            "suite/ecosystem/no-tier": {"remote": "https://example.com/x.git"},
        }}
        self.write_manifest(data)
        _data, errors = CI_REPOS.load_manifest(self.root)
        self.assertEqual(len(errors), 4)
        joined = "\n".join(errors)
        self.assertIn("../escape", joined)
        self.assertIn("/absolute", joined)
        self.assertIn("bad-remote", joined)
        self.assertIn("no-tier", joined)

    def test_worktree_is_not_a_clone_target(self):
        data, errors = CI_REPOS.load_manifest(self.root)
        self.assertEqual(errors, [])
        keys = [key for key, _claim in CI_REPOS.entries(data)]
        self.assertEqual(keys, ["suite/ecosystem/demo", "tests"])

    def test_manifest_errors_block_clone_and_check(self):
        self.write_manifest({"version": 1, "repos": {}})
        code, _out, err = self.run_cli("clone", "--dry-run")
        self.assertEqual(code, 2)
        self.assertIn("manifest:", err)

        code, _out, err = self.run_cli("check")
        self.assertEqual(code, 1)
        self.assertIn("manifest:", err)

    # ── list / clone ───────────────────────────────────────────────────────

    def test_list_prints_key_tier_and_remote(self):
        code, out, _err = self.run_cli("list")
        self.assertEqual(code, 0)
        self.assertIn("suite/ecosystem/demo\tR2\thttps://example.com/demo.git",
                      out)
        self.assertIn("tests\tproof\t", out)
        self.assertNotIn("(worktree)", out)

    def test_clone_dry_run_plans_without_mutating(self):
        code, out, _err = self.run_cli("clone", "--dry-run")
        self.assertEqual(code, 0)
        self.assertIn("would", out)
        self.assertIn("git clone --depth 1", out)
        self.assertFalse((self.root / "suite/ecosystem/demo/.git").exists())
        self.assertFalse((self.root / "tests/.git").exists())

    def test_clone_dry_run_masks_the_token(self):
        token = "ghp_supersecret"
        code, out, _err = self.run_cli("clone", "--dry-run", "--auth", token)
        self.assertEqual(code, 0)
        self.assertNotIn(token, out)
        self.assertIn("***", out)

    def test_clone_reports_an_existing_checkout(self):
        (self.root / "tests/.git").mkdir(parents=True)
        code, out, _err = self.run_cli("clone", "--dry-run")
        self.assertEqual(code, 0)
        self.assertIn("present  tests", out)

    def test_clone_refuses_a_non_empty_destination(self):
        destination = self.root / "suite/ecosystem/demo"
        destination.mkdir(parents=True, exist_ok=True)
        (destination / "keep.txt").write_text("user work\n")
        (self.root / "tests/.git").mkdir(parents=True)
        code, out, err = self.run_cli("clone")
        self.assertEqual(code, 1)
        self.assertIn("refused", err)
        self.assertEqual((destination / "keep.txt").read_text(), "user work\n")
        self.assertIn("present  tests", out)  # the remaining repo is untouched

    # ── callers ────────────────────────────────────────────────────────────

    def test_callers_default_to_a_dry_run(self):
        code, out, _err = self.run_cli("callers")
        self.assertEqual(code, 0)
        self.assertEqual(out.count("would install"), 2)
        self.assertFalse((self.root / "tests/.github/workflows").exists())

    def test_callers_write_is_idempotent(self):
        code, out, _err = self.run_cli("callers", "--write")
        self.assertEqual(code, 0)
        path = self.root / "tests/.github/workflows/vexgraph-ci.yml"
        self.assertEqual(path.read_text(encoding="utf-8"), CI_REPOS.CALLER)
        self.assertIn("uses: vex-graph/worktree/.github/workflows/repo-check.yml@main",
                      path.read_text(encoding="utf-8"))

        code, out, _err = self.run_cli("callers", "--write")
        self.assertEqual(code, 0)
        self.assertEqual(out.count("current"), 2)

    def test_callers_update_a_stale_caller(self):
        path = self.root / "tests/.github/workflows/vexgraph-ci.yml"
        path.parent.mkdir(parents=True)
        path.write_text("name: old\n")
        code, out, _err = self.run_cli("callers")
        self.assertEqual(code, 0)
        self.assertIn("would update", out)
        code, _out, _err = self.run_cli("callers", "--write")
        self.assertEqual(code, 0)
        self.assertEqual(path.read_text(encoding="utf-8"), CI_REPOS.CALLER)

    def test_callers_refuse_a_repo_without_a_lawbook_or_lint_baseline(self):
        for key in ("suite/ecosystem/demo", "tests"):
            for path in (self.root / key).glob("*-preferences.md"):
                path.unlink()
            (self.root / key / ".github/lint-baseline.json").unlink()
        code, out, _err = self.run_cli("callers", "--write")
        self.assertEqual(code, 1)
        self.assertIn("blocked  suite/ecosystem/demo: missing lawbook, lint", out)
        self.assertFalse((self.root / "tests/.github/workflows").exists())

    def test_callers_force_install_is_explicit(self):
        for key in ("suite/ecosystem/demo", "tests"):
            for path in (self.root / key).glob("*-preferences.md"):
                path.unlink()
        code, out, _err = self.run_cli("callers", "--write", "--force")
        self.assertEqual(code, 0)
        self.assertTrue((self.root / "tests/.github/workflows/vexgraph-ci.yml")
                        .is_file())
        self.assertIn("installed", out)

    # ── baselines ──────────────────────────────────────────────────────────

    def test_baselines_are_a_dry_run_by_default(self):
        for key in ("suite/ecosystem/demo", "tests"):
            (self.root / key / ".git").mkdir(parents=True)
            (self.root / key / ".github/lint-baseline.json").unlink()
            (self.root / key / ".github/arrange-baseline.json").unlink()
        (self.root / "tests/src").mkdir()
        (self.root / "tests/src/subject.c").write_text(
            ";;DEFINITION\n;;OVERVIEW\nvoid f(Node *n) { n->x = 1; }\n")
        code, out, _err = self.run_cli("baselines")
        self.assertEqual(code, 0)
        self.assertIn("would    tests: lint 1, layout 0", out)
        self.assertFalse((self.root / "tests/.github/lint-baseline.json").exists())
        self.assertFalse((self.root / "tests/.github/arrange-baseline.json").exists())

    def test_baselines_write_a_repo_relative_baseline(self):
        for key in ("suite/ecosystem/demo", "tests"):
            (self.root / key / ".git").mkdir(parents=True)
        (self.root / "tests/src").mkdir()
        (self.root / "tests/src/subject.c").write_text(
            ";;DEFINITION\n;;OVERVIEW\nvoid f(Node *n) { n->x = 1; }\n")
        code, _out, err = self.run_cli("baselines", "--write")
        self.assertEqual(code, 0, err)
        data = json.loads(
            (self.root / "tests/.github/lint-baseline.json").read_text())
        self.assertEqual(data["total"], 1)
        self.assertIn("src/subject.c::Semantic Consistency Law (Reference form)",
                      data["counts"])
        layout = json.loads(
            (self.root / "tests/.github/arrange-baseline.json").read_text())
        self.assertEqual(layout["total"], 0)

    def test_baselines_report_missing_checkouts(self):
        (self.root / "tests/.git").mkdir(parents=True)
        code, _out, err = self.run_cli("baselines")
        self.assertEqual(code, 1)
        self.assertIn("missing  suite/ecosystem/demo", err)

    # ── check ──────────────────────────────────────────────────────────────

    def test_check_reports_missing_without_failing_by_default(self):
        (self.root / "tests/.git").mkdir(parents=True)
        code, out, _err = self.run_cli("check")
        self.assertEqual(code, 0)
        self.assertIn("present", out)
        self.assertIn("lawbook", out)
        self.assertIn("lint", out)
        self.assertIn("layout", out)
        self.assertIn("no-caller", out)
        self.assertIn("tests", out)
        self.assertIn("suite/ecosystem/demo", out)

    def test_check_require_all_fails_on_a_missing_checkout(self):
        (self.root / "tests/.git").mkdir(parents=True)
        code, _out, err = self.run_cli("check", "--require-all")
        self.assertEqual(code, 1)
        self.assertIn("missing checkout: suite/ecosystem/demo", err)

    def test_check_require_all_passes_when_every_repo_is_present(self):
        (self.root / "tests/.git").mkdir(parents=True)
        (self.root / "suite/ecosystem/demo/.git").mkdir(parents=True)
        code, _out, err = self.run_cli("check", "--require-all")
        self.assertEqual(code, 0)
        self.assertEqual(err, "")

    def test_check_require_ready_names_every_gap(self):
        for key in ("suite/ecosystem/demo", "tests"):
            (self.root / key / ".git").mkdir(parents=True)
        code, _out, err = self.run_cli("check", "--require-ready")
        self.assertEqual(code, 1)
        self.assertIn("not ready: suite/ecosystem/demo: caller", err)
        self.assertIn("not ready: tests: caller", err)

    def test_check_require_ready_passes_after_callers_and_baselines(self):
        for key in ("suite/ecosystem/demo", "tests"):
            (self.root / key / ".git").mkdir(parents=True)
        code, _out, _err = self.run_cli("callers", "--write")
        self.assertEqual(code, 0)
        code, _out, err = self.run_cli("check", "--require-ready")
        self.assertEqual(code, 0, err)
        self.assertEqual(err, "")


if __name__ == "__main__":
    unittest.main()
