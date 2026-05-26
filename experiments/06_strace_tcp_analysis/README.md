# Experiment 06 - strace TCP Analysis

Uses `strace` to inspect syscall behavior during TCP latency experiments on ARM Cortex-A5 hardware.

## Goal

Move beyond latency measurements and inspect the actual Linux syscall activity involved in TCP echo benchmarks.

## Areas Investigated

- socket syscall frequency
- connect/send/recv/close behavior
- syscall timing
- persistent vs non-persistent connection behavior
- TCP_NODELAY behavior
- Linux userspace/kernel interaction

## Tools Used

- strace
- Buildroot Linux
- ARM Cortex-A5 board
- TCP socket benchmarks from previous experiments


## Example Syscall Summary

Observed while tracing the persistent TCP echo server:

    % time     seconds  usecs/call     calls syscall
    64.46    0.198858         198      1000 send
    35.23    0.108685         108      1001 recv
    0.05     0.000165         165         1 accept
    0.03     0.000097          97         1 socket

## Key Observation

Most measured syscall time was concentrated in repeated:

- send
- recv

Connection setup syscalls represented negligible overhead compared to the repeated request/response path.

