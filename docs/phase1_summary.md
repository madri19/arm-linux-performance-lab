# Phase 1 Summary - ARM Embedded Linux Performance Lab

## Overview

Phase 1 focused on building practical systems-performance intuition on a physical ARM embedded Linux board.

The goal was to move from basic board bring-up into measurable investigation of Linux behavior on constrained hardware.

## Platform

### Target Board

- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

### Host Environment

- macOS host
- Lima Linux VM
- Buildroot cross-toolchain
- SSH deployment to target board

## Infrastructure Built

This phase included:

- UART serial console access
- Buildroot image customization
- Dropbear SSH setup
- static IP networking
- remote SCP deployment workflow
- ARM cross-compilation workflow
- local network development loop
- manual userspace tooling deployment

## Experiments Completed

| Experiment | Focus |
|---|---|
| 01 | `/proc/net/dev` parsing overhead |
| 02 | raw procfs read vs userspace parsing |
| 03 | TCP echo round-trip latency |
| 04 | persistent TCP connection latency |
| 05 | TCP_NODELAY latency behavior |
| 06 | syscall tracing with `strace` |
| 07 | payload size scaling |
| 08 | producer/consumer threading overhead |
| 09 | scheduler wakeup timing jitter |
| 10 | sequential vs random memory access |
| 11 | long-running workload observation |

## Key Findings

### procfs and parsing

Reading and parsing `/proc/net/dev` on the Cortex-A5 board took roughly 515us per iteration.

Separating raw reads from parsing showed that most time came from procfs/file access rather than userspace token parsing.

### TCP latency

TCP echo latency stayed near ~3ms across reconnecting sockets, persistent sockets, and TCP_NODELAY tests.

This suggested the dominant latency was not simple connection setup or Nagle buffering.

### syscall tracing

`strace` showed repeated `send` and `recv` calls dominated traced syscall time during the persistent TCP echo workload.

Connection setup syscalls were negligible compared to repeated message exchange behavior.

### payload scaling

Small payloads remained near ~3ms RTT, while 4096B payloads increased to roughly ~4.2ms.

This suggested fixed per-message overhead dominated small payloads, while larger payloads began exposing copy/buffer/network costs.

### threading and scheduler behavior

A producer/consumer synchronization benchmark measured roughly 1.08us average overhead per work item.

A 1ms sleep timing benchmark showed consistent scheduler wakeup overshoot around 90-100us, with occasional higher jitter.

### memory behavior

Random memory traversal was nearly 10x slower than sequential traversal.

This demonstrated the impact of cache locality and predictable memory access patterns even on constrained embedded hardware.

### long-running observation

A 60-second sustained workload showed stable RSS and free memory while load average increased gradually under CPU work.

## Skills Practiced

- embedded Linux bring-up
- Buildroot usage
- ARM cross-compilation
- SSH deployment workflows
- TCP socket programming
- Linux `/proc` inspection
- syscall tracing with `strace`
- benchmark construction
- latency measurement
- scheduler timing analysis
- thread synchronization
- cache locality investigation
- long-running process observation

## Limitations

This board is useful for embedded Linux fundamentals, but limited for deeper systems-performance work.

Limitations include:

- single-core CPU
- older Cortex-A5 architecture
- limited cache/memory hierarchy visibility
- limited profiling tooling
- no multicore scaling analysis
- no GPU or heterogeneous compute

## Transition to Phase 2

Phase 2 should move to a Linux desktop/workstation environment.

The next phase should focus on:

- multicore scaling
- thread contention
- lock-free structures
- cache locality
- CPU affinity
- scheduler analysis
- `perf`
- flamegraphs
- allocator behavior
- workload benchmarking

Phase 1 established the embedded Linux foundation. Phase 2 should scale the same investigation mindset onto modern multicore Linux hardware.

