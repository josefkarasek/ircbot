
#ifndef SOCKET_NETWORK_SERVICE_H
#define SOCKETNETWORK_SERVICE_H

class SocketNetworkService {

public:
    virtual ~SocketNetworkService() {};
    virtual void establishClientConnection() = 0;
};

#endif
