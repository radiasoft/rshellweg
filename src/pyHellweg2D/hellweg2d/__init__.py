import sys
import pyhellweg 

def main():

    ini_filename = sys.argv[1]
    input_filename = sys.argv[2]
    output_filename = sys.argv[3]
    
    pyhellweg.run_beam_solver(ini_filename, input_filename, output_filename)
