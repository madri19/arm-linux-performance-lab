#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Alias for high-resolution clock type.
// Used for benchmark timing measurements.
using Clock = std::chrono::high_resolution_clock;

// Number of benchmark iterations.
// Higher counts reduce timing noise and produce
// more stable averages.
constexpr int iterations = 1000;

// ------------------------------------------------------------
// Benchmark 1:
// Measure cost of ONLY reading /proc/net/dev
// without parsing/tokenizing.
// ------------------------------------------------------------
long benchmark_raw_read() {

    // Start benchmark timer.
    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {

        // Open Linux procfs virtual file.
        //
        // Important:
        // This is NOT a normal disk file.
        // The Linux kernel dynamically generates
        // these contents during read operations.
        std::ifstream file("/proc/net/dev");

        // Verify file opened correctly.
        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return -1;
        }

        // Read entire file contents into a single string.
        //
        // This measures:
        //   - procfs generation overhead
        //   - kernel/userspace transitions
        //   - file I/O overhead
        //   - memory copy behavior
        //
        // But NOT string tokenization/parsing.
        std::string contents(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        // Prevent compiler optimization.
        //
        // Without this, compiler could notice
        // contents is unused and eliminate the
        // read entirely.
        volatile auto sink = contents.size();

        (void)sink;
    }

    // Stop benchmark timer.
    auto end = Clock::now();

    // Return elapsed time in microseconds.
    return std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();
}

// ------------------------------------------------------------
// Benchmark 2:
// Measure cost of reading AND parsing /proc/net/dev
// ------------------------------------------------------------
long benchmark_parsed_read() {

    // Start benchmark timer.
    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {

        // Open procfs network statistics file.
        std::ifstream file("/proc/net/dev");

        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return -1;
        }

        std::string line;

        // Read file line-by-line.
        while (std::getline(file, line)) {

            // Create stringstream object so we can
            // tokenize each line by whitespace.
            std::stringstream ss(line);

            std::string token;

            // Extract whitespace-separated tokens.
            //
            // Example line:
            //
            // eth1: 1838684 1681 0 0 ...
            //
            // Becomes:
            //   eth1:
            //   1838684
            //   1681
            //   0
            //   0
            while (ss >> token) {

                // Prevent compiler optimization.
                volatile auto sink = token.size();

                (void)sink;
            }
        }
    }

    // Stop benchmark timer.
    auto end = Clock::now();

    // Return elapsed time in microseconds.
    return std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();
}

int main() {

    // Run raw read benchmark.
    auto raw_us = benchmark_raw_read();

    // Run parsed read benchmark.
    auto parsed_us = benchmark_parsed_read();

    // Compute average time per iteration.
    double raw_avg =
        static_cast<double>(raw_us) / iterations;

    double parsed_avg =
        static_cast<double>(parsed_us) / iterations;

    // Print benchmark summary.
    std::cout << "Iterations: "
              << iterations << "\n\n";

    // Raw read results.
    std::cout << "[Raw Read]\n";

    std::cout << "Total: "
              << raw_us << " us\n";

    std::cout << "Average: "
              << raw_avg << " us\n\n";

    // Parsed read results.
    std::cout << "[Parsed Read]\n";

    std::cout << "Total: "
              << parsed_us << " us\n";

    std::cout << "Average: "
              << parsed_avg << " us\n\n";

    // Estimate parsing overhead by subtracting
    // raw read cost from parsed read cost.
    //
    // Approximation:
    //   parsing cost ≈ parsed - raw
    //
    // This helps isolate userspace parsing overhead
    // from procfs/file I/O overhead.
    std::cout << "[Estimated Parsing Overhead]\n";

    std::cout << (parsed_avg - raw_avg)
              << " us per iteration\n";

    return 0;
}
