# Tools

## Controller

`gcsc-controller` is a keyboard controller for manually debugging the car over Bluetooth serial.

To use this script, you have to:

1. Pair with the Bluetooth device named `roboS`.
2. Bind the paired device to a serial device, usually `/dev/rfcomm0` on Linux.

```sh
uv run gcsc-controller --port <serial-device>
```

Instructions are printed when the tool starts.

Note that Windows support is implemented but untested.
