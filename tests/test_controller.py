from __future__ import annotations

import unittest

from tools.controller import MACOS_DEFAULT_PORT, default_port


class DefaultPortTests(unittest.TestCase):
    def test_non_windows_default_port_is_robos_device(self) -> None:
        self.assertEqual(default_port(os_name="posix", environ={}), MACOS_DEFAULT_PORT)

    def test_windows_default_port_is_robos_device(self) -> None:
        self.assertEqual(default_port(os_name="nt", environ={}), MACOS_DEFAULT_PORT)

    def test_environment_variable_overrides_platform_default(self) -> None:
        self.assertEqual(
            default_port(os_name="nt", environ={"GCSC_PORT": "COM7"}),
            "COM7",
        )


if __name__ == "__main__":
    unittest.main()
