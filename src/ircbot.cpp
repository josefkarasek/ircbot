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
#include <ctime>
#include <sstream>
#include "UnixUDPService.h"
#include "UnixTCPService.h"
#include "NetworkException.h"

using namespace std;

vector<string> split(string argument, string delimiter);

class Ircbot {

private:
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;
    UnixTCPService *ircService;
    UnixUDPService *syslogService;

public:
    Ircbot(string ircHostname, string port, string channel, string syslogHostName, vector<string> specialWords) {
        this->ircHostName = ircHostname;
        this->channel = channel;
        this->syslogHostName = syslogHostName;
        this->specialWords = specialWords;
        if(port == "")
            this->port = "6667";
        else
            this->port = port;
        this->ircService = new UnixTCPService(ircHostName, this->port);
        this->syslogService = new UnixUDPService(syslogHostName, "6667");
    }



    int run() {

        try {
            syslogService->establishClientConnection();
            syslogService->sendMsg("<134>Sep 21 17:32:18 192.168.0.2 ircbot <xlogin01>: nej predmet na fit je isa");

            string serverResponse;
            ircService->establishClientConnection();
//            ircService->readMsg();
//            ircService->readMsg();
//            ircService->sendMsg("PASS dummy\r\n");
//            ircService->readMsg();
            ircService->sendMsg("NICK NeoWithMustage\r\n");
            serverResponse = ircService->readMsg();
//            printLine(split(serverResponse, "\n"));
            dispatchMessage(split(serverResponse, "\n"));
            ircService->sendMsg("USER xkaras27 xkaras27 xkaras27:xkaras27 xkaras27\r\n");
            serverResponse = ircService->readMsg();
//            printLine(split(serverResponse, "\n"));
            dispatchMessage(split(serverResponse, "\n"));
//            ircService->sendMsg("LIST\r\n");
//            ircService->readMsg();
            ircService->sendMsg("JOIN " + this->channel + "\r\n");
            serverResponse = ircService->readMsg();
//            printLine(split(serverResponse, "\n"));
            dispatchMessage(split(serverResponse, "\n"));
//            ircService->sendMsg("QUIT :Gone to have lunch\r\n");
//            ircService->readMsg();
            while(true) {
                serverResponse = ircService->readMsg();
//                printLine(split(serverResponse, "\n"));
                dispatchMessage(split(serverResponse, "\n"));
            }

        } catch (const NetworkException & e) {
            cerr << e.what() << endl;
            return 1;
        }
        return 0;
    }

    /**
     *
     */
    void printLine(vector<string> compound) {
        int lineNumber = 0;
        vector<string> vec = compound ;
        vector<string>::iterator v = vec.begin();
        while(v != vec.end()) {
            cout << ++lineNumber << ":  " << *v << endl;
            v++;
        }
    }

    /**
     *
     */
    void dispatchMessage(vector<string> message) {
        vector<string> vec = message;
        vector<string>::iterator v = vec.begin();
        while(v != vec.end()) {
            vector<string> vec2 = split(*v, " ");
            vector<string>::iterator v2 = vec2.begin();
            int i = 0;
            while(v2 != vec2.end() && i < 2) {

                if((*v2).compare("PING") == 0) {
                    string pong = "PONG " + *(v2 + 1) + "\r\n";
                    ircService->sendMsg(pong);
                }
                // Error detection
                if((*v2).compare("ERROR") == 0) {
                    cout << "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                            extractLogin(*(v2-1)) + ">" + ": " + extractBody(v2 + 1, vec2.end()) << endl;
                    throw NetworkException("IRC server replied with error messagage.");
                }
                // Checking for error code
                if(i == 1 && ((*v2)[0] == '4' || (*v2)[0] == '5')) {
                    cout << "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                            extractLogin(*(v2-1)) + ">" + ": " + extractBody(v2 + 1, vec2.end()) << endl;
                    throw NetworkException("IRC server replied with error messagage.");
                }
                // Real work is done here:
                if((*v2).compare("PRIVMSG") == 0 || (*v2).compare("NOTICE") == 0) {
                    // If message starts with login information, extract the login
                    string messageBody = extractBody(v2 + 1, vec2.end());
                    cout << "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                            extractLogin(*(v2-1)) + ">" + ": " + messageBody << endl;
                    if(containsSpecialWord(messageBody)) {
                        cout << "Nasel jsem SPECIAL WORD!!!" << endl;
                    }
                }
                i++;
                v2++;
            }
            v++;
        }
    }

    bool containsSpecialWord(string messageBody) {
        stringstream s(messageBody);
        string word;
        for (int i = 0; s >> word; i++) {
            for(unsigned j = 0; j < specialWords.size(); j++) {
                if(word == specialWords[j])
                    return true;
            }
        }
        return false;
    }

    string extractBody(vector<string>::iterator v, vector<string>::iterator end) {
        bool beginConcatenation = false;
        string messageBody = "";
        while(v != end) {
            if(beginConcatenation == false && (*(v))[0] == ':') {
                beginConcatenation = true;
                string first_word = *(v);
                *(v) = first_word.substr(1, first_word.length());
            }
            if(beginConcatenation) {
                if((v+1) != end)
                    messageBody += *v + ' ';
                else
                    messageBody += *v;
            }
            v++;
        }
        return messageBody;
    }

    string extractLogin(string s) {
        int start, end;
        if(s[0] == ':') // We don't want the colon in the login.
            start = 1;
        else
            start = 0;
        if((end = s.find('!')) == -1) // If exclamation mark is present,
            end = s.length();         // rest of string is domain name.
        else                          // We don't want that either.
            end--;                    // Neither the exclamation point.
        return s.substr(start, end);
    }

    /**
     *  Generates current time stamp in format Sep 21 17:33:19.
     */
    string timeStamp() {
        time_t rawtime;
        char buffer[80];

        time(&rawtime);
        strftime(buffer,80,"%b %d %H:%M:%S", localtime(&rawtime));

        string str(buffer);
        return str;
    }

};

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
            "\tircbot irc.server.com:6667 #CHANNEL syslog.server.com lan;wan;tcp ";
    cerr << helpBody << endl;
}

/**
 *  Split string on given delimiter.
 *  @returns Empty vector or vector containing strings split on delimiter. Delimiter isn't returned back.
 */
vector<string> split(string source_string, string delimiter) {
    vector<string> parts;
    int position;
    while(!source_string.empty()) {
        position = source_string.find(delimiter);
        //String doesn't contain given character
        if(position == -1) {
            parts.push_back(source_string);
            break;
        }
        parts.push_back(source_string.substr(0, position));
        source_string.erase(0, position + 1);
    }
    return parts;
}

/**
 *  Parses arguments and validates their values.
 *  @returns Struct arguments filled with respecitve values.
 */
arguments checkAndParseArguments(int argc, char ** argv) {
    //find port number if is present
    string port = "";
    string hostname = argv[1];
    int position = hostname.find(":");
    if(position != -1) {
        vector<string> host_and_port = split(hostname, ":");
        hostname = host_and_port.at(0);
        port = host_and_port.at(1);
    }
    //check whether list of special words is present
    vector<string> specialWords = "";
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
 *  This program starts here.
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

    return bot->run();
}
