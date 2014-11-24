//============================================================================
// Name        : SocketNetworkService.h
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#ifndef SOCKET_NETWORK_SERVICE_H
#define SOCKET_NETWORK_SERVICE_H

#include <string>
using namespace std;

/**
 *  Pure virtual class (interface) that defines interface
 *  which client classes must implement.
 *  Declares basic operations, such as establishing connection
 *  and sending messages back and forth.
 */
class SocketNetworkService {

public:
    virtual ~SocketNetworkService() {};
    virtual void establishClientConnection() = 0;
    virtual void sendMsg(string msg) = 0;
    virtual string readMsg() = 0;
};

#endif
