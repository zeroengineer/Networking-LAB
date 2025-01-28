#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 1024  // Match buffer size with server
#define FILE_NOT_FOUND "File Not Found!"

// Function to clear buffer
void clearBuf(char *b) {
    memset(b, 0, NET_BUF_SIZE);
}

// Function to receive a file
int recvFile(char *buf, int sockfd) {
    if (strncmp(buf, "END_OF_FILE", 11) == 0) {
        return 1; // End of file signal
    }

    printf("%s", buf); // Print the received content
    return 0;
}

int main() {
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    char net_buf[NET_BUF_SIZE];
    FILE *fp;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);
    if (sockfd < 0) {
        printf("Error in socket creation\n");
        return 0;
    } else {
        printf("Socket created successfully\n");
    }

    // Define server address
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("\nPlease enter the filename to receive: ");
        scanf("%s", net_buf);
        sendto(sockfd, net_buf, NET_BUF_SIZE, 0, (struct sockaddr *)&addr_con, addrlen);

        printf("\n--Data Received--\n");

        while (1) {
            clearBuf(net_buf);
            nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, 0, (struct sockaddr *)&addr_con, &addrlen);

            if (recvFile(net_buf, nBytes)) {
                break;
            }
        }
        printf("\nFile transfer complete.\n");
    }
    return 0;
}
