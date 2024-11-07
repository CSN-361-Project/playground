#ifndef QUICSERVER_HPP
#define QUICSERVER_HPP

#include "includes.hpp"

class quicServer{
    public:

        // ------- Data Members ------------
        int sockfd;

        // This server details set by the user
        struct sockaddr_in server_addr;
        int server_port;
        std::string server_ip;

        // Managing status and error logs
        STATUS_CODE status;
        ERROR_LOG errorLog;

        bool isServerListening; // Packets will be recieved or not
        bool isListernerOpen; // ListenerThread will be running or NOT(will be joined to main thread)

        // New Connections
        void* NewConnectionCallbackHandler; // This will be called when a new connection is established
        PACKET_LINKEDLIST newConnectionPackets;

        //Registered Connection 
        QUIC_CONNECTION_LINKED_LIST ConnectionsList;
        connectionIDManager connectionIDs;

        // Recieving packets

        PACKET_LINKEDLIST packetsToSend;

        
        
        /*
        Remark when you get a dataout of packetlists, it only deletes the node and not the data's memory | you need to explicitly delete the data using deconstructor
        And when using delete PACKET_LINKEDLIST, it will delete all the packets in the list and also clear the memory of packetdata
        */
        


        // ------- Member Function Declarations ------------
        quicServer();
        int OpenListener(const struct sockaddr *addr, socklen_t addrlen);
        int StartListener();
        int StopListener();
        int CloseListener();
        void setNewConnectionCallbackHandler(void* callbackHandler);





        // ------- Member Function Implementations ------------
        quicServer(){
            // Initializing the server details/member variables
            isServerListening = false;
            isListernerOpen = false;
            byteSizeOfConnectionID = 2; // 2 bytes [ For now constant ]
            // server_port = PORT;
            // server_ip = SERVER_IP;
            connectionIDs = connectionIDManager();
            ConnectionsList = QUIC_CONNECTION_LINKED_LIST();
            newConnectionPackets = PACKET_LINKEDLIST();
            packetsToSend = PACKET_LINKEDLIST();



            //  [Other things]    


            status = SERVER_CREATE_SUCCESS;
        }


        int OpenListener(const struct sockaddr *addr, socklen_t addrlen){
            isListernerOpen = true;

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                status = SERVER_LISTENER_OPEN_FAILED;
                errorLog = "UDP Socket creation failed";
                return;
            }

            // Bind the socket with the server address
            if (bind(sockfd, addr, addrlen) < 0)
            {
                status = SERVER_LISTENER_OPEN_FAILED;
                errorLog = "UDP Bind failed";
                return -1;
            }
            // the moment server is binded it start listening []


            // ------_Listening Thread Call_------
            // ---TO DO---

            status = SERVER_LISTENER_READY;
            return 0;
        }

        int StartListener(){
            isServerListening = true;
            status = SERVER_LISTENING;
            return 0;
        }

        int StopListener(){
            isServerListening = false;
            status = SERVER_LISTENER_READY;
            return 0;
        }

        int CloseListener(){
            isServerListening = false;
            isListernerOpen = false;
            // ---TO DO---
        }

        void setNewConnectionCallbackHandler(void* callbackHandler){
            NewConnectionCallbackHandler = callbackHandler;
        }

        // Will implement accept in CallBack Handler Style
        int accept();
        //     // ---TO DO---
        // }
};



#endif // QUICSERVER_HPP