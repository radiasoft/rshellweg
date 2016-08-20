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

AnsiString AnsiString::FormatDouble(const char *format, double d) {
    // TODO: proper formatting implementation
    return AnsiString();
}

AnsiString AnsiString::FormatFloat(const char *format, float f) {
    // TODO: proper formatting implementation
    return AnsiString();
}
