CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

BETAS = 1 2 4 8
NS = 5 10 20 40

.PHONY: charge 
.PHONY: plaquette plaquetteMetropolis plaquetteStldib
.PHONY: runCharge plotCharge runPlaquette runBlockingAll
.PHONY: metropolisHastings metropolis gsl stdlib
.PHONY: clean

####

charge: runCharge plotCharge

####

plaquette: metropolisHastings gsl runPlaquette runBlockingAll

plaquetteMetropolis: metropolis gsl runPlaquette runBlockingAll

plaquetteStdlib: metropolisHastings stdlib runPlaquette runBlockingAll

####

runCharge: charge.exe
	@if [ ! -d 'data' ]; then mkdir data; fi
	@for beta in $(BETAS); do ./$< $(beta) 20; done
	@for n in $(NS); do ./$< 4 $(n); done

plotCharge: charge.py
	python plotChargeBetasEvo.py
	python plotChargeBetasHisto.py

runPlaquette: plaquette.exe
	@if [ ! -d 'data' ]; then mkdir data; fi
	./$<

runBlockingAll: blockingAll.exe ./data/plaquette.dat
	./$^

####

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

####

charge.o: charge.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

plaquette.o: plaquette.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

sampling.o: sampling.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.exe *.o *.dat 
	@rm -f sampling.c random.c
	@rm -rf plots/ data/

