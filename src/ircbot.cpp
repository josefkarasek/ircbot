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

using namespace std;

void interruptHandler(int s, Ircbot ircbot);
//
//vector<string> split(string argument, string delimiter);
//string strip(const string &source, const char &delimiter);
//
///**
// *  Simple IRC bot class.
// *  Connects to a IRC server and receives messages from one channel.
// *  Processes messages and logs PRIVATE a NOTICE messages to a SYSLOG server.
// */
//class Ircbot {
//
//private:
//    string ircHostName;
//    string port;
//    string channel;
//    string syslogHostName;
//    vector<string> specialWords;
//    UnixTCPService *ircService;
//    UnixUDPService *syslogService;
//
//public:
//    Ircbot(string ircHostname, string port, string channel, string syslogHostName, vector<string> specialWords) {
//        this->ircHostName = ircHostname;
//        this->channel = channel;
//        this->syslogHostName = syslogHostName;
//        this->specialWords = specialWords;
//        if(port == "")
//            this->port = "6667";
//        else
//            this->port = port;
//        this->ircService = new UnixTCPService(ircHostName, this->port);
//        this->syslogService = new UnixUDPService(syslogHostName, "514");
//    }
//
//    ~Ircbot() {
//        delete(ircService);
//        delete(syslogService);
//    }
//
//    /**
//     *  Control method of this application.
//     *  Connects to IRC and SYSLOG servers.
//     *  Sends and receives messages.
//     *  Handles errors.
//     */
//    int run() {
//
//        try {
//            syslogService->establishClientConnection();
//
//            string serverResponse;
//            ircService->establishClientConnection();
//            ircService->sendMsg("NICK xkaras27\r\n");
//            serverResponse = ircService->readMsg();
//            dispatchMessage(split(serverResponse, "\n"));
//            ircService->sendMsg("USER xkaras27 xkaras27 xkaras27:xkaras27 xkaras27\r\n");
//            serverResponse = ircService->readMsg();
//            dispatchMessage(split(serverResponse, "\n"));
//            ircService->sendMsg("JOIN " + this->channel + "\r\n");
//            serverResponse = ircService->readMsg();
//            dispatchMessage(split(serverResponse, "\n"));
//
//            while(true) {
//                serverResponse = ircService->readMsg();
//                dispatchMessage(split(serverResponse, "\n"));
//            }
//
//        } catch (const NetworkException & e) {
//            cerr << e.what() << endl;
//            return 1;
//        }
//        return 0;
//    }
//
//    /**
//     *  Nicer way of printing messages.
//     */
//    void printLine(vector<string> compound) {
//        int lineNumber = 0;
//        vector<string> vec = compound;
//        vector<string>::iterator v = vec.begin();
//        while(v != vec.end()) {
//            cout << ++lineNumber << ":  " << *v << endl;
//            v++;
//        }
//    }
//
//    /**
//     *  Handle messages. Look for errors and special words.
//     *  Sends messages to SYSLOG server.
//     */
//    void dispatchMessage(vector<string> message) {
//        vector<string> vec = message;
//        vector<string>::iterator v = vec.begin();
//        while(v != vec.end()) {
//            vector<string> vec2 = split(*v, " ");
//            vector<string>::iterator v2 = vec2.begin();
//            int i = 0;
//            while(v2 != vec2.end() && i < 2) {
//
//                if((*v2).compare("PING") == 0) {
//                    string pong = "PONG " + *(v2 + 1) + "\r\n";
//                    ircService->sendMsg(pong);
//                }
//                // Error detection
//                if((*v2).compare("ERROR") == 0) {
//                    string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
//                           extractLogin(*(v2-1)) + ">" + ": " + strip(extractBody(v2 + 1, vec2.end()), '\r');
//                    syslogService->sendMsg(log);
//                    throw NetworkException("IRC server replied with error message.");
//                }
//                // Checking for error code
//                if(i == 1 && ((*v2)[0] == '4' || (*v2)[0] == '5')) {
//                    string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
//                            extractLogin(*(v2-1)) + ">" + ": " + strip(extractBody(v2 + 1, vec2.end()), '\r');
//                    syslogService->sendMsg(log);
//                    throw NetworkException("IRC server replied with error message.");
//                }
//                // Real work is done here:
//                if((*v2).compare("PRIVMSG") == 0 || (*v2).compare("NOTICE") == 0) {
//                    // If message starts with login information, extract the login
//                    string messageBody = strip(extractBody(v2 + 1, vec2.end()), '\r');
//                    string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
//                            extractLogin(*(v2-1)) + ">" + ": " + messageBody;
//
//                    // If no list of words to look for wasn't provided by the user, log everything.
//                    if(this->specialWords.empty()) {
//                        cout << log << endl;
//                        syslogService->sendMsg(log);
//                    } else {
//                        // Otherwise look for searched word(s).
//                        if(containsSpecialWord(messageBody)) {
//                            cout << log << endl;
//                            syslogService->sendMsg(log);
//                        }
//                    }
//                }
//                i++;
//                v2++;
//            }
//            v++;
//        }
//    }
//
//    /**
//     *  Loop through IRC message body and search for special words.
//     */
//    bool containsSpecialWord(string messageBody) {
//        stringstream s(messageBody);
//        string word;
//        for (int i = 0; s >> word; i++) {
//            for(unsigned j = 0; j < specialWords.size(); j++) {
//                if(word == specialWords[j])
//                    return true;
//            }
//        }
//        return false;
//    }
//
//    /**
//     *  Parses text body from IRC messages.
//     */
//    string extractBody(vector<string>::iterator v, vector<string>::iterator end) {
//        bool beginConcatenation = false;
//        string messageBody = "";
//        while(v != end) {
//            if(beginConcatenation == false && (*(v))[0] == ':') {
//                beginConcatenation = true;
//                string first_word = *(v);
//                *(v) = first_word.substr(1, first_word.length());
//            }
//            if(beginConcatenation) {
//                if((v+1) != end)
//                    messageBody += *v + ' ';
//                else
//                    messageBody += *v;
//            }
//            v++;
//        }
//        return messageBody;
//    }
//
//    /**
//     *  Parses login from IRC messages.
//     */
//    string extractLogin(string s) {
//        int start, end;
//        if(s[0] == ':') // We don't want the colon in the login.
//            start = 1;
//        else
//            start = 0;
//        if((end = s.find('!')) == -1) // If exclamation mark is present,
//            end = s.length();         // rest of string is domain name.
//        else                          // We don't want that either.
//            end--;                    // Neither the exclamation point.
//        return s.substr(start, end);
//    }
//
//    /**
//     *  Generates current time stamp in format Sep 21 17:33:19.
//     */
//    string timeStamp() {
//        time_t rawtime;
//        char buffer[80];
//
//        time(&rawtime);
//        strftime(buffer,80,"%b %d %H:%M:%S", localtime(&rawtime));
//
//        string str(buffer);
//        return str;
//    }
//
//};
//
///**
// *  CLI arguments are stored here.
// */
struct arguments {
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;
};
//
///**
// *  Split string on given delimiter.
// *  @returns Empty vector or vector containing strings split on delimiter. Delimiter isn't returned back.
// */
//vector<string> split(string source_string, string delimiter) {
//    vector<string> parts;
//    int position;
//    while(!source_string.empty()) {
//        position = source_string.find(delimiter);
//        //String doesn't contain given character
//        if(position == -1) {
//            parts.push_back(source_string);
//            break;
//        }
//        parts.push_back(source_string.substr(0, position));
//        source_string.erase(0, position + 1);
//    }
//    return parts;
//}
//
///**
// *  Strip string from both sides by given character.
// *  @returns Stripped string
// */
//string strip(const string &source, const char &delimiter) {
//    unsigned int first = 0;
//    while (first <= source.length() && source[first] == delimiter)
//        ++first;
//
//    int last = source.length() -1;
//    while (last >= 0 && source[last] == delimiter)
//        --last;
//
//    return source.substr(first, last - first + 1);
//}

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

void interruptHandler(int s, Ircbot ircbot) {
    ircbot.sendQuit(s);
}
/**
 *  This program starts here.
 */
int main(int argc, char ** argv) {
    if(argc < 4) {
        printHelp();
        return 1;
    }

//    struct sigaction sigIntHandler;
//
//    sigIntHandler.sa_handler = interruptHandler;
//    sigemptyset(&sigIntHandler.sa_mask);
//    sigIntHandler.sa_flags = 0;
//
//    sigaction(SIGINT, &sigIntHandler, NULL);
    arguments a = checkAndParseArguments(argc, argv);

    Ircbot * bot = new Ircbot(a.ircHostName, a.port, a.channel, a.syslogHostName, a.specialWords);
    int err = bot->run();

    delete(bot);

    return err;
}
