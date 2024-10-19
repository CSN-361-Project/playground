#include <iostream>
#include <cstring>     // For memset
#include <arpa/inet.h> // For sockaddr_in and inet_addr
#include <unistd.h>    // For close

#define PORT 12345
#define SERVER_IP "0.0.0.0" // Localhost IP

int main()
{
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Clear server_addr structure
    memset(&server_addr, 0, sizeof(server_addr));

    // Fill server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send message to server
    const char *message = "Hello, UDP Server!";
    sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    std::cout << "Message sent to server." << std::endl;

    // Receive response from server
    socklen_t len = sizeof(server_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &len);
    buffer[n] = '\0'; // Null-terminate the received string
    std::cout << "Received response from server: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
