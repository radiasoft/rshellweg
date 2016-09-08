import os
import py
import rslinac

_DIR = os.path.dirname(os.path.realpath(__file__))
FIXTURE_DIR = py.path.local(_DIR)/'test_files'

def test_beam_solver_runs(tmpdir):
    ini    = FIXTURE_DIR/'rslinac.ini'
    _input = FIXTURE_DIR/'input.txt'
    output = tmpdir/'output.txt'

    assert ini.exists()
    assert _input.exists()
    assert not output.exists()

    rslinac.run_beam_solver(str(ini), str(_input), str(output))
    
    assert output.exists()

