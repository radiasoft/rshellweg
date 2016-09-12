#ifndef _AnsiString_hpp_
#define _AnsiString_hpp_

#include <string>
#include <ostream>

class AnsiString {
    std::string s;

    public:
        friend class TStringList;
        friend class TIniFile;
        friend std::ostream& operator<<(std::ostream &strm, const AnsiString &a);
        
        AnsiString() : s("") {};
        AnsiString(const char * c);
        AnsiString(std::string s) : s(s) {};
        
        const char* c_str() const;
        double ToDouble();
        int ToInt();
        static AnsiString FormatFloat(const char *format, float f);
        
        bool operator==(const AnsiString &other) const;
        bool operator==(const char *other) const;
        bool operator!=(const AnsiString &other) const;
        bool operator!=(const char *other) const;

        AnsiString operator+(const char *other) const;
        AnsiString operator+(const AnsiString &other) const;
        friend AnsiString operator+(const char *c, const AnsiString &as);

        AnsiString& operator+=(const AnsiString &other); 
};

std::ostream& operator<<(std::ostream &strm, const AnsiString &a);
bool operator==(const char* c, const AnsiString &as);
bool operator!=(const char* c, const AnsiString &as);

#endif
