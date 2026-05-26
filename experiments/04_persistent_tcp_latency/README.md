# Experiment 04 - Persistent TCP Latency

Measures TCP round-trip latency using a persistent TCP connection between a macOS host and an ARM Cortex-A5 embedded Linux board.

## Hardware

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

### Host System
- macOS
- Clang toolchain

## Method

Unlike Experiment 03, this benchmark establishes a single TCP connection and reuses it for all iterations.

For each iteration:

1. Send payload to ARM board
2. ARM board echoes payload back
3. Measure round-trip latency

The TCP connection remains open during the full benchmark.

## Result

Iterations: 1000

Average persistent round-trip latency: 3159.95 us

## Interpretation

Persistent connections were expected to reduce latency by avoiding repeated TCP connection establishment and teardown.

However, latency remained similar to Experiment 03.

Experiment 03:
- ~3039.95 us

Experiment 04:
- ~3159.95 us

This suggests TCP connection setup was not the dominant source of latency in this environment.

Likely contributors include:

- scheduler wakeups
- delayed ACK behavior
- Nagle algorithm interactions
- small packet TCP behavior
- Linux/macOS network stack timing behavior

Future experiments may investigate:

- TCP_NODELAY
- payload size scaling
- threaded server behavior
- socket buffering behavior
- polling strategies

