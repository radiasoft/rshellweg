#ifndef _SysUtils_hpp_
#define _SysUtils_hpp_

#include <sys/stat.h>

bool FileExists(const char *filename) {
    if (filename) {
        struct stat buffer;
        return (stat(filename, &buffer) == 0);
    }

    return false;
}

#endif
