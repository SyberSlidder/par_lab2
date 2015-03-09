#!/bin/bash
#SBATCH -J openmp1           # job name
#SBATCH -o result_nomp.txt       # output and error file name (%j expands to jobID)
#SBATCH -n 1              # total number of mpi tasks requested
#SBATCH -N 1              # total number of mpi tasks requested
#SBATCH -A EE382N-20              # total number of mpi tasks requested
#SBATCH -p serial     # queue (partition) -- normal, development, etc.
#SBATCH -t 12:00:00        # run time (hh:mm:ss) - 1.5 hours
make
./scan 1000000000
