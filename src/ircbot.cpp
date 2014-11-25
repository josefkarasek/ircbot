//============================================================================
// Name        : ircbot.cpp
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include "UnixUDPService.h"
#include "UnixTCPService.h"
#include "NetworkException.h"
#include "Ircbot.h"
#include "Utils.h"
#include <signal.h>
#include <stdlib.h>
#include <memory>

using namespace std;

///**
// *  Simple IRC bot class.
// *  Connects to a IRC server and receives messages from one channel.
// *  Processes messages and logs PRIVATE a NOTICE messages to a SYSLOG server.
// */

/**
 *  CLI arguments are stored here.
 */
struct arguments {
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;
};


/**
 *  Display help message.
 */
void printHelp() {
    string helpBody = "Usage:"
            "\n"
            "\tircbot <host>[:<port>] <channel> <syslog_server> [list]"
            "\n"
            "Example usage:"
            "\n"
            "\tircbot irc.server.com:6667 #CHANNEL syslog.server.com lan;wan;tcp";
    cerr << helpBody << endl;
}

/**
 *  Parses arguments and validates their values.
 *  @returns Struct arguments filled with respective values.
 */
arguments checkAndParseArguments(int argc, char ** argv) {
    //find port number if is present
    string port = "";
    string hostname = argv[1];
    vector<string> specialWords;
    int position = hostname.find(":");
    if(position != -1) {
        vector<string> host_and_port = split(hostname, ":");
        hostname = host_and_port.at(0);
        port = host_and_port.at(1);
    }
    //check whether list of special words is present
    if(argc == 5) {
        specialWords = split(static_cast<string>(argv[4]), ";");
    }

    arguments args;
    args.ircHostName = hostname;
    args.port = port;
    args.channel = argv[2];  //TODO: how to check channel's correctness?
    args.syslogHostName = argv[3];
    args.specialWords = specialWords;

    return args;
}

/**
 *  This program starts here.
 */
int main(int argc, char ** argv) {
    if(argc < 4) {
        printHelp();
        return 1;
    }

    arguments a = checkAndParseArguments(argc, argv);

    std::unique_ptr<Ircbot> bot(new Ircbot(a.ircHostName, a.port, a.channel, a.syslogHostName, a.specialWords));

    int err = bot->run();

    return err;
}
