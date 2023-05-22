#!/bin/bash

NODES=`wc -l < $PBS_NODEFILE`
let RANKS=4*$NODES

cd mpi-ata
mpiexec -n $RANKS -ppn 4 ./mpi-ata.out

cd ../mpi-ata-bruck-2
mpiexec -n $RANKS -ppn 4 ./mpi-ata-bruck-2.out

cd ../mpi-ata-bruck-sqrt
mpiexec -n $RANKS -ppn 4 ./mpi-ata-bruck-sqrt.out