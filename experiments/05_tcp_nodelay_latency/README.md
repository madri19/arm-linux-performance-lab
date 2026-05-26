# Experiment 05 - TCP_NODELAY Latency

Measures TCP round-trip latency with TCP_NODELAY enabled between a macOS host and an ARM Cortex-A5 embedded Linux board.

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

This experiment reuses a persistent TCP connection while enabling TCP_NODELAY on both client and server sockets.

TCP_NODELAY disables Nagle's algorithm, which normally attempts to coalesce small TCP packets before transmission.

For each iteration:

1. Send small payload
2. Receive echoed response
3. Measure round-trip latency

## Result

Iterations: 1000

Average TCP_NODELAY round-trip latency: 3436.86 us

## Interpretation

TCP_NODELAY was expected to reduce latency for small packets by preventing packet coalescing delays.

However, latency slightly increased compared to previous experiments.

Previous results:

- Experiment 03 (non-persistent): ~3039.95 us
- Experiment 04 (persistent): ~3159.95 us
- Experiment 05 (TCP_NODELAY): ~3436.86 us

This suggests Nagle buffering was not the dominant latency source in this environment.

Likely contributors include:

- scheduler wakeups
- userspace socket handling overhead
- Linux/macOS TCP stack behavior
- blocking send/recv synchronization
- timing noise from host-side measurements

Future experiments may investigate:

- payload size scaling
- throughput vs latency tradeoffs
- threaded server architectures
- polling mechanisms
- non-blocking sockets
- epoll/select behavior

