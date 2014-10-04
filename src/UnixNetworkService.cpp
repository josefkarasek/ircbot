//#include "SocketNetworkService.h"
//#include "InetAddr.h"
#include "NetworkException.h"
#include "UnixNetworkService.h"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

using namespace std;

    UnixNetworkService::UnixNetworkService(string hostName, string port) {
        this->hostName = hostName;
        this->port = port;
        this->sockfd = -1;
    }
    UnixNetworkService::~UnixNetworkService() {
        //..be coming
    }

    void UnixNetworkService::establishClientConnection() {
        //resolve hostname to IP address

        struct addrinfo hints, *res, *p;
        int status;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(hostName.c_str(), port.c_str(), &hints, &res)) != 0) {
            throw NetworkException("Error: couldn't resolve hostname to IP address.");
        }

        //Search through list of addresses and pick first one. IPv4 preferred.
        void *addr;
        for(p = res;p != NULL; p = p->ai_next) {
            if (p->ai_family == AF_INET) { // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                break;
            } else { // IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                break;
            }
        }

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
            throw NetworkException("Error when creating socket.");
        }

        //freeaddrinfo(res);

    }

    string UnixNetworkService::readMsg() {
        memset(&msg_buffer, 0, MSG_LEN);
        int bytes_read = recv(sockfd, &msg_buffer, MSG_LEN, 0);
        if(bytes_read <= 0) {
            throw NetworkException("Error when receiving message.");
        }

        cout << bytes_read << msg_buffer << endl;
        string returnMsg = msg_buffer;
        return returnMsg;
    }

    void UnixNetworkService::sendMsg(string msg) {
        usleep(1000000);
        int bytes_sent = send(sockfd, msg.c_str(), msg.length(), 0);
        if(bytes_sent < 0) {
            throw NetworkException("Error when sending message.");
        }

    }


















