#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Monotonic clock for elapsed-time measurement.
using Clock = std::chrono::steady_clock;

// Total observation duration.
constexpr int DURATION_SECONDS = 60;

// How often to print system/process observations.
constexpr int SAMPLE_INTERVAL_SECONDS = 5;

// Size of in-memory working set.
//
// 256K integers ≈ 1MB.
constexpr size_t WORKING_SET_SIZE = 256 * 1024;

// ------------------------------------------------------------
// Reads the first line in a procfs file matching a prefix.
//
// Used for extracting specific values from:
// - /proc/meminfo
// - /proc/self/status
// - /proc/loadavg
// ------------------------------------------------------------
std::string read_first_matching_line(
    const std::string& path,
    const std::string& prefix
) {
    std::ifstream file(path);

    std::string line;

    while (std::getline(file, line)) {

        // Empty prefix means return the first line.
        if (prefix.empty() || line.rfind(prefix, 0) == 0) {
            return line;
        }
    }

    return "not found";
}

// ------------------------------------------------------------
// CPU workload over fixed-size vector.
//
// Mutates the vector so compiler cannot remove the work.
// ------------------------------------------------------------
void cpu_workload(std::vector<int>& data) {

    volatile long long sink = 0;

    for (size_t i = 0;
         i < data.size();
         ++i) {

        sink += data[i];

        data[i] =
            static_cast<int>(
                (data[i] + sink) % 1024
            );
    }
}

int main() {

    // Fixed-size working set used for sustained workload.
    std::vector<int> data(
        WORKING_SET_SIZE,
        1
    );

    auto start = Clock::now();

    int sample = 0;

    while (true) {

        auto now = Clock::now();

        auto elapsed_seconds =
            std::chrono::duration_cast<
                std::chrono::seconds>(
                    now - start
                ).count();

        if (elapsed_seconds >= DURATION_SECONDS) {
            break;
        }

        // Perform repeated CPU work between samples.
        for (int i = 0; i < 100; ++i) {
            cpu_workload(data);
        }

        // Print one sample every SAMPLE_INTERVAL_SECONDS.
        if (elapsed_seconds / SAMPLE_INTERVAL_SECONDS >= sample) {

            std::cout
                << "=== Sample "
                << sample
                << " ===\n";

            std::cout
                << "Elapsed: "
                << elapsed_seconds
                << " seconds\n";

            std::cout
                << read_first_matching_line(
                    "/proc/meminfo",
                    "MemTotal:"
                )
                << "\n";

            std::cout
                << read_first_matching_line(
                    "/proc/meminfo",
                    "MemFree:"
                )
                << "\n";

            std::cout
                << read_first_matching_line(
                    "/proc/self/status",
                    "VmRSS:"
                )
                << "\n";

            std::cout
                << read_first_matching_line(
                    "/proc/loadavg",
                    ""
                )
                << "\n\n";

            ++sample;
        }
    }

    std::cout << "Observation complete.\n";

    return 0;
}
