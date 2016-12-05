CC = gcc
CFLAGS = -std=c11 -O3 -Wall -lm -lgsl -lgslcblas

####

testPlaquette: main.exe analysis.exe
	./main.exe params/plaquetteTorus.dat
	./analysis.exe params/plaquetteTorus.dat

####

main.exe: main.o lattice.o random.o
	$(CC) $(CFLAGS) $^ -o $@

analysis.exe: analysis.c
	$(CC) $(CFLAGS) $^ -o $@

####

main.o: main.c lattice.h random.h
	$(CC) $(CFLAGS) -c $< -o $@

lattice.o: lattice.c lattice.h random.h
	$(CC) $(CFLAGS) -c $< -o $@

random.o: random.c random.h
	$(CC) $(CFLAGS) -c $< -o $@
