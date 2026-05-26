# ARM Linux Performance Lab

Systems-performance experiments on embedded ARM Linux hardware.

Focused on:
- Profiling
- Threading
- Linux tooling
- ARM behavior
- Networking
- Debugging
- Buildroot
- Performance analysis on Cortex-A5 hardware

## Hardware

- Atmel ATSAMA5D36
- ARM Cortex-A5
- ARMv7-A
- Buildroot Linux
- Linux 5.10
- Ethernet networking
- UART serial console

## Areas of Exploration

- Linux scheduling behavior
- TCP/networking performance
- `/proc` and syscall overhead
- Thread synchronization
- Memory allocation behavior
- ARM/Linux profiling workflows
- Embedded Linux debugging
- Buildroot-based systems development

## Development Environment

- macOS host
- Lima Linux VM
- Buildroot cross-toolchain
- Remote SSH deployment to target board


## Tools Used / Skills Practiced

- Buildroot cross-compilation
- ARM Linux deployment
- UART serial console debugging
- Dropbear SSH setup
- SCP remote deployment
- Linux `/proc` inspection
- TCP socket programming
- Latency benchmarking
- Hypothesis-driven performance testing
- macOS host ↔ ARM target workflows

## Next Tooling Focus

After Experiment 05, the investigation begins shifting more heavily into Linux performance tooling and syscall-level analysis.

Upcoming experiments will add deeper Linux performance/debugging tools:

- `strace` for syscall inspection
- `perf` for CPU/profile analysis where available
- `top`/`ps` for runtime process observation
- `/proc` and `/sys` for kernel/userspace interface inspection


## Experiments

| Experiment | Description |
|---|---|
| [01_proc_net_dev_benchmark](experiments/01_proc_net_dev_benchmark) | Benchmarking `/proc/net/dev` parsing overhead on ARM Cortex-A5 hardware |
| [02_raw_vs_parsed_procfs](experiments/02_raw_vs_parsed_procfs) | Comparing procfs read overhead against userspace parsing overhead |
| [03_tcp_echo_latency](experiments/03_tcp_echo_latency) | Measuring TCP round-trip latency between macOS and ARM Cortex-A5 hardware |
| [04_persistent_tcp_latency](experiments/04_persistent_tcp_latency) | Measuring latency using a persistent TCP connection between macOS and ARM Cortex-A5 hardware |
| [05_tcp_nodelay_latency](experiments/05_tcp_nodelay_latency) | Measuring TCP latency with TCP_NODELAY enabled |

## Related Project

- https://github.com/madri19/sama5d3-network-monitor

