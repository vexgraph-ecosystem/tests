"""Native-Haven-to-host answer smoke. Offline fake CLI by default; explicit
PRIMEAGEN_LIVE_MODEL=opencode-go/deepseek-v4.1-flash authorizes one live short
answer, no tools or interactive gallery. Records no identity/token/server dump.
"""
from pathlib import Path
import json
import os
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / "personal/primeagen"


class SessionAnswerTest(unittest.TestCase):
    def test_native_haven_to_host_answer(self):
        live = os.environ.get("PRIMEAGEN_LIVE_MODEL")
        if live and live != "opencode-go/deepseek-v4.1-flash":
            self.fail("live answer model is not the user-authorized Go model")
        with tempfile.TemporaryDirectory() as directory:
            home = Path(directory)
            vex = ROOT / "ecosystem/repos/vexspoke/src"
            api = ROOT / "ecosystem/repos/api-haven/src"
            output = home / "client"
            subprocess.run(["clang", "-std=gnu23", "-Wall", "-Wextra", "-Werror", "-O1",
                            "-mcpu=apple-m1", "-mmacosx-version-min=14.0", f"-I{APP}", f"-I{vex}", f"-I{api}",
                            str(Path(__file__).with_suffix(".c")), str(APP / "session/session_bridge.c"),
                            str(APP / "personality/owo.c"), str(vex / "net/json.c"), str(api / "harness/harness.c"),
                            str(api / "harness/engine_provider.c"), "-o", str(output)], check=True, capture_output=True, timeout=60)
            env = dict(os.environ)
            if not live:
                fake = home / "opencode"
                fake.write_text('#!/usr/bin/env python3\nimport json,sys\np=sys.argv[3]\n'
                                'rules=[{"action":"*","resource":"*","effect":"deny"}]\n'
                                'if p=="/api/session": r={"data":{"id":"ses_test","permissions":rules}}\n'
                                'elif p=="/api/session/ses_test": r={"data":{"permissions":rules}}\n'
                                'elif p.endswith("/prompt"):\n b=json.loads(sys.argv[-1]);open("message","w").write(b["id"]);r={"data":{}}\n'
                                'else: r={"data":[{"type":"idle","outcome":"succeeded"},{"type":"assistant","content":[{"type":"text","text":"2 + 2 is 4, owo!"}]},{"id":open("message").read()}]}\n'
                                'print(json.dumps(r))\n')
                fake.chmod(0o700)
                env["PATH"] = directory + ":" + env["PATH"]
            request_read, request_write = os.pipe()
            response_read, response_write = os.pipe()
            worker = None
            try:
                os.set_blocking(request_write, False)
                os.set_blocking(response_read, False)
                argv = [sys.executable, str(APP / "src/session_host.py"), "--worker"]
                if live:
                    argv += ["--model", live]
                worker = subprocess.Popen(argv, stdin=request_read, stdout=response_write,
                                          stderr=subprocess.PIPE, cwd=directory, env=env)
                os.close(request_read)
                os.close(response_write)
                request_read = response_write = -1
                result = subprocess.run([str(output), str(response_read), str(request_write)],
                                        pass_fds=(response_read, request_write), capture_output=True, timeout=130)
                self.assertEqual(result.returncode, 0, result.stderr)
                answer = json.loads(result.stdout)
                self.assertIn("4", answer)
                print(("Live Go" if live else "Offline fake") + " native Haven answer: " + json.dumps(answer, ensure_ascii=True))
            finally:
                if worker:
                    worker.terminate()
                    try:
                        worker.wait(timeout=3)
                    except subprocess.TimeoutExpired:
                        worker.kill()
                        worker.wait(timeout=3)
                    worker.stderr.close()
                for fd in (request_read, request_write, response_read, response_write):
                    if fd >= 0:
                        os.close(fd)


if __name__ == "__main__":
    unittest.main()
