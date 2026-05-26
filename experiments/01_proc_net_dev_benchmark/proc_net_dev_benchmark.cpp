#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    constexpr int iterations = 1000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        std::ifstream file("/proc/net/dev");

        if (!file.is_open()) {
            std::cerr << "Failed to open /proc/net/dev\n";
            return 1;
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

    auto end = std::chrono::high_resolution_clock::now();

    auto total_us =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count();

    double avg_us = static_cast<double>(total_us) / iterations;

    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Total time: " << total_us << " us\n";
    std::cout << "Average per iteration: " << avg_us << " us\n";

    return 0;
}
