#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "includes.hpp"

// Declarations ----------------------------
void* RecieverThread(void* arg);



// Implementations ----------------------------
void* RecieverThread(void* arg){
    quicServer *server = (quicServer *)arg; // we pass the whole server object over to the thread to have full access to this

    while(server->isListernerOpen){

        // Recieve the packet
        char buffer[MAXLINE];
        socklen_t len;
        sockaddr sender_addr;
        int bytes_read = recvfrom(server->sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *)&sender_addr,&len);
        buffer[bytes_read] = '\0';
        
        // ----TODO----- we need to make sure data is in Network -> Host Byte Order


        if(server->isServerListening){
            // Add the packet to the packetList
            // -- TODO -- Mutual Access to the packetList competing with the PacketProcessingThread
            packet *packetRecieved = new packet(buffer, bytes_read);

            // server->recievedPackets->addPacket(packetData);

            if(packetRecieved->getPacketType() == Initial){
                // Forward the packet to the newConnectionPackets
                server->newConnectionPackets.addPacket(packetRecieved);

                // --TODO== Some Signal Method to notify callback handler

            }else{
                // check Connection ID and forward the packet to the respective connection
                quicConnection* WhichConnection = server->connectionIDs.getConnection(packetRecieved->getDestinationConnectionID());

                if(WhichConnection != NULL){
                    // Forward the packet to the connection
                    WhichConnection->RecievedPackets->addPacket(packetRecieved);
                }
                // else{
                    // Discard the packet
                // }
            }
        }

        // else{
        //     // Discard the packet
        // }
    }
    return NULL;
}





#endif // RECIEVER_HPP