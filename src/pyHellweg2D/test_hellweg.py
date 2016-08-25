from pyhellweg import ffi, lib

ret = lib.lib_hellweg_run_beam_solver('./Hellweg2.ini'.encode(), './input.txt'.encode(), './output.txt'.encode())
