#ifndef QUICSERVER_HPP
#define QUICSERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include "./statusCode.hpp"




class quicServer{
    private:
        int sockfd;

        // This server details set by the user
        struct sockaddr_in servaddr;
        int server_port;
        string server_ip;

        // Managing status and error logs
        statusCode serverStatus;
        errorLog serverErrorLog;
    
    public:
        quicServer(){
            serverStatus = SERVER_SOCKET_CREATION_INITIATED;

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                serverStatus = SERVER_UDP_SOCKET_CREATION_FAILED;
                serverErrorLog = "Socket creation failed";
                return;
            }

            serverStatus = SERVER_SOCKET_CREATION_SUCCESS;
        }

        int bind(const struct sockaddr *addr, socklen_t addrlen){
            serverStatus = SERVER_UDP_SOCKET_BIND_INITIATED;
            // Bind the socket with the server address
            if (::bind(sockfd, addr, addrlen) < 0)
            {
                serverStatus = SERVER_UDP_SOCKET_BIND_FAILED;
                serverErrorLog = "Bind failed";
                return -1;
            }

            serverStatus = SERVER_UDP_SOCKET_BIND_SUCCESS;
            return 0;
        }

        int listen(){
            // ---TO DO---
        }

        int accept(){
            // ---TO DO---
        }
};

#endif // QUICSERVER_HPP