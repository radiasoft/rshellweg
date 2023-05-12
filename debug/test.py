from rshellweg.solver import BeamSolver

INI = "rshellweg.ini"
INPUT = "input.txt"
OUTPUT_BIN = "output.bin"
OUTPUT_TXT = "output.txt"


solver = BeamSolver(INI, INPUT)
solver.solve()
solver.dump_bin(OUTPUT_BIN)
solver.save_output(OUTPUT_TXT)
