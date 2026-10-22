"""Owner: CLI-independent agent policy, hostile actions, budgets and recovery.

Scripted providers establish loop behavior only, never live model competence.
Tools are externally serialized; no thread-safety/remote-provider claim.
"""
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "personal/primeagen/src"))
from agent import Agent, strict_json
from cli import display


class AgentTest(unittest.TestCase):
    def provider(self, actions):
        from types import SimpleNamespace
        iterator = iter(actions)
        return SimpleNamespace(complete=lambda text: next(iterator))

    def tools(self, result="denied by user"):
        from types import SimpleNamespace
        self.calls = []
        def execute(name, args):
            self.calls.append((name, args))
            return result
        return SimpleNamespace(execute=execute)

    def test_answer_tool_denial_history_and_reset(self):
        provider = self.provider(['{"tool":"read","arguments":{"path":"file"}}', '{"answer":"Permission denied; no read happened."}', '{"answer":"hello"}'])
        agent = Agent(provider, self.tools())
        self.assertIn("Permission denied", agent.ask("read file"))
        self.assertEqual(self.calls, [("read", {"path": "file"})])
        self.assertEqual(agent.history[2]["result"], "denied by user")
        self.assertEqual(agent.ask("hello"), "hello")
        self.assertEqual(len(agent.history), 6)
        agent.reset()
        self.assertEqual(agent.history, [])

    def test_malformed_duplicate_mixed_invalid_and_budget_preserve_state(self):
        for action in ['{}', '[]', '{"answer":""}', '{"answer":"one","answer":"two"}',
                       '{"tool":"read","arguments":{},"approved":true}', 'plain answer',
                       '{"answer":"x","tool":"read","arguments":{}}']:
            agent = Agent(self.provider([action]), self.tools())
            with self.assertRaises(ValueError):
                agent.ask("hello")
            self.assertEqual(agent.history, [])
            self.assertEqual(self.calls, [])
        with self.assertRaises(ValueError):
            strict_json('{"tool":"read","arguments":{"path":"a","path":"b"}}')
        with self.assertRaisesRegex(ValueError, "nesting"):
            strict_json("[" * 2000 + "0" + "]" * 2000)
        agent = Agent(self.provider(['{"answer":"ok"}']), self.tools(), history_cap=10)
        with self.assertRaises(ValueError):
            agent.ask("hello")
        self.assertEqual(agent.history, [])
        for prompt in (None, "", "   ", "hello\0"):
            with self.assertRaises(ValueError):
                agent.ask(prompt)
        for kwargs in ({"max_steps": 0}, {"max_steps": True}, {"history_cap": 0}):
            with self.assertRaises(ValueError):
                Agent(None, None, **kwargs)

    def test_completed_steps_survive_limit_and_result_truncation_is_observable(self):
        agent = Agent(self.provider(['{"tool":"read","arguments":{"path":"file"}}']), self.tools("x" * 10000), max_steps=1, history_cap=7000)
        with self.assertRaisesRegex(ValueError, "step budget"):
            agent.ask("read")
        self.assertEqual(len(self.calls), 1)
        self.assertIn("truncated", agent.history[-1]["error"])
        self.assertEqual(len(agent.history), 3)
        agent.provider = self.provider(['{"answer":"Recovered"}'])
        self.assertEqual(agent.ask("continue"), "Recovered")
        self.assertNotIn("\x1b", display("hello\x1b[2J\r\x9bworld"))


if __name__ == "__main__":
    unittest.main()
