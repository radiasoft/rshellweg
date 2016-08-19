#ifndef _AnsiString_hpp_
#define _AnsiString_hpp_

#include <string>

class AnsiString {
    std::string s;
    public:
        AnsiString(const char * c);
        const char* c_str();
};

#endif
