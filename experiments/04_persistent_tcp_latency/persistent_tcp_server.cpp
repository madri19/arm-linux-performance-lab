#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 9091;
constexpr int BUFFER_SIZE = 1024;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        std::cerr << "socket() failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        std::cerr << "bind() failed\n";
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        std::cerr << "listen() failed\n";
        return 1;
    }

    std::cout << "Persistent TCP echo server listening on port " << PORT << "\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

        if (client_fd < 0) {
            std::cerr << "accept() failed\n";
            continue;
        }

        std::cout << "Client connected\n";

        char buffer[BUFFER_SIZE];

        while (true) {
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

            if (bytes_received <= 0) {
                break;
            }

            send(client_fd, buffer, bytes_received, 0);
        }

        close(client_fd);
        std::cout << "Client disconnected\n";
    }

    close(server_fd);
    return 0;
}
