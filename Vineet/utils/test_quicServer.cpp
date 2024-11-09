#include "support.hpp"

#define SERVER_PORT "12345"


void NewConnectionHandler(quicServer *server); 
void NewStreamHandler(quicStream *newStream);
// ----TODO---- [Will see how to implement this]


int main(){
    // code for server

    // Server setup [ Certificates, Keys, etc. ]
    quicServer server = quicServer();


    // Then we specify a callback function to handle newConnection
    server.setNewConnectionCallbackHandler(NewConnectionHandler);
    

    // Use a Listener Socket [ We assign a Port and IP to This Internet Socket for Listening ]
    char* port = SERVER_PORT;
    server.OpenListener(port);

    server.StartListener();

    sleep(10); // wait for 10sec; 
}

void NewConnectionHandler(quicServer *server){
    // This function will be called when a new connection is established

    quicConnection newConnection = server->acceptConnection();

    // Set the callback handler for new stream
    newConnection.setNewStreamCallbackHandler(NewStreamHandler);
}


void NewStreamHandler(quicStream *newStream){
    // This function will be called when a new stream is established
    // Here is the API Code where server will handle it's requests

    // ----TODO---- [Will see how to implement this]
    int bufferlen = 1024;
    char* clientRequest = new char[bufferlen];
    int length = newStream->receive(clientRequest);

    // Here we will process the clientRequest and send the response
    char* request1 = "I am a client";
    char* response1 = "Hay there, I am a server";

    char* defaultResponse = "I don't understand";

    if(strcmp(clientRequest, request1) == 0){
        // Print the request
        std::cout << "Request : " << clientRequest << std::endl;

        // Print the response
        std::cout << "Response : " << response1 << std::endl;
        std::cout << "----------------" << std::endl;
        newStream->send(response1, strlen(response1));
    }
    else{
        // Print the request
        std::cout << "Request : " << clientRequest << std::endl;

        // Print the response
        std::cout << "Response : " << defaultResponse << std::endl;
        std::cout << "----------------" << std::endl;
        newStream->send(defaultResponse, strlen(defaultResponse));
    }
}