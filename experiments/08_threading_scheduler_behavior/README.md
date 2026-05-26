# Experiment 08 - Threading and Scheduler Behavior

Measures producer/consumer synchronization overhead on an ARM Cortex-A5 embedded Linux board.

## Goal

Introduce threading and scheduler behavior into the ARM Embedded Linux Performance Lab.

This experiment measures the overhead of passing work between two userspace threads using:

- `std::thread`
- `std::mutex`
- `std::condition_variable`
- shared queue synchronization

## Hardware

### Target Board
- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10.80-linux4microchip

## Method

The benchmark creates:

- one producer thread
- one consumer thread
- a shared queue
- a mutex protecting the queue
- a condition variable for wakeups

The producer pushes 100,000 work items into the queue.

The consumer waits on a condition variable, wakes when work is available, and removes work items from the queue.

## Result

Iterations: 100000

Total time: 108474 us

Average producer/consumer overhead: 1.08474 us

## Interpretation

This experiment provides a first baseline for userspace thread synchronization overhead on the Cortex-A5 board.

The measured overhead includes:

- mutex lock/unlock cost
- condition variable wakeups
- queue access
- userspace scheduler behavior
- single-core ARM execution constraints

Because the board is single-core, this does not measure multicore parallel execution. It measures coordination overhead between threads sharing one CPU.

## Why This Matters

The NVIDIA role emphasizes operating systems, concurrency, performance analysis, and complex software stack behavior.

This experiment adds a concurrency/scheduler dimension to the repo, complementing earlier networking and syscall experiments.

