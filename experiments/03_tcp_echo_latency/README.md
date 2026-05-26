# Experiment 03 - TCP Echo Latency

Measures TCP round-trip latency between a macOS host and an ARM Cortex-A5 embedded Linux board over Ethernet.

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

The benchmark performs 1000 TCP echo iterations.

For each iteration:

1. Create TCP socket
2. Connect from macOS host to ARM board
3. Send test payload
4. ARM board echoes payload back
5. Receive echoed response
6. Measure round-trip time
7. Close TCP connection

The ARM board runs a simple single-threaded blocking TCP echo server.

## Result

Iterations: 1000

Average round-trip latency: 3039.95 us

## Notes

This benchmark includes:

- TCP connection establishment overhead
- socket syscall overhead
- Linux network stack processing
- Ethernet transport latency
- scheduler wakeups
- ARM userspace processing time
- TCP teardown/close overhead

Because a new TCP connection is created for every iteration, this measures full transaction latency rather than steady-state socket throughput.

Future experiments may compare:

- persistent vs non-persistent connections
- threaded vs single-threaded server models
- payload size impact
- polling frequency impact
- socket configuration tuning

