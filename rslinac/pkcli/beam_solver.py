import rslinac
from argh import arg

@arg('ini', help='path configuration file in INI format')
@arg('input', help='path to file with input data')
@arg('output', help='path to file to write output data')
def run(ini, input, output):
    """runs the beam solver"""
    rslinac.run_beam_solver(ini, input, output)
