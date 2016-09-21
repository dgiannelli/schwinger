CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

####

.PHONY: charge plaquette
.PHONY: runCharge plotCharge 
.PHONY: runPlaquette runBlockingAll
.PHONY: clean

####

runPlaquette: plaquette.exe
	./$<

runCharge: charge.exe
	./charge.exe

plotCharge:
	./plot*.py

####

plaquette.exe: plaquette.o lattice.o random.o jackknife.o
	$(CC) $(CFLAGS) -o $@ $^

charge.exe: charge.o lattice.o random.o jackknife.o
	$(CC) $(CFLAGS) -o $@ $^

####

plaquette.o: plaquette.c lattice.h random.h jackknife.h
	$(CC) $(CFLAGS) -c -o $@ $<

charge.o: charge.c lattice.h random.h jackknife.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c -o $@ $<

####


clean:
	@rm -f *.exe *.o *.pyc *.dat 
	@rm -rf plots/ data/

