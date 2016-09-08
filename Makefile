CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

####

.PHONY: charge

charge: metropolisHastings gsl runCharge plotCharge

####

.PHONY: plaquette plaquetteMetropolis plaquetteStdlib

plaquette: metropolisHastings gsl runPlaquette runBlockingAll

plaquetteMetropolis: metropolis gsl runPlaquette runBlockingAll

plaquetteStdlib: metropolisHastings stdlib runPlaquette runBlockingAll

####

DTC = data/torus/charge
.PHONY: runCharge plotCharge 

runCharge: charge.exe
	@if [ ! -d '$(DTC)' ]; then mkdir -p $(DTC)/fixed $(DTC)/phys; fi
	#Scaling of <Q^2> at fixed beta=1.0
	./$< 1.0  5 $(DTC)/fixed/b1.0n05.dat
	./$< 1.0 10 $(DTC)/fixed/b1.0n10.dat
	./$< 1.0 20 $(DTC)/fixed/b1.0n20.dat
	./$< 1.0 40 $(DTC)/fixed/b1.0n40.dat
	#Scaling at constant physics
	./$< 0.8  8 $(DTC)/phys/b0.8n08.dat
	./$< 1.8 12 $(DTC)/phys/b1.8n12.dat
	./$< 3.2 16 $(DTC)/phys/b3.2n16.dat
	./$< 5.0 20 $(DTC)/phys/b5.0n20.dat
	./$< 7.2 24 $(DTC)/phys/b7.2n24.dat
	./$< 9.8 28 $(DTC)/phys/b9.8n28.dat

plotCharge:
	./plotFixedCharge.py
	./plotPhysCharge.py

####

.PHONY: runPlaquette runBlockingAll

runPlaquette: plaquette.exe
	@if [ ! -d 'data' ]; then mkdir data; fi
	./$<

runBlockingAll: blockingAll.exe ./data/plaquette.dat
	./$^

####

.PHONY: metropolisHastings metropolis gsl stdlib

metropolisHastings: ./samplingImplements/metropolisHastings.c
	@if ! cmp $< sampling.c >/dev/null 2>&1; then cp $< sampling.c; fi

metropolis: ./samplingImplements/metropolis.c
	@if ! cmp $< sampling.c >/dev/null 2>&1; then cp $< sampling.c; fi

gsl: ./randomImplements/randomGsl.c
	@if ! cmp $< random.c >/dev/null 2>&1; then cp $< random.c; fi

stdlib: ./randomImplements/randomStdlib.c
	@if ! cmp $< random.c >/dev/null 2>&1; then cp $< random.c; fi

####

charge.exe: charge.o lattice.o sampling.o random.o
	$(CC) $(CFLAGS) -o $@ $^

plaquette.exe: plaquette.o lattice.o sampling.o random.o
	$(CC) $(CFLAGS) -o $@ $^

blockingAll.exe: blockingAll.c
	$(CC) $(CFLAGS) -o $@ $<

####

charge.o: charge.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

plaquette.o: plaquette.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

sampling.o: sampling.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c -o $@ $<

####

.PHONY: clean

clean:
	@rm -f *.exe *.o *.pyc *.dat 
	@rm -f sampling.c random.c
	@rm -rf plots/ data/

