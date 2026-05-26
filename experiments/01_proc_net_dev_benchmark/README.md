# Experiment 01 - /proc/net/dev Benchmark

Measures the cost of reading and parsing `/proc/net/dev`
on an ARM Cortex-A5 embedded Linux system.

## Hardware

- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

## Method

The benchmark:
1. Opens `/proc/net/dev`
2. Reads all lines
3. Tokenizes/parses contents
4. Repeats 1000 iterations
5. Measures elapsed execution time

## Result

Iterations: 1000

Total time: 515322 us

Average per iteration: 515.322 us

## Notes

This measures combined overhead from:
- procfs access
- kernel/userspace transitions
- file I/O
- string parsing
- libc/libstdc++ runtime behavior
- ARM Cortex-A5 execution characteristics

