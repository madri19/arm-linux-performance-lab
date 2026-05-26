# Experiment 07 - Payload Size Scaling

Measures how TCP round-trip latency changes as payload size increases between a macOS host and an ARM Cortex-A5 embedded Linux board.

## Goal

Investigate whether latency is dominated by:

- fixed syscall/scheduler overhead
- or payload transfer/copy cost

This experiment helps distinguish constant per-message overhead from scaling behavior as payload size grows.

## Hardware

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

### Host System
- macOS

## Method

A persistent TCP connection is established between the macOS host and ARM board.

The client sends payloads of increasing size:

- 16 bytes
- 64 bytes
- 256 bytes
- 1024 bytes
- 4096 bytes

For each payload size:

1. Send payload to server
2. Receive echoed payload
3. Measure round-trip latency
4. Repeat for 1000 iterations

## Results

Iterations per payload: 1000

| Payload Size | Average RTT |
|---|---|
| 16 bytes | 3071.41 us |
| 64 bytes | 2929.4 us |
| 256 bytes | 3166 us |
| 1024 bytes | 3259.5 us |
| 4096 bytes | 4204.89 us |

## Key Observation

Latency remained relatively stable from:

- 16B
- 64B
- 256B
- 1024B

All stayed near ~3ms RTT.

At 4096B, latency increased noticeably to ~4.2ms.

## Interpretation

Small payload latency appears dominated by fixed overhead such as:

- scheduler wakeups
- syscall overhead
- userspace/kernel synchronization
- TCP stack processing

Larger payloads begin introducing measurable:

- memory copy cost
- socket buffer overhead
- network transfer cost

This suggests the system transitions from fixed-overhead-dominated latency into payload-transfer-dominated latency as message size grows.

## Why This Matters

This experiment expands the investigation from:

- connection behavior
- syscall tracing

into:

- payload scaling behavior
- latency vs throughput tradeoffs
- buffer/copy overhead analysis
- network stack scaling characteristics

