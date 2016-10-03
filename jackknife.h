#ifndef JACKKNIFE_H
#define JACKKNIFE_H

#include <stdio.h>

void ReadFile(FILE *file, double *data, int size, int therm);

void Jackknife(double (*fJack)(int jStart, int jEnd, double *data, int size), \
               double *data, int size, int jSets, double *jMean, double *jVar);

double JackMean(int jStart, int jEnd, double *data, int size);

double JackSquareMean(int jStart, int jEnd, double *data, int size);

double JackEvenOdd(int jStart, int jEnd, double *data, int size);

#endif
