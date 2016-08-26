import sh
from pyhellweg import ffi, lib

cpp_filt = sh.Command('c++filt')
unmangle_cpp_class = cpp_filt.bake('-t')

err_info = ffi.new('LIB_HELLWEG_ERR_INFO *', (0, b'\x00'))

if not lib.lib_hellweg_run_beam_solver('./Hellweg2.ini'.encode(), './input.txt'.encode(), 
        './output.txt'.encode(), err_info):
    print(ffi.string(err_info.msg))
    if err_info.type == lib.CPP_EXCEPT:
        print(unmangle_cpp_class(ffi.string(err_info.exc)))
