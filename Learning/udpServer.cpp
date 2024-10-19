#include <iostream>
#include <cstring>     // For memset
#include <arpa/inet.h> // For sockaddr_in and inet_addr
#include <unistd.h>    // For close

#define PORT 12345

int main()
{
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Clear server_addr structure
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Fill server information
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface
    server_addr.sin_port = htons(PORT);       // Port number

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "UDP Server is up and listening..." << std::endl;

    socklen_t len = sizeof(client_addr); // Length of client address

    while (true)
    {
        // Receive message from client
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0'; // Null-terminate the received string
        std::cout << "Received message: " << buffer << std::endl;

        // Send response to client
        const char *response = "Message received";
        sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&client_addr, len);
        std::cout << "Response sent." << std::endl;
    }

    close(sockfd);
    return 0;
}
