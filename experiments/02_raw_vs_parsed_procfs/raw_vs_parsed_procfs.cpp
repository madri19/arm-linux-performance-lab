#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using Clock = std::chrono::high_resolution_clock;

constexpr int iterations = 1000;

long benchmark_raw_read() {

    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {

        std::ifstream file("/proc/net/dev");

        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return -1;
        }

        // Read entire file into one string.
        std::string contents(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        // Prevent optimization.
        volatile auto sink = contents.size();
        (void)sink;
    }

    auto end = Clock::now();

    return std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();
}

long benchmark_parsed_read() {

    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {

        std::ifstream file("/proc/net/dev");

        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return -1;
        }

        std::string line;

        while (std::getline(file, line)) {

            std::stringstream ss(line);

            std::string token;

            while (ss >> token) {

                volatile auto sink = token.size();
                (void)sink;
            }
        }
    }

    auto end = Clock::now();

    return std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();
}

int main() {

    auto raw_us = benchmark_raw_read();

    auto parsed_us = benchmark_parsed_read();

    double raw_avg =
        static_cast<double>(raw_us) / iterations;

    double parsed_avg =
        static_cast<double>(parsed_us) / iterations;

    std::cout << "Iterations: "
              << iterations << "\n\n";

    std::cout << "[Raw Read]\n";
    std::cout << "Total: "
              << raw_us << " us\n";

    std::cout << "Average: "
              << raw_avg << " us\n\n";

    std::cout << "[Parsed Read]\n";
    std::cout << "Total: "
              << parsed_us << " us\n";

    std::cout << "Average: "
              << parsed_avg << " us\n\n";

    std::cout << "[Estimated Parsing Overhead]\n";
    std::cout << (parsed_avg - raw_avg)
              << " us per iteration\n";

    return 0;
}
