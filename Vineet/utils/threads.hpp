#ifndef THREADS_HPP
#define THREADS_HPP

#include "support.hpp"

void *newConnectionListener(void *arg);
void *connectionThread(void *arg);
void ProcessRecievedData(quicConnection *connection, char *data, int length);
void CheckDataToSend(quicConnection *connection);


// Something like a deamon thread
void *newConnectionThread(void *arg){
    quicServer *server = (quicServer *)arg;

    // run in while loop the callbackHandler;
    while(server->isListening){
        // run the callbackHandler
        server->NewConnectionHandle(server);
    }
}

void *connectionThread(void *arg){
    // This Thread will handle data to send and recieved data
    quicConnection *connection = (quicConnection *)arg;

    while(1){

        // Recieve Data
        char* data = new char[1024];
        int length = recv(connection->connectionfd, data, 1024, 0); // recieve data from TCP connection

        ProcessRecievedData(connection, data, length);

        // Check Data to send
        CheckDataToSend(connection);

        // Also Run the PeerCreatedStreamHandler
        if(connection->streams.hasPeerCreatedStreams){
            // Run the callbackHandler for each stream
            
        }
    }
}

/* Packets Format
    4byte StreamID (int)
    4byte Length (int)
    data
*/

void ProcessRecievedData(quicConnection *connection, char *data, int length){
    // Recieve Data can have multiple Packets
    int index = 0;
    while(index < length){
        int streamID = 0;
        for(int i=0; i<4; i++){
            streamID = streamID | (data[index] << (i*8));
            index++;
        }

        int dataLength = 0;
        for(int i=0; i<4; i++){
            dataLength = dataLength | (data[index] << (i*8));
            index++;
        }

        char *streamData = new char[dataLength];
        for(int i=0; i<dataLength; i++){
            streamData[i] = data[index];
            index++;
        }

        // Add data to stream
        connection->streams.addDataToRespectiveStream(streamID, streamData, dataLength);
    }
}

void CheckDataToSend(quicConnection *connection){
    int bufferlen = 10*1024;
    char* buffer = new char[bufferlen];
    int length = connection->streams.getStreamDataTobeSend(buffer, bufferlen);
    send(connection->connectionfd, buffer, length, 0);
}



#endif // THREADS_HPP