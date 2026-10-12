"""Owner: bounded subprocess seam, literal argv, pipe pressure and cleanup.

Bounded synthetic children prove legal POSIX use, not containment of malicious
detached processes or Windows. Each case has an external subprocess watchdog.
"""
from pathlib import Path
import sys
import time
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from process import run


class ProcessTest(unittest.TestCase):
    def test_large_bidirectional_input_literal_argv_and_normal_quiet_exit(self):
        source = "import sys; data=sys.stdin.buffer.read(); sys.stdout.buffer.write(data); print(sys.argv[1])"
        data = b"payload" * 10000
        result = run([sys.executable, "-c", source, "; $(touch /no-such-artifact)"], data=data)
        self.assertEqual(result, data + b"; $(touch /no-such-artifact)\n")
        self.assertEqual(run([sys.executable, "-c", "pass"]), b"")

    def test_overflow_error_timeout_and_cancel_paths_then_recovery(self):
        for source, kwargs, message in [("import sys; sys.stdout.write('x'*10000)", {"cap": 100}, "truncated"),
                                       ("import sys; sys.stderr.write('bad')", {}, "diagnostics"),
                                       ("raise SystemExit(4)", {}, "exit 4"),
                                       ("import time; time.sleep(30)", {"timeout": 0.1}, "deadline")]:
            start = time.monotonic()
            with self.assertRaisesRegex(ValueError, message):
                run([sys.executable, "-c", source], **kwargs)
            self.assertLess(time.monotonic() - start, 5)
            self.assertEqual(run([sys.executable, "-c", "print('ok')"]), b"ok\n")
        for argv, kwargs in [([], {}), (["bad\0"], {}), ([sys.executable], {"timeout": float("nan")}),
                             ([sys.executable], {"cap": 0}), ([sys.executable], {"data": b"x" * (1024 * 1024 + 1)})]:
            with self.assertRaises(ValueError):
                run(argv, **kwargs)

    def test_descendant_inherited_pipe_is_in_whole_job_deadline(self):
        source = "import subprocess,sys; subprocess.Popen([sys.executable,'-c','import time; time.sleep(30)'])"
        with self.assertRaisesRegex(ValueError, "deadline"):
            run([sys.executable, "-c", source], timeout=0.2)

    def test_short_bash_overflow_preserves_failure_then_recovers(self):
        with self.assertRaisesRegex(ValueError, "truncated"):
            run(["/bin/bash", "--noprofile", "--norc", "-c", "printf '%1024s' x"], cap=128)
        self.assertEqual(run(["/bin/bash", "--noprofile", "--norc", "-c", "printf recovery"]), b"recovery")

    def test_selector_admission_failure_reaps_real_child(self):
        from unittest.mock import patch
        import process
        import subprocess
        real_spawn = subprocess.Popen
        children = []
        def spawn(*args, **kwargs):
            child = real_spawn(*args, **kwargs)
            children.append(child)
            return child
        with patch.object(process.subprocess, "Popen", side_effect=spawn), \
             patch.object(process.selectors, "DefaultSelector", side_effect=OSError("selector admission")):
            with self.assertRaisesRegex((OSError, ValueError), "selector admission"):
                run([sys.executable, "-c", "import time; time.sleep(30)"], timeout=0.2)
        self.assertEqual(len(children), 1)
        self.assertIsNotNone(children[0].poll())
        self.assertTrue(all(stream.closed for stream in (children[0].stdin, children[0].stdout, children[0].stderr)))

    def test_refused_group_cleanup_is_loud_and_no_implicit_wait(self):
        from unittest.mock import patch, MagicMock
        import process
        child = MagicMock()
        child.poll.return_value = None
        child.wait.return_value = 0
        with patch.object(process.subprocess, "Popen", return_value=child), \
             patch.object(process.selectors, "DefaultSelector", side_effect=OSError("selector admission")), \
             patch.object(process.os, "killpg", side_effect=PermissionError("refused")):
            with self.assertRaisesRegex(ValueError, "process-group cleanup refused"):
                run(["trusted-test-child"], timeout=0.2)
        child.kill.assert_called_once()
        child.wait.assert_called_once_with(timeout=5)
        child.__exit__.assert_not_called()


if __name__ == "__main__":
    unittest.main()
