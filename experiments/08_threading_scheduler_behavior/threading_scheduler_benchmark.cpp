#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using Clock = std::chrono::high_resolution_clock;

constexpr int ITERATIONS = 100000;

// Shared queue between producer and consumer.
std::queue<int> work_queue;

// Mutex protecting queue access.
std::mutex queue_mutex;

// Condition variable for thread wakeups.
std::condition_variable cv;

// Signals benchmark completion.
bool done = false;

// ------------------------------------------------------------
// Consumer thread.
//
// Waits for work from producer.
// ------------------------------------------------------------
void consumer_thread() {

    while (true) {

        std::unique_lock<std::mutex> lock(queue_mutex);

        // Sleep until queue has work or benchmark ends.
        cv.wait(lock, [] {
            return !work_queue.empty() || done;
        });

        // Exit if benchmark finished.
        if (done && work_queue.empty())
            break;

        // Remove work item.
        work_queue.pop();
    }
}

int main() {

    // --------------------------------------------------------
    // Start consumer thread.
    // --------------------------------------------------------
    std::thread consumer(consumer_thread);

    auto start = Clock::now();

    // --------------------------------------------------------
    // Producer loop.
    //
    // Pushes work items and wakes consumer.
    // --------------------------------------------------------
    for (int i = 0; i < ITERATIONS; ++i) {

        {
            std::lock_guard<std::mutex> lock(queue_mutex);

            work_queue.push(i);
        }

        // Wake consumer thread.
        cv.notify_one();
    }

    // Signal benchmark completion.
    {
        std::lock_guard<std::mutex> lock(queue_mutex);

        done = true;
    }

    cv.notify_one();

    // Wait for consumer thread to exit.
    consumer.join();

    auto end = Clock::now();

    auto total_us =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start).count();

    double avg_us =
        static_cast<double>(total_us)
            / ITERATIONS;

    std::cout
        << "Iterations: "
        << ITERATIONS
        << "\n";

    std::cout
        << "Total time: "
        << total_us
        << " us\n";

    std::cout
        << "Average producer/consumer overhead: "
        << avg_us
        << " us\n";

    return 0;
}
