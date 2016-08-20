#ifndef _radia_utils_h_
#define _radia_utils_h_

#include <sys/stat.h>

bool FileExists(const char *filename) {
    if (filename) {
        struct stat buffer;
        return (stat(filename, &buffer) == 0);
    }

    return false;
}

#endif
