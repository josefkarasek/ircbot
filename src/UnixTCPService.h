#ifndef UNIX_TCP_NETWORK_SERVICE_H
#define UNIX_TCP_NETWORK_SERVICE_H

#include <string>
#include "SocketNetworkService.h"

using namespace std;

class UnixTCPService :  SocketNetworkService {
public:
    UnixTCPService(string hostName, string port);
    ~UnixTCPService();
    void establishClientConnection();
    void sendMsg(string msg);
    string readMsg();
    string getMyIP();

private:
    string hostName;
    string port;
    int sockfd;
    static const int MSG_LEN = 1500;
    char msg_buffer[MSG_LEN];
    bool IPv6;
};

#endif
