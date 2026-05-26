#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// ------------------------------------------------------------
// Monotonic timing clock.
//
// steady_clock avoids wall clock adjustments and is better
// suited for benchmark timing.
// ------------------------------------------------------------
using Clock = std::chrono::steady_clock;

// ------------------------------------------------------------
// Number of integers in dataset.
//
// 1,048,576 integers ≈ 4MB.
//
// Large enough to create meaningful cache behavior.
// ------------------------------------------------------------
constexpr size_t ELEMENT_COUNT = 1024 * 1024;

// Number of benchmark passes.
constexpr int ITERATIONS = 10;

// ------------------------------------------------------------
// Sequential memory traversal benchmark.
//
// Traverses memory linearly:
//
// data[0], data[1], data[2], ...
//
// Sequential traversal benefits from:
//
// - spatial locality
// - hardware prefetching
// - predictable cache access
// ------------------------------------------------------------
long benchmark_sequential(
    const std::vector<int>& data
) {
    // Volatile prevents optimizer elimination.
    volatile long long sink = 0;

    auto start = Clock::now();

    for (int iter = 0;
         iter < ITERATIONS;
         ++iter) {

        for (size_t i = 0;
             i < data.size();
             ++i) {

            sink += data[i];
        }
    }

    auto end = Clock::now();

    return
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();
}

// ------------------------------------------------------------
// Random memory traversal benchmark.
//
// Traverses memory using shuffled indices.
//
// Random traversal causes:
//
// - cache misses
// - poor spatial locality
// - unpredictable access patterns
// ------------------------------------------------------------
long benchmark_random(
    const std::vector<int>& data,
    const std::vector<size_t>& indices
) {
    volatile long long sink = 0;

    auto start = Clock::now();

    for (int iter = 0;
         iter < ITERATIONS;
         ++iter) {

        for (size_t index : indices) {

            sink += data[index];
        }
    }

    auto end = Clock::now();

    return
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();
}

int main() {

    // --------------------------------------------------------
    // Allocate integer dataset.
    // --------------------------------------------------------
    std::vector<int> data(ELEMENT_COUNT);

    // Fill dataset with predictable values.
    std::iota(
        data.begin(),
        data.end(),
        1
    );

    // --------------------------------------------------------
    // Create randomized traversal order.
    // --------------------------------------------------------
    std::vector<size_t> indices(ELEMENT_COUNT);

    std::iota(
        indices.begin(),
        indices.end(),
        0
    );

    // Fixed RNG seed ensures reproducibility.
    std::mt19937 rng(42);

    std::shuffle(
        indices.begin(),
        indices.end(),
        rng
    );

    // --------------------------------------------------------
    // Run sequential traversal benchmark.
    // --------------------------------------------------------
    long sequential_us =
        benchmark_sequential(data);

    // --------------------------------------------------------
    // Run random traversal benchmark.
    // --------------------------------------------------------
    long random_us =
        benchmark_random(
            data,
            indices
        );

    double sequential_avg =
        static_cast<double>(
            sequential_us
        ) / ITERATIONS;

    double random_avg =
        static_cast<double>(
            random_us
        ) / ITERATIONS;

    // --------------------------------------------------------
    // Print benchmark results.
    // --------------------------------------------------------
    std::cout
        << "Elements: "
        << ELEMENT_COUNT
        << "\n";

    std::cout
        << "Iterations: "
        << ITERATIONS
        << "\n\n";

    std::cout
        << "Sequential total: "
        << sequential_us
        << " us\n";

    std::cout
        << "Sequential avg: "
        << sequential_avg
        << " us\n\n";

    std::cout
        << "Random total: "
        << random_us
        << " us\n";

    std::cout
        << "Random avg: "
        << random_avg
        << " us\n\n";

    std::cout
        << "Random / Sequential ratio: "
        << random_avg / sequential_avg
        << "x\n";

    return 0;
}
