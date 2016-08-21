#include <cstring>

#include "AnsiString.hpp" 
       
AnsiString& AnsiString::operator+=(const AnsiString &other) {
    s += other.s;
    return *this;
}

bool AnsiString::operator==(const char *other) const {
    return s == std::string(other);
}

bool AnsiString::operator!=(const char *other) const {
    return s != std::string(other);
}

AnsiString AnsiString::operator+(const AnsiString &other) const {
    return AnsiString(this->s + other.s);
}
        
AnsiString AnsiString::operator+(const char *other) const {
    return AnsiString();
}

AnsiString operator+(const char *c, const AnsiString &as) {
    return AnsiString();
}

AnsiString::AnsiString(const char *c) {
    s = std::string(c);
}

const char* AnsiString::c_str() const {
    return s.c_str();
}

double AnsiString::ToDouble() {
    return std::stod(s);
}

int AnsiString::ToInt() {
    return std::stoi(s);
}

const char* FormatMap[][2] = {
    {"#0", "%0.0<D-k>f"},
    {"#0.0", "%0.1f"},
    {"#0.00", "%0.2f"},
    {"#0.000", "%0.3f"},
    {"#0.0000", "%0.4f"},
    {"#0.00000", "%0.5f"},
    {"#0.000000", "%0.6f"},
    {"#0.0000000", "%0.7f"},
    {NULL, NULL}
};

AnsiString AnsiString::FormatFloat(const char *format, float f) {
    char buf[32]= { 0 };
    unsigned int i = 0;
    const char *_format = NULL;

    do {
        if (std::strcmp(format, FormatMap[i][0]) == 0) {
            _format = FormatMap[i][1];
            break;
        }     
    } while (FormatMap[i++][0]);

    if (!_format) {
        _format = "%f";
    }

    std::snprintf(buf, 32, _format);
     
    return AnsiString(buf);
}
