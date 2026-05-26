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

