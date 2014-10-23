#! /usr/bin/python
import sys

name = sys.argv[1] if len(sys.argv) > 1 else 'zombie'

s = """#!/bin/bash

# The name of the job:
#SBATCH --job-name="%s"

# Number of BlueGene compute nodes used by the job (multiply by 16 to get CPU cores):
#SBATCH --nodes=1

# The maximum running time of the job in days-hours:mins
#SBATCH --time=0-1:0

# The job command(s):
srun --ntasks-per-node=1 zombie %s
"""

param = '-n %s -t'

for i in range(1, 65):
    f = open('slurm_job%s' % i, 'w')
    p = param % i
    f.write(s % (name, p))
    f.close
    