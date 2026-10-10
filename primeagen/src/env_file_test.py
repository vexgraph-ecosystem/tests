"""Private-file owner: literal parsing, atomic admission, privacy, size and type
boundaries. Uses disposable fake tokens only; never reads the real app .env.
"""
from pathlib import Path
import os
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"
sys.path.insert(0, str(APP / "src"))
from env_file import load_env, ENV_FILE_BYTES_MAX


class EnvFileTest(unittest.TestCase):
    def test_missing_empty_literal_quotes_crlf_and_environment_precedence(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / ".env"
            env = {"OPENAI_API_KEY": "from-terminal", "OTHER": "preserved"}
            load_env(path, env)
            self.assertEqual(env["OPENAI_API_KEY"], "from-terminal")
            path.write_bytes(b"# test\r\n\r\n OPENAI_API_KEY='fake-openai'\r\nANTHROPIC_API_KEY=\"fake-anthropic\"\r\n")
            load_env(path, env)
            self.assertEqual(env, {"OPENAI_API_KEY": "from-terminal", "OTHER": "preserved", "ANTHROPIC_API_KEY": "fake-anthropic"})
            path.write_text("OPENAI_API_KEY=\nANTHROPIC_API_KEY=\n")
            blank = {}
            load_env(path, blank)
            self.assertEqual(blank, {"OPENAI_API_KEY": "", "ANTHROPIC_API_KEY": ""})
            path.write_text("OPENAI_API_KEY=fake\n")
            load_env(path, blank)
            self.assertEqual(blank["OPENAI_API_KEY"], "")
            path.write_text("")
            load_env(path, {})

    def test_invalid_files_preserve_environment_without_echoing_contents(self):
        cases = [b"OPENAI_API_KEY=fake-secret\nBOGUS=fake-secret", b"export OPENAI_API_KEY=fake-secret",
                 b"OPENAI_API_KEY=fake-secret\nOPENAI_API_KEY=duplicate", b"OPENAI_API_KEY='fake-secret",
                 b"OPENAI_API_KEY=fake secret", b"OPENAI_API_KEY=fake-secret\x00",
                 b"OPENAI_API_KEY=fake-secret\x1b", b"OPENAI_API_KEY=\xff", b"no-assignment",
                 b"ANTHROPIC_API_KEY=fake-secret\nOPENAI_API_KEY=\"multi\nline\""]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / ".env"
            for data in cases:
                with self.subTest(data=data):
                    path.write_bytes(data)
                    env = {"UNCHANGED": "sentinel"}
                    with self.assertRaises(ValueError) as error:
                        load_env(path, env)
                    self.assertNotIn("fake-secret", str(error.exception))
                    self.assertEqual(env, {"UNCHANGED": "sentinel"})
            path.write_text("OPENAI_API_KEY=recovered\n")
            env = {}
            load_env(path, env)
            self.assertEqual(env["OPENAI_API_KEY"], "recovered")

    def test_exact_size_next_rejection_symlink_directory_and_fifo(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / ".env"
            path.write_bytes(b"#" + b"x" * (ENV_FILE_BYTES_MAX - 1))
            load_env(path, {})
            path.write_bytes(b"#" + b"x" * ENV_FILE_BYTES_MAX)
            with self.assertRaises(ValueError):
                load_env(path, {})
            link = Path(directory) / "link"
            link.symlink_to(path)
            with self.assertRaises(ValueError):
                load_env(link, {})
            with self.assertRaises(ValueError):
                load_env(Path(directory), {})
            fifo = Path(directory) / "fifo"
            os.mkfifo(fifo)
            with self.assertRaises(ValueError):
                load_env(fifo, {})

    def test_literal_not_shell_and_git_ignore_without_reading_real_file(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / ".env"
            path.write_text("OPENAI_API_KEY=$(id)\nANTHROPIC_API_KEY=`id`\n")
            env = {}
            load_env(path, env)
            self.assertEqual(env, {"OPENAI_API_KEY": "$(id)", "ANTHROPIC_API_KEY": "`id`"})
        result = subprocess.run(["git", "-C", str(APP), "check-ignore", "--", ".env"], capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 0)
        self.assertIn("load_env", (APP / "src/cli.py").read_text())
        self.assertIn("automatically", (APP / "README.md").read_text())

    def test_chat_root_loading_and_doctor_no_load_offline(self):
        # A relocated disposable host with stub agent/provider/tools validates the
        # CLI lifecycle without real keys, model calls or source-dependency builds.
        with tempfile.TemporaryDirectory() as directory:
            app = Path(directory) / "app"
            src = app / "src"
            src.mkdir(parents=True)
            for name in ("cli.py", "env_file.py"):
                (src / name).write_text((APP / "src" / name).read_text())
            (src / "agent.py").write_text("class Agent:\n def __init__(self,*args): pass\n def reset(self): pass\n")
            (src / "provider.py").write_text("import os\nclass Provider:\n def __init__(self,*args,**kw):\n  assert os.environ['OPENAI_API_KEY']=='fake-project-key'\n")
            (src / "tools.py").write_text("class Tools:\n def __init__(self,*args): pass\n def close(self): pass\n")
            (app / ".env").write_text("OPENAI_API_KEY=fake-project-key\n")
            env = {k: v for k, v in os.environ.items() if k not in ("OPENAI_API_KEY", "ANTHROPIC_API_KEY")}
            result = subprocess.run([sys.executable, str(src / "cli.py"), "chat", "--provider", "openai", "--model", "fake"],
                                    input=b"/quit\n", cwd=directory, env=env, capture_output=True, timeout=10)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertNotIn(b"fake-project-key", result.stdout + result.stderr)
            (app / ".env").write_text("invalid-secret-line")
            result = subprocess.run([sys.executable, str(src / "cli.py"), "doctor"], cwd=directory,
                                    env=env, capture_output=True, timeout=10)
            self.assertEqual(result.returncode, 0)
            result = subprocess.run([sys.executable, str(src / "cli.py"), "chat", "--provider", "openai", "--model", "fake"],
                                    input=b"/quit\n", cwd=directory, env=env, capture_output=True, timeout=10)
            self.assertNotEqual(result.returncode, 0)
            self.assertNotIn(b"invalid-secret-line", result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()
