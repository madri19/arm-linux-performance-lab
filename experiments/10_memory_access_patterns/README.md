# Experiment 10 - Memory Access Patterns
Measures the performance difference between sequential and random memory access patterns on an ARM Cortex-A5 embedded Linux board.
## Goal
Investigate the impact of memory locality and cache behavior on execution performance.
This experiment compares:
- sequential array traversal
- randomized array traversal
using the same dataset and workload size.
## Hardware
### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip
## Method
The benchmark allocates a vector containing:
- 1,048,576 integers
Two traversal patterns are measured.
### Sequential Access
The array is traversed linearly:
    data[0], data[1], data[2], ...
### Random Access
The same array is traversed using shuffled indices.
Each workload is repeated 10 times.
Execution time is measured using `std::chrono::steady_clock`.
## Result
Elements: 1048576
Iterations: 10
Sequential total: 231008 us
Sequential avg: 23100.8 us
Random total: 2228990 us
Random avg: 222899 us
Random / Sequential ratio: 9.64897x
## Interpretation
Random memory access was nearly 10x slower than sequential traversal.
Sequential traversal benefits from:
- cache locality
- hardware prefetching
- predictable memory access patterns
- reduced cache miss frequency
Random traversal introduces:
- frequent cache misses
- reduced spatial locality
- unpredictable memory access behavior
- increased memory fetch overhead
This experiment demonstrates how memory access patterns can dominate performance even when the amount of work remains identical.
## Why This Matters
This experiment expands the repository into memory behavior and cache locality analysis.
The benchmark introduces:
- cache-friendly vs cache-unfriendly workloads
- memory locality effects
- sequential vs random traversal behavior
- performance sensitivity to access patterns
This complements earlier networking, syscall, threading, and scheduler experiments by examining memory-system behavior directly.
