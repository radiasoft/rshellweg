#include <boost/property_tree/ini_parser.hpp>

#include "IniFiles.hpp"

TIniFile::TIniFile(AnsiString filepath) {
    pt::ini_parser::read_ini(filepath.c_str(), tree);
}

long TIniFile::ReadInteger(const AnsiString section, const AnsiString ident, long d) {
    const char* locator = (section + "." + ident).c_str();
    return tree.get<long>(locator);
}

double TIniFile::ReadFloat(const AnsiString section, const AnsiString ident, double d) {
    const char* locator = (section + "." + ident).c_str();
    return tree.get<double>(locator);
}

