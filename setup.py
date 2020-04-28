"""
This module build the module vnr_internal_energy that solves the evolution
of internal energy in the Von-Neumann Richtmeyer scheme

Modified version of https://github.com/m-pilia/disptools
"""
import os
import platform
import subprocess
import sys
import sysconfig
from pprint import pprint
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, cmake_lists_dir='.', sources=[], **kwa):
        Extension.__init__(self, name, sources=sources, **kwa)
        self.cmake_lists_dir = os.path.abspath(cmake_lists_dir)


class CMakeBuild(build_ext):

    def build_extensions(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError('Cannot find CMake executable')

        for ext in self.extensions:

            extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            cfg = os.environ.get('Debug', 'Release')

            cmake_args = [
                '-DCMAKE_BUILD_TYPE=%s' % cfg,
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}'.format(extdir),
                '-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={}'.format(self.build_temp),
                '-DPYTHON_LIBRARY_NAME={}'.format(ext.name + sysconfig.get_config_var('EXT_SUFFIX'))
            ]

            if platform.system() == 'Windows':
                plat = ('x64' if platform.architecture()[0] == '64bit' else 'Win32')
                cmake_args += [
                    '-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE',
                    '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir),
                ]
                if self.compiler.compiler_type == 'msvc':
                    cmake_args += [
                        '-DCMAKE_GENERATOR_PLATFORM=%s' % plat,
                    ]
                else:
                    cmake_args += [
                        '-G', 'MinGW Makefiles',
                    ]

            pprint(cmake_args)

            if not os.path.exists(self.build_lib):
                os.makedirs(self.build_lib)
            if not os.path.exists(self.build_temp):
                os.makedirs(self.build_temp)

            # Config and build the extension
            subprocess.check_call(['cmake', ext.cmake_lists_dir] + cmake_args,
                                  cwd=self.build_temp)
            subprocess.check_call(['cmake', '--build', '.', '--config', cfg],
                                  cwd=self.build_temp)

# The following line is parsed by Sphinx
version = '1.0.0'

setup(name='vnr_internal_energy',
      version=version,
      description='Compute internal energy evolution in the Von-Neumann Richtmeyer scheme',
      author='Guillaume Peillex',
      author_email='guillaume.peillex@gmail.com',
      url='https://github.com/hippo91/nonlinear_solver',
      keywords=['hydrocode', 'vnr', 'equation of state'],
      long_description=open('README.md').read(),
      long_description_content_type='text/markdown',
      install_requires=['numpy>=1.15.1'],
      packages=['python_extension'],
      package_dir={'':'src'},
      ext_modules=[CMakeExtension('_vnr_internal_energy')],
      cmdclass={'build_ext': CMakeBuild},
      zip_safe=False,
      classifiers=[
          "Programming Language :: Python :: 3",
          "Operating System :: POSIX :: Linux",
      ],
      )