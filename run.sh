#!/bin/bash

NODES=`wc -l < $PBS_NODEFILE`
let N=`nproc`
let PPN=$(( N / 2 ))

while [ $PPN -ge 2 ]
do
    let RANKS=$PPN*$NODES

    mpiexec -n $RANKS -ppn $PPN ./alltoall/alltoall.out $NODES $PPN
    mpiexec -n $RANKS -ppn $PPN ./spreadout/spreadout.out $NODES $PPN
    mpiexec -n $RANKS -ppn $PPN ./bruck2/bruck2.out $NODES $PPN
    mpiexec -n $RANKS -ppn $PPN ./brucksqrt/brucksqrt.out $NODES $PPN

    PPN=$(( PPN / 2 ))
done