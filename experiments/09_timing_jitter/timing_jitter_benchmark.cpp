#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// Steady monotonic clock.
//
// Better suited for interval timing than wall clock time.
using Clock = std::chrono::steady_clock;

// Number of timing samples to collect.
constexpr int ITERATIONS = 1000;

// Requested sleep duration.
//
// 1000 microseconds = 1 millisecond.
constexpr int TARGET_SLEEP_US = 1000;

int main() {

    // --------------------------------------------------------
    // Stores measured sleep durations.
    // --------------------------------------------------------
    std::vector<long> observed_us;

    observed_us.reserve(ITERATIONS);

    // --------------------------------------------------------
    // Main timing measurement loop.
    // --------------------------------------------------------
    for (int i = 0; i < ITERATIONS; ++i) {

        // Record start timestamp.
        auto start = Clock::now();

        // ----------------------------------------------------
        // Request 1ms sleep from Linux scheduler.
        // ----------------------------------------------------
        std::this_thread::sleep_for(
            std::chrono::microseconds(
                TARGET_SLEEP_US
            )
        );

        // Record wakeup timestamp.
        auto end = Clock::now();

        // Calculate actual observed sleep duration.
        auto elapsed =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start
                ).count();

        observed_us.push_back(elapsed);
    }

    // --------------------------------------------------------
    // Find minimum and maximum observed timings.
    // --------------------------------------------------------
    auto min_it =
        std::min_element(
            observed_us.begin(),
            observed_us.end()
        );

    auto max_it =
        std::max_element(
            observed_us.begin(),
            observed_us.end()
        );

    long min_observed = *min_it;
    long max_observed = *max_it;

    // --------------------------------------------------------
    // Calculate average observed timing.
    // --------------------------------------------------------
    long total = 0;

    for (auto value : observed_us) {
        total += value;
    }

    double avg =
        static_cast<double>(total)
            / observed_us.size();

    // --------------------------------------------------------
    // Sort samples for percentile analysis.
    // --------------------------------------------------------
    std::sort(
        observed_us.begin(),
        observed_us.end()
    );

    // Median timing.
    auto p50 =
        observed_us[
            observed_us.size() * 50 / 100
        ];

    // 90th percentile timing.
    auto p90 =
        observed_us[
            observed_us.size() * 90 / 100
        ];

    // 99th percentile timing.
    auto p99 =
        observed_us[
            observed_us.size() * 99 / 100
        ];

    // --------------------------------------------------------
    // Print timing statistics.
    // --------------------------------------------------------
    std::cout
        << "Iterations: "
        << ITERATIONS
        << "\n";

    std::cout
        << "Target sleep: "
        << TARGET_SLEEP_US
        << " us\n";

    std::cout
        << "Min observed: "
        << min_observed
        << " us\n";

    std::cout
        << "Avg observed: "
        << avg
        << " us\n";

    std::cout
        << "P50 observed: "
        << p50
        << " us\n";

    std::cout
        << "P90 observed: "
        << p90
        << " us\n";

    std::cout
        << "P99 observed: "
        << p99
        << " us\n";

    std::cout
        << "Max observed: "
        << max_observed
        << " us\n";

    return 0;
}
