from setuptools import setup, find_packages
from setuptools.command.install import install
from distutils import util
from distutils.command.build import build
from subprocess import call

import os
import shutil
from glob import glob

try:
    import pyHellweg2D_builder
except ImportError:
    import pip 
    pip.main(['install', 'cffi'])
    import pyHellweg2D_builder


if 'macosx' in util.get_platform():
    DYLIB = 'dylib'
else:
    DYLIB = 'so'

BASE_DIR = os.path.abspath(os.path.dirname(__file__))
BUILD_DIR = os.path.join(BASE_DIR, 'build')
LIB_DIR = os.path.abspath(os.path.join(BASE_DIR, '..', 'libHellweg2D'))
LIB_BUILD_DIR = os.path.join(BUILD_DIR, 'libHellweg2D.{}'.format(util.get_platform()))

FFI_BUILDER = pyHellweg2D_builder.get_ffibuilder([LIB_DIR], [LIB_BUILD_DIR])

class PyHellweg2DBuild(build):
    def exec_call(self, *a, **kw):
        exec_msg = kw.pop('exec_msg', '')
        l = lambda: call(*a, **kw)
        self.execute(l, [], exec_msg)

    def build_libHellweg2D(self):
        self.mkpath(LIB_BUILD_DIR)

        self.exec_call(['cmake', LIB_DIR], cwd=LIB_BUILD_DIR, 
                exec_msg='Configuring libHellweg2D')
        
        self.exec_call(['make'], cwd=LIB_BUILD_DIR, 
                exec_msg='Building libHellweg2D')

    def copy_libHellweg2D(self):
        for filepath in glob(os.path.join(LIB_BUILD_DIR, 'libHellweg2D*.{0}'.format(DYLIB))):
            self.copy_file(filepath, self.build_lib)

    def run(self):
        self.build_libHellweg2D()
        build.run(self)
        self.copy_libHellweg2D()

setup(
    name='hellweg2d',
    version='0.0.1',
    ext_modules=[FFI_BUILDER.distutils_extension()],
    packages=find_packages(exclude=['tests']),
    install_requires=['sh'],
    setup_requires=['cffi'],
    cmdclass={
        'build': PyHellweg2DBuild,
    },
    entry_points = {
        'console_scripts': ['hellweg2d.py=hellweg2d.cmd:main']
    }
)
