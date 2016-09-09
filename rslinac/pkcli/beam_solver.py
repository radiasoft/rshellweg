# -*- coding: utf-8 -*-
u"""Run the beam solver library

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
from __future__ import absolute_import, division, print_function
from rslinac.pyhellweg import PyHellwegCppException
import os
import six
import sys
import rslinac.pyhellweg


def run(ini_file, input_file, output_file):
    # TODO(elventear) Provide links that document the input formats
    """Call `pyhellweg.run_beam_solver` with inputs and write result.

    Read `ini_file` and `input_file`, execute the beam solver, and write
    result to `output_file`. Arguments may be `py.path.local` objects
    or strings.

    Args:
        ini_file (object): path to configuration settings in INI format
        input_file (object): path to input data
        output_file (object): path to write the result

    Raises:
        IOError: if `ini` or `input` cannot be opened
        ValueError: if `output` file already exists
        PyHellwegCppException: if an error occurrs within the Beam Solver
    """
    # Test if input files are accessible
    args = []
    for a in ini_file, input_file, output_file:
        # Normalize if py.path.local
        a = str(a)
        if isinstance(a, six.text_type):
            a = a.encode()
        args.append(a)
    with open(args[0], 'r'), open(args[1], 'r'):
        pass
    if os.path.exists(args[2]):
        raise ValueError('{}: output path exists'.format(args[2]))
    rslinac.pyhellweg.run_beam_solver(*args)
