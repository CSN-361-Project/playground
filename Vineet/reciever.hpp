#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "includes.hpp"

// Declarations ----------------------------
void* RecieverThread(void* arg);
void *PacketProcessingThread(void* arg);




// Implementations ----------------------------


void* RecieverThread(void* arg){
    quicServer *server = (quicServer *)arg; // we pass the whole server object over to the thread to have full access to this

    while(1){

        // Recieve the packet
        char buffer[MAXLINE];
        socklen_t len;
        sockaddr sender_addr;
        int bytes_read = recvfrom(server->sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *)&sender_addr,&len);
        buffer[bytes_read] = '\0';
        

        if(server->isServerRunning){
            // Add the packet to the packetList
            // -- TODO -- Mutual Access to the packetList competing with the PacketProcessingThread
            packet *packetData = new packet(buffer, bytes_read);
            server->packetList->addPacket(packetData);
        }

        // else{
        //     // Discard the packet
        // }
    }
}



void *PacketProcessingThread(void* arg){
    
}




#endif // RECIEVER_HPP