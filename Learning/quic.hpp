#ifndef QUIC_H
#define QUIC_H

#include <iostream>
#include <cstring>     // For memset
#include <arpa/inet.h> // For sockaddr_in and inet_addr
#include <unistd.h>    // For close

//~~~~~~~~~Error Flags~~~~~~~~~
#define SOCKET_CREATION_FAILED -1
#define BIND_FAILED -2
#define FAILED_CONNECTION -3

//~~~~~~~~~Connection Status~~~~~~~~~
#define CONNECTION_REJECTED_ByUDP -4
#define CONNECTION_REJECTED_ByQUIC -5
#define CONNECTION_ACCEPTED_ByUDP 0
#define CONNECTION_ACCEPTED_ByQUIC 1 // this will be after handshake

class QUICConnection
{

public:
    int connectionfd;
    struct sockaddr_in peer_addr;
    int connectionStatus;

};

class QUICSocket
{
private:
    /* data */

public:
    int socketfd;
    
    // client and server
    int createSocket()
    { // output resports error
        socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketfd < 0)
        {
            return SOCKET_CREATION_FAILED;
        }
        return 0;
    }

    // client only
    
    // server only
    int bindSocket(const struct sockaddr_in &server_addr)
    {

        if (socketfd < 0)
        {
            return SOCKET_CREATION_FAILED;
        }

        if (bind(socketfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            return BIND_FAILED;
        }
        return 0;
    }

    // server only
    


};

#endif // QUIC_H