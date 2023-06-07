#!/bin/bash

# NODES=`wc -l < $PBS_NODEFILE`
# let N=`nproc`
# let PPN=$(( N / 2 ))

NODES=1
PPN=(32 64)
RADIX=(2 4 8 16 24 32 40 48 56 64 72 80)

for i in "${PPN[@]}"
do
    :
    let RANKS=$i*$NODES
    for j in "${RADIX[@]}"
    do
        : 
        mpiexec -n $RANKS -ppn $PPN ./bruckcontrol/bruckcontrol.out $NODES $PPN $j
        # echo "mpiexec -n ${RANKS} -ppn ${PPN} ./bruckcontrol/bruckcontrol.out ${NODES} ${PPN} ${j}"
    done
done