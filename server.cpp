/*
UDP Server :

1. Create a UDP socket.
2. Bind the socket to the server address.
3. Wait until the datagram packet arrives from the client.
4. Process the datagram packet and send a reply to the client.
5. Go back to Step 3.

*/

#include <iostream>
#include <fstream>
// #include <bits/stdc++.h>

/*
This is a standard C library header that provides functions for performing general functions
such as memory allocation, process control, conversions, and others.
*/
#include <cstdlib>
/*
 This is a POSIX standard header that provides access to the POSIX operating system API.
 It includes various constants, types, and function declarations for system calls
 such as read, write, close, fork, exec, and others.
 */
#include <unistd.h>
#include <cstring>
#include <sys/types.h>  // his header file defines data types used in system calls. It includes definitions for types such as pid_t, off_t, size_t, and others.
#include <sys/socket.h> //This header file defines socket-related functions and data structures. It includes definitions for creating and manipulating sockets, which are endpoints for communication.
#include <arpa/inet.h>  // This header file provides definitions for internet operations. It includes functions for manipulating IP addresses and converting them between different formats.
#include <netinet/in.h> // This header file defines constants and structures needed for internet domain addresses. It includes definitions for the sockaddr_in structure, which is used to specify addresses for sockets.

#define PORT 8080
#define MAXLINE 1024

void printSockaddrIn(const struct sockaddr_in &addr)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    std::cout << "sockaddr_in {" << std::endl;
    std::cout << "  sin_family: " << addr.sin_family << std::endl;
    std::cout << "  sin_port: " << ntohs(addr.sin_port) << std::endl;
    std::cout << "  sin_addr: " << ip << std::endl;
    std::cout << "}" << std::endl;
}

void listenToClient(int &sockfd, sockaddr_in &cliaddr, char buffer[])
{
    socklen_t len;
    int n;

    len = sizeof(cliaddr); // len is value/result

    std::cout << "Listening for client\n";

    if ((n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                      MSG_WAITALL, (struct sockaddr *)&cliaddr,
                      &len)) < 0)
    {
        perror("receive failed");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    // Print client info
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(cliaddr.sin_port);

    std::cout << "Client IP: " << client_ip << std::endl;
    std::cout << "Client Port: " << client_port << std::endl;

    printf("\tClient : %s\n", buffer);
}

int main()
{
    /**
     * Socket file descriptor
     */
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    int _domain = AF_INET;  // IPv4
    int _type = SOCK_DGRAM; // UDP
    int _protocol = 0;      // Default protocol for UDP

    // create socket file descriptor
    if ((sockfd = socket(_domain, _type, _protocol)) < 0)
    {
        // std::cerr << "Error creating socket" << std::endl;
        // return 1;
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    std::memset(&servaddr, 0, sizeof(servaddr));
    std::memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Print the struct
    printSockaddrIn(servaddr);

    // bind the socket with server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Binding done\n";

    // listenToClient(sockfd, cliaddr, buffer);

    socklen_t len;
    int n;

    len = sizeof(cliaddr); // len is value/result

    std::cout << "Listening for client\n";

    if ((n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                      MSG_WAITALL, (struct sockaddr *)&cliaddr,
                      &len)) < 0)
    {
        perror("receive failed");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    // // Print the raw data received
    //         std::cout
    //     << "Received " << n << " bytes from client: ";
    // for (int i = 0; i < n; ++i)
    // {
    //     std::cout << std::hex << (int)buffer[i] << " ";
    // }
    // std::cout << std::dec << std::endl; // Switch back to decimal

    printf("Client : %s\n", buffer);

    // // Write the raw buffer to a file
    // std::ofstream outFile("message.txt", std::ios::binary);
    // if (outFile.is_open())
    // {
    //     // outFile.write(buffer, n);
    //     for (int i = 0; i < n; ++i)
    //     {
    //         outFile << std::hex << (int)buffer[i] << " ";
    //     }
    //     outFile.close();
    //     std::cout << "Data written to message.txt" << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Failed to open message.txt for writing" << std::endl;
    // }

    if (sendto(sockfd, (const char *)hello, strlen(hello),
               MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
               len) < 0)
    {
        perror("send failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Hello message sent." << std::endl;

    while (true)
    {
        listenToClient(sockfd, cliaddr, buffer);
    }

    return 0;
}