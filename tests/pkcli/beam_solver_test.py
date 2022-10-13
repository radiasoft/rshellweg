# -*- coding: utf-8 -*-
"""rslinac.pkcli.beam_solver tests

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
#TODO(elventear) Test correctness of the beam solver output
from pykern import pkunit
import argh
import pytest

if __name__ == '__main__':
    from rslinac.solver import BeamSolver
    solver = BeamSolver('rslinac.ini', 'input.txt')
    solver.solve()

def test_beam_solver():
    """Ensure BeamSolver interface solves and produces output"""
    from pykern import pkio
    from pykern.pkunit import pkeq
    from rslinac.solver import BeamSolver
    f = _files()
    with pkunit.save_chdir_work():
        pkio.write_text('Solenoid.txt', pkio.read_text(pkunit.data_dir().join('Solenoid.txt')))
        solver = BeamSolver(f['ini'], f['input'])
        solver.solve()
        solver.save_output(f['output'])
        assert f['output'].exists()
        v = solver.get_structure_parameters(1)
        assert v[2] == 0.0006
        solver.dump_bin('all-data.bin')
        for outfile in ('PARSED.TXT', 'test1.pid'):
            expect = pkio.read_text(pkunit.data_dir().join(outfile))
            actual = pkio.read_text(pkunit.work_dir().join(outfile))
            pkeq(expect, actual)
        solver.load_bin('all-data.bin')
        v = solver.get_structure_parameters(1)
        assert v[2] == 0.0006

def test_run_beam_solver():
    """Ensure pyhellweg.run_beam_solver produces output and does not crash"""
    from pykern import pkio
    from pykern.pkunit import file_eq
    import os
    f = _files()
    with pkunit.save_chdir_work():
        pkio.write_text('Solenoid.txt', pkio.read_text(pkunit.data_dir().join('Solenoid.txt')))
        # for now beam_solver modifies some global state so it needs a new process
        os.system(f"rslinac beam_solver run {f['ini']} {f['input']} {f['output']}")
        assert f['output'].exists()
        for outfile in ('PARSED.TXT', 'test1.pid'):
            expect = pkunit.data_dir().join(outfile)
            actual = pkunit.work_dir().join(outfile)
            file_eq(expect_path=expect, actual_path=actual)

def test_run_deviance():
    """Incorrect arguments should raise exceptions"""
    from rslinac.pkcli import beam_solver
    from pykern import pkcli
    f = _files()
    with pytest.raises(pkcli.CommandError) as exc:
        beam_solver.run(f['not found'], f['input'], f['output'])
    assert not f['output'].exists(), \
        'If arguments are invalid, {} is not written'.format(f['ouput'])
    assert f['existing'].exists(), \
        'Could not find file {}, which is supposed to exist'.format(f['existing'])
    with pytest.raises(pkcli.CommandError) as exc:
        beam_solver.run(f['ini'], f['input'], f['existing'])


def _files():
    """Return dict of file args in data and work directories"""
    d = pkunit.data_dir()
    w = pkunit.empty_work_dir()

    existing_filename = w.join('exists.txt')
    with existing_filename.open('w'):
        pass

    return {
        'existing': existing_filename,
        'ini': d.join('rslinac.ini'),
        'input': d.join('input.txt'),
        'output': w.join('output.txt'),
        'not found': w.join('guaranteed not to exist'),
    }
