#ifndef PACKET_H
#define PACKET_H

#include "includes.hpp"

int MAX_CONNECTION_ID_SIZE = 8; // 8 bytes = 64 bits

/*
Some Restrictions

1. We are chosing our Connection ID of Fixed Lenght of 16bits - 2 bytes

*/


enum packetType
{
    Initial,
    ZeroRTT,
    Handshake,
    Retry,
    OneRTT,
    Invalid
};


class packet
{
public:
    char *data;
    int size;
    packetType type;


    // Declarations ----------------------------
    packet(); // Just a default constructor
    packet(const char *buffer, const int size_of_data);
    ~packet();
    const packetType getPacketType();
    const CONNECTION_ID getDestinationConnectionID();



    // Implementations ----------------------------
    packet(){
        data = NULL;
        size = 0;
        type = Invalid;
    }

    packet(const char *buffer, const int size_of_data)
    {
        char *data = (char *)malloc(size_of_data + 1);
        // use memcpy to copy the buffer to data
        memcpy(data, buffer, size_of_data);
        data[size_of_data] = '\0'; // null terminate the string

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



class packetNode{
    public:
        packet *packetData;
        packetNode *next;
        packetNode(packet *packetData){
            this->packetData = packetData;
            this->next = NULL;
        }

        ~packetNode(){
            delete packetData;
        }
};

class PACKET_LINKEDLIST{
    public:
        packetNode *head;
        packetNode *tail;
        int size; // will be used for Producer-Consumer Approach
        PACKET_LINKEDLIST(){
            head = NULL;
            tail = NULL;
            size = 0;
        }

        // Declarations of functions -------------------
        void addPacket(packet *packetData); // at tail
        void addPacketAtTail(packet *packetData);
        void addPacketAtHead(packet *packetData);
        packet *peekHead(); // don't remove node
        packet *extractHead(); // also remove ndoe
        packet *peekTail();
        packet *extractTail();
        ~PACKET_LINKEDLIST(); // called using 'delete' keyword



        // Implementations of functions -------------------
        void addPacket(packet *packetData){
            addPacketAtTail(packetData);
        }

        void addPacketAtTail(packet *packetData){
            packetNode *newNode = new packetNode(packetData);
            if(head == NULL){
                head = newNode;
                tail = newNode;
            }else{
                tail->next = newNode;
                tail = newNode;
            }
            size++;
        }

        void addPacketAtHead(packet *packetData){
            packetNode *newNode = new packetNode(packetData);
            if(head == NULL){
                head = newNode;
                tail = newNode;
            }else{
                newNode->next = head;
                head = newNode;
            }
            size++;
        }

        packet *peekHead(){ 
            return head->packetData;
        }

        packet *extractHead(){ 
            packet *packetData = head->packetData;
            packetNode *temp = head;
            head = head->next;
            delete temp;
            size--;
            return packetData; // Remember we are not deleting the packetData, we are just removing the node
        }

        packet *peekTail(){ 
            return tail->packetData;
        }

        packet *extractTail(){ 
            packet *packetData = tail->packetData;
            packetNode *temp = head;
            while(temp->next != tail){
                temp = temp->next;
            }
            delete tail;
            tail = temp;
            size--;
            return packetData; // Remember we are not deleting the packetData, we are just removing the node
        }

        ~PACKET_LINKEDLIST(){ 
            while(head != NULL){
                packetNode *temp = head;
                head = head->next;
                free(temp->packetData); // Deleting the packetData
                delete temp; //deleting the node
            }
        }

};


#endif // PACKET_H
