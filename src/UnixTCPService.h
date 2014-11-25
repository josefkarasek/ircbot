//============================================================================
// Name        : UnixTCPService.h
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#ifndef UNIX_TCP_NETWORK_SERVICE_H
#define UNIX_TCP_NETWORK_SERVICE_H

#include <string>
#include "SocketNetworkService.h"

using namespace std;

class UnixTCPService : SocketNetworkService {
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
    bool IPv6;
    void *addr;
    struct addrinfo *res, *p;
//    struct addrinfo hints;
};

#endif
