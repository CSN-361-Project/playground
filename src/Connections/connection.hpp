#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../includes.hpp"

//Forward Declaration
class quicConnectionNode;


// const quicConnection FAILED_CONNECTION = quicConnection();



class quicConnection
{
    // This is a class that will be used to handle the connection between the server and the client
public:
    quicConnectionNode *selfNode; // required during deletion of the connection from list

    // Data Members ----------------
    STATUS_CODE status;

    // PACKETS
    PACKET_LINKEDLIST RecievedPackets;
    PACKET_LINKEDLIST SendPackets;

    // FRAMES
    FRAME_LINKED_LIST ToSendFrames;

    // peer details [-- will be using for sending packets back to the peer]
    SocketAddress peerAddress;
    PeerConnectionIDManager peerConnectionIDs;

    // Member Function Declarations ----------------
    quicConnection();
    Stream openNewStream();
    // Sending and Recieving and closing data is handled by streams
    int closeConnection();

    // Member Function Implementations ----------------
    quicConnection(){
        // Initialising the connection
        status = CONNECTION_REQUESTED;
        SendPackets = PACKET_LINKEDLIST();
        RecievedPackets = PACKET_LINKEDLIST();
        ToSendFrames = FRAME_LINKED_LIST();
        peerAddress = SocketAddress();
        peerConnectionIDs = PeerConnectionIDManager();

    }

};

// Linked List for Connections
class quicConnectionNode
{
public:
    quicConnection *connection;
    quicConnectionNode *next;
    quicConnectionNode *prev;
    quicConnectionNode(quicConnection *connection)
    {
        this->connection = connection;
        connection->selfNode = this;
        this->next = NULL;
        this->prev = NULL;
    }

    ~quicConnectionNode()
    {
        // Node deletion will not delete the connection object
    }
};

class QUIC_CONNECTION_LINKED_LIST
{
public:
    // Data Members -----
    quicConnectionNode *head;
    quicConnectionNode *tail;
    int size;

    // Member Function Declarations ----------------
    void addConnection(quicConnection *connection);
    void addConnectionAtTail(quicConnection *connection);
    void addConnectionAtHead(quicConnection *connection);
    int removeNode(quicConnectionNode *current);      // To remove the node from the list
    int removeConnection(quicConnection *connection); // To remove the connection from the list
    quicConnection *peekHead();
    quicConnection *extractHead();
    quicConnection *peekTail();
    quicConnection *extractTail();



    // Member Function Implementations ----------------
    void addConnection(quicConnection *connection)
    {
        addConnectionAtTail(connection);
    }

    void addConnectionAtTail(quicConnection *connection)
    {
        quicConnectionNode *newNode = new quicConnectionNode(connection);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void addConnectionAtHead(quicConnection *connection)
    {
        quicConnectionNode *newNode = new quicConnectionNode(connection);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            head->prev = newNode;
            newNode->next = head;
            head = newNode;
        }
    }

    int removeNode(quicConnectionNode *current)
    {
        if (current == NULL)
        {
            return -1; // self Link to node don't exist
        }

        if (tail == current)
        {
            tail = current->prev;
        }
        if (head == current)
        {
            head = current->next;
        }

        if (current->prev != NULL)
        {
            current->prev->next = current->next;
        }

        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }

        delete current;
        return 0;
    }

    int removeConnection(quicConnection *connection)
    {
        // we already have the connection object, so we can directly remove it
        quicConnectionNode *current = connection->selfNode;
        return removeNode(current);
    }

    quicConnection *peekHead()
    {
        if (head == NULL)
        {
            return NULL;
        }
        return head->connection;
    }

    quicConnection *extractHead()
    {
        if (head == NULL)
        {
            return NULL;
        }
        quicConnection *connection = head->connection;
        removeNode(head);
        return connection;
    }

    quicConnection *peekTail()
    {
        if (tail == NULL)
        {
            return NULL;
        }
        return tail->connection;
    }

    quicConnection *extractTail()
    {
        if (tail == NULL)
        {
            return NULL;
        }
        quicConnection *connection = tail->connection;
        removeNode(tail);
        return connection;
    }

    


    ~QUIC_CONNECTION_LINKED_LIST()
    {
        quicConnectionNode *current = head;
        while (current != NULL)
        {
            quicConnectionNode *temp = current;
            current = current->next;
            delete temp;
        }
    }
};

#endif // CONNECTION_HPP