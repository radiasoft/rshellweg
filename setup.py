# -*- coding: utf-8 -*-
"""Build and install hellweg2d command line"""
from __future__ import division, absolute_import, print_function

import os
import sysconfig
from glob import glob
from setuptools import setup, Extension

LIB_DIR = os.path.join('src', 'libHellweg2D')
PHYS_DIR = os.path.join('src', 'physics')

def get_src_files(dir, ext):
    return glob(os.path.join(dir, '*.{}'.format(ext)))

def get_compile_args():
    try:
        CC = os.environ['CC']
    except KeyError:
        CC = sysconfig.get_config_var('CC')

    if 'gcc' in CC:
        return ['-std=c++11']

setup(
    name='hellweg2d',
    version='0.0.1',
    packages=['hellweg2d'],
    install_requires=['sh'],
    entry_points={
        'console_scripts': ['hellweg2d.py=hellweg2d:main']
    },
    ext_modules=[
        Extension( name='pyhellweg', define_macros=[('RADIA', 1)],
            include_dirs=[LIB_DIR, PHYS_DIR, '/opt/local/include'],
            sources=get_src_files(PHYS_DIR, 'cpp') + get_src_files(LIB_DIR, 'cpp') + ['pyhellweg.c'],
            extra_compile_args=get_compile_args(),
        )    
    ]
)
