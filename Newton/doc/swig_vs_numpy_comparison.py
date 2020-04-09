#!/usr/bin/env python2.7
# coding: utf-8

## Use of swig and numpy to compute the evolution of internal energy in a vNR scheme

from IPython import get_ipython
import os

##### We will use numpy extensively

import numpy as np

##### Import the module created by swig as a classical python module

import vnrinternalenergy as vnr_ext

##### Import classical numpy modules for performance comparison purpose

from xvof.solver.functionstosolve.vnrenergyevolutionforveformulation import VnrEnergyEvolutionForVolumeEnergyFormulation
from xvof.solver.newtonraphson import NewtonRaphson
from xvof.equationsofstate.miegruneisen import MieGruneisen

### Creation of the data

pb_size = 1000

old_density = np.ndarray(pb_size, dtype=np.float64, order='c')

new_density = np.ndarray(pb_size, dtype=np.float64, order='c')

pressure= np.ndarray(pb_size, dtype=np.float64, order='c')

internal_energy = np.ndarray(pb_size, dtype=np.float64, order='c')

new_internal_energy = np.ndarray(pb_size, dtype=np.float64, order='c')

new_pressure = np.ndarray(pb_size, dtype=np.float64, order='c')

new_soundspeed = np.ndarray(pb_size, dtype=np.float64, order='c')

mask = np.ndarray(pb_size, dtype=np.bool, order='c')

##### Initialization of the data

old_density[:] = 7500.

new_density[:] = 9500.

pressure[:] = 1e+09

internal_energy[:] = 1e+06

new_internal_energy[:] = 0.

new_pressure[:] = 0.

new_soundspeed[:] = 0.

mask[:] = True
### Test of performance of swig made module
tmp_ie = np.ndarray(pb_size, dtype=np.float64, order='c')
tmp_p = np.ndarray(pb_size, dtype=np.float64, order='c')
tmp_ss = np.ndarray(pb_size, dtype=np.float64, order='c')
print "Swig performance :"
get_ipython().magic(u'''timeit vnr_ext.launch_vnr_resolution(old_density[mask], new_density[mask], pressure[mask], internal_energy[mask], tmp_ie, tmp_p, tmp_ss)''')

new_internal_energy[mask] = tmp_ie
new_pressure[mask] = tmp_p
new_soundspeed[mask] = tmp_ss

c_pressure = np.copy(new_pressure)
c_internal_energy = np.copy(new_internal_energy)
c_soundspeed = np.copy(new_soundspeed)

### Test of performance of classical numpy module

new_internal_energy[:] = 0.

new_pressure[:] = 0.

new_soundspeed[:] = 0.

function_to_vanish = VnrEnergyEvolutionForVolumeEnergyFormulation()
solver = NewtonRaphson(function_to_vanish)

my_variables = {'EquationOfState': MieGruneisen(),
                'OldDensity': old_density,
                'NewDensity': new_density,
                'Pressure': pressure,
                'OldEnergy': internal_energy}

function_to_vanish.setVariables(my_variables)

print "Classical numpy module performance :"
get_ipython().magic(u'timeit ref_internal_energy = solver.computeSolution(internal_energy)')

ref_internal_energy = solver.computeSolution(internal_energy[mask])

ref_pressure = np.ndarray(pb_size, dtype=np.float64, order='c')
ref_soundspeed = np.ndarray(pb_size, dtype=np.float64, order='c')
dummy = np.ndarray(pb_size, dtype=np.float64, order='c')

tmp_p[:] = 0.
tmp_ss[:] = 0.
my_variables['EquationOfState'].solveVolumeEnergy(1. / new_density[mask], ref_internal_energy[mask], tmp_p, tmp_ss, dummy[mask])
ref_pressure[mask] = tmp_p
ref_soundspeed[mask] = tmp_ss

print "Pressure difference between reference and swig computations : %15.9g"%(max(abs(ref_pressure - c_pressure)))
print "Internal energy difference between reference and swig computations : %15.9g"%(max(abs(ref_internal_energy - c_internal_energy)))
print "Sound speed difference between reference and swig computations : %15.9g"%(max(abs(ref_soundspeed - c_soundspeed)))

