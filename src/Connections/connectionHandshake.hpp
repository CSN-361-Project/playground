#ifndef CONNECTION_HANDSHAKE_HPP
#define CONNECTION_HANDSHAKE_HPP

#include "../includes.hpp"

void *HandshakeProcessing(void *arg)
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

    }
}












#endif // CONNECTION_HANDSHAKE_HPP