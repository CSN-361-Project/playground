#ifndef ROUGHCODE_HPP
#define ROUGHCODE_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

using STREAM_ID = int;

// #include "threads.hpp"
// Forward Declaration

class quicStream
{
public:
    int connectionfd;
    char *toSendData;
    int toSendDataLength;
    char *recievedData;
    int recievedDataLength;

    STREAM_ID streamID;

    // Functions Declaration
    quicStream(){
        streamID = -1;
        toSendData = NULL;
        toSendDataLength = 0;
        recievedData = NULL;
        recievedDataLength = 0;
    }
    quicStream(STREAM_ID id);
    void enqueueSendData(char *data, int lenght);
    void enqueueRecieveData(char *data, int lenght);
    int dequeueSendData(char *data, int lenght);
    int dequeueRecieveData(char *data, int lenght);
    ~quicStream();
    void sendData(char *data, int lenght);
    int receiveData(char *data, int len);
};

class StreamManager
{
public:
    int connectionfd;
    u_int8_t whoAmI; // 0 for server, 1 for client
    std::map<STREAM_ID, quicStream> streams;
    bool hasPeerCreatedStreams;
    std::queue<quicStream> peerCreatedStreams;
    STREAM_ID lastID;

    // client initiated streams will have odd ID's
    // server initiated streams will have even ID's

    // Functions Declaration
    StreamManager(u_int8_t who);
    quicStream openNewStream();
    void addDataToRespectiveStream(STREAM_ID id, char *data, int length);
    int getStreamDataTobeSend(char *buffer, int bufferlen);
};

class quicConnection
{
public:
    u_int8_t whoAmI; // 0 for server, 1 for client
    // Data members
    int connectionfd;
    pthread_t bakcgroundThread;
    bool threadRunning;
    bool isConnected;
    void (*NewStreamCallbackHandler)(quicStream *stream);
    StreamManager streams;
    // StreamManager

    // Functions Declaration
    quicConnection(u_int8_t who);
    void setNewStreamCallbackHandler(void (*callbackHandler)(quicStream *stream));
    quicStream openNewStream();
    void setFd(int fd);
    int recieveData(char *data, int len);
    int sendData(char *data, int len);  
    int closeConnection();

    

};

class quicServer
{
public:
    // Data members
    int listenfd;
    bool isListening;
    // void (*NewConnectionHandle)(quicServer *server);
    int port;
    pthread_t deamonThread;

    // Functions Declaration
    quicServer();
    int OpenListener(char* port);
    int StartListener();
    int StopListener();
    int CloseListener();
    // void setNewConnectionCallbackHandler(void (*callbackHandler)(quicServer *server));
    quicConnection acceptConnection();
};

class quicClient
{
public:
    // Functions Declaration
    quicClient();
    quicConnection connectToServer(std::string ip, int port);
};

   
// void *newConnectionThread(void *arg);
void *connectionThread(void *arg);
void ProcessRecievedData(quicConnection *connection, char *data, int length);
void CheckDataToSend(quicConnection *connection);



// Functions Implementation
void quicConnection::setFd(int fd)
{
    connectionfd = fd;
    streams.connectionfd = fd;
}


int quicConnection::sendData(char *data, int len)
{
    int s = write(connectionfd, data, len);
    // std::cout << "Connection Sending data |" << data << "| on fd : " << connectionfd << std::endl;
    return s;
}


int quicConnection::recieveData(char *data, int len)
{
    int s = read(connectionfd, data, len-1);
    data[s] = '\0';
    // std::cout << "Connection Recieved data |" << data << "| on fd : " << connectionfd << std::endl;
    return s;
}

quicServer::quicServer() : listenfd(-1), isListening(false), port(-1)
{
    // Just Initialising the server
    }

    int quicServer::OpenListener(char *port)
    { // open listener on given port

        int s;
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // we have got a file-descriptor for the socket

        struct addrinfo hints, *result;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;       /* IPv4 only */
        hints.ai_socktype = SOCK_STREAM; /* TCP */
        hints.ai_flags = AI_PASSIVE;     // use my IP address | this is for server side

        s = getaddrinfo(NULL, port, &hints, &result); // 1234 is the port number, NULL is for localhost, hints is the address info, result is the address info
        if (s != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(1);
        }

        if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0)
        {
            perror("bind()");
            exit(1);
        }

        this->port = atoi(port);
        listenfd = sock_fd;

        return 0;
    }

    int quicServer::StartListener()
    {
        isListening = true;
        listen(listenfd, 10); // 10 is the maximum number of connections that can be queued
        // call the thread
        // pthread_create(&deamonThread, NULL, newConnectionThread, (void *)this);
        return 0;
    }

    int quicServer::StopListener()
    {
        isListening = false;
        // stop the thread
        // pthread_join(deamonThread, NULL);
        return 0; // nothing to do here
    }

    int quicServer::CloseListener()
    {
        close(listenfd);
        return 0;
    }

    // void quicServer::setNewConnectionCallbackHandler(void (*callbackHandler)(quicServer *server))
    // {
    //     NewConnectionHandle = callbackHandler;
    // }

    quicConnection quicServer::acceptConnection()
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
        // std::cout << "listenFD : " << listenfd << std::endl;
        quicConnection newConnection = quicConnection(0); // 0 for server
        newConnection.setFd(client_fd);
        std::cout << "New Connection by " << inet_ntoa(client_addr.sin_addr) << std::endl;
        return newConnection;
    }



// Functions Implementation of quicClient
    quicClient::quicClient()
    {
    // Nothing to do
    }   

    quicConnection quicClient::connectToServer(std::string ip, int port)
    {
        int sockfd;
        struct sockaddr_in serv_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("ERROR opening socket");
            exit(1);
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("ERROR connecting");
            exit(1);
        }
        quicConnection newConnection = quicConnection(1); // 1 for client
        newConnection.setFd(sockfd);
        return newConnection;
    }

// UserFunctions
    void quicStream::sendData(char *data, int lenght)
{
    // std::cout << "Stream Sending data |" << data << "| on fd : " << connectionfd << std::endl;
    // std::cout << "Sending Data - " << data << std::endl;
    int t = write(connectionfd, data, lenght);
}

    int quicStream::receiveData(char *data, int len)
    {
        
        int t = read(connectionfd, data, len-1);
        // std::cout << "Stream Recieved data |" << data << "| on fd : " << connectionfd << std::endl;
        return t;
        // return dequeueRecieveData(data, len);
    }

    quicStream::quicStream(STREAM_ID id)
    {
        streamID = id;
        toSendData = NULL;
        toSendDataLength = 0;
        recievedData = NULL;
        recievedDataLength = 0;
    }

    void quicStream::enqueueSendData(char *data, int lenght)
    {
        char *temp = new char[toSendDataLength + lenght];
        for (int i = 0; i < toSendDataLength; i++)
        {
            temp[i] = toSendData[i];
        }
        for (int i = 0; i < lenght; i++)
        {
            temp[toSendDataLength + i] = data[i];
        }
        // delete[] toSendData;
        toSendData = temp;
        toSendDataLength += lenght;
    }

    void quicStream::enqueueRecieveData(char *data, int lenght)
    {
        char *temp = new char[recievedDataLength + lenght];
        for (int i = 0; i < recievedDataLength; i++)
        {
            temp[i] = recievedData[i];
        }
        for (int i = 0; i < lenght; i++)
        {
            temp[recievedDataLength + i] = data[i];
        }
        // delete[] recievedData;
        recievedData = temp;
        recievedDataLength += lenght;
    }

    int quicStream::dequeueSendData(char *data, int len)
    {
        if(toSendDataLength == 0){
            return 0;
        }
        if(len < toSendDataLength){
            return -1; // buffer is not sufficient
        }

        for (int i = 0; i < toSendDataLength; i++)
        {
            data[i] = toSendData[i];
        }
        delete[] toSendData;
        return toSendDataLength; // return the length of the data
    }

    int quicStream::dequeueRecieveData(char *data, int len)
    {
        if(recievedDataLength == 0){
            return 0;
        }
        if(len < recievedDataLength){
            return -1; // buffer is not sufficient
        }
        for (int i = 0; i < recievedDataLength; i++)
        {
            data[i] = recievedData[i];
        }
        delete[] recievedData;
        return recievedDataLength; // return the length of the data
    }

    quicStream::~quicStream()
    {
        
    }


    
    
    
    // Functions Implementation of StreamManager
    StreamManager::StreamManager(u_int8_t who) : whoAmI(who), lastID(0), hasPeerCreatedStreams(false)
    {
        
    }

    quicStream StreamManager::openNewStream()
    {
        quicStream newStream = quicStream(lastID);
        newStream.connectionfd = this->connectionfd;
        // std::cout << "New Stream Opened with fd : " << newStream.connectionfd << std::endl;
        streams[lastID] = newStream;
        lastID += 2;
        return newStream;
    }

    void StreamManager::addDataToRespectiveStream(STREAM_ID id, char *data, int length)
    {
        if (streams.find(id) == streams.end())
        {
            // if not found it's a new stream
            if (id % 2 != whoAmI)
            { // it's okay a new stream was created at my other end
                lastID = id;
                quicStream newStream = quicStream(id);
                streams[id] = newStream;
                peerCreatedStreams.push(newStream);
                hasPeerCreatedStreams = true;

                // The call backHanlder is Running on Connection Thread
            }

            // ignore the data; [I have not created this stream]
        }
        streams[id].enqueueSendData(data, length);
    }

    int StreamManager::getStreamDataTobeSend(char *buffer, int bufferlen)
    {
        // The input buffer must be sufficient to hold all the data
        // here we need to check if any stream has any data to send
        std::queue<quicStream> wantToSend;
        int lotalLenght = 0;
        char *tempBuffer = buffer;
        int bufferindex = 0;
        // Packet Format [ 4byte StreamID, 4byte Length, data]
        for (auto it = streams.begin(); it != streams.end(); it++)
        {
            if (it->second.toSendDataLength > 0)
            {
                // check if we have space in buffer
                if (lotalLenght + 8 + it->second.toSendDataLength > bufferlen)
                {
                    break; // rest of data will be sent in next call
                }

                // we need to send this data
                int streamID = it->first;
                int length = it->second.toSendDataLength;
                for (int i = 0; i < 4; i++)
                {
                    buffer[bufferindex] = (streamID >> (i * 8)) & 0xFF;
                    bufferindex++;
                }
                for (int i = 0; i < 4; i++)
                {
                    buffer[bufferindex] = (length >> (i * 8)) & 0xFF;
                    bufferindex++;
                }
                for (int i = 0; i < length; i++)
                {
                    buffer[bufferindex] = it->second.toSendData[i];
                    bufferindex++;
                }
                lotalLenght += 8 + length;
            }
        }
        return lotalLenght;
    }


// Functions Implementation of quicConnection
quicConnection::quicConnection(u_int8_t who) : streams(who), whoAmI(who), connectionfd(-1), NewStreamCallbackHandler(NULL)
{
    setFd(connectionfd);
    isConnected = true;
    threadRunning = false;
}
void quicConnection::setNewStreamCallbackHandler(void (*callbackHandler)(quicStream *stream))
{
    NewStreamCallbackHandler = callbackHandler;
    pthread_create(&bakcgroundThread, NULL, connectionThread, (void *)this);
    this->threadRunning = true;
}

quicStream quicConnection::openNewStream()
{
    return streams.openNewStream();
}

int quicConnection::closeConnection()
{
    isConnected = false;
    close(connectionfd);
    return 0;
}

// void *newConnectionThread(void *arg)
// {
//     quicServer *server = (quicServer *)arg;

//     // run in while loop the callbackHandler;
//     while (server->isListening)
//     {
//         // run the callbackHandler
//         server->NewConnectionHandle(server);
//     }
//     return NULL;
// }

void *connectionThread(void *arg)
{
    // This Thread will handle data to send and recieved data
    quicConnection *connection = (quicConnection *)arg;

    while (connection->isConnected)
    {
        // First Check if there is any data to send
        CheckDataToSend(connection);

        // Also Run the PeerCreatedStreamHandler
        if (connection->streams.hasPeerCreatedStreams)
        {
            // Run the callbackHandler for each stream
            while (!connection->streams.peerCreatedStreams.empty())
            {
                quicStream stream = connection->streams.peerCreatedStreams.front();
                connection->NewStreamCallbackHandler(&stream);
                connection->streams.peerCreatedStreams.pop();
            }
            connection->streams.hasPeerCreatedStreams = false;
        }

        // Recieve Data
        char *data = new char[1024];
        int length = recv(connection->connectionfd, data, 1024, 0); // recieve data from TCP connection

        ProcessRecievedData(connection, data, length);

        sleep(1); // sleep for 1 sec
    }
    return NULL;
}

/* Packets Format
    4byte StreamID (int)
    4byte Length (int)
    data
*/

void ProcessRecievedData(quicConnection *connection, char *data, int length)
{
    // Recieve Data can have multiple Packets
    int index = 0;
    while (index < length)
    {
        int streamID = 0;
        for (int i = 0; i < 4; i++)
        {
            streamID = streamID | (data[index] << (i * 8));
            index++;
        }

        int dataLength = 0;
        for (int i = 0; i < 4; i++)
        {
            dataLength = dataLength | (data[index] << (i * 8));
            index++;
        }

        char *streamData = new char[dataLength];
        for (int i = 0; i < dataLength; i++)
        {
            streamData[i] = data[index];
            index++;
        }

        // Add data to stream
        connection->streams.addDataToRespectiveStream(streamID, streamData, dataLength);
    }
}

void CheckDataToSend(quicConnection *connection)
{
    int bufferlen = 10 * 1024;
    char *buffer = new char[bufferlen];
    int length = connection->streams.getStreamDataTobeSend(buffer, bufferlen);
    if (length > 0)
    {
        send(connection->connectionfd, buffer, length, 0);
    }
}

#endif // ROUGHCODE_HPP