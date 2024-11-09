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
    quicStream stream = connection.openNewStream();

    // Send a message
    char *request1 = "I am a client";

    stream.send(request1, strlen(request1));

    // Recieve the response
    int bufferlen = 1024;
    char *response = new char[bufferlen];
    int length = stream.receive(response);

    // Print the response
    std::cout << "Response : " << response << std::endl;
    
}