CC = gcc
CFLAGS = -g -std=c11 -O3 -Wall -lm -lgsl -lgslcblas

.PHONY = testPlaquette clean

####

testPlaquette: main.exe analysis.exe
	parallel ./main.exe ::: params/plaquette*.par 
	for par in params/plaquette*.par; do ./analysis.exe $$par; done

runCharge: main.exe analysis.exe
	parallel ./main.exe ::: params/infty*.par
	for par in params/infty*.par; do ./analysis.exe $$par; done

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

