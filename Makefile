CC = gcc
CFLAGS = -O3 -Wall -lm -lgsl -lgslcblas

default: schwinger.exe jackknife.exe
	./schwinger.exe
	./jackknife.exe plaquette.dat

heatBath: schwingerHeathBath.exe jackknife.exe
	./schwingerHeathBath.exe
	./jackknife.exe plaquette.dat

stdlib: schwingerStdlib.exe jackknife.exe
	./schwingerStdlib.exe
	./jackknife.exe plaquette.dat

####

schwinger.exe: schwinger.o lattice.o metropolis.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerHeatBath.exe: schwinger.o lattice.o heathBath.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerStdlib.exe: schwinger.o lattice.o metropolis.o randomStdlib.o
	$(CC) $(CFLAGS) -o $@ $^

jackknife.exe: jackknife.c
	$(CC) $(CFLAGS) -o $@ $<

####

schwinger.o: schwinger.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

metropolis.o: metropolis.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

heathBath.o: heathBath.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

randomGsl.o: randomGsl.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

randomStdlib.o: randomStdlib.c sampling.h lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.exe *.o *.dat 

