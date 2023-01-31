#ifndef _IniFiles_hpp_
#define _IniFiles_hpp_

#include <boost/property_tree/ptree.hpp>

#include "AnsiString.hpp"

namespace pt = boost::property_tree;

class TIniFile {
    pt::iptree tree;
    std::string getLocator(const AnsiString &section, const AnsiString &ident);
    public:
        TIniFile(AnsiString filepath);
        TIniFile() {};
        long ReadInteger(const AnsiString section, const AnsiString ident, long d);
        double ReadFloat(const AnsiString section, const AnsiString ident, double d);
        bool ReadBool(const AnsiString section, const AnsiString ident, bool d);
        AnsiString ReadString(const AnsiString section, const AnsiString ident, const AnsiString d);
        void fromString(std::string &s);
};

#endif
