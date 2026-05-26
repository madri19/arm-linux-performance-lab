#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

// High-resolution timer alias.
using Clock = std::chrono::high_resolution_clock;

// TCP server port.
constexpr int PORT = 9092;

// Number of benchmark iterations.
constexpr int ITERATIONS = 1000;

int main(int argc, char* argv[]) {

    // ------------------------------------------------------------
    // Expect server IP address.
    // ------------------------------------------------------------
    if (argc != 2) {

        std::cerr << "Usage: "
                  << argv[0]
                  << " <server-ip>\n";

        return 1;
    }

    const char* server_ip = argv[1];

    const char* message = "latency-test";

    // ------------------------------------------------------------
    // Create persistent TCP socket.
    // ------------------------------------------------------------
    int sock =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    if (sock < 0) {

        std::cerr << "socket() failed\n";

        return 1;
    }

    // ------------------------------------------------------------
    // Disable Nagle's algorithm.
    //
    // Forces immediate packet transmission.
    // ------------------------------------------------------------
    int flag = 1;

    setsockopt(sock,
               IPPROTO_TCP,
               TCP_NODELAY,
               &flag,
               sizeof(flag));

    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET,
              server_ip,
              &server_addr.sin_addr);

    // Connect to ARM board.
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
    // Main benchmark loop.
    // ------------------------------------------------------------
    for (int i = 0; i < ITERATIONS; ++i) {

        auto start = Clock::now();

        // Send small payload immediately.
        send(sock,
             message,
             strlen(message),
             0);

        // Wait for echoed response.
        recv(sock,
             buffer,
             sizeof(buffer),
             0);

        auto end = Clock::now();

        total_us +=
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start).count();
    }

    close(sock);

    double avg_us =
        static_cast<double>(total_us)
            / ITERATIONS;

    std::cout << "Iterations: "
              << ITERATIONS << "\n";

    std::cout
        << "Average TCP_NODELAY round-trip latency: "
        << avg_us
        << " us\n";

    return 0;
}
