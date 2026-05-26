#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {

    // Number of times we want to repeat the benchmark.
    // Repeating many times reduces noise and gives a more stable average.
    constexpr int iterations = 1000;

    // Start high-resolution timer before benchmark loop begins.
    auto start = std::chrono::high_resolution_clock::now();

    // Main benchmark loop.
    // Each iteration performs:
    //   1. open /proc/net/dev
    //   2. read all lines
    //   3. tokenize contents
    //   4. close file automatically
    for (int i = 0; i < iterations; ++i) {

        // Open Linux virtual procfs file containing
        // network interface statistics.
        //
        // Example interfaces:
        //   eth0
        //   eth1
        //   lo
        //   can0
        //
        // Important:
        // /proc/net/dev is NOT a real disk file.
        // The Linux kernel dynamically generates this
        // content when userspace reads it.
        std::ifstream file("/proc/net/dev");

        // Verify file opened successfully.
        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return 1;
        }

        std::string line;

        // Read file line-by-line.
        while (std::getline(file, line)) {

            // Create string stream so we can tokenize
            // the line by whitespace.
            std::stringstream ss(line);

            std::string token;

            // Extract tokens from the line.
            //
            // Example line:
            //
            // eth1: 1838684 1681 0 0 ...
            //
            // Becomes tokens:
            //   eth1:
            //   1838684
            //   1681
            //   0
            //   0
            while (ss >> token) {

                // Prevent compiler optimization.
                //
                // Without this, the compiler could notice
                // token is unused and optimize away the
                // parsing work entirely.
                volatile auto sink = token.size();

                // Explicitly mark variable as used.
                (void)sink;
            }
        }

        // File automatically closes here when
        // std::ifstream goes out of scope.
    }

    // Stop timer after benchmark completes.
    auto end = std::chrono::high_resolution_clock::now();

    // Convert elapsed time into microseconds.
    auto total_us =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count();

    // Compute average time per iteration.
    double avg_us =
        static_cast<double>(total_us) / iterations;

    // Print benchmark results.
    std::cout << "Iterations: "
              << iterations << "\n";

    std::cout << "Total time: "
              << total_us << " us\n";

    std::cout << "Average per iteration: "
              << avg_us << " us\n";

    return 0;
}
