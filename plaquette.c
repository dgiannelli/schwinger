/*
 * Main program for generating field configurations with torus boundaries
 * and measuring the plaquette mean value.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lattice.h"
#include "random.h"

#define BETA 4. //Action beta parameter
#define N 20 //Lattice size

#define ITERS 10000 //Number of measures
#define JSETS 20 //Number of Jackknife sets

double fJack(jStart, jEnd, data, size)
{
    double mean = 0.;
    for (int i=0; i<size; i++)
    {
        if (i<jStart || i>jEnd) mean += data[i];
    }
    return mean/(size+jStart-jEnd-1);


int main(int argc, char *argv[])
{

    RndInit();

    NewLattice(BETA,N,"torus","plaquette"); 

    double jMean, jVar, *data = calloc(ITERS, sizeof(double));
    GetMeasures(data, ITERS);
    Jackknife(f, data, ITERS, JSETS, &jMean, &jVar);
    free(data);

    DeleteLattice();

    RndFinalize();

    printf("Plaquette at beta = %.1f and lattice size = %i:\n%.16e +/- %.16e\n", BETA, N, jMean, sqrt(jVar));

    return 0;
}
