#include <cassert>
#include <cstring>

#include "AnsiString.hpp"

int AnsiString::Length() const {
    return s.length();
}

const char& AnsiString::operator[](size_t i) const {
    assert(i > 0);
    // http://docs.embarcadero.com/products/rad_studio/radstudio2007/RS2007_helpupdates/HUpdate4/EN/html/delphivclwin32/System__AnsiString__[]@int.html
    // This System::AnsiString::[] returns the byte in the string at index value idx. The System::AnsiString::[] assumes a base index of 1.
    return this->s[i-1];
}

std::ostream& operator<<(std::ostream &strm, const AnsiString &a) {
  return strm << "AnsiString(" << a.s << ")";
}

AnsiString& AnsiString::operator+=(const char &other) {
    s += other;
    return *this;
}

AnsiString& AnsiString::operator+=(const AnsiString &other) {
    s += other.s;
    return *this;
}

bool operator==(const char* c, const AnsiString &as) {
    return AnsiString(c) == as;
}

bool AnsiString::operator==(const AnsiString &other) const {
    return s == other.s;
}

bool AnsiString::operator==(const char *other) const {
    return s == std::string(other);
}

bool operator!=(const char* c, const AnsiString &as) {
    return AnsiString(c) != as;
}

bool AnsiString::operator!=(const AnsiString &other) const {
    return s != other.s;
}

bool AnsiString::operator!=(const char *other) const {
    return s != std::string(other);
}

AnsiString AnsiString::operator+(const AnsiString &other) const {
    return AnsiString(this->s + other.s);
}

AnsiString AnsiString::operator+(const char *other) const {
    return *this + AnsiString(other);
}

AnsiString operator+(const char *c, const AnsiString &as) {
    return AnsiString(c) + as;
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
    {"#0", "%0.0f"},
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
    } while (FormatMap[++i][0]);

    if (!_format) {
        _format = "%f";
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    std::snprintf(buf, 32, _format, f);
#pragma GCC diagnostic pop

    return AnsiString(buf);
}
