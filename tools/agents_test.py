"""Check agent signatures offline against a fake OpenCode API."""

import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
STUB = r'''
opencode() {
    if [ "$2" = post ]; then
        printf '%s\n' "$5" >> "$CAPTURE"
    elif [ "$3" = /api/session/active ]; then
        printf '%s\n' '{"data":{"ses_sender123":{},"ses_target456":{}}}'
    else
        printf '%s\n' '{"data":[{"id":"ses_sender123","title":"Sender agent"},{"id":"ses_target456","title":"Target agent"}]}'
    fi
}
export -f opencode
bash "$@"
'''


class AgentsTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name)
        self.env = {k: v for k, v in os.environ.items()
                    if k not in ["AGENTS_RAW", "AGENTS_NAME", "AGENTS_NAMES_DIR", "AGENTS_BUS"]}
        self.env.update(OPENCODE_SESSION_ID="ses_sender123", AGENTS_NAME="alice",
                        AGENTS_NAMES_DIR=str(self.path / "names"),
                        AGENTS_BUS=str(self.path / "bus.md"), CAPTURE=str(self.path / "capture"))

    def cli(self, *args):
        return subprocess.check_output(["bash", "-c", STUB, "stub", str(ROOT / "tools/agents.sh"), *args],
                                       env=self.env, text=True)

    def message(self):
        lines = (self.path / "capture").read_text().splitlines()
        # jq emits pretty JSON, so parse the entire one-message capture.
        return json.loads("\n".join(lines))["text"]

    def test_say_is_signed(self):
        self.cli("say", "Target agent", "handoff")
        self.assertEqual(self.message(), "handoff\n\n— alice (agent sender12)")

    def test_tell_excludes_self_and_is_signed(self):
        out = self.cli("tell", "scope")
        self.assertNotIn("sent to ses_sender123", out)
        self.assertEqual(out.count("sent to"), 1)
        self.assertEqual(self.message(), "scope\n\n— alice (agent sender12)")

    def test_unsigned_opt_out(self):
        self.env["AGENTS_RAW"] = "1"
        self.cli("say", "Target agent", "raw")
        self.assertEqual(self.message(), "raw")

    def test_persisted_name_and_env_override(self):
        del self.env["AGENTS_NAME"]
        self.cli("name", "frame-lane")
        self.assertEqual(self.cli("name").strip(), "frame-lane")
        self.env["AGENTS_NAME"] = "override"
        self.assertEqual(self.cli("name").strip(), "override")

    def test_bus_records_name_and_unix_convertible_time(self):
        from datetime import datetime
        self.cli("bus", "scope")
        line = (self.path / "bus.md").read_text()
        self.assertIn("alice (sender12)  scope", line)
        self.assertGreater(datetime.fromisoformat(line.split()[0].replace("Z", "+00:00")).timestamp(), 0)


if __name__ == "__main__":
    unittest.main()
