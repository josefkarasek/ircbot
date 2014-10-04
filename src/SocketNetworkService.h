#include <string>

#ifndef SOCKET_NETWORK_SERVICE_H
#define SOCKETNETWORK_SERVICE_H

using namespace std;
class SocketNetworkService {

public:
    virtual ~SocketNetworkService() {};
    virtual void establishClientConnection() = 0;
    virtual void sendMsg(string msg) = 0;
    virtual string readMsg() = 0;
};

#endif
