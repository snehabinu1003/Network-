#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void main() {
    int server_sock;
    struct sockaddr_in server_addr;
    socklen_t addr_size;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error\n");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1025);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.100");

    connect(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];
    int choice;
    char filename[100];
    FILE *fptr;

    while (1) {
        printf("\n1. Upload a file (PUT)\n2. Download a file (GET)\n3. Exit (BYE)\nEnter choice: ");
        scanf("%d", &choice);
