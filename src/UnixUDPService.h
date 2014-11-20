#ifndef UNIX_UDP_NETWORK_SERVICE_H
#define UNIX_UDP_NETWORK_SERVICE_H

#include <string>
#include "SocketNetworkService.h"

using namespace std;

class UnixUDPService : SocketNetworkService {
public:
    UnixUDPService(string hostName, string port);
    ~UnixUDPService();
    void establishClientConnection();
    void sendMsg(string msg);
    string readMsg();

private:
    string hostName;
    string port;
    int sockfd;
    static const int MSG_LEN = 1500;
    char msg_buffer[MSG_LEN];
    bool IPv6;
    void *addr;
};

#endif
