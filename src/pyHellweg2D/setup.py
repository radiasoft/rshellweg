from setuptools import setup
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


BASE_DIR = os.path.abspath(os.path.dirname(__file__))
BUILD_DIR = os.path.join(BASE_DIR, 'build')
LIB_DIR = os.path.abspath(os.path.join(BASE_DIR, '..', 'libHellweg2D'))
LIB_BUILD_DIR = os.path.join(BUILD_DIR, 'libHellweg2D-{}'.format(util.get_platform()))

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
        
        self.exec_call(['make', '-j2'], cwd=LIB_BUILD_DIR, 
                exec_msg='Building libHellweg2D')

    def copy_libHellweg2D(self):
        build_path = os.path.abspath(self.build_temp)
        for filepath in glob(os.path.join(LIB_BUILD_DIR, '*.dylib')):
            shutil.copy(filepath, build_path)
            filename = os.path.basename(filepath)
            print('copy {0}->{1}'.format(filepath, os.path.join(build_path, filename)))


    def run(self):
        self.build_libHellweg2D()
        build.run(self)

class PyHellweg2DInstall(install):
    pass




setup(
    name='pyhellweg',
    version='0.0.1',
    ext_modules=[FFI_BUILDER.distutils_extension()],
    cmdclass={
        'build': PyHellweg2DBuild,
        'install': PyHellweg2DInstall
    }
)
