# -*- coding: utf-8 -*-
u"""Python to C++ Proxy interface for Solvers

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
import rslinac.pyhellweg


class BeamSolver(object):
    def __init__(self, ini_path, input_path):
        self._solver = rslinac.pyhellweg.PyHellwegBeamSolver(
            str(ini_path),
            str(input_path),
        )

    def solve(self):
        self._solver.solve()

    def dump_bin(self, output_path):
        self._solver.dump_bin(str(output_path))

    def get_structure_parameters(self, param):
        return self._solver.get_structure_parameters(param)

    def load_bin(self, input_path):
        self._solver.load_bin(str(input_path))

    def save_output(self, output_path):
        self._solver.save_output(str(output_path))
