#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

string strip(const string &source, const char &delimiter);
vector<string> split(string source_string, string delimiter);

#endif
