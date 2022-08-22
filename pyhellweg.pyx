# cython: language_level=3
import subprocess as sp

def unmangle_cpp_class(mangled_str):
    return sp.check_output(['c++filt', '-t', mangled_str]).strip()

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

    cdef cppclass HellwegBeamSolver:
        HellwegBeamSolver(const char*, const char*) except +
        void solve();
        void dump_bin(const char*);
        int get_number_of_points();
        int get_number_of_particles();
        double* get_structure_parameters(int);
        void load_bin(const char*);
        void save_output(const char*);

class PyHellWegError(Exception): pass

class PyHellwegInputError(PyHellWegError): pass

class PyHellwegCppException(PyHellWegError):
    def __init__(self, exc, msg):
        self.exc_class = unmangle_cpp_class(exc)
        self.exc_msg = msg
        PyHellWegError.__init__(self, 'Exception {0.exc_class}: {0.exc_msg}'.format(self))


def run_beam_solver(ini_path, input_path, output_path):
    cdef LIB_HELLWEG_ERR_INFO err_info

    if not lib_hellweg_run_beam_solver(ini_path, input_path, output_path, &err_info):
        if err_info.type == CPP_EXCEPT:
            raise PyHellwegCppException(err_info.exc, err_info.msg)
        elif err_info.type == CPP_EXCEPT_UNKNOWN:
            raise PyHellWegError('Unknown exception: {0}'.format(err_info.msg))
        else:
            raise PyHellwegInputError(err_info.msg)

cdef class PyHellwegBeamSolver:
    cdef HellwegBeamSolver *_solver

    def __init__(self, ini_path, input_path):
        self._solver = new HellwegBeamSolver(ini_path, input_path)

    def solve(self):
        self._solver.solve()

    def dump_bin(self, output_path):
        self._solver.dump_bin(output_path)

    def get_structure_parameters(self, param):
        res = []
        cdef double *values = self._solver.get_structure_parameters(param)
        for i in xrange(self._solver.get_number_of_points()):
            res.append(values[i])
        #TODO(pjm): release values memory
        return res

    def load_bin(self, input_path):
        self._solver.load_bin(input_path)

    def save_output(self, output_path):
        self._solver.save_output(output_path)
