import sh

cpp_filt = sh.Command('c++filt')
unmangle_cpp_class = cpp_filt.bake('-t')

cdef extern from "libHellweg2D.h":
    ctypedef enum lib_hellweg_err_type:
        CPP_EXCEPT
        CPP_EXCEPT_UNKNOWN
        INPUT_ERROR
        
    ctypedef struct LIB_HELLWEG_ERR_INFO:
        lib_hellweg_err_type type
        char msg[1024]
        char exc[1024]
    
    bint lib_hellweg_run_beam_solver(const char*, const char*, const char*, LIB_HELLWEG_ERR_INFO*)

class PyHellWegError(Exception): pass

class PyHellwegInputError(PyHellWegError): pass

class PyHellwegCppException(PyHellWegError): 
    def __init__(self, exc, msg):
        self.exc_class = unmangle_cpp_class(exc)
        self.exc_msg = msg
        PyHellWegError.__init__(self, 'Exception {0.exc_class}: {0.exc_msg}'.format(self))


def run_beam_solver(ini_path, input_path, output_path):
    cdef LIB_HELLWEG_ERR_INFO err_info = { 0 }

    if not lib_hellweg_run_beam_solver(ini_path, input_path, output_path, &err_info):
        if err_info.type == CPP_EXCEPT:
            raise PyHellwegCppException(err_info.exc, err_info.msg)
        elif err_info.type == CPP_EXCEPT_UNKNOWN:
            raise PyHellWegError('Unknown exception: {0}'.format(err_info.msg))
        else:
            raise PyHellwegInputError(err_info.msg)
