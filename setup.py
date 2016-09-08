# -*- coding: utf-8 -*-
"""rslinac setup script

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
import os
import sysconfig

from glob import glob
from setuptools import Extension

try:
    from pykern import pksetup
except ImportError:
    import pip
    pip.main(['install', 'pykern'])
    from pykern import pksetup

LIB_DIR = os.path.join('src', 'libHellweg2D')

PHYS_DIR = os.path.join('src', 'physics')


def get_src_files(directory, extension):
    """get list of files in `directory` with `extension`"""
    return glob(os.path.join(directory, '*.{}'.format(extension)))


def get_compile_args():
    """returns compiler arguments based on the environment"""
    try:
        # Has the compiler been overrident with environment variables?
        compiler = os.environ['CC']
    except KeyError:
        # Use Python default compiler
        compiler = sysconfig.get_config_var('CC')

    # libHellweg2D uses c++11 features
    if 'gcc' in compiler:
        return ['-std=c++11']


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
        Extension(name='pyhellweg', define_macros=[('RSLINAC', 1)],
                  include_dirs=[LIB_DIR, PHYS_DIR, '/opt/local/include'],
                  sources=get_src_files(PHYS_DIR, 'cpp')
                  + get_src_files(LIB_DIR, 'cpp')
                  + ['pyhellweg.c'],
                  extra_compile_args=get_compile_args(),
                 )
    ]
)
