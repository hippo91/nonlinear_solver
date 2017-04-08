#!/bin/bash
#----------------------------------------
# A bash script that wraps the call
# to python program comparing performance
# between swig and numpy.
#----------------------------------------
usage() {
  echo $(basename ${0}) "<path_to_xvof> <omp_threads_number>"
  exit $1
}

if [[ ${#} != 2 ]]
then
  usage 1
fi

path_to_xvof=$1
omp_threads_number=$2

export PYTHONPATH=$path_to_xvof
export OMP_NUM_THREADS=$omp_threads_number
ipython ./$(basename --suffix ".sh" $0).py
