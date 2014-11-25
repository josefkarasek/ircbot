//============================================================================
// Name        : UnixUDPService.cpp
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

/**
 *  Implementation of UnixUDPService.h interface.
 */
#include "UnixUDPService.h"
#include "NetworkException.h"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

UnixUDPService::UnixUDPService(const string &hostName, const string &port) {
    this->hostName = hostName;
    this->port = port;
    this->sockfd = -1;
    this->IPv6 = false;
    this->addr = nullptr;
    this->res = nullptr;
}

UnixUDPService::~UnixUDPService() {
    freeaddrinfo(res);
    close(sockfd);
}

void UnixUDPService::establishClientConnection() {
    //resolve hostname to IP address
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    if((status = getaddrinfo(hostName.c_str(), port.c_str(), &hints, &res)) != 0)
        throw NetworkException("Error: couldn't resolve hostname to IP address.");

    //create new socket
    addr = (sockaddr *) res->ai_addr;
    if((this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        throw NetworkException("Error: couldn't create a socket.");
}

void UnixUDPService::sendMsg(string msg) {
    int err = 0;
    if((err = sendto(this->sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*) (this->addr), 18)) <= 0) {
        throw NetworkException("Error: sendto failed.");
    }
}

/**
 *  Reading messages is not necessary for this kind of tasks.
 */
string UnixUDPService::readMsg() {
    return "";
}
