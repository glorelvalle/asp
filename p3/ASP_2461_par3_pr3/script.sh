#!/bin/bash
#
#$-cwd
#$-j y
#$-S /bin/bash
#$-pe mpi_4_openmp 12
#$-q mpi.q
#
export OMP_NUM_THREADS=2
