CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

.PHONY: charge plaquette plaquetteMetropolis plaquetteStdlib blockingAll clean

####
charge: charge.py charge.dat
	python $<

charge.dat: charge.exe
	./$<

plaquette: plaquette.exe blockingAll.exe
	./$<
	make blockingAll

plaquetteMetropolis: plaquetteMetropolis.exe blockingAll.exe
	./$<
	make blockingAll

plaquetteStdlib: plaquetteStdlib.exe blockingAll.exe
	./$<
	make blockingAll

####

blockingAll: blockingAll.exe plaquette.dat
	./$< plaquette.dat

####

charge.exe: charge.o lattice.o metropolisHastings.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

plaquette.exe: plaquette.o lattice.o metropolisHastings.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

plaquetteMetropolis.exe: plaquette.o lattice.o metropolis.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

plaquetteStdlib.exe: plaquette.o lattice.o metropolisHastings.o randomStdlib.o
	$(CC) $(CFLAGS) -o $@ $^

blockingAll.exe: blockingAll.c
	$(CC) $(CFLAGS) -o $@ $^

####

plaquette.o: plaquette.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

metropolisHastings.o: ./samplingImplements/metropolisHastings.c sampling.h lattice.h random.h
	@cp $< sampling.c
	$(CC) $(CFLAGS) -c -o $@ sampling.c
	@rm sampling.c

metropolis.o: ./samplingImplements/metropolis.c sampling.h lattice.h random.h
	@cp $< sampling.c
	$(CC) $(CFLAGS) -c -o $@ sampling.c
	@rm sampling.c

randomGsl.o: ./randomImplements/randomGsl.c sampling.h lattice.h random.h
	@cp $< random.c
	$(CC) $(CFLAGS) -c -o $@ random.c
	@rm random.c

randomStdlib.o: ./randomImplements/randomStdlib.c sampling.h lattice.h random.h
	@cp $< random.c
	$(CC) $(CFLAGS) -c -o $@ random.c
	@rm random.c

clean:
	@rm -f *.exe *.o *.dat 
	@rm -f sampling.c random.c
	@rm -rf plots/

