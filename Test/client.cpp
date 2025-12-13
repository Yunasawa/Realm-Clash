#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 5500
#define BACKLOG 5

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr{}, client_addr{};
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024] = "Welcome to the server!\n";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return 1;
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Listen
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept client
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        perror("accept failed");
        return 1;
    }

    std::cout << "Client connected!\n";

    // Send welcome message
    send(client_fd, buffer, strlen(buffer), 0);

    close(client_fd);
    close(server_fd);

    return 0;
}
