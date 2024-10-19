#include "quic.hpp"

#define PORT 12344
#define SERVER_IP "0.0.0.0"

int main(){

    QUICSocket quicSocketObj;
    quicSocketObj.createSocket();
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    QUICConnection connectionObj = quicSocketObj.connectSocket(server_addr);

    // check status of connection
    switch (connectionObj.connectionStatus)
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