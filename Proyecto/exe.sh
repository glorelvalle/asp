#!/bin/bash
#
#$ -S /bin/bash
#$ -cwd
#$ -o deap.out
#$ -j y
/share/apps/miniconda/bin/python3.7 -m scoop -n 16 tsp_scoop.py
