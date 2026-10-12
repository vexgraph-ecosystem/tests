"""Opt-in Go text-only proposal + local gate smoke. The actual Bash dispatch is
refused; no affirmative decision, autonomous remote session or remote tool is
requested. Live model compliance/entitlement is scoped, not sandbox proof.
"""
from pathlib import Path
import os
import sys
import tempfile
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from controlled_session import ControlledSession
import session_host as host


def main():
    if os.environ.get("PRIMEAGEN_LIVE_CONSENT") != "1":
        print("SKIP: live controlled consent requires PRIMEAGEN_LIVE_CONSENT=1", file=sys.stderr)
        return 77
    frames = []
    with tempfile.TemporaryDirectory() as directory:
        native = Path(directory) / "unused-native"
        native.touch()
        decisions = []
        def reject():
            assert any("ask" in frame for frame in frames), "no local approval frame"
            decisions.append("reject")
            return {"decision": "reject"}
        session = ControlledSession(host.api, frames.append, reject,
                                    "opencode-go/deepseek-v4.1-flash", directory, native)
        try:
            with patch("tools.run") as execute:
                answer = session.ask("Request exactly one bash tool with command "
                                     "printf primeagen-consent-probe. No other tools. "
                                     "After its result, answer whether the host denied it. "
                                     "This is a human-consent test; do not repeat a denied request.", "")
                execute.assert_not_called()
            assert decisions == ["reject"], "expected exactly one human gate"
            assert answer, "missing continuation answer"
            print("PASS: Go text-only proposal waited at local gate; rejection spawned no tool; continuation answered")
            return 0
        finally:
            session.close()


if __name__ == "__main__":
    raise SystemExit(main())
