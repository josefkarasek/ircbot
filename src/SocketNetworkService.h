
#ifndef SOCKET_NETWORK_SERVICE_H
#define SOCKET_NETWORK_SERVICE_H

#include <string>
using namespace std;

class SocketNetworkService {

public:
    virtual ~SocketNetworkService() {};
    virtual void establishClientConnection() = 0;
    virtual void sendMsg(string msg) = 0;
    virtual string readMsg() = 0;
//    virtual string getMyIP() = 0;
};

#endif
