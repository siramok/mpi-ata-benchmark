.PHONY : all

all:
	$(MAKE) -C mpi-ata
	$(MAKE) -C mpi-ata-bruck

clean:
	$(MAKE) -C mpi-ata clean
	$(MAKE) -C mpi-ata-bruck clean