#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "includes.hpp"

#define MAXLINE 1024

class clientSocket{
    private:
        int sockfd;

        // Server Details
        struct sockaddr_in servaddr;
        int server_port;
        std::string server_ip;

        // Managing status and error logs
        STATUS_CODE status;
        ERROR_LOG errorLog;

        // Each time we receive a message we also get a sender address
        struct sockaddr_in sender_addr;
        socklen_t len;

    public:
        clientSocket(std::string server_ip, int server_port)
        : server_ip(server_ip), server_port(server_port)
        {
            status = CLIENT_SOCKET_CREATION_INITIATED;

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                status = CLIENT_UDP_SOCKET_CREATION_FAILED;
                clientErrorLog = "Socket creation failed";
                return;
            }

            memset(&servaddr, 0, sizeof(servaddr));

            // Filling server information [ we need this each time we send a message to server ]
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(server_port);
            servaddr.sin_addr.s_addr = std::stoi(server_ip);
            // servaddr.sin_len = sizeof(servaddr);

            clientStatus = CLIENT_SOCKET_CREATION_SUCCESS;
        }

        int connect(){
            // This will send handshake message to server
            clientStatus = CLIENT_CONNECTION_INITIATED;

            std::string handshakeMessage = HANDSHAKE_MESSAGE;
            int check = sendto(sockfd, (const char *)handshakeMessage.c_str(), handshakeMessage.length(), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
            if(check < 0){
                clientStatus = CLIENT_CONNECTION_REQUEST_SEND_FAILED;
                clientErrorLog = "Handshake message failed";
                perror("Handshake message failed");
                return -1;
            }
            clientStatus = CLIENT_CONNECTION_WAITING_FOR_RESPONSE;
            // Wait for server to respond
            
            char buffer[MAXLINE];
            int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&sender_addr, &len);
            buffer[n] = '\0'; 

            std::cout << "Server says : " << buffer << std::endl;
            std::string ACCEPTANCE_CODE = HANDSHAKE_RESPONSE;
            if (buffer != ACCEPTANCE_CODE)
            {
                clientStatus = CLIENT_CONNECTION_REQUEST_REJECTED;
                clientErrorLog = "Server rejected the connection request : " + std::string(buffer);
                return -1;
            }
            clientStatus = CLIENT_CONNECTION_SUCCESS;
        }

        STATUS_CODE getStatus(){
            return clientStatus;
        }

        ERROR_LOG getErrorLog(){
            return clientErrorLog;
        }

        ~clientSocket(){
            close(sockfd);
        }

        int send(std::string message){
            // check if connection is established
            if(clientStatus != CLIENT_CONNECTION_SUCCESS){
                clientErrorLog = "Connection not established";
                return -1;
            }

            // ---TO DO---

        }

        int recv(){
            // check if connection is established
            if(clientStatus != CLIENT_CONNECTION_SUCCESS){
                clientErrorLog = "Connection not established";
                return -1;
            }

            // ---TO DO---
        }
};


#endif // CLIENTSOCKET_HPP