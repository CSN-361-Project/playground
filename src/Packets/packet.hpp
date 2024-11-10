#ifndef PACKET_H
#define PACKET_H

#include "../includes.hpp"



enum packetType
{
    Initial,
    ZeroRTT,
    Handshake,
    Retry,
    OneRTT,
    Invalid
};

struct SocketAddress
{
    sockaddr_in address;
    socklen_t len;
};

class packet
{
public:
    char *data;
    int size;
    packetType type;
    SocketAddress peerAddress;

    // Declarations ----------------------------
    packet(); // Just a default constructor
    packet(const char *buffer, const int size_of_data, SocketAddress senderAddress);
    ~packet();
    const packetType getPacketType();
    const CONNECTION_ID getDestinationConnectionID();



    // Implementations ----------------------------
    packet(){
        data = NULL;
        size = 0;
        type = Invalid;

    }

    packet(const char *buffer, const int size_of_data, SocketAddress senderAddress)
    {
        char *data = (char *)malloc(size_of_data + 1);
        // use memcpy to copy the buffer to data
        memcpy(data, buffer, size_of_data);
        data[size_of_data] = '\0'; // null terminate the string

        // Set address [ we need to create a copy of it ]
        peerAddress.address = senderAddress.address;
        peerAddress.len = senderAddress.len;

        type = Invalid;
        // set packet type
        if (data[0] & 0x80 == 0)
        {
            // short header packet
            type = OneRTT;
        }
        else
        {
            int packet_bits = data[0] & 0x30;
            if(packet_bits == 0){
                type = Initial;
            }else if(packet_bits == 1){
                type = ZeroRTT;
            }else if(packet_bits == 2){
                type = Handshake;
            }else if(packet_bits == 3){
                type = Retry;
            }
        }
    }

    ~packet()
    {
        free(data);
    }

    const packetType getPacketType()
    {
        return type;
    }

    const CONNECTION_ID getDestinationConnectionID()
    {
        CONNECTION_ID connectionID = 0;
        // we need to read from byte 1 to (1+byteSizeOfConnectionID-1)
        int start;
        int end;
        u_int x=0;
        if (type == OneRTT)
        {
            start = 1;
            end = 1 + byteSizeOfConnectionID - 1;
        }
        else if(type!=Invalid){
            int bytesSizeAsPerSender = data[5];
            // if it's not of our size means it's a new connection by default [ ]
            start = 6;
            end = 6 + bytesSizeAsPerSender - 1;
        }

        for (int i = start; i <= end; i++)
        {
            x = x << 8;
            x = x | data[i];
        }
        connectionID = x;
        return connectionID;
    }
    


    // ---Info-------------------------
    /* Long Header Packet Structure
    1. Long Header Packets
    {
    byte 0
        Header Form (1) = 1, // to show it's of long header type
        Fixed Bit (1) = 1, // must in this version
        Long Packet Type (2), //
        Type-Specific Bits (4),
    byte 1-4
        Version (32),
    byte 5
        Destination Connection ID Length (8), // he byte following the version contains the length in bytes of the Destination Connection ID field that follows it.
    byte 6 TO (6+Destination Connection ID Length)
        Destination Connection ID (0..160),
    byte 6+Destination Connection ID Length
        Source Connection ID Length (8),
    byte (6+Destination Connection ID Length + 1) TO (6+Destination Connection ID Length + Source Connection ID Length)
        Source Connection ID (0..160),
    
    FRAMES
        Type-Specific Payload (..),
    }


    */

    /* Short Header Packet Structure

    1-RTT Packet 
    {
    byte 0
        Header Form (1) = 0,
        Fixed Bit (1) = 1,
        Spin Bit (1),
        Reserved Bits (2),
        Key Phase (1),
        Packet Number Length (2), // given is unsigned integer which show bytes length 
    byte 1 TO (1+Packet Number Length)
        Destination Connection ID (0..160),
        Packet Number (8..32),
        Packet Payload (8..),
    }


    */

};



#endif // PACKET_H
