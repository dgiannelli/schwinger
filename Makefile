CC = gcc
CFLAGS = -O3 -lm

schwingerPlot: schwinger clover.py
	python clover.py

schwinger: schwinger.exe blocking.exe
	./schwinger.exe
	./blocking.exe clover.dat

schwinger.exe: schwinger.o lattice.o
	$(CC) $(CFLAGS) -o schwinger.exe schwinger.o lattice.o

schwinger.o: schwinger.c lattice.h
	$(CC) $(CFLAGS) -c -o schwinger.o schwinger.c

lattice.o: lattice.c lattice.h
	$(CC) $(CFLAGS) -c -o lattice.o lattice.c

blocking.exe: blocking.c
	$(CC) $(CFLAGS) -o blocking.exe blocking.c

clean:
	@rm -f *.exe *.o *.dat 

