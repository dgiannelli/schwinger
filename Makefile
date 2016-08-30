CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

####

default: blockingAll.exe schwinger
	./$< plaquette.dat

heatBath: blockingAll.exe schwingerHeatBath
	./$< plaquette.dat

stdlib: blockingAll.exe schwingerStdlib
	./$< plaquette.dat

####

blockingAll: blockingAll.exe plaquette.dat
	./$^

####

schwinger: schwinger.exe
	./$<

schwingerHeatBath: schwingerHeatBath.exe
	./$<

schwingerStdlib: schwingerStdlib.exe
	./$<

####

schwinger.exe: schwinger.o lattice.o metropolis.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerHeatBath.exe: schwinger.o lattice.o heatBath.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerStdlib.exe: schwinger.o lattice.o metropolis.o randomStdlib.o
	$(CC) $(CFLAGS) -o $@ $^

blockingAll.exe: blockingAll.c
	$(CC) $(CFLAGS) -o $@ $<

####

schwinger.o: schwinger.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

metropolis.o: metropolis.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

heatBath.o: heatBath.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

randomGsl.o: randomGsl.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

randomStdlib.o: randomStdlib.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.exe *.o *.dat 

