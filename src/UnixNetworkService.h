#include <string>
#include "SocketNetworkService.h"

using namespace std;

#ifndef UNIX_NETWORK_SERVICE_H
#define UNIX_NETWORK_SERVICE_H

class UnixNetworkService :  SocketNetworkService {
public:
    UnixNetworkService(string hostName, string port);
    ~UnixNetworkService();
    void establishClientConnection();
    void sendMsg(string msg);
    string readMsg();

private:
    string hostName;
    string port;
    int sockfd;
    static const int MSG_LEN = 1500;
    char msg_buffer[MSG_LEN];
};

#endif
