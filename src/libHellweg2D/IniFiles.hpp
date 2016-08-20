#ifndef _IniFiles_hpp_
#define _IniFiles_hpp_

#include "AnsiString.hpp"

class TIniFile {
    public:
        // TODO: Implementation
        TIniFile(AnsiString filepath) {};
        long ReadInteger(const AnsiString section, const AnsiString ident, long d);
        double ReadFloat(const AnsiString section, const AnsiString ident, double d);
};

#endif
