.PHONY : all

all:
	$(MAKE) -C mpi-ata
	# $(MAKE) -C mpi-atav
	$(MAKE) -C mpi-ata-bruck-2
	$(MAKE) -C mpi-ata-bruck-sqrt
	# $(MAKE) -C mpi-ata-bruckv
	# $(MAKE) -C mpi-ata-spreadout

clean:
	$(MAKE) -C mpi-ata clean
	# $(MAKE) -C mpi-atav clean
	$(MAKE) -C mpi-ata-bruck-2 clean
	$(MAKE) -C mpi-ata-bruck-sqrt clean
	# $(MAKE) -C mpi-ata-bruckv clean
	# $(MAKE) -C mpi-ata-spreadout clean