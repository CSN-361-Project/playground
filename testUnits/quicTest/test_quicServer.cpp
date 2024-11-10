#include "../../quicAPI.hpp"





int main(){
    // code for server

    // Server setup [ Certificates, Keys, etc. ]
    quicServer server = quicServer();

    // Use a Listener Socket [ We assign a Port and IP to This Internet Socket for Listening ]
    char *port = SERVER_PORT;
    server.OpenListener(port);
    std::cout << "Server is listening on port : " << port << std::endl;
    int length = 1024;
    char *request = new char[length];

    server.StartListener();
    quicConnection newConnection = quicConnection(0); // 0 for server

    while(server.isListening){

        std::cout << "Waiting for New Connection" << std::endl;
        newConnection = server.acceptConnection();

        int t = newConnection.recieveData(request, length);
        request[t] = '\0';
        std::cout << "Recieved Data : " << request << std::endl;

        // Send a response
        quicStream newStream = newConnection.openNewStream();
        std::cout << "New Stream Opened" << std::endl;

        char response[] = "Hello Client\n";
        newStream.sendData(response, strlen(response));
        std::cout << "Response Sent : " << response << std::endl;
        
        std::cout << "Closing Connection" << std::endl;
        newConnection.closeConnection();

        std::cout << "---------------------------------" << std::endl;
    }

    // sleep(100); // wait for 10sec; 

    server.StopListener();
    server.CloseListener();
    
}

