#include "./includes.hpp"

// Server API User Defined Callbacks

void NewConnectionHandler(quicServer *server); 
// ----TODO---- [Will see how to implement this]

int main(){
    // code for server

    // Server setup [ Certificates, Keys, etc. ]
    quicServer serverObj;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    serverObj.OpenListener((struct sockaddr *)&server_addr, sizeof(server_addr));

    serverObj.StartListener();

    // Then we specify a callback function to handle newConnection

    serverObj.setNewConnectionCallbackHandler(&NewConnectionHandler);



    // Use a Listener Socket [ We assign a Port and IP to This Internet Socket for Listening ]
    



    // Start Listening



    // Accepting Connections Code


    // For now we handle only one connection


}