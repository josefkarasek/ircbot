//============================================================================
// Name        : UnixTCPService.cpp
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#include "NetworkException.h"
#include "UnixTCPService.h"
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

using namespace std;

UnixTCPService::UnixTCPService(const string hostName, const string port) {
    this->hostName = hostName;
    this->port = port;
    this->sockfd = -1;
    this->IPv6 = false;
    this->addr = nullptr;
    this->p = nullptr;
    this->res = nullptr;
}

UnixTCPService::~UnixTCPService() {
//    freeaddrinfo(res);  //If exception is thrown, these two calls have problems to finish.
//    freeaddrinfo(p);    //Better off let leak some memory than have a seg faults.
    close(sockfd);
}

void UnixTCPService::establishClientConnection() {
    //resolve hostname to IP address
    struct addrinfo hints;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((status = getaddrinfo(hostName.c_str(), port.c_str(), &hints, &res)) != 0) {
        throw NetworkException("Error: couldn't resolve hostname to IP address.");
    }

    //Search through list of addresses and pick first one. IPv4 preferred.
    //Original at beej.us
    for(p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) { // IPv4
            break;
        } else { // IPv6
            IPv6 = true;
            break;
        }
    }
    if((this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        throw NetworkException("Error: couldn't create a socket.");

    // Use connect only if TCP communication is used.
    if(connect(sockfd, p->ai_addr, p->ai_addrlen) < 0)
        throw NetworkException("Error when creating socket.");
}

string UnixTCPService::getMyIP() {
    struct sockaddr_in ipv4;
    struct sockaddr_in6 ipv6;
    unsigned lenv4 = sizeof(ipv4);
    unsigned lenv6 = sizeof(ipv6);
    memset (&ipv4, 0, lenv4);
    memset (&ipv6, 0, lenv6);
    string ipAddress;
    char address[40];  // This is dummy variable, is never used and is not useful at all.
    memset(&address, 0, 40);

    if(this->IPv6) {
        getsockname(sockfd, (struct sockaddr *) &ipv6, &lenv6);
        ipAddress = inet_ntop(AF_INET6, (struct sockaddr *)&ipv6, address, 40);
    } else {
        getsockname(sockfd, (struct sockaddr *) &ipv4, &lenv4);
        ipAddress = inet_ntoa(ipv4.sin_addr);
    }
    return ipAddress;
}

string UnixTCPService::readMsg() {
    char buf[MSG_LEN];
    memset(&buf, 0, MSG_LEN);

    int bytes_read = recv(sockfd, &buf, MSG_LEN, 0);
    if(bytes_read == 0)
        throw NetworkException("Client closed connection.");
    if(bytes_read < 0)
        throw NetworkException("Error when receiving message.");

    string returnMsg(buf);
    return returnMsg;
}

void UnixTCPService::sendMsg(string msg) {
    int bytes_sent = send(sockfd, msg.c_str(), msg.length(), 0);
    if(bytes_sent < 0)
        throw NetworkException("Error when sending message.");
}
