# Experiment 09 - Timing Jitter

Measures scheduler and wakeup timing behavior for short userspace sleeps on an ARM Cortex-A5 embedded Linux board.

## Goal

Investigate how accurately Linux userspace threads wake up after short sleep intervals.

This experiment measures timing jitter and scheduler wakeup overhead using repeated 1ms sleep intervals.

## Hardware

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

## Method

The benchmark performs 1000 iterations.

For each iteration:

1. Record current timestamp
2. Sleep for 1000 microseconds using `std::this_thread::sleep_for`
3. Record wakeup timestamp
4. Measure actual elapsed time

Statistics collected:

- minimum observed sleep
- average observed sleep
- P50 latency
- P90 latency
- P99 latency
- maximum observed sleep

## Result

Iterations: 1000

Target sleep: 1000 us

Min observed: 1081 us

Avg observed: 1094.15 us

P50 observed: 1093 us

P90 observed: 1103 us

P99 observed: 1108 us

Max observed: 1311 us

## Interpretation

The measured sleep intervals consistently overshot the 1000us target.

Observed behavior included:

- roughly 90-100us average overshoot
- relatively tight wakeup distribution
- occasional higher scheduler jitter spikes

This reflects normal Linux scheduler timing behavior on non-real-time systems.

Potential contributors include:

- scheduler wakeup latency
- timer granularity
- interrupt timing
- userspace/kernel transition overhead
- single-core scheduling constraints

## Why This Matters

This experiment expands the repository into scheduler timing analysis and wakeup jitter measurement.

The benchmark introduces:

- timing distribution analysis
- latency percentile measurement
- scheduler wakeup behavior
- short-duration timing accuracy investigation

This complements earlier networking, syscall, and threading experiments by examining Linux timing behavior directly.

