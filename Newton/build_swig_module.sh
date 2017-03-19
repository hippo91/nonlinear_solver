#!/bin/bash
swig3.0 -Wall -v -python launch_vnr_resolution.i
python setup.py build_ext --inplace
