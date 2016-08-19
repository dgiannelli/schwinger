CC = gcc
CFLAGS = -O3 -lm

schwinger: schwinger.exe
	./schwinger.exe

schwinger.exe: schwinger.o lattice.o
	$(CC) $(CFLAGS) -o schwinger.exe schwinger.o lattice.o

schwinger.o: schwinger.c lattice.h
	$(CC) $(CFLAGS) -c -o schwinger.o schwinger.c

lattice.o: lattice.c lattice.h
	$(CC) $(CFLAGS) -c -o lattice.o lattice.c

clean:
	rm *.exe *.o *.dat

