#ifndef QUICSERVER_HPP
#define QUICSERVER_HPP

#include "includes.hpp"

class quicServer{
    public:
        int sockfd;

        // This server details set by the user
        struct sockaddr_in server_addr;
        int server_port;
        std::string server_ip;

        // Managing status and error logs
        statusCode serverStatus;
        errorLog serverErrorLog;
        bool isServerRunning;

        // Recieving packets
        PACKET_LINKEDLIST *packetList;
        /*
        Remark when you get a dataout of packetlists, it only deletes the node and not the data's memory | you need to explicitly delete the data using deconstructor

        And when using delete PACKET_LINKEDLIST, it will delete all the packets in the list and also clear the memory of packetdata
        */

    public:
        quicServer(){
            isServerRunning = false;

            // -- TO DO [Other things]    


            serverStatus = SERVER_CREATE_SUCCESS;
        }

        // the moment server is binded it start listening []

        int OpenListener(const struct sockaddr *addr, socklen_t addrlen){

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                serverStatus = SERVER_LISTENER_OPEN_FAILED;
                serverErrorLog = "UDP Socket creation failed";
                return;
            }

            // Bind the socket with the server address
            if (::bind(sockfd, addr, addrlen) < 0)
            {
                serverStatus = SERVER_LISTENER_OPEN_FAILED;
                serverErrorLog = "UDP Bind failed";
                return -1;
            }

            

            // here we would need to make a thread that would keep getting packets on the port but discard them since we are not listening yet

            // ------_Listening Thread Call_------

            serverStatus = SERVER_LISTENER_READY;
            return 0;
        }

        int StartListener(){
            isServerRunning = true;
            serverStatus = SERVER_LISTENING;
            return 0;
        }

        int StopListener(){
            isServerRunning = false;
            serverStatus = SERVER_LISTENER_READY;
            return 0;
        }

        int CloseListener(){
            // ---TO DO---
        }

        // Will implement accept in CallBack Handler Style
        // int accept(){
        //     // ---TO DO---
        // }
};

#endif // QUICSERVER_HPP