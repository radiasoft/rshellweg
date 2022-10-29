# -*- coding: utf-8 -*-
"""sample execution of the Hellweg library

:copyright: Copyright (c) 2022 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
import os
from rshellweg.solver import BeamSolver

working_dir = '/home/vagrant/jupyter/repos/rslinac/tests/pkcli/'
null_file = os.path.join(working_dir+'beam_solver_data/', 'rshellweg.ini')
input_file = os.path.join(working_dir+'beam_solver_data/', 'input.txt')

print(' ')
print(' Invoking the solver...')
solver = BeamSolver(null_file, input_file)
solver.solve()
