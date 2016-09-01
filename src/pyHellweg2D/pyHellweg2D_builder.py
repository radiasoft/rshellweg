import os
from cffi import FFI

FFI_SOURCE = \
"""
#include <libHellweg2D.h>
"""

FFI_CDEF = \
"""
typedef enum {
    CPP_EXCEPT,
    CPP_EXCEPT_UNKNOWN,
    INPUT_ERROR
} lib_hellweg_err_type;

typedef struct {
    lib_hellweg_err_type type;
    char msg[1024];
    char exc[1024];
} LIB_HELLWEG_ERR_INFO;

bool lib_hellweg_run_beam_solver(const char*, const char*, const char*, LIB_HELLWEG_ERR_INFO*);
"""

def get_ffibuilder(include_dirs=[], library_dirs=[]):
    ffibuilder = FFI()
    
    ffibuilder.set_source("pyhellweg", FFI_SOURCE, libraries=['Hellweg2D'], 
            include_dirs=include_dirs, library_dirs=library_dirs)   
    
    ffibuilder.cdef(FFI_CDEF)

    return ffibuilder

if __name__ == "__main__":
    ffibuilder = get_ffibuilder(['../libHellweg2D'], ['../libHellweg2D/build'])
    ffibuilder.compile(verbose=True)
