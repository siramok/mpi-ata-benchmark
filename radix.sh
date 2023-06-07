#!/bin/bash

# NODES=`wc -l < $PBS_NODEFILE`
# let N=`nproc`
# let PPN=$(( N / 2 ))

# NODES=1
NODES=`wc -l < $PBS_NODEFILE`
PPN=(32 64)
RADIX=(2 4 8 16 24 32 40 48 56 64 72 80 128 256 512 1024 2048 4096)

for i in "${PPN[@]}"
do
    :
    let RANKS=$i*$NODES
    for j in "${RADIX[@]}"
    do
        : 
        mpiexec --oversubscribe -n $RANKS -ppn $PPN ./bruckcontrol/bruckcontrol.out $NODES $PPN $j
        # echo "mpiexec -n ${RANKS} -ppn ${PPN} ./bruckcontrol/bruckcontrol.out ${NODES} ${PPN} ${j}"
    done
done