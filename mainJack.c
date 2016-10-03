#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "jackknife.h"

#define JSETS 20

int main(int argc, char *argv[])
{
    double beta = 0.;
    int n = 0;
    int sweeps = 0;
    int therm = 0;
    int getPlaquette = 0;
    char dataPlaquette[40] = "";
    char jackPlaquette[40] = "";
    int getCharge = 0;
    char dataCharge[40] = "";
    char jackCharge[40] = "";
    int getChargeEvenOdd = 0;
    char dataChargeEvenOdd[40] = "";
    char jackChargeEvenOdd[40] = "";

    assert(argc==2);
    
    FILE *paramFile = fopen(argv[1], "r"); assert(paramFile);
    char varName[40], varValue[40];
    while ( fscanf(paramFile, "%s %s", varName, varValue) == 2)
    {
        if      (!strcmp(varName, "beta")) beta = atof(varValue);
        else if (!strcmp(varName, "n")) n = atoi(varValue);
        else if (!strcmp(varName, "sweeps")) sweeps = atoi(varValue);
        else if (!strcmp(varName, "therm")) therm = atoi(varValue);
        else if (!strcmp(varName, "getPlaquette")) getPlaquette = atoi(varValue);
        else if (!strcmp(varName, "dataPlaquette")) strcpy(dataPlaquette, varValue);
        else if (!strcmp(varName, "jackPlaquette")) strcpy(jackPlaquette, varValue);
        else if (!strcmp(varName, "getCharge")) getCharge = atoi(varValue);
        else if (!strcmp(varName, "dataCharge")) strcpy(dataCharge, varValue);
        else if (!strcmp(varName, "jackCharge")) strcpy(jackCharge, varValue);
        else if (!strcmp(varName, "getChargeEvenOdd")) getChargeEvenOdd = atoi(varValue);
        else if (!strcmp(varName, "dataChargeEvenOdd")) strcpy(dataChargeEvenOdd, varValue);
        else if (!strcmp(varName, "jackChargeEvenOdd")) strcpy(jackChargeEvenOdd, varValue);
    }
    fclose(paramFile);

    printf("thermalization iteration = %d\n", therm);
    if (getPlaquette) printf("jack plaquette out = %s\n", jackPlaquette);
    if (getCharge) printf("jack charge out = %s\n", jackCharge);
    if (getChargeEvenOdd) printf("jack chargeEvenOdd out = %s\n", jackChargeEvenOdd);

    double *data = malloc((sweeps-therm)*sizeof(double));
    double jMean, jVar;
    FILE *inputFile, *outputFile;

    if (getPlaquette)
    {
        inputFile = fopen(dataPlaquette, "r"); assert(inputFile);
        outputFile = fopen(jackPlaquette, "a"); assert(outputFile);
        ReadFile(inputFile, data, sweeps, therm);
        Jackknife(JackMean, data, sweeps-therm, JSETS, &jMean, &jVar);
        fprintf(outputFile, "%.1f\t%i\t%.16e\t%.16e\n", beta, n, jMean, sqrt(jVar));
        printf("%.16e +/- %.16e\n", jMean, sqrt(jVar));
        fclose(inputFile);
        fclose(outputFile);
    }

    if (getCharge)
    {
        inputFile = fopen(dataCharge, "r"); assert(inputFile);
        outputFile = fopen(jackCharge, "a"); assert(outputFile);;
        ReadFile(inputFile, data, sweeps, therm);
        Jackknife(JackSquareMean, data, sweeps-therm, JSETS, &jMean, &jVar);
        fprintf(outputFile, "%.1f\t%i\t%.16e\t%.16e\n", beta, n, jMean, sqrt(jVar));
        printf("%.16e +/- %.16e\n", jMean, sqrt(jVar));
        fclose(inputFile);
        fclose(outputFile);
    }

    if (getChargeEvenOdd)
    {
        inputFile = fopen(dataChargeEvenOdd, "r"); assert(inputFile);
        outputFile = fopen(jackChargeEvenOdd, "a"); assert(outputFile);;
        ReadFile(inputFile, data, sweeps, therm);
        Jackknife(JackEvenOdd, data, sweeps-therm, JSETS, &jMean, &jVar);
        fprintf(outputFile, "%.1f\t%i\t%.16e\t%.16e\n", beta, n, jMean, sqrt(jVar));
        printf("%.16e +/- %.16e\n", jMean, sqrt(jVar));
        fclose(inputFile);
        fclose(outputFile);
    }

    return 0;
}

