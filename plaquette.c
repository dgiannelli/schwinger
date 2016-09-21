/*
 * Main program for generating field configurations with torus boundaries
 * and measuring the plaquette mean value.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lattice.h"
#include "random.h"
#include "jackknife.h"

#define BETA 4. //Action beta parameter
#define N 20 //Lattice size

#define ITERS 10000 //Number of measures
#define JSETS 20 //Number of Jackknife sets

double fJack(int jStart, int jEnd, double *data, int size)
{
    double mean = 0.;
    for (int i=0; i<jStart; i++)
    {
        mean += data[i];
    }
    for (int i=jEnd; i<size; i++)
    {
        mean += data[i];
    }
    return mean/(size+jStart-jEnd);
}

int main(int argc, char *argv[])
{

    RndInit();

    double jMean, jVar, *data = calloc(ITERS, sizeof(double));

    NewLattice(BETA,N,"torus","plaquette"); 
    GetMeasures(data, ITERS);
    DeleteLattice();

    Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
    free(data);

    RndFinalize();

    printf("Plaquette at beta = %.1f and lattice size = %i:\n%.16e +/- %.16e\n", BETA, N, jMean, sqrt(jVar));

    return 0;
}
