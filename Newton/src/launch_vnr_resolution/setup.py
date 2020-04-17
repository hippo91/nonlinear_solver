#!/usr/bin/env python

"""
setup.py file for vNR scheme internal energy evolution swig project
"""

#from distutils.core import setup, Extension
from distutils.core import *
from distutils      import sysconfig

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

vnrinternalenergy_module = Extension('_vnrinternalenergy',
                           sources=['launch_vnr_resolution_wrap.c', 'incrementations_methods.c', 'launch_vnr_resolution.c',
                                    'miegruneisen.c', 'newton.c', 'stop_criterions.c', 'utils.c',
                                    'vnr_internalenergy_evolution.c'],
                           include_dirs = [numpy_include],
                           extra_compile_args=['-O3', '-march=native', '-mtune=native', '-std=gnu99', '-fopenmp'],
                           extra_link_args=['-fopenmp'],
                           )

setup (name = 'vnrinternalenergy',
       version = '0.1',
       author      = "hippo91",
       description = """This module compute the evolution of the internal energy in the vNR scheme for a MieGruneisen eos""",
       ext_modules = [vnrinternalenergy_module],
       py_modules = ["vnrinternalenergy"],
       )
