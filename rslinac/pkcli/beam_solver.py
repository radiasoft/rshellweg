# -*- coding: utf-8 -*-
"""rslinac beam solver
"""
from pyhellweg import PyHellwegCppException

import os
import pyhellweg
import sys


def run(ini, input, output):
    """run beam solver

    Read the input files, execute the beam solver and write the output to the provided destination.

    Args:
        ini (:obj:`str): path to configuration settings in INI format.
        input (:obj:`str): path to input data.
        output (:obj:`str): path to write output data.

    Raises:
        IOError: if `ini` or `input` cannot be open.
        RuntimeError: if `output` file already exists.
        PyHellwegCppException: if an error occurrs within the Beam Solver.

    Todo:
        * Provide links that document the input formats.
    """

    # Test if input files are accessible
    with open(ini, 'r'), open(input, 'r'):
        pass

    if os.path.exists(output):
        raise RuntimeError('Output path exists: {}'.format(output))

    if sys.version_info >= (3, 0):
        ini = ini.encode()
        input = input.encode()
        output = output.encode()

    pyhellweg.run_beam_solver(ini, input, output)
