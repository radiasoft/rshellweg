from rslinac.solver import BeamSolver

INI = 'rslinac.ini'
INPUT = 'input.txt'
OUTPUT_BIN = 'output.bin'
OUTPUT_TXT = 'output.txt'

if __name__ == '__main__':
    solver = BeamSolver(INI, INPUT)
    solver.solve()
    solver.dump_bin(OUTPUT_BIN)
    solver.save_output(OUTPUT_TXT)
