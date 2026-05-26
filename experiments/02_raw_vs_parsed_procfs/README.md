# Experiment 02 - Raw vs Parsed procfs Read

Compares raw `/proc/net/dev` read overhead against read + token parsing overhead on ARM Cortex-A5 hardware.

## Hardware

- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

## Method

The benchmark runs 1000 iterations of two modes:

1. Raw read
   - open `/proc/net/dev`
   - read entire contents into a string

2. Parsed read
   - open `/proc/net/dev`
   - read line-by-line
   - tokenize each line using `std::stringstream`

## Result

Iterations: 1000

Raw read total: 406246 us  
Raw read average: 406.246 us

Parsed read total: 511532 us  
Parsed read average: 511.532 us

Estimated parsing overhead: 105.286 us per iteration

## Interpretation

Most of the cost comes from opening and reading the kernel-generated procfs file.

Approximate breakdown:

- procfs/file read overhead: ~79%
- userspace parsing overhead: ~21%

This suggests future optimization work should first investigate file access frequency and polling design before optimizing parser implementation.
