# Experiment 11 - Long-Running Observation

Runs a sustained workload on the ARM Cortex-A5 board while periodically observing Linux process and system state.

## Goal

Move beyond short microbenchmarks and observe embedded Linux behavior over time.

This experiment tracks:

- process memory usage
- free system memory
- load average
- process stability
- sustained workload behavior

## Hardware

### Target Board

- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

## Method

The benchmark runs for 60 seconds.

During the run, it repeatedly performs CPU work over a fixed-size in-memory working set.

Every 5 seconds, it samples:

- `/proc/meminfo`
- `/proc/self/status`
- `/proc/loadavg`

The goal is to observe whether memory usage remains stable and how load changes under sustained execution.

## Result

    Duration: 60 seconds
    Sample interval: 5 seconds

    Initial MemFree: 224184 kB
    Final MemFree:   224184 kB

    Initial VmRSS:   2668 kB
    Final VmRSS:     2668 kB

    Initial loadavg: 0.08 0.02 0.01
    Final loadavg:   0.63 0.18 0.06

## Key Observation

Memory usage remained stable during the full 60-second run.

The process RSS stayed constant at approximately 2668 kB.

Free memory also remained constant, suggesting no visible memory growth or leak behavior during the observation window.

Load average gradually increased while the workload was running, showing the system accumulating CPU load under sustained execution.

## Interpretation

This experiment demonstrates basic operational observation of a long-running workload on embedded Linux.

The workload remained stable over time:

- no visible RSS growth
- no visible free-memory degradation
- no process crash
- gradual load average increase under sustained CPU work

This complements earlier microbenchmarks by introducing longer-duration system observation.

## Why This Matters

Real systems work is not only about isolated benchmark numbers.

Long-running observation helps identify:

- memory growth
- stability issues
- load behavior
- process runtime characteristics
- system-level behavior over time

This experiment closes the Phase 1 embedded Linux lab by adding operational runtime observation to the earlier benchmarking and profiling work.

