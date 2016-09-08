# -*- coding: utf-8 -*-
"""rslinac beam solver tests

Contains all tests to ensure that the beam solver is functioning properly.

Attributes:
    FIXTURE_DIR (:obj:`py.path.local`): contains the base directory where fixtures
    for these tests are stored.
    INI (:obj:`py.path.local`): INI configuration from the default example
    INPUT (:obj:`py.path.local`): input file from the default example


Todo:
    * Test correctness of the beam solver output
"""
import os
import contextlib
import py
import pytest
import rslinac.pkcli.beam_solver

_DIR = os.path.dirname(os.path.realpath(__file__))

FIXTURE_DIR = py.path.local(_DIR)/'test_files'

INI = FIXTURE_DIR/'rslinac.ini'
INPUT = FIXTURE_DIR/'input.txt'

def test_beam_solver_runs(tmpdir):
    """run beam solver
  
    Simple test, ensures rslinac can interface with C++ backend and
    execute a calculation without crashing.
 
    Args:
        tmpdir (:obj:`py.path.local`): py.test tmpdir_ fixture, provides a temporary directory where
        outputfiles can be written.

    .. _tmpdir: http://doc.pytest.org/en/latest/tmpdir.html
    """
    output = tmpdir/'output.txt'

    assert INI.exists()
    assert INPUT.exists()
    assert not output.exists()

    rslinac.pkcli.beam_solver.run(str(INI), str(INPUT), str(output))

    assert output.exists()

def test_io_exception(tmpdir):
    """io error exceptions

    Test that exceptions are triggered when the I/O parameters specified are erroneous

    Args:
        tmpdir (:obj:`py.path.local`): py.test tmpdir_ fixture, provides a temporary directory where
        outputfiles can be written.

    .. _tmpdir: http://doc.pytest.org/en/latest/tmpdir.html
    """
    output = tmpdir/'output.txt'

    with pytest.raises(IOError):
        rslinac.pkcli.beam_solver.run('', str(INPUT), str(output))
    assert not output.exists()

    with pytest.raises(IOError):
        rslinac.pkcli.beam_solver.run(str(INI), '', str(output))
    assert not output.exists()

    with contextlib.closing(output.open('w')):
        pass

    assert output.exists()
    with pytest.raises(RuntimeError):
        rslinac.pkcli.beam_solver.run(str(INI), str(INPUT), str(output))
