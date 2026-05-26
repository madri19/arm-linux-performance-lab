#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// TCP port used by server.
constexpr int PORT = 9091;

// Maximum receive buffer size.
constexpr int BUFFER_SIZE = 1024;

int main() {

    // ------------------------------------------------------------
    // Create TCP socket.
    //
    // AF_INET     = IPv4
    // SOCK_STREAM = TCP
    // ------------------------------------------------------------
    int server_fd =
        socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {

        std::cerr << "socket() failed\n";

        return 1;
    }

    // ------------------------------------------------------------
    // Allow socket address reuse.
    //
    // Helps avoid bind() failures after restart.
    // ------------------------------------------------------------
    int opt = 1;

    setsockopt(server_fd,
               SOL_SOCKET,
               SO_REUSEADDR,
               &opt,
               sizeof(opt));

    // ------------------------------------------------------------
    // Configure server address structure.
    // ------------------------------------------------------------
    sockaddr_in address{};

    address.sin_family = AF_INET;

    // Accept connections on all interfaces.
    address.sin_addr.s_addr = INADDR_ANY;

    // Convert port to network byte order.
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
    // Start listening for incoming TCP connections.
    // ------------------------------------------------------------
    if (listen(server_fd, 1) < 0) {

        std::cerr << "listen() failed\n";

        return 1;
    }

    std::cout
        << "Persistent TCP echo server listening on port "
        << PORT
        << "\n";

    // ------------------------------------------------------------
    // Main server loop.
    // ------------------------------------------------------------
    while (true) {

        sockaddr_in client_addr{};

        socklen_t client_len =
            sizeof(client_addr);

        // --------------------------------------------------------
        // Accept incoming TCP connection.
        // --------------------------------------------------------
        int client_fd =
            accept(server_fd,
                   reinterpret_cast<sockaddr*>(
                       &client_addr),
                   &client_len);

        if (client_fd < 0) {

            std::cerr << "accept() failed\n";

            continue;
        }

        std::cout << "Client connected\n";

        char buffer[BUFFER_SIZE];

        // --------------------------------------------------------
        // Persistent connection loop.
        //
        // Same socket reused repeatedly.
        // --------------------------------------------------------
        while (true) {

            // Receive payload from client.
            ssize_t bytes_received =
                recv(client_fd,
                     buffer,
                     sizeof(buffer),
                     0);

            // Connection closed or error.
            if (bytes_received <= 0) {

                break;
            }

            // Echo payload back to client.
            send(client_fd,
                 buffer,
                 bytes_received,
                 0);
        }

        // Close client socket after disconnect.
        close(client_fd);

        std::cout << "Client disconnected\n";
    }

    close(server_fd);

    return 0;
}
