# -*- coding: utf-8 -*-
u"""Run the beam solver library

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
#rjn we always put this in the top. It's part of the template:
#rjn https://github.com/biviosoftware/home-env/blob/master/emacs.d/b-python.el#L102
from __future__ import absolute_import, division, print_function
#rjn No vertical space in the import list. No import is really more important
#rjn than any other unless there's "a reason"
#rjn This is unused. Does it need to be imported?
from rslinac.pyhellweg import PyHellwegCppException
import os
import six
import sys
import rslinac.pyhellweg


#rjn "input" is a python word. In emacs it highlights as blue so I
#rjn recommend input_file or input_name
def run(ini_file, input_file, output_file):
    #rjn https://google.github.io/styleguide/pyguide.html#TODO_Comments
    #rjn Also I dont' think it is useful for the comment to be in the
    #rjn docstring. It's a TODO and therefore doesn't help the user
    #rjn to see it with the help
    # TODO(elventear) Provide links that document the input formats
    """Call `pyhellweg.run_beam_solver` with inputs and write result.

    #rjn line was too long.
    Read `ini_file` and `input_file`, execute the beam solver, and write
    result to `output_file`. Arguments may be `py.path.local` objects
    or strings.

    Args:
        #rjn The nice thing about google style is that you don't
        #rjn need much RST. Don't end the descriptions with periods
        #rjn either since they are not sentences.
        ini_file (object): path to configuration settings in INI format
        input_file (object): path to input data
        output_file (object): path to write the result

    #rjn not a huge fan of "raises" statements, because you don't
    #rjn really know all the errors that my be raised in this case.
    #rjn It is open() that raises IOError. Rather I would
    #rjn just document that files must and must not exist
    Raises:
        IOError: if `ini` or `input` cannot be opened
        ValueError: if `output` file already exists
        PyHellwegCppException: if an error occurrs within the Beam Solver
    """
    #rjn this comment is unnecessary if the error message is clear
    # Test if input files are accessible
    #rjn I find it incongruent using os.path.exists and open. I see the
    #rjn purpose and I don't have a better alternative
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
        #rjn Use a more specific exception than RuntimeError. ValueError
        #rjn indicates the problem is with a value.
        #rjn Put the object of the error first, followed by the error
        #rjn This is modeled after Unix: cat: foo: No such file or directory
        raise ValueError('{}: output path exists'.format(args[2]))
    #rjn Avoid version tests. It's more robust to do feature tests.
    #rjn In this case you want to know if you have to decode unicode. For
    #rjn py2 that's unicode and for py3 it's str. "six" provides this
    #rjn abstraction
    rslinac.pyhellweg.run_beam_solver(*args)
