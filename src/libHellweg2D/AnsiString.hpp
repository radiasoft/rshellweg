#ifndef _AnsiString_hpp_
#define _AnsiString_hpp_

#include <string>

class AnsiString {
    std::string s;
    public:
        AnsiString(const char * c);
        const char* c_str();
        AnsiString() : s("") {};
        AnsiString(std::string s) : s(s) {};
        double ToDouble();
        int ToInt();
        static AnsiString FormatDouble(const char *format, double d);
        static AnsiString FormatFloat(const char *format, float f);
        
        bool operator==(const char *other) const;
        bool operator!=(const char *other) const;

        AnsiString operator+(const char *other) const;
        AnsiString operator+(const AnsiString &other) const;
        friend AnsiString operator+(const char *c, const AnsiString &as);

        AnsiString& operator+=(const AnsiString &other); 
};
        

#endif
