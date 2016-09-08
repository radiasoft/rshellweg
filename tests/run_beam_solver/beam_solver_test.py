# -*- coding: utf-8 -*-
"""rslinac beam solver tests

Contains all tests to ensure that the beam solver is functioning properly.

Attributes:
    FIXTURE_DIR (:obj:`py.path.local`): contains the base directory where fixtures
    for these tests are stored.

Todo:
    * Test correctness of the beam solver output
"""
import os
import py
import rslinac.pkcli.beam_solver

_DIR = os.path.dirname(os.path.realpath(__file__))

FIXTURE_DIR = py.path.local(_DIR)/'test_files'


def test_beam_solver_runs(tmpdir):
    """run beam solver
  
    Simple test, ensures rslinac can interface with C++ backend and
    execute a calculation without crashing.
 
    Args:
        tmpdir (:obj:`py.path.local`): py.test tmpdir fixture, provides a temporary directory where
        outputfiles can be written.
    """
    ini = FIXTURE_DIR/'rslinac.ini'
    _input = FIXTURE_DIR/'input.txt'
    output = tmpdir/'output.txt'

    assert ini.exists()
    assert _input.exists()
    assert not output.exists()

    rslinac.pkcli.beam_solver.run(str(ini), str(_input), str(output))

    assert output.exists()
