#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <string>
// using errorCode = int;
using statusCode = int;
using errorLog = std::string;

// We use negative for error codes and positive for success codes

// Funtions return -1 if something went wrong, then you can check the status code for more information

const string HANDSHAKE_MESSAGE = "Hello from client";
const string HANDSHAKE_RESPONSE = "Hello from server";


const statusCode CLIENT_SOCKET_CREATION_INITIATED = 1;
const statusCode CLIENT_SOCKET_CREATION_SUCCESS = 2;
const statusCode CLIENT_UDP_SOCKET_CREATION_FAILED = -1;
const statusCode CLIENT_CONNECTION_INITIATED = 3;
const statusCode CLIENT_CONNECTION_WAITING_FOR_RESPONSE = 4;
const statusCode CLIENT_CONNECTION_SUCCESS = 5;
const statusCode CLIENT_CONNECTION_REQUEST_SEND_FAILED = -2;
const statusCode CLIENT_CONNECTION_REQUEST_REJECTED = -3;


const statusCode SERVER_SOCKET_CREATION_INITIATED = 1;
const statusCode SERVER_UDP_SOCKET_CREATION_FAILED = -1;
const statusCode SERVER_SOCKET_CREATION_SUCCESS = 2;

const statusCode SERVER_UDP_SOCKET_BIND_INITIATED = 4;
const statusCode SERVER_UDP_SOCKET_BIND_FAILED = -2;
const statusCode SERVER_UDP_SOCKET_BIND_SUCCESS = 5;

const statusCode SERVER_CONNECTION_SUCCESS = 5;



#endif // STATUSCODE_HPP
