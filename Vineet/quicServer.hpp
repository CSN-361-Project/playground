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

        // New Connections Requests
        void* NewConnectionCallbackHandler; // This will be called when a new connection is established
        PACKET_LINKEDLIST ConnectionRequestPackets; // Packets for new connections

        // HandshakePhase Connections
        QUIC_CONNECTION_LINKED_LIST ToHandShakeConnections;

        // To be accepted Connections
        QUIC_CONNECTION_LINKED_LIST ToAcceptConnections;

        //Established Connection 
        QUIC_CONNECTION_LINKED_LIST EstablishedConnections; // List of all the connections
        
        // Connection ID Manager for
        connectionIDManager connectionIDs; // Connection ID Manager

        // Packets to be sent
        PACKET_LINKEDLIST packetsToSend; // Packets to be sent to the [repective] client

        // Deamon Threads
        pthread_t ListenerThread; // takes in Packets
        pthread_t ConnectionRequestProcessingThread;
        pthread_t HandShakeProcessingThread;
        pthread_t EstablishedConnectionManagerThread;
        pthread_t SenderThread;

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
        quicConnection acceptConnection(); // Implemented in NewConnections.hpp file


        // ------- Member Function Implementations ------------
        quicServer(){
            // Initializing the server details/member variables
            isServerListening = false;
            isListernerOpen = false;
            byteSizeOfConnectionID = 2; // 2 bytes [ For now constant ]
            // server_port = PORT;
            // server_ip = SERVER_IP;
            connectionIDs = connectionIDManager();
            ConnectionRequestPackets = PACKET_LINKEDLIST();
            ToHandShakeConnections = QUIC_CONNECTION_LINKED_LIST();
            ToAcceptConnections = QUIC_CONNECTION_LINKED_LIST();
            EstablishedConnections = QUIC_CONNECTION_LINKED_LIST();
            packetsToSend = PACKET_LINKEDLIST();



            //  [Other things]    


            status = SERVER_CREATE_SUCCESS;
        }


        int OpenListener(char* port){
            isListernerOpen = true;

            


            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                status = SERVER_LISTENER_OPEN_FAILED;
                errorLog = "UDP Socket creation failed";
                return;
            }

            struct addrinfo hints, *res;
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET; // IPv4
            hints.ai_socktype = SOCK_DGRAM; // UDP
            hints.ai_flags = AI_PASSIVE; // use my IP
            

            int s = getaddrinfo(NULL, port, &hints, &res);

            // Bind the socket with the server address
            if(bind(sockfd, res->ai_addr, res->ai_addrlen) < 0){
                status = SERVER_LISTENER_OPEN_FAILED;
                errorLog = "UDP Socket bind failed";
                return;
            }

            // set the server details
            server_addr = *(struct sockaddr_in *)res->ai_addr;
            server_port = atoi(port);



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

        quicConnection acceptConnection(){
            // ----TO DO----
            // Will move connection ToAcceptConnections to EstablishedConnections
            // Will return the connection object
        }

};



#endif // QUICSERVER_HPP