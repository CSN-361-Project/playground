#ifndef INCLUDES_HPP
#define INCLUDES_HPP

// C++ Libraries
    #include <iostream>
    #include <vector>
    #include <string>
    #include <fstream>
    #include <sstream>
    #include <algorithm>
    #include <iterator>
    #include <map>
    #include <string>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <net/if.h>

// API Libraries
    #include "packet.hpp"
    #include "frame.hpp"
    #include "packet.hpp"
    #include "quicServer.hpp"
    #include "quicClient.hpp"
    #include "connection.hpp"
    #include "connectionID.hpp"

// #defines
    #define PORT 12345
    #define SERVER_IP


// Using aliases
    using CONNECTION_ID = u_int64_t; // set it to max 8bytes
    using STATUS_CODE = int;
    using ERROR_LOG = std::string;


// Global Constants
    int MAX_CONNECTION_ID_SIZE = 8; // 8 bytes = 64 bits
    int MAXLINE = 1024; // 1 KB

// Global variable 
    int byteSizeOfConnectionID = 2; // will be re-set by serverCreationConstructor


// Status and Error Management
    // We use negative for error codes and positive for success codes

    // Funtions return -1 if something went wrong, then you can check the status code for more information

    const std::string HANDSHAKE_MESSAGE = "Hello from client";
    const std::string HANDSHAKE_RESPONSE = "Hello from server";

    // Make sure to not use anysame interger for status codes
    const STATUS_CODE CLIENT_SOCKET_CREATION_INITIATED = 1;
    const STATUS_CODE CLIENT_SOCKET_CREATION_SUCCESS = 2;
    const STATUS_CODE CLIENT_UDP_SOCKET_CREATION_FAILED = -1;
    const STATUS_CODE CLIENT_CONNECTION_INITIATED = 3;
    const STATUS_CODE CLIENT_CONNECTION_WAITING_FOR_RESPONSE = 4;
    const STATUS_CODE CLIENT_CONNECTION_SUCCESS = 5;
    const STATUS_CODE CLIENT_CONNECTION_REQUEST_SEND_FAILED = -2;
    const STATUS_CODE CLIENT_CONNECTION_REQUEST_REJECTED = -3;

    const STATUS_CODE SERVER_CREATE_SUCCESS = 6;
    const STATUS_CODE SERVER_LISTENER_OPEN_FAILED = -4;
    const STATUS_CODE SERVER_LISTENER_READY = 7;
    const STATUS_CODE SERVER_LISTENING = 8;



#endif // INCLUDES_HPP