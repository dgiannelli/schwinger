CC = gcc
CFLAGS = -std=gnu11 -O3 -Wall -lm -lgsl -lgslcblas 

####

.PHONY: charge plaquette
.PHONY: runCharge plotCharge 
.PHONY: runPlaquette runBlockingAll
.PHONY: clean

####

runPlaquette: main.exe
	./$< params/plaquette.par

runCharge: ./infty.sh ./cont.sh
	./infty.sh
	./cont.sh

plotCharge:
	if [ ! -d 'plots' ]; then mkdir -p plots; fi
	pdflatex -output-directory=plots -shell-escape freezing.tex
	pdflatex -output-directory=plots -shell-escape badGuy.tex
	pdflatex -output-directory=plots -shell-escape moebEvo.tex
	pdflatex -output-directory=plots -shell-escape inftyTorus.tex 
	pdflatex -output-directory=plots -shell-escape inftyMoeb.tex
	pdflatex -output-directory=plots -shell-escape inftyBoth.tex
	pdflatex -output-directory=plots -shell-escape contTorus.tex
	pdflatex -output-directory=plots -shell-escape contMoeb.tex
	pdflatex -output-directory=plots -shell-escape contBoth.tex
	#pdflatex -output-directory=plots -shell-escape evenOdd.tex

####

main.exe: main.o lattice.o random.o jackknife.o
	$(CC) $(CFLAGS) -o $@ $^

####

main.o: main.c lattice.h random.h jackknife.h
	$(CC) $(CFLAGS) -c -o $@ $<

lattice.o: lattice.c lattice.h random.h
	$(CC) $(CFLAGS) -c -o $@ $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c -o $@ $<

####


clean:
	@rm -f *.exe *.o *.dat *.aux *.log

cleanData:
	@rm -rf data/
	
cleanPlots:
	@rm -rf plots/

