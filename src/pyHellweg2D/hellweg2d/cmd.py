import sys
import sh
from pyhellweg import ffi, lib

cpp_filt = sh.Command('c++filt')
unmangle_cpp_class = cpp_filt.bake('-t')

def main():
    err_info = ffi.new('LIB_HELLWEG_ERR_INFO *', (0, b'\x00'))

    ini_filename = sys.argv[1]
    input_filename = sys.argv[2]
    output_filename = sys.argv[3]
    
    if not lib.lib_hellweg_run_beam_solver(
            ini_filename.encode(), 
            input_filename.encode(), 
            output_filename.encode(), 
            err_info):
        print(ffi.string(err_info.msg))
        if err_info.type == lib.CPP_EXCEPT:
            print(unmangle_cpp_class(ffi.string(err_info.exc)))

