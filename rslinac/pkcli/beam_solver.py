# -*- coding: utf-8 -*-
"""
"""
import pyhellweg
import sys


def run(ini, input, output):
    """runs the HellWeg2D Beam Solver"""

    if sys.version_info >= (3, 0):
        ini = ini.encode()
        input = input.encode()
        output = output.encode()

    pyhellweg.run_beam_solver(ini, input, output)
