# ARM Embedded Linux Performance Lab

Systems-performance experiments on embedded ARM Linux hardware.

This repository focuses on understanding Linux systems behavior on constrained ARM hardware through hands-on benchmarking, profiling, networking, threading, and tooling experiments.

## Phase 1 Scope

This repository represents Phase 1 of a broader systems-performance engineering learning path.

Phase 1 focuses on:

- embedded Linux
- Buildroot
- ARM userspace behavior
- networking
- syscall behavior
- threading and scheduler behavior
- remote SSH workflows
- constrained hardware performance analysis

Future phases will move to separate repositories:

- Phase 2 — Linux desktop/workstation systems performance
- Phase 3 — GPU/HPC-style heterogeneous compute performance

## Hardware Platform

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10
- Ethernet networking
- UART serial console

### Host Environment
- macOS
- Lima Linux VM
- Buildroot cross-toolchain
- Remote SSH deployment workflow

## Areas of Exploration

- Linux scheduling behavior
- TCP/networking performance
- `/proc` and syscall overhead
- thread synchronization
- memory behavior
- ARM/Linux profiling workflows
- embedded Linux debugging
- Buildroot-based systems development

## Tooling and Skills Practiced

- Buildroot cross-compilation
- ARM Linux deployment
- UART serial debugging
- Dropbear SSH setup
- SCP remote deployment
- Linux `/proc` inspection
- TCP socket programming
- latency benchmarking
- hypothesis-driven performance testing
- syscall tracing with `strace`
- remote Linux performance workflows

## Linux Tooling Progression

The repository began with benchmark-focused experiments and gradually expanded into Linux tracing and systems investigation workflows.

Current tooling includes:

- `strace`
- `/proc`
- Buildroot userspace customization
- SSH deployment/debug workflows

Planned tooling areas:

- `perf`
- `top`
- `ps`
- packet capture tooling
- non-blocking socket analysis
- scheduler behavior analysis

## Experiments

| Experiment | Description |
|---|---|
| [01_proc_net_dev_benchmark](experiments/01_proc_net_dev_benchmark) | Benchmarking `/proc/net/dev` parsing overhead on ARM Cortex-A5 hardware |
| [02_raw_vs_parsed_procfs](experiments/02_raw_vs_parsed_procfs) | Comparing procfs read overhead against userspace parsing overhead |
| [03_tcp_echo_latency](experiments/03_tcp_echo_latency) | Measuring TCP round-trip latency between macOS and ARM Cortex-A5 hardware |
| [04_persistent_tcp_latency](experiments/04_persistent_tcp_latency) | Measuring latency using a persistent TCP connection between macOS and ARM Cortex-A5 hardware |
| [05_tcp_nodelay_latency](experiments/05_tcp_nodelay_latency) | Measuring TCP latency with TCP_NODELAY enabled |
| [06_strace_tcp_analysis](experiments/06_strace_tcp_analysis) | Inspecting TCP echo syscall behavior using `strace` on ARM Linux |
| [07_payload_size_scaling](experiments/07_payload_size_scaling) | Measuring TCP latency scaling as payload size increases |
| [08_threading_scheduler_behavior](experiments/08_threading_scheduler_behavior) | Measuring producer/consumer synchronization overhead and scheduler behavior on ARM Linux |

## Related Project

- https://github.com/madri19/sama5d3-network-monitor

