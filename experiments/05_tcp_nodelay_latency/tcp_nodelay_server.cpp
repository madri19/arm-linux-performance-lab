#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

// TCP server port.
constexpr int PORT = 9092;

// Maximum receive buffer size.
constexpr int BUFFER_SIZE = 1024;

int main() {

    // ------------------------------------------------------------
    // Create TCP socket.
    // ------------------------------------------------------------
    int server_fd =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    if (server_fd < 0) {

        std::cerr << "socket() failed\n";

        return 1;
    }

    // ------------------------------------------------------------
    // Allow socket address reuse.
    // ------------------------------------------------------------
    int opt = 1;

    setsockopt(server_fd,
               SOL_SOCKET,
               SO_REUSEADDR,
               &opt,
               sizeof(opt));

    // ------------------------------------------------------------
    // Configure server address.
    // ------------------------------------------------------------
    sockaddr_in address{};

    address.sin_family = AF_INET;

    address.sin_addr.s_addr = INADDR_ANY;

    address.sin_port = htons(PORT);

    // ------------------------------------------------------------
    // Bind socket to TCP port.
    // ------------------------------------------------------------
    if (bind(server_fd,
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) < 0) {

        std::cerr << "bind() failed\n";

        return 1;
    }

    // ------------------------------------------------------------
    // Begin listening for incoming connections.
    // ------------------------------------------------------------
    if (listen(server_fd, 1) < 0) {

        std::cerr << "listen() failed\n";

        return 1;
    }

    std::cout
        << "TCP_NODELAY echo server listening on port "
        << PORT
        << "\n";

    // ------------------------------------------------------------
    // Main server loop.
    // ------------------------------------------------------------
    while (true) {

        sockaddr_in client_addr{};

        socklen_t client_len =
            sizeof(client_addr);

        // Accept incoming client connection.
        int client_fd =
            accept(server_fd,
                   reinterpret_cast<sockaddr*>(
                       &client_addr),
                   &client_len);

        if (client_fd < 0) {

            std::cerr << "accept() failed\n";

            continue;
        }

        // --------------------------------------------------------
        // Disable Nagle's algorithm on client socket.
        //
        // TCP_NODELAY forces packets to send immediately
        // instead of waiting for coalescing.
        // --------------------------------------------------------
        int flag = 1;

        setsockopt(client_fd,
                   IPPROTO_TCP,
                   TCP_NODELAY,
                   &flag,
                   sizeof(flag));

        char buffer[BUFFER_SIZE];

        // Persistent echo loop.
        while (true) {

            ssize_t bytes_received =
                recv(client_fd,
                     buffer,
                     sizeof(buffer),
                     0);

            if (bytes_received <= 0) {

                break;
            }

            send(client_fd,
                 buffer,
                 bytes_received,
                 0);
        }

        close(client_fd);
    }

    close(server_fd);

    return 0;
}
