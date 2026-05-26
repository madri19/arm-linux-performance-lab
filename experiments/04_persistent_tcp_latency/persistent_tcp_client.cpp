#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// High-resolution timer alias.
using Clock = std::chrono::high_resolution_clock;

// TCP server port.
constexpr int PORT = 9091;

// Number of benchmark iterations.
constexpr int ITERATIONS = 1000;

int main(int argc, char* argv[]) {

    // ------------------------------------------------------------
    // Expect server IP as command-line argument.
    // ------------------------------------------------------------
    if (argc != 2) {

        std::cerr << "Usage: "
                  << argv[0]
                  << " <server-ip>\n";

        return 1;
    }

    const char* server_ip = argv[1];

    // Test payload sent to server.
    const char* message = "latency-test";

    // ------------------------------------------------------------
    // Create ONE persistent TCP socket.
    // ------------------------------------------------------------
    int sock =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    if (sock < 0) {

        std::cerr << "socket() failed\n";

        return 1;
    }

    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORT);

    // Convert string IP into binary form.
    inet_pton(AF_INET,
              server_ip,
              &server_addr.sin_addr);

    // ------------------------------------------------------------
    // Connect once to ARM board.
    //
    // Unlike Experiment 03, this connection
    // remains open for all iterations.
    // ------------------------------------------------------------
    if (connect(sock,
                reinterpret_cast<sockaddr*>(
                    &server_addr),
                sizeof(server_addr)) < 0) {

        std::cerr << "connect() failed\n";

        return 1;
    }

    char buffer[1024];

    long total_us = 0;

    // ------------------------------------------------------------
    // Main latency benchmark loop.
    // ------------------------------------------------------------
    for (int i = 0; i < ITERATIONS; ++i) {

        // Start round-trip timer.
        auto start = Clock::now();

        // Send payload to ARM board.
        send(sock,
             message,
             strlen(message),
             0);

        // Wait for echoed response.
        recv(sock,
             buffer,
             sizeof(buffer),
             0);

        // Stop timer.
        auto end = Clock::now();

        // Compute elapsed latency.
        auto elapsed =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start).count();

        total_us += elapsed;
    }

    // Close persistent TCP connection.
    close(sock);

    // Compute average latency.
    double avg_us =
        static_cast<double>(total_us)
            / ITERATIONS;

    // Print benchmark results.
    std::cout << "Iterations: "
              << ITERATIONS << "\n";

    std::cout
        << "Average persistent round-trip latency: "
        << avg_us
        << " us\n";

    return 0;
}
