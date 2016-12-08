#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gsl/gsl_math.h>

#define THERMRATIO 0.2
#define NBINS 10

double beta;
int n;
char bounds[10];
int sweeps;
int getPlaquette = 0;
int getChargeSq = 0;
int getChargeEvenOdd = 0;
char plaquetteHistory[40];
char chargeSqHistory[40];
char chargeEvenOddHistory[40];
char plaquetteAnalysis[40] = "";
char chargeSqAnalysis[40] = "";
char chargeEvenOddAnalysis[40] = "";

void Bunching(FILE *output, FILE *input)
{
    const int therm = sweeps*THERMRATIO;
    const int size = sweeps - therm;
    const int binSize = ( size + NBINS - 1 ) / NBINS;

    for (int i=0; i<therm; i++) fscanf(input, "%*f");

    double mean = 0.0;
    double sqMean = 0.0;
    double sqMeanBin = 0.0;
    for (int bin=0; bin<NBINS; bin++)
    {
        double meanBin = 0.0;
        int i = 0;
        for(double temp; i<binSize && fscanf(input, "%lf", &temp) == 1; i++)
        {
            if (getChargeSq) temp = gsl_pow_2(temp);
            meanBin += temp;
            sqMean += gsl_pow_2(temp);
        }
        meanBin /= i;
        mean += meanBin;
        sqMeanBin += gsl_pow_2(meanBin);
    }
    mean /= NBINS;
    sqMeanBin /= NBINS;
    sqMean /= size;

    const double errorSq = (sqMeanBin-gsl_pow_2(mean)) / (NBINS - 1); 
    const double sigmaSq = (sqMean-gsl_pow_2(mean)) / (size - 1);
    const int tau = round( ( ceil(errorSq/sigmaSq) - 1.0 ) / 2.0 );

    fprintf(output, "%.16e\t%.16e\t%.1f\t%i\t%s\t%i\n", mean, sqrt(errorSq), beta, n, bounds, tau);
}

int main(int argc, char *argv[])
{
    assert(argc==2);

    FILE *paramFile = fopen(argv[1], "r"); assert(paramFile);
    char paramName[40], paramValue[40];
    while ( fscanf(paramFile, "%s %s", paramName, paramValue) == 2 )
    {
        if      (!strcmp(paramName,"beta")) beta = atof(paramValue);
        else if (!strcmp(paramName,"n")) n = atoi(paramValue);
        else if (!strcmp(paramName,"bounds")) strcpy(bounds,paramValue);
        else if (!strcmp(paramName,"sweeps")) sweeps = atoi(paramValue);
        else if (!strcmp(paramName,"getPlaquette")) getPlaquette = atoi(paramValue);
        else if (!strcmp(paramName,"getChargeSq")) getChargeSq = atoi(paramValue);
        else if (!strcmp(paramName,"getChargeEvenOdd")) getChargeEvenOdd = atoi(paramValue);
        else if (!strcmp(paramName,"plaquetteHistory")) strcpy(plaquetteHistory,paramValue);
        else if (!strcmp(paramName,"chargeSqHistory")) strcpy(chargeSqHistory,paramValue);
        else if (!strcmp(paramName,"chargeEvenOddHistory")) strcpy(chargeEvenOddHistory,paramValue);
        else if (!strcmp(paramName,"plaquetteAnalysis")) strcpy(plaquetteAnalysis,paramValue);
        else if (!strcmp(paramName,"chargeSqAnalysis")) strcpy(chargeSqAnalysis,paramValue);
        else if (!strcmp(paramName,"chargeEvenOddAnalysis")) strcpy(chargeEvenOddAnalysis,paramValue);
    }

    FILE *plaquetteHistoryFile, *plaquetteAnalysisFile;
    FILE *chargeSqHistoryFile, *chargeSqAnalysisFile;
    FILE *chargeEvenOddHistoryFile, *chargeEvenOddAnalysisFile;

    if (getPlaquette)
    {
        assert(plaquetteHistoryFile = fopen(plaquetteHistory, "r"));
        if (strcmp(plaquetteAnalysis,"")) assert(plaquetteAnalysisFile = fopen(plaquetteAnalysis, "a"));
        else plaquetteAnalysisFile = stdout;

        Bunching(plaquetteAnalysisFile, plaquetteHistoryFile);

        if (strcmp(plaquetteAnalysis,"")) fclose(plaquetteAnalysisFile);
    }

    if (getChargeSq)
    {
        assert(chargeSqHistoryFile = fopen(chargeSqHistory, "r"));
        if (strcmp(chargeSqAnalysis,"")) assert(chargeSqAnalysisFile = fopen(chargeSqAnalysis, "a"));
        else chargeSqAnalysisFile = stdout;

        Bunching(chargeSqAnalysisFile, chargeSqHistoryFile);

        if (strcmp(chargeSqAnalysis,"")) fclose(chargeSqAnalysisFile);
    }

    if (getChargeEvenOdd)
    {
        assert(chargeEvenOddHistoryFile = fopen(chargeEvenOddHistory, "r"));
        if (strcmp(chargeEvenOddAnalysis,"")) assert(chargeEvenOddAnalysisFile = fopen(chargeEvenOddAnalysis, "a"));
        else chargeEvenOddAnalysisFile = stdout;

        Bunching(chargeEvenOddAnalysisFile, chargeEvenOddHistoryFile);

        if (strcmp(chargeEvenOddAnalysis,"")) fclose(chargeEvenOddAnalysisFile);
    }

    return 0;
}

