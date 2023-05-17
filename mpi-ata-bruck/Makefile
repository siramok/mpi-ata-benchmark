TARGET = mpi-ata-bruck
COMMON = ../common
SRC = $(TARGET).cpp

build:
	mpic++ -o $(TARGET).out $(SRC) 

test:
	mpiexec -n 2 ./$(TARGET).out

run: build test

clean:
	rm -f $(TARGET).out
	rm -f *.log

.PHONY: build
