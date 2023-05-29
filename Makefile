.PHONY : all

all:
	$(MAKE) -C alltoall
	$(MAKE) -C bruck2
	$(MAKE) -C brucksqrt
	$(MAKE) -C spreadout

clean:
	$(MAKE) -C alltoall clean
	$(MAKE) -C bruck2 clean
	$(MAKE) -C brucksqrt clean
	$(MAKE) -C spreadout clean