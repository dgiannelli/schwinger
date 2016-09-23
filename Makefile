CC = gcc
CFLAGS = -fopenmp -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas 

####

.PHONY: charge plaquette
.PHONY: runCharge plotCharge 
.PHONY: runPlaquette runBlockingAll
.PHONY: clean

####

runPlaquette: plaquette.exe
	./$<

runCharge: charge.exe
	OMP_NUM_THREADS=2 ./charge.exe

plotCharge:
	if [ ! -d 'plots' ]; then mkdir -p plots; fi
	pdflatex -output-directory=plots inftyTorus.tex 
	pdflatex -output-directory=plots cntnmTorus.tex
	pdflatex -output-directory=plots freezing.tex
	pdflatex -output-directory=plots badGuy.tex

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
	@rm -f *.exe *.o *.pyc *.dat *.aux *.log

cleanData:
	@rm -rf data/
	
cleanPlots:
	@rm -rf plots/

