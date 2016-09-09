# -*- coding: utf-8 -*-
"""rslinac.pkcli.beam_solver tests

#rjn this is unnecessary:
#rjn Contains all tests to ensure that the beam solver is functioning properly.
:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
#TODO(elventear) Test correctness of the beam solver output
from pykern import pkunit
import pytest

#rjn Tests should match the structure of the module paths not
#rjn introduce new paths. This future proofs name collisions so
#rjn renamed to match the path pkcli.beam_solver.

#rjn Abuse of operator overloading. One of the great fears of operator
#rjn overloading in the SIGPLAN Wars of the 1980s was that people
#rjn would do things like `FIXTURE_DIR/'rslinac.ini'`. This is not
#rjn only illogical (divide does not augment it reduces), but more
#rjn importantly, it is very difficult to find uses of library APIs
#rjn with operators. If we decide to switch away from `py.path` (real
#rjn possibility), it is very difficult to find "uses" of py.path
#rjn operators. If one uses the word `join`, one can grep for it. You
#rjn can't grep for "/", and it is unlikely the library we move to
#rjn would support that natively.
#rjn INI = FIXTURE_DIR/'rslinac.ini'

#rjn contextually, we know we are testing beam_solver so the only
#rjn qualifying name is "run".
def test_run():
    """Ensure pyhellweg.run_beam_solver produces output and does not crash"""
    #rjn Don't import until you actually need the object, because this
    #rjn can throw an exception. In this case a simple qualified import
    #rjn is ok
    from rslinac.pkcli import beam_solver
    f = _files()
    #TODO(robnagler) Do we want unicode tests???
    #rjn these tests are unecessary. It's not likely to break:
    #rjn http://c2.com/cgi/wiki?TestEverythingThatCouldPossiblyBreak
    #rjn assert INI.exists()
    #rjn assert INPUT.exists()
    #rjn assert not output.exists()
    #rslinac.pkcli.beam_solver.run(f['ini'], f['input'], f['output'])
    assert f['output'].exists(), \
        'After execution, {} should exist'.format(f['output'])


#rjn Sometimes we call it "dev" instead of "deviance", but it's what
#rjn this test does.
def test_run_deviance():
    #rjn With behavioral "phrasing" I think we might want docstrings
    #rjn like this one.
    """Incorrect arguments should raise exceptions"""
    from rslinac.pkcli import beam_solver
    #rjn '' is a bad test because it might also indicate a valid file (''
    #rjn converts to '.' in py.path.local) so better to have a name we
    #rjn know does not exist, which also self-documents it's non-existence
    f = _files()
    with pytest.raises(IOError):
        beam_solver.run(f['not found'], f['input'], f['output'])
    assert not f['output'].exists(), \
        'If arguments are invalid, {} is not written'.format(f['ouput'])
    #rjn Redundant test. It's just not likely to break
    #rjn with pytest.raises(IOError):
    #rjn    rslinac.pkcli.beam_solver.run(str(INI), '', str(output))
    #rjn I don't think you need contextlib.closing here, because
    #rjn because it already closes when
    #rjn with contextlib.closing(output.open('w')):
    #rjn pass
    #rjn this tests that open creates a file, which is not testing
    #rjn anything about pyhellweg.
    #rjn assert output.exists()
    with pytest.raises(ValueError) as exc:
        beam_solver.run(f['ini'], f['input'], f['output'])
    assert 'output path exists' in exc, \
        'If output ({}) exists, then thrown an exception'


def _files():
    """Return dict of file args in data and work directories"""
    #rjn These functions do quite a bit for you.
    #rjn The work (tmp) directory is emptied at the start of the program, not
    #rjn cleaned up afterwards. This makes debugging easier since you can see
    #rjn the output. In addition there is save_chdir_work, which allows you to
    #rjn create the work dir, chdir to it, in a context function. The result
    #rjn of work_dir and data_dir is a py.path.local which is practical to use
    #rjn especially for testing.
    d = pkunit.data_dir()
    w = pkunit.empty_work_dir()
    return {
        'ini': d.join('rslinac.ini'),
        'input': d.join('input.txt'),
        'output': w.join('output.txt'),
        'not found': w.join('guaranteed not to exist'),
    }
