#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 1025
#define BUFFER_SIZE 1024

void handle_client(int client_sock);

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("172.16.9.9");

    // Bind the server socket
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        close(server_sock);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sock, 1024) < 0) {
        perror("Listen error");
        close(server_sock);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &addr_size);
    if (client_sock < 0) {
        perror("Accept error");
        close(server_sock);
        exit(1);
    }

    // Handle client request
    handle_client(client_sock);

    // Close server socket after client disconnects
    close(server_sock);
    return 0;
}

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    char filename[100];
    FILE *fptr;

    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive request from client
        ssize_t recv_len = recv(client_sock, buffer, sizeof(buffer), 0);
        if (recv_len <= 0) {
            if (recv_len == 0) {
                printf("Client disconnected\n");
            } else {
                perror("Recv error");
            }
            break;
        }

        // Handle PUT request
        if (strcmp(buffer, "PUT") == 0) {
            memset(buffer, 0, sizeof(buffer));

            // Receive filename from client
            recv(client_sock, buffer, sizeof(buffer), 0);
            strcpy(filename, buffer);
            memset(buffer, 0, sizeof(buffer));

            // Open file to write
            fptr = fopen(filename, "w");
            if (fptr == NULL) {
                perror("File open error");
                break;
            }

            // Receive file data
            while (1) {
                recv(client_sock, buffer, sizeof(buffer), 0);
                if (strcmp(buffer, "END$") == 0) {
                    break;
                }
                fprintf(fptr, "%s", buffer);
                memset(buffer, 0, sizeof(buffer));
            }

            printf("File '%s' received successfully\n", filename);
            fclose(fptr);
        }

        // Handle GET request
        else if (strcmp(buffer, "GET") == 0) {
            memset(buffer, 0, sizeof(buffer));

            // Send server process ID
            sprintf(buffer, "%d", getpid());
            send(client_sock, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));

            // Receive filename from client
            recv(client_sock, buffer, sizeof(buffer), 0);
            strcpy(filename, buffer);
memset(buffer, 0, sizeof(buffer));

            // Open file to read
            fptr = fopen(filename, "r");
            if (!fptr) {
                printf("File '%s' does not exist\n", filename);
                strcpy(buffer, "404");
                send(client_sock, buffer, sizeof(buffer), 0);
            } else {
                strcpy(buffer, "200");
                send(client_sock, buffer, sizeof(buffer), 0);

                // Send file data
                while (fgets(buffer, sizeof(buffer), fptr)) {
                    send(client_sock, buffer, sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }

                // Send END signal
                strcpy(buffer, "END$");
                send(client_sock, buffer, sizeof(buffer), 0);
                printf("File '%s' sent successfully\n", filename);
                fclose(fptr);
            }
        }

        // Handle BYE request (close connection)
        else if (strcmp(buffer, "BYE") == 0) {
            close(client_sock);
            printf("Connection closed\n");
            break;
        }
    }
}
