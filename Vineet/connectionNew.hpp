#ifndef CONNECTION_NEW_HPP
#define CONNECTION_NEW_HPP

#include "includes.hpp"

void* ConnectionRequestProcessing(void* arg){
    quicServer* server = (quicServer*)arg;

    while(1){

        packet* RequestPacket = server->ConnectionRequestPackets.extractHead();

        if(RequestPacket == NULL){
            // No new connection request
            continue;
        }

        quicConnection newConnection = quicConnection();
        

        // will send this to InitialPacketProcessing Thread
        int check = processInitialPacket(&newConnection, RequestPacket);
        // Will handle all the stuff by this function


        if(check == -1){
            // Error in processing the packet
            continue;
        }
    }

}



#endif // CONNECTION_NEW_HPP