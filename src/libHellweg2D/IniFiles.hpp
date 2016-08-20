#ifndef _IniFiles_hpp_
#define _IniFiles_hpp_

#include <boost/property_tree/ptree.hpp>

#include "AnsiString.hpp"

namespace pt = boost::property_tree;

class TIniFile {
    pt::ptree tree;
    public:
        TIniFile(AnsiString filepath);
        long ReadInteger(const AnsiString section, const AnsiString ident, long d);
        double ReadFloat(const AnsiString section, const AnsiString ident, double d);
};

#endif
