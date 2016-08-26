

from cffi import FFI
ffibuilder = FFI()

ffibuilder.set_source("pyhellweg",
    """
    #include <libHellweg2D.h>
    """,
    libraries=['Hellweg2D'], include_dirs=['../libHellweg2D'])   

ffibuilder.cdef("""
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
""")

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
