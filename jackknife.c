#include "jackknife.h"

void Jackknife(double (*fJack)(int jStart, int jEnd, double *data, int size), \
               double *data, int size, int jSets, double *jMean, double *jVar)
{
    for (int i=0; i<jSets; i++)
        int jStart = i * (size/jSets);
    

