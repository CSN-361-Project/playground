#ifndef CONNECTIONID_HPP
#define CONNECTIONID_HPP


#include "includes.hpp"

// Connection ID Manager
class connectionIDManager{
    // Data Members ----------------
    int byteSizeOfConnectionID;
    std::map<CONNECTION_ID, quicConnection *> connectionIDMap;

    public:
    // Member Function Declarations ----------------
    connectionIDManager(); // set 0-8
    CONNECTION_ID getNewConnectionID();
    int registerConnectionID(CONNECTION_ID connectionID, quicConnection *connection);
    int deRegisterConnectionID(CONNECTION_ID connectionID);
    int deRegisterALLConnectionID();
    int deRegisterConnection(quicConnection *connection);
    quicConnection *getConnection(CONNECTION_ID connectionID);



    // Member Function Implementations ----------------
    


    

};

#endif // CONNECTIONID_HPP