#include <stdio.h> #include <stdlib.h>
#include <strings.h>
#include <assert.h>

#define THERMRATIO 0.2
#define NBINS 20

double beta;
int n;
int sweeps;
int getPlaquette = 0;
char plaquetteHistory[40];
char plaquetteAnalysis[40];

int main(int argc, char *argv[])
{
    assert(argc==2);


    FILE *paramFile = fopen(argv[1], "r");
    char paramName[40], paramValue[40];
    while ( fscanf(paramFile, "%s %s", &paramName, &paramValue) == 2 )
    {
        if (!strcmp(paramName,"beta")) beta = atof(paramValue);
        else if (!strcmp(paramName,"n")) n = atoi(paramValue);
        else if (!strcmp(paramName,"sweeps")) sweeps = atoi(paramValue);
        else if (!strcmp(paramName,"getPlaquette")) getPlaquette = atoi(paramValue);
        else if (!strcmp(paramName,"plaquetteHistory")) strcpy(plaquetteHistory,paramValue);
        else if (!strcmp(paramName,"plaquetteAnalysis")) strcpy(plaquetteAnalysis,paramValue);
    }

    FILE *plaquetteHistoryFile, plaquetteAnalysisFile;
    if (getPlaquette)
    {
        assert(plaquetteHistoryFile = fopen(plaquetteHistory, "r"));
        if (!plaquetteAnalysis) assert(plaquetteAnalysisFile = fopen(plaquetteAnalysis, "w"));
        else plaquetteAnalysisFile = stdout;
    }

    const int therm = sweeps*THERMRATIO;
    const int size = sweeps - therm;
    const int binSize = ( size + NBINS - 1 ) / NBINS;
    for (int i=0; i<therm; i++) fscanf(plaquetteHistoryFile, "%*lf");
    for (int bin=0; i<NBINS; i++)
    {
        for(int i=0; i<binSize && fscanf(


