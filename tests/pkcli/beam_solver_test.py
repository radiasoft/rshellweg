# -*- coding: utf-8 -*-
"""rslinac.pkcli.beam_solver tests

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
#TODO(elventear) Test correctness of the beam solver output
from pykern import pkunit
import argh
import pytest


def test_run():
    """Ensure pyhellweg.run_beam_solver produces output and does not crash"""
    from pykern import pkio
    from pykern.pkunit import pkeq
    from rslinac.pkcli import beam_solver
    f = _files()
    with pkunit.save_chdir_work():
        beam_solver.run(f['ini'], f['input'], f['output'])
        for outfile in ('PARSED.TXT', 'output.txt', 'test1.pid'):
            expect = pkio.read_text(pkunit.data_dir().join(outfile))
            actual = pkio.read_text(pkunit.work_dir().join(outfile))
            pkeq(expect, actual)


def test_run_deviance():
    """Incorrect arguments should raise exceptions"""
    from rslinac.pkcli import beam_solver
    f = _files()
    with pytest.raises(argh.CommandError) as exc:
        beam_solver.run(f['not found'], f['input'], f['output'])
    assert not f['output'].exists(), \
        'If arguments are invalid, {} is not written'.format(f['ouput'])
    assert f['existing'].exists(), \
        'Could not find file {}, which is supposed to exist'.format(f['existing'])
    with pytest.raises(argh.CommandError) as exc:
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
