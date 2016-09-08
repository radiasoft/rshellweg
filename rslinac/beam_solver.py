import sys
import pyhellweg

def run_beam_solver(ini_filename, input_filename, output_filename):
    u"""runs the HellWeg2D Beam Solver"""

    if sys.version_info >= (3, 0):
        ini_filename = ini_filename.encode()
        input_filename = input_filename.encode()
        output_filename = output_filename.encode()

    pyhellweg.run_beam_solver(ini_filename, input_filename, output_filename)
