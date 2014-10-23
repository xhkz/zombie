#! /usr/bin/python
import sys

mpi = (sys.argv[-1] == 'mpi')

name = 'zombie_mpi' if mpi else 'zombie'
nodes = 2 if mpi else 1

s = """#!/bin/bash

# The name of the job:
#SBATCH --job-name="%s"

# Number of BlueGene compute nodes used by the job (multiply by 16 to get CPU cores):
#SBATCH --nodes=%s

# The maximum running time of the job in days-hours:mins
#SBATCH --time=0-6:0

# The job command(s):
srun --ntasks-per-node=1 zombie %s
"""

param = '-n %s -t'
threads = [1, 2, 4, 8, 12, 16, 24, 32, 64]

for i in threads:
    f = open('slurm_job%s' % i, 'w')
    p = param % i
    f.write(s % (name, nodes, p))
    f.close
    