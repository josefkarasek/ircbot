//============================================================================
// Name        : ircbot.h
// Author      : xkaras27@stud.fit.vutbr.cz
// Version     : 0.1
// Copyright   : Apache License, Version 2.0 You may obtain a copy of the
//               License at http://www.apache.org/licenses/LICENSE-2.0
// Description : Simple IRC bot
//============================================================================

#ifndef IRCBOT_H
#define IRCBOT_H

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <memory>

using namespace std;

class Ircbot {
public:
    Ircbot(string ircHostname, string port, string channel, string syslogHostName, vector<string> specialWords);
    ~Ircbot();
    int run();
    void printLine(vector<string> compound);
    void sendQuit(int s);

private:
    string ircHostName;
    string port;
    string channel;
    string syslogHostName;
    vector<string> specialWords;
//    UnixTCPService *ircService;
//    UnixUDPService *syslogService;
    unique_ptr<UnixTCPService> ircService;
    unique_ptr<UnixUDPService> syslogService;

    void dispatchMessage(vector<string> message);
    bool containsSpecialWord(string messageBody);
    string extractBody(vector<string>::iterator v, vector<string>::iterator end);
    string extractLogin(string s);
    string timeStamp();
};

#endif

