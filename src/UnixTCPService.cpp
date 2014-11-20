//#include "SocketNetworkService.h"
//#include "InetAddr.h"
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
}

UnixTCPService::~UnixTCPService() {
    //..be coming
}

void UnixTCPService::establishClientConnection() {
    //resolve hostname to IP address
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((status = getaddrinfo(hostName.c_str(), port.c_str(), &hints, &res)) != 0) {
        throw NetworkException("Error: couldn't resolve hostname to IP address.");
    }

    //Search through list of addresses and pick first one. IPv4 preferred.
    void *addr; //Is never used!!
    for(p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) { // IPv4
//            cout << "Volim si tebe IPv4!\n";
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
//            this->address = ipv4;
            break;
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
//            this->address = ipv6;
            IPv6 = true;
            break;
        }
    }
    if((this->sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
        throw NetworkException("Error: couldn't create a socket.");

    // Use connect only if TCP communication is used.
    if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        throw NetworkException("Error when creating socket.");
    }
    //freeaddrinfo(res);
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
    memset(&msg_buffer, 0, MSG_LEN);
    int bytes_read = recv(sockfd, &msg_buffer, MSG_LEN, 0);
    if(bytes_read < 0) {
        throw NetworkException("Error when receiving message.");
    }
    string returnMsg = msg_buffer;
    return returnMsg;
}

void UnixTCPService::sendMsg(string msg) {
    usleep(1000000);

    int bytes_sent;
    bytes_sent = send(sockfd, msg.c_str(), msg.length(), 0);

    if(bytes_sent < 0)
        throw NetworkException("Error when sending message.");
}
