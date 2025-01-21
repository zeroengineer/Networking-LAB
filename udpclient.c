#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5001
#define BUFFER_SIZE 1024

int main() 
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t server_addr_len;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    const char *message = "Hello, Server!";
    if (sendto(client_socket, message, strlen(message), 0, 
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    printf("Message sent to the server.\n");
    server_addr_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, 
                                      (struct sockaddr*)&server_addr, &server_addr_len);
    if (bytes_received < 0) 
    {
        perror("Receive failed");
    } 
    else 
    {
        buffer[bytes_received] = '\0';  
        printf("Response from server: %s\n", buffer);
    }
    close(client_socket);
    return 0;
}
