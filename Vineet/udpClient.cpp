// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string>

using namespace std;

#define PORT 8080
#define MAXLINE 1024

    // Driver code
    int main(){
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t n, len;
    int a = 1;
    while(1){
        string message = "Hello from client's ID : " + to_string(getpid()) + " Message no - " + to_string(a);
        a++;
        sendto(sockfd, (const char *)message.c_str(), message.length(), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        cout << "Message sent : " << message << endl;
        sleep(2);
    }

    close(sockfd);
    return 0;
}
