#include <exception>

#ifndef NETWORK_EXCEPTION_H
#define NETWORK_EXCEPTION_H

using namespace std;

class NetworkException : public exception {
public:
    NetworkException(const char * errMessage) : errMessage_ (errMessage) {}

    const char * what () const throw () {
        return errMessage_;
    }
private:
    const char* errMessage_;
};

#endif

