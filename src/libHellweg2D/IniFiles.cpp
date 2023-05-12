#include <boost/property_tree/ini_parser.hpp>

#include "IniFiles.hpp"

TIniFile::TIniFile(AnsiString filepath) {
    pt::ini_parser::read_ini(filepath.c_str(), tree);
}

long TIniFile::ReadInteger(const AnsiString section, const AnsiString ident, long d) {
    return tree.get(getLocator(section, ident), d);
}

double TIniFile::ReadFloat(const AnsiString section, const AnsiString ident, double d) {
    return tree.get(getLocator(section, ident), d);
}

AnsiString TIniFile::ReadString(const AnsiString section, const AnsiString ident, AnsiString d) {
    return AnsiString(tree.get(getLocator(section, ident), d.s));
}

std::string TIniFile::getLocator(const AnsiString &section, const AnsiString &ident) {
    return section.s + "." + ident.s;
}


void TIniFile::fromString(std::string &s) {
    std::istringstream is(s);
    pt::ini_parser::read_ini(is, tree);
}
