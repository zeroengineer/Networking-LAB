#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5001
#define BUFFER_SIZE 1024

int main() 
{
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("UDP server is waiting for data...\n");
    client_addr_len = sizeof(client_addr);
    ssize_t bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, 
                                      (struct sockaddr*)&client_addr, &client_addr_len);
    if (bytes_received < 0) 
    {
        perror("Receive failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0'; 
    printf("Received: %s from %s:%d\n", buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    const char *response = "Hello, Client!";
    if (sendto(server_socket, response, strlen(response), 0, 
               (struct sockaddr*)&client_addr, client_addr_len) < 0) 
               {
        perror("Send failed");
    }
    close(server_socket);
    
    return 0;
}
