#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// TCP server port.
constexpr int PORT = 9093;

// Maximum receive buffer size.
//
// Large enough for all payload tests.
constexpr int BUFFER_SIZE = 8192;

int main() {

    // ------------------------------------------------------------
    // Create TCP socket.
    // ------------------------------------------------------------
    int server_fd =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    // ------------------------------------------------------------
    // Allow immediate socket reuse after restart.
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

    address.sin_addr.s_addr = INADDR_ANY;

    address.sin_port = htons(PORT);

    // ------------------------------------------------------------
    // Bind socket to TCP port.
    // ------------------------------------------------------------
    bind(server_fd,
         reinterpret_cast<sockaddr*>(&address),
         sizeof(address));

    // ------------------------------------------------------------
    // Begin listening for incoming TCP connections.
    // ------------------------------------------------------------
    listen(server_fd, 1);

    std::cout
        << "Payload size server listening on port "
        << PORT
        << "\n";

    // ------------------------------------------------------------
    // Main server loop.
    // ------------------------------------------------------------
    while (true) {

        // Accept client connection.
        int client_fd =
            accept(server_fd,
                   nullptr,
                   nullptr);

        char buffer[BUFFER_SIZE];

        // --------------------------------------------------------
        // Persistent echo loop.
        // --------------------------------------------------------
        while (true) {

            // Receive payload from client.
            ssize_t n =
                recv(client_fd,
                     buffer,
                     sizeof(buffer),
                     0);

            // Connection closed or error.
            if (n <= 0)
                break;

            // Echo payload back.
            send(client_fd,
                 buffer,
                 n,
                 0);
        }

        close(client_fd);
    }

    close(server_fd);

    return 0;
}
