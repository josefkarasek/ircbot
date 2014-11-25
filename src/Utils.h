#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

/**
 *  Strip string from both sides by given character.
 *  @returns Stripped string
 */
string strip(const string &source, const char &delimiter);

/**
 *  Split string on given delimiter.
 *  @returns Empty vector or vector containing strings split on delimiter. Delimiter isn't returned back.
 */
vector<string> split(string source_string, string delimiter);

#endif
