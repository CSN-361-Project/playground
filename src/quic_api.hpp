#ifndef H_QUIC_API
#define H_QUIC_API

#include <iostream>
#include <cstring>     // For memset
#include <arpa/inet.h> // For sockaddr_in and inet_addr
#include <unistd.h>

class QuicSocket{
    public:
        QuicSocket();
        ~QuicSocket();
        void bind(int port);
        
    private:
        int sockfd;
        struct sockaddr_in server_addr, client_addr;
};

QuicSocket quicSocket(int domain, int type){
    
}








#endif // H_QUIC_API