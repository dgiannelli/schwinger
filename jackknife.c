#include <assert.h>

#include "jackknife.h"

void ReadFile(FILE *file, double *data, int size, int therm)
{
    double buff;
    int i;
    for (i=0; i<therm; i++) assert(fscanf(file, "%le", &buff)==1);
    for (i=0; i<size-therm; i++) assert(fscanf(file, "%le", data+i)==1);
}

void Jackknife(double (*fJack)(int jStart, int jEnd, double *data, int size), \
               double *data, int size, int jSets, double *jMean, double *jVar)
{
    const int jSetSize = (size+jSets-1)/jSets;
    double mean = 0, var = 0;
    for (int i=0; i<jSets; i++)
    {
        int jStart = i * jSetSize;
        int jEnd = jStart + jSetSize < size ? jStart + jSetSize : size;

        double theta = fJack(jStart, jEnd, data, size);
        mean += theta;
        var += theta*theta;
    }
    mean /= jSets;
    *jVar = (jSets-1) * ( (var/jSets) - mean*mean);
    *jMean = mean;
}

double JackMean(int jStart, int jEnd, double *data, int size)
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

double JackSquareMean(int jStart, int jEnd, double *data, int size)
{
    double var = 0;
    for (int i=0; i<jStart; i++)
    {
        var += data[i]*data[i];
    }
    for (int i=jEnd; i<size; i++)
    {
       var += data[i]*data[i];
    }
    return var/(size+jStart-jEnd);
}

double JackEvenOdd(int jStart, int jEnd, double *data, int size)
{
    int even = 0;
    int odd = 0;

    for (int i=0; i<jStart; i++)
    {
        if (data[i]-0.5<0.) even++;
        else odd++;
    }
    for (int i=jEnd; i<size; i++)
    {
        if (data[i]-0.5<0.) even++;
        else odd++;
    }
    return (double)odd/even;
}

