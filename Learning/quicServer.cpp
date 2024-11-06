#include "quic.hpp"

#define PORT 12344
#define SERVER_IP "0.0.0.0"

int main(int argc, char const *argv[])
{
    struct sockaddr_in server_addr;

    // Fill server information
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface
    server_addr.sin_port = htons(PORT);

    // create socket
    QUICSocket quiicSocket;
    quiicSocket.createSocket();
    
    quiicSocket.bindSocket(server_addr);
    
    quiicSocket.listenSocket();
    std::cout << "Server is up and listening..." << std::endl;
    
    auto conn = quiicSocket.acceptSocket();

    // check status of connection
    // check status of connection
    switch (conn.connectionStatus)
    {
    case CONNECTION_ACCEPTED_ByUDP:
        std::cout << "Connection accepted by UDP" << std::endl;
        break;
    case CONNECTION_REJECTED_ByUDP:
        std::cout << "Connection rejected by UDP" << std::endl;
        break;
    default:
        std::cout << "Unknown connection status" << std::endl;
        break;
    }

    return 0;
}
