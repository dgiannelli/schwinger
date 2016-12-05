#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gsl/gsl_math.h>

#define THERMRATIO 0.2
#define NBINS 20

double beta;
int n;
char bounds[10];
int sweeps;
int getPlaquette = 0;
char plaquetteHistory[40];
char plaquetteAnalysis[40] = "";

void Bunching(FILE *file)
{
    const int therm = sweeps*THERMRATIO;
    const int size = sweeps - therm;
    const int binSize = ( size + NBINS - 1 ) / NBINS;

    for (int i=0; i<therm; i++) fscanf(file, "%*f");

    double mean = 0.0;
    double meanSq = 0.0;
    for (int bin=0; bin<NBINS; bin++)
    {
        double meanBin = 0.0;
        int i = 0;
        for(double temp; i<binSize && fscanf(file, "%lf", &temp); i++)
        {
            meanBin += temp;
        }
        meanBin /= i;
        mean += meanBin;
        meanSq += gsl_pow_2(meanBin);
    }
    mean /= NBINS;
    meanSq /= NBINS;

    const double error = sqrt( (meanSq-gsl_pow_2(mean)) / (NBINS - 1) ); 

    fprintf(file, "%.16e\t%.16e\t%.1f\t%i\t%s\n", mean, error, beta, n, bounds);
}

int main(int argc, char *argv[])
{
    assert(argc==2);

    FILE *paramFile = fopen(argv[1], "r");
    char paramName[40], paramValue[40];
    while ( fscanf(paramFile, "%s %s", paramName, paramValue) == 2 )
    {
        if      (!strcmp(paramName,"beta")) beta = atof(paramValue);
        else if (!strcmp(paramName,"n")) n = atoi(paramValue);
        else if (!strcmp(paramName,"bounds")) strcpy(bounds,paramValue);
        else if (!strcmp(paramName,"sweeps")) sweeps = atoi(paramValue);
        else if (!strcmp(paramName,"getPlaquette")) getPlaquette = atoi(paramValue);
        else if (!strcmp(paramName,"plaquetteHistory")) strcpy(plaquetteHistory,paramValue);
        else if (!strcmp(paramName,"plaquetteAnalysis")) strcpy(plaquetteAnalysis,paramValue);
    }

    FILE *plaquetteHistoryFile, *plaquetteAnalysisFile;
    if (getPlaquette)
    {
        assert(plaquetteHistoryFile = fopen(plaquetteHistory, "r"));
        if (strcmp(plaquetteAnalysis,"")) assert(plaquetteAnalysisFile = fopen(plaquetteAnalysis, "a"));
        else plaquetteAnalysisFile = stdout;

        Bunching(plaquetteAnalysisFile);

        if (strcmp(plaquetteAnalysis,"")) fclose(plaquetteAnalysisFile);
    }

    return 0;
}

