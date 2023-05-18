.PHONY : all

all:
	$(MAKE) -C mpi-ata
	$(MAKE) -C mpi-atav
	$(MAKE) -C mpi-ata-bruck
	$(MAKE) -C mpi-ata-bruckv
	$(MAKE) -C mpi-ata-spreadout

clean:
	$(MAKE) -C mpi-ata clean
	$(MAKE) -C mpi-atav clean
	$(MAKE) -C mpi-ata-bruck clean
	$(MAKE) -C mpi-ata-bruckv clean
	$(MAKE) -C mpi-ata-spreadout clean