#ifndef CONNECTION_NEW_HPP
#define CONNECTION_NEW_HPP

#include "includes.hpp"

quicConnection quicServer::acceptConnection(){
    /* This function will be called by the user in the callback handler
    - will return a connection object, which the callback handler can work with

    */

    // This function will extract one packet from the newConnectionPackets list and create a new connection object

    packet* RequestPacket = ConnectionRequestPackets.extractHead();
    if(RequestPacket == NULL){
        // No new connection request
        return; 
    }

    quicConnection newConnection = quicConnection(RequestPacket);
}












#endif // CONNECTION_NEW_HPP