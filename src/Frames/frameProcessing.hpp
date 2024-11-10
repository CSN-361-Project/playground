#ifndef FRAMEPROCESSING_HPP
#define FRAMEPROCESSING_HPP

#include "includes.hpp"

// Frame Processing Functions Declarations

void *FrameProcessing(void *arg);







// Frame Processing Functions Implementations

void *FrameProcessing(void *arg)
{
    quicServer *server = (quicServer *)arg;

    while (1)
    {
        quicConnection *connection = server->ToHandShakeConnections.extractHead();

        if (connection == NULL)
        {
            // No new connection request
            continue;
        }

        // Process the connection
        int check = processHandshake(connection);

        if (check == -1)
        {
            // Error in processing the packet
            continue;
        }
    }
}









#endif // FRAMEPROCESSING_HPP