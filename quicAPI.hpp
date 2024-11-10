#ifndef QUIC_API_HPP
#define QUIC_API_HPP

// C++ Libraries
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>

// API Libraries
#include "src/Others/support.hpp"


// Global Constants
int MAX_CONNECTION_ID_SIZE = 8; // 8 bytes = 64 bits
int MAXLINE = 1024;             // 1 KB
char *SERVER_PORT = "12345";

// Global variable
int byteSizeOfConnectionID = 2; // will be re-set by serverCreationConstructor

// Status and Error Management
// We use negative for error codes and positive for success codes

// Funtions return -1 if something went wrong, then you can check the status code for more information

const std::string HANDSHAKE_MESSAGE = "Hello from client";
const std::string HANDSHAKE_RESPONSE = "Hello from server";

#endif // QUIC_API_HPP