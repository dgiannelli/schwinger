/*
 * Main program for generating field configurations
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "lattice.h"
#include "random.h"
#include "jackknife.h"

#define JSETS 20 //Number of Jackknife sets

int main(int argc, char *argv[])
{
    assert(argc == 2);
    FILE *paramFile = fopen(argv[1], "r"); assert(paramFile);

    double beta;
    int n;
    int therm;
    int sweeps;
    char obs[10];

    char varName[20], varValue[20];
    int linenum = 0;
    while ( fscanf(paramFile, "%s %s", varName, varValue) == 2)
    {
        linenum++;
        if (!strcmp(varName, "beta")) beta = atof(varValue);
        else if (!strcmp(varName, "n")) n = atoi(varValue);
        else if (!strcmp(varName, "therm")) therm = atoi(varValue);
        else if (!strcmp(varName, "sweeps")) sweeps = atoi(varValue);
        else if (!strcmp(varName, "obs")) strcpy(obs, varValue);
    }
    assert(linenum == 5);

    double (*JackFunc)(int jStart, int jEnd, double *data, int size);
    if (!strcmp(obs, "plaquette")) JackFunc = JackMean;
    if (!strcmp(obs, "charge")) JackFunc = JackSquareMean;

    puts("**********\n");
    printf("beta = %f\n", beta);
    printf("lattice side = %d\n", n);
    printf("thermalization sweeps = %d\n", therm);
    printf("number of measures = %d\n", sweeps);
    printf("observable being measured = %s\n", obs);
    puts("");



    RndInit();

    double jMean, jVar, *data = calloc(sweeps, sizeof(double));

    NewLattice(beta, n, therm, "torus", obs); 
    GetMeasures(data, sweeps);
    DeleteLattice();

    Jackknife(JackFunc, data, sweeps, JSETS, &jMean, &jVar);

    printf("Torus: %.16e +/- %.16e\n\n", jMean, sqrt(jVar));

    //FILE *historyFile = fopen("his



    NewLattice(beta, n, therm, "moebius", obs); 
    GetMeasures(data, sweeps);
    DeleteLattice();

    Jackknife(JackFunc, data, sweeps, JSETS, &jMean, &jVar);

    printf("Moebius: %.16e +/- %.16e\n\n", jMean, sqrt(jVar));
    puts("**********\n");

    free(data);
    RndFinalize();

    return 0;
}

