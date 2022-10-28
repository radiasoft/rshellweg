# -*- coding: utf-8 -*-
"""Run the beam solver library

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
from __future__ import absolute_import, division, print_function
from pykern import pkcli
from rshellweg.pyhellweg import PyHellwegCppException
import os
import py.path
import six
import sys
import rshellweg.pyhellweg


def _normalize_path_arg(path, **kw):
    """Normalizes the path, to bytes or str depending on the Python version

    Args:
        path (object): path to normalize
        **kwargs: Supports `py.path.local.check_`

    .. _py.path.local.check_: http://py.readthedocs.io/en/latest/path.html#py._path.svnwc.SvnWCCommandPath.check
    """

    idx = lambda x: tuple(sorted(x.items()))

    a = py.path.local(path)
    if not a.check(**kw):
        msg = {
            idx({"file": True, "exists": True}): 'Error, file "{}" does not exist',
            idx({"exists": False}): 'Error, file "{}" exists',
        }.get(idx(kw), 'Error with file "{}"')

        pkcli.command_error(msg, path)

    a = str(path)
    if isinstance(a, six.text_type):
        a = a.encode()
    return a


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
        PyHellwegCppException: if an error occurrs within the Beam Solver
    """
    rshellweg.pyhellweg.run_beam_solver(
        _normalize_path_arg(ini_file, file=True, exists=True),
        _normalize_path_arg(input_file, file=True, exists=True),
        _normalize_path_arg(output_file, exists=False),
    )
