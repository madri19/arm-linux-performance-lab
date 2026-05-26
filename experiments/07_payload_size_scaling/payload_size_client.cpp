#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// High-resolution timer alias.
using Clock = std::chrono::high_resolution_clock;

// TCP server port.
constexpr int PORT = 9093;

// Benchmark iterations per payload size.
constexpr int ITERATIONS = 1000;

// ------------------------------------------------------------
// Executes one benchmark run for a specific payload size.
// ------------------------------------------------------------
long run_test(const char* server_ip,
              int payload_size) {

    // Create TCP socket.
    int sock =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORT);

    // Convert server IP string into binary form.
    inet_pton(AF_INET,
              server_ip,
              &server_addr.sin_addr);

    // Connect to ARM board.
    connect(sock,
            reinterpret_cast<sockaddr*>(
                &server_addr),
            sizeof(server_addr));

    // --------------------------------------------------------
    // Create payload buffer of requested size.
    // --------------------------------------------------------
    std::string payload(payload_size, 'x');

    // Receive buffer.
    std::vector<char> buffer(payload_size);

    long total_us = 0;

    // --------------------------------------------------------
    // Main latency benchmark loop.
    // --------------------------------------------------------
    for (int i = 0; i < ITERATIONS; ++i) {

        auto start = Clock::now();

        // Send payload.
        send(sock,
             payload.data(),
             payload.size(),
             0);

        // ----------------------------------------------------
        // Receive full echoed payload.
        //
        // recv() may return partial data.
        // ----------------------------------------------------
        size_t received = 0;

        while (received < payload.size()) {

            ssize_t n =
                recv(sock,
                     buffer.data() + received,
                     buffer.size() - received,
                     0);

            if (n <= 0)
                break;

            received += n;
        }

        auto end = Clock::now();

        total_us +=
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start).count();
    }

    close(sock);

    return total_us;
}

int main(int argc, char* argv[]) {

    // Expect server IP argument.
    if (argc != 2) {

        std::cerr
            << "Usage: "
            << argv[0]
            << " <server-ip>\n";

        return 1;
    }

    // Payload sizes to benchmark.
    int sizes[] = {
        16,
        64,
        256,
        1024,
        4096
    };

    std::cout
        << "Iterations per payload: "
        << ITERATIONS
        << "\n\n";

    // --------------------------------------------------------
    // Run benchmark for each payload size.
    // --------------------------------------------------------
    for (int size : sizes) {

        long total_us =
            run_test(argv[1], size);

        double avg_us =
            static_cast<double>(total_us)
                / ITERATIONS;

        std::cout
            << "Payload: "
            << size
            << " bytes\n";

        std::cout
            << "Average RTT: "
            << avg_us
            << " us\n\n";
    }

    return 0;
}
