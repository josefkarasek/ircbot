/*
 * UnixUDPService.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: pepa
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

UnixUDPService::UnixUDPService(string hostName, string port) {
    this->hostName = hostName;
    this->port = port;
    this->sockfd = -1;
    this->IPv6 = false;
    this->addr = nullptr;
}

UnixUDPService::~UnixUDPService() {

}

void UnixUDPService::establishClientConnection() {
    //resolve hostname to IP address
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if((status = getaddrinfo(hostName.c_str(), port.c_str(), &hints, &res)) != 0) {
        throw NetworkException("Error: couldn't resolve hostname to IP address.");
    }

    addr = (sockaddr *) res->ai_addr;
    if((this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        throw NetworkException("Error: couldn't create a socket.");

    //freeaddrinfo(res);

//    int err = sendto(sockfd, msg.c_str(), msg.length(), 0, res->ai_addr,res->ai_addrlen);
}

void UnixUDPService::sendMsg(string msg) {
//    sendto(dgram_socket, secret_message, strlen(secret_message)+1, 0,
//           (struct sockaddr*)&dest, sizeof dest);
    cout << "Sending :" << msg << endl;
    cout << "IPv4: " << inet_ntoa( ((struct sockaddr_in *)addr)->sin_addr ) << endl;
    int err = 0;
    if((err = sendto(this->sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*) (this->addr), 18)) <= 0) {
        throw NetworkException("Error: sendto failed.");
    }
    cout << "Odeslano " << err << "bajtu." << endl;
}

string UnixUDPService::readMsg() {
    return "";
}













