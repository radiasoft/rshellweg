# -*- coding: utf-8 -*-
"""rslinac setup script

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
#rjn The first import has to be pykern, because of the auto-install.
#rjn setuptools does a lot of caching. (I will update setup.py.jinja
#rjn to include this.
try:
    from pykern import pksetup
except ImportError:
    import pip
    pip.main(['install', 'pykern'])
    from pykern import pksetup

#rjn sort imports alphabetically. Emacs has a function "sort-lines" (C-x C-l)
#rjn which handles this so the froms will come before the imports line. This
#rjn keeps imports in a logical and manageable order.
from glob import glob
from setuptools import Extension
import os
import sysconfig

#rjn PEP8 says module level declarations are separated by two lines
#rjn but we only use one line for variables. Global variables not
#rjn intended for export should begin with underscore.
#rjn share 'src'
_LIB_DIR = os.path.join('src', 'libHellweg2D')

_PHYS_DIR = os.path.join('src', 'physics')


def _get_src_files(directory, extension):
    #rjn This comment is sufficient for an internal function.
    #rjn I do think capitalization is nice.
    """Get list of files in `directory` with `extension`"""
    return glob(os.path.join(directory, '*.{}'.format(extension)))


def _get_compile_args():
    #rjn Specifying "$CC" is more useful to the reader. I realize this
    #rjn is pedantic, but the style is used elsewhere.
    """Returns compiler arguments based on $CC or Python's default CC"""
    #rjn EAFP is noisy in this case. get() is clearer imiho but arguable
    compiler = os.environ.get('CC', sysconfig.get_config_var('CC'))
    #rjn much argument here about spacing, but my argument is that if
    #rjn vertical whitespace is required then it probably needs to be
    #rjn a separate function. In this case, the blank line is overkill
    #rjn due to the simplification above
    # libHellweg2D uses c++11 features
    if 'gcc' in compiler:
        return ['-std=c++11']
    #rjn in this case I think it is important to have a return,
    #rjn because I looked at the next line and wondered why there wasn't
    #rjn a return.
    return None


pksetup.setup(
    name='rslinac',
    author='RadiaSoft LLC',
    author_email='pip@radiasoft.net',
    description='description',
    license='http://www.apache.org/licenses/LICENSE-2.0.html',
    url='https://github.com/radiasoft/rslinac',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Natural Language :: English',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python',
        'Topic :: Utilities',
    ],
    ext_modules=[
        Extension(
            #rjn all lines should line up on a 4 character boundary so
            #rjn they are not dependent on identifier lengths. Global
            #rjn replacements are much easier.
            name='pyhellweg',
            define_macros=[('RSLINAC', 1)],
            #rjn /opt/local/include should not be hardwired. This must be
            #rjn available from sysconfig?
            include_dirs=[_LIB_DIR, _PHYS_DIR, '/opt/local/include'],
            #rjn We put operators at the end of lines to show that the
            #rjn lines are continued. Alternative argument is that a line
            #rjn beginning with an operator is a continuation. This doesn't
            #rjn work with '+' or '-' which are unary operators. The
            #rjn continued line is indented by 4
            sources=_get_src_files(_PHYS_DIR, 'cpp') +
                _get_src_files(_LIB_DIR, 'cpp') +
                ['pyhellweg.c'],
            extra_compile_args=_get_compile_args(),
        #rjn All lines in lists end in ',' so that you can rearrange the sections
        #rjn without getting syntax errors.
        ),
    ],
)
