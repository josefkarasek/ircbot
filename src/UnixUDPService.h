//============================================================================
// Name        : UnixUDPService.h
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#ifndef UNIX_UDP_NETWORK_SERVICE_H
#define UNIX_UDP_NETWORK_SERVICE_H

#include <string>
#include "SocketNetworkService.h"

using namespace std;

class UnixUDPService : SocketNetworkService {
public:
    UnixUDPService(const string &hostName, const string &port);
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
    struct addrinfo *res;
//    struct addrinfo hints;
};

#endif
