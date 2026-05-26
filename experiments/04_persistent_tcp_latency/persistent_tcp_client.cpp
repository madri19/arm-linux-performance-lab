#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using Clock = std::chrono::high_resolution_clock;

constexpr int PORT = 9091;
constexpr int ITERATIONS = 1000;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server-ip>\n";
        return 1;
    }

    const char* server_ip = argv[1];
    const char* message = "latency-test";

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        std::cerr << "socket() failed\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "connect() failed\n";
        return 1;
    }

    char buffer[1024];
    long total_us = 0;

    for (int i = 0; i < ITERATIONS; ++i) {
        auto start = Clock::now();

        send(sock, message, strlen(message), 0);
        recv(sock, buffer, sizeof(buffer), 0);

        auto end = Clock::now();

        auto elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        total_us += elapsed;
    }

    close(sock);

    double avg_us = static_cast<double>(total_us) / ITERATIONS;

    std::cout << "Iterations: " << ITERATIONS << "\n";
    std::cout << "Average persistent round-trip latency: " << avg_us << " us\n";

    return 0;
}
