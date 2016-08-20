#ifndef _TStringList_hpp_
#define _TStringList_hpp_

#include <iostream>
#include <vector>

#include "AnsiString.hpp"

class TStringList {
    std::vector<std::string> string_list;

    public:
        unsigned int Count;
        unsigned int Add(const char *c);
        unsigned int Add(std::string s);
        unsigned int Add(AnsiString as);
        void Clear();
        void Delete(unsigned int index);
        void SaveToFile(const char* filename);
        void SaveToFile(const AnsiString &filename);
        void AddStrings(TStringList *sl);
        TStringList() : Count(0) {};
};

#endif
