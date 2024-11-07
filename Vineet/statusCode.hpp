#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

#include <string>
// using errorCode = int;
using STATUS_CODE = int;
using ERROR_LOG = std::string;

// Will change this -----TODO----- [make it dynamic]
using CONNECTION_ID = u_int64_t; // set it to max 8bytes

// We use negative for error codes and positive for success codes

// Funtions return -1 if something went wrong, then you can check the status code for more information

const std::string HANDSHAKE_MESSAGE = "Hello from client";
const std::string HANDSHAKE_RESPONSE = "Hello from server";



// Make sure to not use anysame interger for status codes
const STATUS_CODE CLIENT_SOCKET_CREATION_INITIATED = 1;
const STATUS_CODE CLIENT_SOCKET_CREATION_SUCCESS = 2;
const STATUS_CODE CLIENT_UDP_SOCKET_CREATION_FAILED = -1;
const STATUS_CODE CLIENT_CONNECTION_INITIATED = 3;
const STATUS_CODE CLIENT_CONNECTION_WAITING_FOR_RESPONSE = 4;
const STATUS_CODE CLIENT_CONNECTION_SUCCESS = 5;
const STATUS_CODE CLIENT_CONNECTION_REQUEST_SEND_FAILED = -2;
const STATUS_CODE CLIENT_CONNECTION_REQUEST_REJECTED = -3;


const STATUS_CODE SERVER_CREATE_SUCCESS = 6;
const STATUS_CODE SERVER_LISTENER_OPEN_FAILED = -4;
const STATUS_CODE SERVER_LISTENER_READY = 7;
const STATUS_CODE SERVER_LISTENING = 8;



#endif // STATUS_CODE_HPP
