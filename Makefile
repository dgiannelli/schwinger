CC = gcc
CFLAGS = -g -std=c11 -O3 -Wall -lm -lgsl -lgslcblas

.PHONY = testPlaquette clean

####

testPlaquette: main.exe analysis.exe
	./main.exe params/plaquetteTorus.par
	./analysis.exe params/plaquetteTorus.par

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

####

clean:
	rm *.o *.exe

