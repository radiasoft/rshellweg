# -*- coding: utf-8 -*-
"""sample execution of the Hellweg library

:copyright: Copyright (c) 2022 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
import os
from rshellweg.solver import BeamSolver

cwd = os.getcwd()
null_file = os.path.join(cwd, 'rshellweg.ini')
input_file = os.path.join(cwd, 'test_01.txt')

print(' ')
print(' Invoking the solver...')
solver = BeamSolver(null_file, input_file)
solver.solve()
