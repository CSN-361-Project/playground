#ifndef PACKETPROCESSING_HPP
#define PACKETPROCESSING_HPP

#include "includes.hpp"

// Packet Processing Functions Declarations

int processInitialPacket(quicConnection *connection, packet *packetData);
packet *generateInitialPacket(quicConnection *connection);
/*Initial Packet Format
    {
        Header Form (1) = 1,
        Fixed Bit (1) = 1,
        Long Packet Type (2) = 0,
        Reserved Bits (2),
        Packet Number Length (2),
        Version (32),
        Destination Connection ID Length (8),
        Destination Connection ID (0..160),
        Source Connection ID Length (8),
        Source Connection ID (0..160),
        Token Length (i),
        Token (..),
        Length (i),
        Packet Number (8..32),
        Packet Payload (8..),
}
*/

// Packet Processing Functions Implementations

int processInitialPacket(quicConnection *connection, packet *packetData)
{
    if (packetData->getPacketType() != Initial)
    {
        return -1;
    }

    length packetNoLength = (packetData->data[0] & 0x03) + 1;
    length destConnIDLength = packetData->data[5];

    index sourceConnIDStart = 6 + destConnIDLength;
    length sourceConnIDLength = packetData->data[sourceConnIDStart];

    index tokenStart = 6 + destConnIDLength + sourceConnIDLength;
    varInt tokenLength = variablelenghtDecode(packetData->data, tokenStart);

    index packetLenghtStart = tokenStart + tokenLength;
    varInt packetLength = variablelenghtDecode(packetData->data, packetLenghtStart);

    index packetNumberStart = packetLenghtStart + packetLength;
    Byte4 packetNumber = readByte8(packetData->data, packetNumberStart, packetNoLength);

    // Rest is payload
    length payloadSize = packetLength - packetNoLength;

    // We need to Extract Payload and Frames and process each frame
    // -------TODO Code Frame Processing Units
}

#endif // PACKETPROCESSING_HPP