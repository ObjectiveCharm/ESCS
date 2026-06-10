from __future__ import annotations

import unittest
from contextlib import redirect_stdout
from dataclasses import dataclass
from io import StringIO
from unittest.mock import patch

from tools.gesture_controller import GestureState, classify_gesture, send_if_changed


@dataclass(frozen=True)
class Point:
    x: float
    y: float


class FakeConnection:
    def __init__(self) -> None:
        self.commands: list[bytes] = []

    def write(self, payload: bytes) -> int:
        self.commands.append(payload)
        return len(payload)

    def flush(self) -> None:
        pass


def hand_with(index_tip: Point, folded: bool = True) -> list[Point]:
    points = [Point(0.5, 0.7) for _ in range(21)]
    index_mcp = Point(0.5, 0.5)
    points[0] = Point(0.5, 0.7)
    points[5] = index_mcp
    points[6] = Point(
        index_mcp.x + (index_tip.x - index_mcp.x) * 0.35,
        index_mcp.y + (index_tip.y - index_mcp.y) * 0.35,
    )
    points[8] = index_tip

    if folded:
        for pip_index, tip_index in ((10, 12), (14, 16), (18, 20)):
            points[pip_index] = Point(0.5, 0.45)
            points[tip_index] = Point(0.5, 0.5)
    else:
        for pip_index, tip_index in ((10, 12), (14, 16), (18, 20)):
            points[pip_index] = Point(0.5, 0.45)
            points[tip_index] = Point(0.5, 0.1)

    return points


def fist() -> list[Point]:
    points = hand_with(Point(0.5, 0.5))
    points[6] = Point(0.5, 0.45)
    points[8] = Point(0.5, 0.5)
    return points


class GestureClassificationTests(unittest.TestCase):
    def test_index_up_is_forward(self) -> None:
        state = classify_gesture(hand_with(Point(0.5, 0.1)))

        self.assertEqual(state, GestureState("F", "forward"))

    def test_index_down_is_backward(self) -> None:
        state = classify_gesture(hand_with(Point(0.5, 0.95)))

        self.assertEqual(state, GestureState("B", "backward"))

    def test_index_left_is_left(self) -> None:
        state = classify_gesture(hand_with(Point(0.1, 0.5)))

        self.assertEqual(state, GestureState("L", "left"))

    def test_index_right_is_right(self) -> None:
        state = classify_gesture(hand_with(Point(0.9, 0.5)))

        self.assertEqual(state, GestureState("R", "right"))

    def test_fist_is_stop(self) -> None:
        state = classify_gesture(fist())

        self.assertEqual(state, GestureState("S", "stop"))

    def test_other_extended_fingers_are_ignored(self) -> None:
        state = classify_gesture(hand_with(Point(0.5, 0.1), folded=False))

        self.assertIsNone(state)

    def test_incomplete_landmarks_are_ignored(self) -> None:
        state = classify_gesture([Point(0.0, 0.0)])

        self.assertIsNone(state)


class SerialCommandTests(unittest.TestCase):
    def test_send_if_changed_skips_duplicate_command(self) -> None:
        connection = FakeConnection()

        with patch("tools.gesture_controller.send") as send, redirect_stdout(StringIO()):
            last_command = send_if_changed(
                connection,
                GestureState("F", "forward"),
                None,
            )
            last_command = send_if_changed(
                connection,
                GestureState("F", "forward"),
                last_command,
            )
            send_if_changed(connection, GestureState("S", "stop"), last_command)

        self.assertEqual(
            [call.args[1] for call in send.call_args_list],
            ["F", "S"],
        )


if __name__ == "__main__":
    unittest.main()
