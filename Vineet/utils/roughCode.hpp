#ifndef ROUGHCODE_HPP
#define ROUGHCODE_HPP

#include "support.hpp"

// Forward Declaration
class quicStream;
class quicServer;
class quicClient;
class quicConnection;


class quicServer
{
public:
    // Data members
    int listenfd;
    bool isListening;
    void (*NewConnectionHandle)(quicServer *server);
    int port;
    pthread_t deamonThread;

    // Functions Declaration
    quicServer();
    int OpenListener(const struct sockaddr *addr, socklen_t addrlen);
    int StartListener();
    int StopListener();
    int CloseListener();
    void setNewConnectionCallbackHandler(void *callbackHandler);
    quicConnection acceptConnection();

    // Functions Implementation
    quicServer() : listenfd(-1), isListening(false), NewConnectionHandle(NULL), port(-1)
    {
        // Just Initialising the server
    }

    int OpenListener(char *port)
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

    int StartListener()
    {
        isListening = true;
        listen(listenfd, 10); // 10 is the maximum number of connections that can be queued

        // call the thread
        pthread_create(&deamonThread, NULL, newConnectionThread, (void *)this);
    }

    int StopListener()
    {
        isListening = false;

        // stop the thread
        pthread_join(deamonThread, NULL);
        return 0; // nothing to do here
    }

    int CloseListener()
    {
        close(listenfd);
        return 0;
    }

    void setNewConnectionCallbackHandler(void (*callbackHandler)(quicServer *server))
    {
        NewConnectionHandle = callbackHandler;
    }

    quicConnection acceptConnection()
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
        quicConnection newConnection = quicConnection(0); // 0 for server
        newConnection.connectionfd = client_fd;
        return newConnection;
    }
};

class quicClient
{
public:
    // Functions Declaration
    quicClient();
    quicConnection connectToServer(std::string ip, int port);

    quicClient()
    {
        // Nothing to do
    }

    quicConnection connectToServer(std::string ip, int port)
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
        newConnection.connectionfd = sockfd;
        return newConnection;
    }
};

class quicConnection
{
public:
    u_int8_t whoAmI; // 0 for server, 1 for client
    // Data members
    int connectionfd;
    void (*NewStreamCallbackHandler)(quicStream *stream);
    StreamManager streams;
    // StreamManager

    // Functions Declaration
    quicConnection(u_int8_t who);
    void setNewStreamCallbackHandler(void (*callbackHandler)(quicStream *stream));
    quicStream openNewStream();
    int closeConnection();

    // Functions Implementation
    quicConnection(u_int8_t who) : streams(who), whoAmI(who), connectionfd(-1), NewStreamCallbackHandler(NULL)
    {
        // Just Initialising the connection
    }
    void setNewStreamCallbackHandler(void (*callbackHandler)(quicStream *stream))
    {
        NewStreamCallbackHandler = callbackHandler;
    }

    quicStream openNewStream()
    {
        return streams.openNewStream();
    }
};

class quicStream
{
public:
    char *toSendData;
    int toSendDataLength;
    char *recievedData;
    int recievedDataLength;

    STREAM_ID streamID;

    // Functions Declaration
    quicStream(STREAM_ID id);
    void enqueueSendData(char *data, int lenght);
    void enqueueRecieveData(char *data, int lenght);
    int dequeueSendData(char *data);
    int dequeueRecieveData(char *data);
    ~quicStream();

    // UserFunctions
    void send(char *data, int lenght)
    {
        enqueueSendData(data, lenght);
    }

    int receive(char *data)
    {
        // must be blocking in Nature 
        // Bruteforce Implementation of Blocking Call
        char *temp = new char[1024];
        int length = dequeueRecieveData(temp);
        while(length == 0){
            length = dequeueRecieveData(temp); // blocking call
        }
        data = (char *)realloc(data, length);
        for (int i = 0; i < length; i++)
        {
            data[i] = temp[i];
        }
        return length;
    }

    quicStream(STREAM_ID id)
    {
        streamID = id;
        toSendData = NULL;
        toSendDataLength = 0;
        recievedData = NULL;
        recievedDataLength = 0;
    }

    void enqueueSendData(char *data, int lenght)
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
        delete[] toSendData;
        toSendData = temp;
        toSendDataLength += lenght;
    }

    void enqueueRecieveData(char *data, int lenght)
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
        delete[] recievedData;
        recievedData = temp;
        recievedDataLength += lenght;
    }

    int dequeueSendData(char *data)
    {
        data = (char *)realloc(data, toSendDataLength);
        for (int i = 0; i < toSendDataLength; i++)
        {
            data[i] = toSendData[i];
        }
        return toSendDataLength; // return the length of the data
    }

    int dequeueRecieveData(char *data)
    {
        data = (char *)realloc(data, recievedDataLength);
        for (int i = 0; i < recievedDataLength; i++)
        {
            data[i] = recievedData[i];
        }
        return recievedDataLength; // return the length of the data
    }

    ~quicStream()
    {
        delete[] toSendData;
        delete[] recievedData;
    }
};

class StreamManager
{
public:
    u_int8_t whoAmI; // 0 for server, 1 for client
    std::map<STREAM_ID, quicStream> streams;
    bool hasPeerCreatedStreams;
    std::queue<quicStream> peerCreatedStreams;
    int lastID;

    // client initiated streams will have odd ID's
    // server initiated streams will have even ID's

    StreamManager(u_int8_t who) : whoAmI(who), lastID(0), hasPeerCreatedStreams(false)
    {
        // Just Initialising the StreamManager
    }

    quicStream openNewStream()
    {
        quicStream newStream = quicStream(lastID);
        streams[lastID] = newStream;
        lastID += 2;
        return newStream;
    }

    void addDataToRespectiveStream(STREAM_ID id, char *data, int length)
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

    int getStreamDataTobeSend(char *buffer, int bufferlen)
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
};

#endif // ROUGHCODE_HPP