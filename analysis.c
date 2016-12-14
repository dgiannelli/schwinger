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
int getPlaquette;
int getChargeSq;
int getChargeEvenOdd;
char plaquetteHistory[40];
char chargeSqHistory[40];
char chargeEvenOddHistory[40];
char plaquetteAnalysis[40] = "";
char chargeSqAnalysis[40] = "";
char chargeEvenOddAnalysis[40] = "";

void Bunching(double *mean, double *error, double *data, int size)
{
    assert(size%NBINS==0);
    const int binSize = size / NBINS;

    double meanBin = 0.0;
    double sqMeanBin = 0.0;
    for (int bin=0; bin<NBINS; bin++)
    {
        double temp = 0.0;
        for (int i=0; i<binSize; i++)
        {
            temp += data[i+bin*binSize];
        }
        temp /= binSize;
        meanBin += temp;
        sqMeanBin += gsl_pow_2(temp);
    }
    meanBin /= NBINS;
    sqMeanBin /= NBINS;

    *mean = meanBin;
    *error = sqrt( (sqMeanBin-gsl_pow_2(meanBin)) / (NBINS-1) );
}
        

double Tau(double *data, int size)
{
    assert(size%NBINS==0);
    const int binSize = size / NBINS;

    double mean = 0.0;
    double sqMean = 0.0;
    double meanBin = 0.0;
    double sqMeanBin = 0.0;
    for (int bin=0; bin<NBINS; bin++)
    {
        double tempBin = 0.0;
        for (int i=0; i<binSize; i++)
        {
            const double temp = data[i+bin*binSize];
            tempBin += temp;
            mean += temp;
            sqMean += gsl_pow_2(temp);
        }
        tempBin /= binSize;
        meanBin += tempBin;
        sqMeanBin += gsl_pow_2(tempBin);
    }
    mean /= size;
    sqMean /= size;
    meanBin /= NBINS;
    sqMeanBin /= NBINS;

    const double sigmaSq = ( sqMean - gsl_pow_2(mean) ) / ( size - 1 );
    const double errorSq = ( sqMeanBin - gsl_pow_2(meanBin) ) / ( NBINS - 1);

    return errorSq/sigmaSq;
}

void Jackknife(double *mean, double *error, double (*F)(double *data, int size), double *data, int size)
{
    assert(size%NBINS==0);
    const int binSize = size / NBINS;
    
    const int dataJackSize = size - binSize;

    double *dataJack;
    double meanJack = 0.0;
    double sqMeanJack = 0.0;
    for (int bin=0; bin<NBINS; bin++)
    {
        dataJack = malloc(dataJackSize*sizeof(double));
        int j=0;
        for (int i=0; i<bin*binSize; i++)
        {
            dataJack[j++] = data[i];
        }
        for (int i=(bin+1)*binSize; i<size; i++)
        {
            dataJack[j++] = data[i];
        }
        const double temp = F(dataJack, dataJackSize);
        meanJack += temp;
        sqMeanJack += gsl_pow_2(temp);
        
        free(dataJack);
    }
    meanJack /= NBINS;
    sqMeanJack /= NBINS;

    //*mean = NBINS*F(data,size) - (NBINS-1)*meanJack;
    *mean = meanJack;
    *error = sqrt( (NBINS-1) * (sqMeanJack-gsl_pow_2(meanJack)) );
}

void Analysis(FILE *output, double *data, int size)
{
    double mean, error, tau, dtau;

    Bunching(&mean, &error, data, size);
    Jackknife(&tau, &dtau, Tau, data, size);

    fprintf(output, "%.1f\t%i\t%s\t%.16e\t%.16e\t%.16e\t%.16e\n", beta, n, bounds, mean, error, tau, dtau);
}

int main(int argc, char *argv[])
{
    assert(argc == 2);
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

    const int therm = sweeps * THERMRATIO;
    const int binSize = (sweeps-therm) / NBINS;
    const int size = binSize * NBINS;

    if (getPlaquette)
    {
        FILE *plaquetteFile, *plaquetteAnalysisFile;
        assert( plaquetteFile = fopen(plaquetteHistory, "r") );
        double *dataPlaquette = malloc(size*sizeof(double));
        for (int i=0; i<therm; i++) assert( fscanf(plaquetteFile, "%*f") == 0 );
        for (int i=0; i<size; i++) assert( fscanf(plaquetteFile, "%lf", dataPlaquette+i) == 1 );
        fclose(plaquetteFile);

        if (strcmp(plaquetteAnalysis,"")) assert(plaquetteAnalysisFile = fopen(plaquetteAnalysis, "a"));
        else plaquetteAnalysisFile = stdout;

        Analysis(plaquetteAnalysisFile, dataPlaquette, size);

        if (strcmp(plaquetteAnalysis,"")) fclose(plaquetteAnalysisFile);
        free(dataPlaquette);
    }

    if (getChargeSq) 
    {
        FILE *chargeSqFile, *chargeSqAnalysisFile;
        assert( chargeSqFile = fopen(chargeSqHistory, "r") );
        double *dataChargeSq = malloc(size*sizeof(double));
        for (int i=0; i<therm; i++) assert( fscanf(chargeSqFile, "%*f") == 0 );
        for (int i=0; i<size; i++) 
        {
            double temp;
            assert( fscanf(chargeSqFile, "%lf", &temp) == 1 );
            dataChargeSq[i] = gsl_pow_2(temp);
        }
        fclose(chargeSqFile);

        if (strcmp(chargeSqAnalysis,"")) assert(chargeSqAnalysisFile = fopen(chargeSqAnalysis, "a"));
        else chargeSqAnalysisFile = stdout;

        Analysis(chargeSqAnalysisFile, dataChargeSq, size);

        if (strcmp(chargeSqAnalysis,"")) fclose(chargeSqAnalysisFile);
        free(dataChargeSq);
    }

    if (getChargeEvenOdd) 
    {
        FILE *chargeEvenOddFile, *chargeEvenOddAnalysisFile;
        assert( chargeEvenOddFile = fopen(chargeEvenOddHistory, "r") );
        double *dataChargeEvenOdd = malloc(size*sizeof(double));
        for (int i=0; i<therm; i++) assert( fscanf(chargeEvenOddFile, "%*f") == 0 );
        for (int i=0; i<size; i++) assert( fscanf(chargeEvenOddFile, "%lf", dataChargeEvenOdd+i) == 1 );
        fclose(chargeEvenOddFile);

        if (strcmp(chargeEvenOddAnalysis,"")) assert(chargeEvenOddAnalysisFile = fopen(chargeEvenOddAnalysis, "a"));
        else chargeEvenOddAnalysisFile = stdout;

        Analysis(chargeEvenOddAnalysisFile, dataChargeEvenOdd, size);

        if (strcmp(chargeEvenOddAnalysis,"")) fclose(chargeEvenOddAnalysisFile);
        free(dataChargeEvenOdd);
    }

    return 0;
}

