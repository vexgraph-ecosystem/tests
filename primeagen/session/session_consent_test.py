"""Opt-in real OpenCode Go shell-consent owner. No affirmative approval is sent.

PRIMEAGEN_LIVE_CONSENT=1 authorizes one harmless printf proposal to the selected
Go model. The test requires a real pending shell request, rejects it, checks it
is removed and deletes only its own disposable session. It never changes config,
saved approvals or credentials. This is not all-tools or sandbox proof.
"""
from pathlib import Path
import os
import sys
import time

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
import session_host as host


def main():
    if os.environ.get("PRIMEAGEN_LIVE_CONSENT") != "1":
        print("SKIP: live shell-consent probe requires PRIMEAGEN_LIVE_CONSENT=1", file=sys.stderr)
        return 77
    session = host.create("opencode-go/deepseek-v4.1-flash")
    try:
        host.inspect(session)
        turn = host.begin(session, "Request exactly one shell tool call with this exact command: "
                          "/bin/bash --noprofile --norc -c 'printf primeagen-consent-probe'. "
                          "Do not read files or request anything else. This tests human approval; stop if rejected.", "")
        deadline = time.monotonic() + 90
        while time.monotonic() < deadline:
            _, ask, done = host.advance(session, turn)
            if ask is not None:
                assert ask["action"] == "shell", "unexpected requested action"
                host.decide(session, ask, "reject")
                assert host.request(session) is None, "rejected request remained pending"
                print("PASS: real Go shell request waited for human consent; No reached OpenCode")
                return 0
            if done is not None:
                page = host.api("get", "/api/session/" + session + "/message?order=desc&limit=20",
                                cap=host.MESSAGE_CAP)["data"]
                states = [(part[2], part[3].get("status")) for item in page
                          for part in host.parts(item) if part[0] == "tool"]
                raise AssertionError("turn completed without human shell consent; tool states=" + repr(states))
            # IO cadence only; correctness waits for an observed server request.
            time.sleep(.1)
        raise AssertionError("no shell approval within the external probe bound")
    finally:
        host.interrupt(session)
        host.api("delete", "/api/session/" + session, empty=True)


if __name__ == "__main__":
    raise SystemExit(main())
