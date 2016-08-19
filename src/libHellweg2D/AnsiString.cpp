#include "AnsiString.hpp" 

AnsiString::AnsiString(const char *c) {
    s = std::string(c);
}

const char* AnsiString::c_str() {
    return s.c_str();
}
