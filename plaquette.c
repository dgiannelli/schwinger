/*
 * Main program for generating field configurations with torus boundaries
 * and measuring the plaquette mean value.
 */

#include <stdio.h>

#include "lattice.h"
#include "random.h"

#define N 20 //Lattice size
#define ITERS 10000 //Number of measurements

#define BETA 4. //Action beta parameter

int main(int argc, char *argv[])
{
    RndInit();

	system("if [ ! -d 'data/torus/plaquette' ]; then mkdir -p data/torus/plaquette; fi");
    FILE *file = fopen("./data/torus/plaquette/plaquette.dat", "w");

    NewLattice(BETA,N,"torus","plaquette"); 
    GetMeasurement(ITERS,file);
    DeleteLattice();

    fclose(file);
    RndFinalize();

    return 0;
}
