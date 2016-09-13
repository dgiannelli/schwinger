/*
 * Main program for generating field configurations with torus boundaries
 * and measuring the plaquette mean value.
 */

#include <stdio.h>

#include "lattice.h"
#include "random.h"

#define N 20 //Lattice size
#define ITERS 30000 //Number of measurement

#define BETA 4. //Action beta parameter

int main(int argc, char *argv[])
{
    RndInit();
    FILE *file = fopen("./data/torus/plaquette.dat", "w");
    NewLattice(BETA,N); 

    GetMeasurement(ITERS,file);

    DeleteLattice();
    fclose(file);
    RndFinalize();

    return 0;
}
