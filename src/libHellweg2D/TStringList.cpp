#include <fstream>

#include "TStringList.hpp"

unsigned int TStringList::Add(const char *c) {
    return Add(std::string(c));
} 

unsigned int TStringList::Add(std::string s) {
    string_list.push_back(s);
    // In C++ Builder Add returns the index of the position where the s is located, which is Count - 1
    return Count++;
} 

unsigned int TStringList::Add(AnsiString as) {
    return Add(as.s);
} 

void TStringList::Clear() {
    string_list.clear();
    Count = 0;
}

void TStringList::Delete(unsigned int index) {
    string_list.erase(string_list.begin()+index);
}

void TStringList::SaveToFile(const AnsiString &filename) {
    SaveToFile(filename.c_str());
}

void TStringList::SaveToFile(const char *filename) {
    if (filename) {
        std::ofstream file;
        file.open(filename);

        for (auto &s: string_list) {
            file << s << "\n";
        }

        file.close();
    }
}

void TStringList::AddStrings(TStringList *sl) {
    if (sl) {
        string_list.insert( string_list.end(), sl->string_list.begin(), sl->string_list.end() );
    }
}
