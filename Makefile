CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

####

.PHONY: charge plaquette
.PHONY: runCharge plotCharge 
.PHONY: runPlaquette runBlockingAll
.PHONY: clean

####

charge: runCharge plotCharge

plaquette: runPlaquette runBlockingAll

####

runCharge: charge.exe
	@if [ ! -d '$(DTC)' ]; then mkdir -p $(DTC)/fixed $(DTC)/phys; fi
	./charge.exe

plotCharge:
	./plotFixedCharge.py
	./plotPhysCharge.py

####

runPlaquette: plaquette.exe
	@if [ ! -d 'data/torus/plaquette' ]; then mkdir -p data/torus/plaquette; fi
	./$<

runBlockingAll: blockingAll.exe ./data/torus/plaquette/plaquette.dat
	./$^

####

charge.exe: charge.o lattice.o random.o
	$(CC) $(CFLAGS) -o $@ $^

plaquette.exe: plaquette.o lattice.o random.o
	$(CC) $(CFLAGS) -o $@ $^

blockingAll.exe: blockingAll.c
	$(CC) $(CFLAGS) -o $@ $^

####

charge.o: charge.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

plaquette.o: plaquette.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c -o $@ $<

####


clean:
	@rm -f *.exe *.o *.pyc *.dat 
	@rm -rf plots/ data/

