#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 15050
#define BUFFER_SIZE 1024

// Function to clear buffer
void clearBuffer(char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
}

// Function to send file
int sendFile(FILE *file, int sockfd, struct sockaddr_in clientAddr) {
    char buffer[BUFFER_SIZE];
    int nBytes;
    socklen_t addr_len = sizeof(clientAddr);

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        // Send each line from the file
        nBytes = sendto(sockfd, buffer, strlen(buffer), 0,
                        (struct sockaddr *)&clientAddr, addr_len);
        if (nBytes == -1) {
            perror("Error sending file data");
            return -1;
        }
        clearBuffer(buffer);
    }

    // Notify client file transfer is done
    strcpy(buffer, "END_OF_FILE");
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addr_len);
    return 0;
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    FILE *file;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("FTP Server is running on port %d...\n", PORT);

    socklen_t addr_len = sizeof(clientAddr);

    while (1) {
        // Clear buffer and wait for client request
        clearBuffer(buffer);
        printf("Waiting for file name from client...\n");

        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addr_len);
        printf("Client requested file: %s\n", buffer);

        // Open the requested file
        file = fopen(buffer, "r");
        if (file == NULL) {
            perror("File not found");
            strcpy(buffer, "File not found");
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addr_len);
        } else {
            printf("Sending file %s to client...\n", buffer);
            sendFile(file, sockfd, clientAddr);
            fclose(file);
            printf("File sent successfully.\n");
        }
    }

    close(sockfd);
    return 0;
}
