#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    
    // Create and bind the socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);  // return file descriptor for new socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));

    // Listen for connections
    listen(server_socket, 5);  // maximum pending connections
    printf("[*] server listening on port %d...\n", SERVER_PORT);

    // Accept a connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&address, 
                             (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("[*] Client connected\n");

    // Receive and save file
    FILE *file = fopen("recv.txt", "wb");
    int bytes_read;
    while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_read, file);
    }

    printf(" [*] Received at 'recv.txt'\n");
    fclose(file);
    close(client_socket);
    close(server_socket);
    return 0;
}
