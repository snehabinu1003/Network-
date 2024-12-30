#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>  // Add this header for read(), write(), and close()

int main() {
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char message[50];

    // Create socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Initialize server address structure
    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
perror("Bind failed");
        exit(1);
    }

    // Prepare for incoming connections
    bzero((char*)&clientaddr, sizeof(clientaddr));
    len = sizeof(clientaddr);

    // Listen for incoming connections
    if (listen(serversocket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("\nWaiting for client connection...\n");
    clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
    if (clientsocket < 0) {
        perror("Client connection failed");
        exit(1);
    }

    printf("\nClient connectivity received.\n");
    printf("\nReading message from the client.\n");

    // Read the message from the client
    read(clientsocket, message, sizeof(message)-1);
message[sizeof(message)-1] = '\0';  // Null-terminate the message
    printf("\nThe client has sent: %s\n", message);

    printf("\nSending message to the client.\n");

    // Send response to the client
    write(clientsocket, "YOUR MESSAGE RECEIVED.", strlen("YOUR MESSAGE RECEIVED."));

    // Close the client and server sockets
    close(clientsocket);
    close(serversocket);

    return 0;
}


