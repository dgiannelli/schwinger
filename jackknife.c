#include "jackknife.h"

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

