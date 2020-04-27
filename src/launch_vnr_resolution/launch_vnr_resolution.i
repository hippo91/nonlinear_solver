/* File: launch_vnr_resolution.i */
%module launch_vnr_resolution // This setting maybe overriden by CMake commands

%{
#define SWIG_FILE_WITH_INIT
#include "array.h"
#include "launch_vnr_resolution.h"
%}

%include "numpy.i"

%init %{
import_array();
%}

%apply (int DIM1, double* IN_ARRAY1) {(int od_size, double* old_specific_volume), (int nd_size, double* new_specific_volume),
                                      (int p_size, double* pressure), (int ie_size, double* internal_energy),
                                      (int nie_size, double* new_internal_energy), (int np_size, double* new_pressure),
                                      (int nv_size, double* new_soundspeed)}

%include "launch_vnr_resolution.h"
%rename (launch_vnr_resolution) wrap_launch_vnr_resolution;

%inline %{
  void wrap_launch_vnr_resolution(int od_size, double* old_specific_volume, int nd_size, double* new_specific_volume,
                                  int p_size, double* pressure, int ie_size, double* internal_energy,
                                  int nie_size, double* new_internal_energy, int np_size, double* new_pressure,
                                  int nv_size, double* new_soundspeed) {
    int pb_size = od_size;
    if ((pb_size != nd_size) || (pb_size != p_size) || (pb_size != ie_size) ||
        (pb_size != nie_size) || (pb_size != np_size) || (pb_size != nv_size)) {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d, %d, %d, %d, %d, %d, %d) given", pb_size, nd_size, p_size,
          ie_size, nie_size, np_size, nv_size);
    }
    s_array arr_old_specific_volume = {od_size, "OldSpecificVolume", old_specific_volume};
    s_array arr_new_specific_volume = {nd_size, "NewSpecificVolume", new_specific_volume};
    s_array arr_pressure = {p_size, "Pressure", pressure};
    s_array arr_internal_energy = {ie_size, "InternalEnergy", internal_energy};
    s_array arr_new_internal_energy = {nie_size, "NewInternalEnergy", new_internal_energy};
    s_array arr_new_pressure = {np_size, "NewPressure", new_pressure};
    s_array arr_new_soundspeed = {nv_size, "NewSoundSpeed", new_soundspeed};
    launch_vnr_resolution(&arr_old_specific_volume, &arr_new_specific_volume, &arr_pressure, &arr_internal_energy,
                          &arr_new_internal_energy, &arr_new_pressure, &arr_new_soundspeed);
  }
%}
