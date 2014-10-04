//============================================================================
// Name        : ircbot.cpp
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     :
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include "UnixNetworkService.h"
#include "NetworkException.h"

using namespace std;

class Ircbot {

private:
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;

public:
    Ircbot(string ircHostname, string port, string channel, string syslogHostName, vector<string> specialWords) {
        this->ircHostName = ircHostname;
        this->port = port;
        this->channel = channel;
        this->syslogHostName = syslogHostName;
        this->specialWords = specialWords;
    }



    int run() {
        UnixNetworkService *svc = new UnixNetworkService(ircHostName, port);

        try {
            svc->establishClientConnection();
            svc->readMsg();
            svc->readMsg();
            svc->sendMsg("PASS dummy\r\n");
            svc->readMsg();
            svc->sendMsg("NICK NeoWithMustage\r\n");
            svc->readMsg();
            svc->sendMsg("USER guest tolmoon tolsun :Ronnie Reagan\r\n");
            svc->readMsg();
            svc->sendMsg("QUIT :Gone to have lunch\r\n");
            svc->readMsg();
            svc->readMsg();

        } catch (NetworkException const& e) {
            cerr << e.what() << endl;
            return 1;
        }
        return 0;
    }

};

struct arguments {
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;
};

/**
 *
 */
void printHelp() {
    string helpBody = "Usage:"
            "\n"
            "\tircbot <host>[:<port>] <channel> <syslog_server> [list]"
            "\n"
            "Example usage:"
            "\n"
            "\tircbot irc.server.com:6667 #CHANNEL syslog.server.com lan;wan;tcp ";
    cerr << helpBody << endl;
}

/**
 *
 */
vector<string> split(string argument, string delimiter) {
    vector<string> parts;
    int pos;
    while(!argument.empty()) {
        pos = argument.find(delimiter);
        //String doesn't contain given character
        if(pos == -1) {
            parts.push_back(argument.substr(0, pos));
            break;
        }
        parts.push_back(argument.substr(0, pos));
        argument.erase(0, pos + 1);
    }
    return parts;
}

/**
 *
 */
arguments checkAndParseArguments(int argc, char ** argv) {
    //find port number if is present
    string port = "";
    string hostname = argv[1];
    int pos = hostname.find(":");
    if(pos != -1) {
        vector<string> host_and_port = split(hostname, ":");
        hostname = host_and_port.at(0);
        port = host_and_port.at(1);
    }
    //check whether list of special words is present
    vector<string> specialWords;
    if(argc == 5) {
        specialWords = split(static_cast<string>(argv[4]), ";");
    }

    arguments args;
    args.ircHostName = hostname;
    args.port = port;
    args.channel = argv[2];
    args.syslogHostName = argv[3];
    args.specialWords = specialWords;

    return args;
}

/**
 *
 */
int main(int argc, char ** argv) {
    if(argc < 4) {
        printHelp();
        return 1;
    }

    arguments a = checkAndParseArguments(argc, argv);
    cout << a.ircHostName << "  " << a.port << "  " << a.channel << "  " << a.syslogHostName << "  " << endl;

    vector<string> vec = a.specialWords ;
    vector<string>::iterator v = vec.begin();
    while(v != vec.end()) {
        cout << *v << endl;
        v++;
    }

    Ircbot * bot = new Ircbot(a.ircHostName, a.port, a.channel, a.syslogHostName, a.specialWords);

    return bot->run();;
}







































