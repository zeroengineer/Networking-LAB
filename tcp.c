#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5060
#define BUFFER_SIZE 1024
int main() 
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");
    const char *message = "Hello, Server!";
    if (send(client_socket, message, strlen(message), 0) < 0) 
    {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to the server.\n");
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) 
    {
        perror("Receive failed");
    } 
    else 
    {
        printf("Response from server: %s\n", buffer);
    }
    close(client_socket);
    return 0;
}
