CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas

.PHONY: default metropolis stdlib blockingAll schwinger schwingerMetropolis schwingerStdlib clean

####

default: schwinger blockingAll

metropolis: schwingerMetropolis blockingAll

stdlib: schwingerStdlib blockingAll

####

blockingAll: blockingAll.exe plaquette.dat
	./$^

####

schwinger: schwinger.exe
	./$<

schwingerMetropolis: schwingerMetropolis.exe
	./$<

schwingerStdlib: schwingerStdlib.exe
	./$<

####

schwinger.exe: schwinger.o lattice.o metropolisHastings.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerMetropolis.exe: schwinger.o lattice.o metropolis.o randomGsl.o
	$(CC) $(CFLAGS) -o $@ $^

schwingerStdlib.exe: schwinger.o lattice.o metropolisHastings.o randomStdlib.o
	$(CC) $(CFLAGS) -o $@ $^

blockingAll.exe: blockingAll.c
	$(CC) $(CFLAGS) -o $@ $^

####

schwinger.o: schwinger.c lattice.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h sampling.h
	$(CC) $(CFLAGS) -c -o $@ $<

metropolisHastings.o: ./samplingImplements/metropolisHastings.c sampling.h lattice.h random.h
	cp $< sampling.c
	$(CC) $(CFLAGS) -c -o $@ sampling.c

metropolis.o: ./samplingImplements/metropolis.c sampling.h lattice.h random.h
	cp $< sampling.c
	$(CC) $(CFLAGS) -c -o $@ sampling.c

randomGsl.o: ./randomImplements/randomGsl.c sampling.h lattice.h random.h
	cp $< random.c
	$(CC) $(CFLAGS) -c -o $@ random.c

randomStdlib.o: ./randomImplements/randomStdlib.c sampling.h lattice.h random.h
	cp $< random.c
	$(CC) $(CFLAGS) -c -o $@ random.c

clean:
	@rm -f *.exe *.o *.dat 
	@rm -f sampling.c random.c

