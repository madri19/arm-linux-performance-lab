# Experiment 06 - strace TCP Analysis

Uses `strace` to inspect Linux syscall behavior during the persistent TCP latency benchmark from Experiment 04.

## Goal

Investigate which Linux syscalls dominate the TCP echo request/response path on ARM Cortex-A5 hardware.

This experiment shifts from measuring latency externally to inspecting internal userspace/kernel interaction using Linux tracing tools.

## Hardware

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

### Host System
- macOS

## Tooling Used

- `strace`
- Buildroot Linux
- Dropbear SSH
- Persistent TCP echo benchmark from Experiment 04

## Procedure

The persistent TCP echo server from Experiment 04 was executed under `strace -c` on the ARM board:

    strace -c /root/persistent_tcp_server

The macOS client then executed 1000 persistent TCP echo iterations:

    ./persistent_tcp_client 192.168.1.188

After the benchmark completed, the server was interrupted and `strace` produced a syscall timing summary.

## Benchmark Result

Client-side latency result:

    Iterations: 1000
    Average persistent round-trip latency: 3286.43 us

## Syscall Summary

Observed server syscall activity:

    % time     seconds  usecs/call     calls syscall
    64.46    0.198858         198      1000 send
    35.23    0.108685         108      1001 recv
    0.05     0.000165         165         1 accept
    0.03     0.000097          97         1 socket

Total traced syscalls:

- 1000 send calls
- 1001 recv calls
- 1 accept call
- 1 socket call

## Key Observation

Most measured syscall time was concentrated in repeated:

- send
- recv

Connection setup syscalls represented negligible overhead compared to the repeated request/response path.

This supports earlier experiments showing that persistent TCP connections did not significantly reduce overall latency in this environment.

## Interpretation

The dominant latency cost appears to come from repeated message exchange behavior rather than connection setup.

Possible contributors include:

- scheduler wakeups
- blocking socket behavior
- Linux/macOS TCP stack timing
- userspace/kernel synchronization overhead
- Ethernet driver/network stack behavior

## Why This Matters

This experiment introduces Linux syscall tracing and low-level performance tooling into the workflow.

The repo now includes:

- latency benchmarking
- syscall-level analysis
- ARM Linux performance investigation
- Buildroot tooling workflows
- networking behavior analysis

