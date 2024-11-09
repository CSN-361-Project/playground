#include "support.hpp"

#define SERVER_PORT "12345"
#define LOCALHOST INADDR_ANY

int main(){

    // Client Code
    quicClient client = quicClient();

    // Server IP and Port
    std::string ip = LOCALHOST;
    int port = 12345;
    quicConnection connection = client.connectToServer(ip, port);

    // Open a new stream
    
}