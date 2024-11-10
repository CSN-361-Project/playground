#ifndef CONNECTIONID_HPP
#define CONNECTIONID_HPP


#include "../includes.hpp"
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
template <class T>
using ordered_set = __gnu_pbds::tree<
    T,
    __gnu_pbds::null_type,
    std::less<T>,
    __gnu_pbds::rb_tree_tag,
    __gnu_pbds::tree_order_statistics_node_update>;

std::mt19937_64 randomNumberGenerator(std::chrono::steady_clock::now().time_since_epoch().count());

class connectionIDManager
{
    // Data Members ----------------
    int byteSizeOfConnectionID;
    ordered_set<unsigned long long> usedID;
    std::map<CONNECTION_ID, quicConnection *> connectionIDMap;
    std::map<quicConnection *, std::set<CONNECTION_ID> > connectionMap;

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

    CONNECTION_ID getNewConnectionID()
    {
        unsigned long long S = (1ULL << (8 * byteSizeOfConnectionID)) - usedID.size();
        std::uniform_int_distribution<unsigned long long> dist(0, S);
        unsigned long long indexOfNewID = dist(randomNumberGenerator);
        if (usedID.size() == 0)
        {
            return 0ULL;
        }
        unsigned long long low = 0, high = usedID.size() - 1, lowerIndex = -1;
        while (low <= high)
        {
            unsigned long long mid = low + (high - low) / 2;
            unsigned long long missedTillNow = *usedID.find_by_order(mid) - mid;
            if (missedTillNow < indexOfNewID)
                lowerIndex = mid, low = mid + 1;
            else
                high = mid - 1;
        }
        CONNECTION_ID newID = lowerIndex + indexOfNewID;
        usedID.insert(newID);
        return newID;
    }

    int registerConnectionID(CONNECTION_ID connectionID, quicConnection *connection)
    {
        if (connectionIDMap.count(connectionID))
        {
            return -1;
        }
        connectionIDMap[connectionID] = connection;
        connectionMap[connection].insert(connectionID);
        return 0;
    }

    int deRegisterConnectionID(CONNECTION_ID connectionID)
    {
        if (!connectionIDMap.count(connectionID))
        {
            return -1;
        }
        quicConnection *connection = connectionIDMap[connectionID];
        connectionIDMap.erase(connectionID);
        connectionMap[connection].erase(connectionID);
        if (connectionMap[connection].size() == 0)
        {
            connectionMap.erase(connection);
        }

        return 0;
    }

    int deRegisterAllConnectionID()
    {
        connectionIDMap.clear();
        connectionMap.clear();
        return 0;
    }

    int deRegisterConnection(quicConnection *connection)
    {
        if (!connectionMap.count(connection))
        {
            return -1;
        }
        for (auto registeredID : connectionMap[connection])
        {
            connectionIDMap.erase(registeredID);
        }
        connectionMap.erase(connection);
        return 0;
    }

    quicConnection *getConnection(CONNECTION_ID connectionID)
    {
        return connectionIDMap[connectionID];
    }
};

class PeerConnectionIDManager
{
    // Data Members ----------------
    int sizeofPeerConnectionID;
    std::set<CONNECTION_ID> usedID;
    public:
    // Member Function Declarations ----------------
    PeerConnectionIDManager(); // set 0-8
    int setSizeOfPeerConnectionID(int sizeofPeerConnectionID);
    int addConnectionID(CONNECTION_ID peerID);
    int retireConnectionID(CONNECTION_ID peerConnectionID);
    int getSizeOfPeerConnectionID();
    int retireAllConnectionID();
    CONNECTION_ID getRandomConnectionID();


    // Member Function Implementations ----------------
    PeerConnectionIDManager()
    {
        sizeofPeerConnectionID = 2;
    }

    int setSizeOfPeerConnectionID(int sizeofPeerConnectionID)
    {
        this->sizeofPeerConnectionID = sizeofPeerConnectionID;
    }

    int addConnectionID(CONNECTION_ID peerID)
    {
        if (usedID.count(peerID))
        {
            return -1;
        }
        usedID.insert(peerID);
        return 0;
    }

    int retireConnectionID(CONNECTION_ID peerConnectionID)
    {
        if (!usedID.count(peerConnectionID))
        {
            return -1;
        }
        usedID.erase(peerConnectionID);
        return 0;
    }

    int getSizeOfPeerConnectionID()
    {
        return sizeofPeerConnectionID;
    }

    int retireAllConnectionID()
    {
        usedID.clear();
        return 0;
    }

    CONNECTION_ID getRandomConnectionID()
    {
        // get any connection ID from the set
        int size = usedID.size();
        if (size == 0)
        {
            return NO_PEERS_CONNECTIONID_AVAILABLE;
        }
        std::uniform_int_distribution<int> dist(0, size - 1);
        int index = dist(randomNumberGenerator);
        auto it = usedID.begin();
        std::advance(it, index);
        return *it;
    }

};

#endif // CONNECTIONID_HPP