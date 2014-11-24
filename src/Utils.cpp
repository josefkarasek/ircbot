
#include <string>
#include <vector>
#include "Utils.h"

using namespace std;

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
 *  Strip string from both sides by given character.
 *  @returns Stripped string
 */
string strip(const string &source, const char &delimiter) {
    unsigned int first = 0;
    while (first <= source.length() && source[first] == delimiter)
        ++first;

    int last = source.length() -1;
    while (last >= 0 && source[last] == delimiter)
        --last;

    return source.substr(first, last - first + 1);
}
