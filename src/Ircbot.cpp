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
#include <unistd.h>
#include <memory>

using namespace std;

Ircbot::Ircbot(string ircHostname, string port, string channel, string syslogHostName, vector<string> specialWords) {
    this->ircHostName = ircHostname;
    this->channel = channel;
    this->syslogHostName = syslogHostName;
    this->specialWords = specialWords;
    if(port == "")
        this->port = "6667";
    else
        this->port = port;
//    this->ircService = new UnixTCPService(ircHostName, this->port);
//    this->syslogService = new UnixUDPService(syslogHostName, "514");

    ircService.reset(new UnixTCPService(ircHostName, this->port));
    syslogService.reset(new UnixUDPService(syslogHostName, "514"));
}

Ircbot::~Ircbot() {
//    delete(ircService);
//    delete(syslogService);
}

/**
 *  Control method of this application.
 *  Connects to IRC and SYSLOG servers.
 *  Sends and receives messages.
 *  Handles errors.
 */
int Ircbot::run() {

    try {
        syslogService->establishClientConnection();

        string serverResponse;
        ircService->establishClientConnection();
        ircService->sendMsg("NICK xkaras27\r\n");
        serverResponse = ircService->readMsg();
        dispatchMessage(split(serverResponse, "\n"));
        ircService->sendMsg("USER xkaras27 xkaras27 xkaras27:xkaras27 xkaras27\r\n");
        serverResponse = ircService->readMsg();
        dispatchMessage(split(serverResponse, "\n"));
        string joinChannel = "JOIN " + this->channel + "\r\n";
        ircService->sendMsg(joinChannel);  //TODO: WTF proc ma problem s "JOIN " + channel + "\r\n" ??
        serverResponse = ircService->readMsg();
        dispatchMessage(split(serverResponse, "\n"));

        while(true) {
            serverResponse = ircService->readMsg();
            dispatchMessage(split(serverResponse, "\n"));
        }

    } catch (const NetworkException & e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}

/**
 *  Nicer way of printing messages.
 */
void Ircbot::printLine(vector<string> compound) {
    int lineNumber = 0;
    vector<string> vec = compound;
    vector<string>::iterator v = vec.begin();
    while(v != vec.end()) {
        cout << ++lineNumber << ":  " << *v << endl;
        v++;
    }
}

/**
 *  Handle messages. Look for errors and special words.
 *  Sends messages to SYSLOG server.
 */
void Ircbot::dispatchMessage(vector<string> message) {
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
                string login;
                if(i == 0)
                    login = this->ircHostName;
                else
                    login = extractLogin(*(v2-1));
                string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                       login + ">" + ": " + strip(extractBody(v2 + 1, vec2.end()), '\r');
                cout << log << endl;
                syslogService->sendMsg(log);
                throw NetworkException("IRC server replied with error message.");
            }
            // Checking for error code
            if(i == 1 && ((*v2)[0] == '4' || (*v2)[0] == '5')) {
                string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                        extractLogin(*(v2-1)) + ">" + ": " + strip(extractBody(v2 + 1, vec2.end()), '\r');
                syslogService->sendMsg(log);
                throw NetworkException("IRC server replied with error message.");
            }
            // Real work is done here:
            if((*v2).compare("PRIVMSG") == 0 || (*v2).compare("NOTICE") == 0) {
                string login;
                if(i == 0)
                    login = this->ircHostName;
                else
                    login = extractLogin(*(v2-1));

                // If message starts with login information, extract the login
                string messageBody = strip(extractBody(v2 + 1, vec2.end()), '\r');
                string log = "<134>" + timeStamp() + " " + ircService->getMyIP() + " ircbot " + "<" +
                        login + ">" + ": " + messageBody;

                // If the list of words to look for wasn't provided by the user, log everything.
                if(this->specialWords.empty()) {
                    cout << log << endl;
                    syslogService->sendMsg(log);
                } else {
                    // Otherwise look for searched word(s).
                    if(containsSpecialWord(messageBody)) {
                        cout << log << endl;
                        syslogService->sendMsg(log);
                    }
                }
            }
            i++;
            v2++;
        }
        v++;
    }
}

/**
 *  Loop through IRC message body and search for special words.
 */
bool Ircbot::containsSpecialWord(string messageBody) {
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

/**
 *  Parses text body from IRC messages.
 */
string Ircbot::extractBody(vector<string>::iterator v, vector<string>::iterator end) {
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

/**
 *  Parses login from IRC messages.
 */
string Ircbot::extractLogin(string s) {
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
string Ircbot::timeStamp() {
    time_t rawtime;
    char buffer[80];

    time(&rawtime);
    strftime(buffer,80,"%b %d %H:%M:%S", localtime(&rawtime));

    string str(buffer);
    return str;
}
