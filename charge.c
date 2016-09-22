/*
 * Main program for generating field configurations and measuring the topological charge with both
 * torus and moebius boundary conditions in order to get the infinite volume and the continuum limit.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "lattice.h"
#include "random.h"
#include "jackknife.h"

#define ITERS 10000
#define JSETS 20

double fJack(int jStart, int jEnd, double *data, int size)
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

int main()
{
    RndInit();

    double jMean, jVar, *data = calloc(ITERS, sizeof(double));

	system("if [ ! -d 'data' ]; then mkdir -p data; fi");

    puts("\n**** Fixed beta = 1 and different values of N: ****\n");

    {
        const double betas[] = {1., 1., 1., 1., 1., 1.};
        const int ns[] = {5, 10, 15, 20, 25, 30};

        FILE *fileTorus = fopen("data/inftyTorus.dat", "w"); assert(fileTorus);
        FILE *fileMoeb = fopen("data/inftyMoeb.dat", "w"); assert(fileMoeb);

        for (int i=0; i<6; i++)
        {
            NewLattice(betas[i], ns[i], "torus", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileTorus, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));

            NewLattice(betas[i], ns[i], "moebius", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileMoeb, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));
        }

        fclose(fileTorus);
        fclose(fileMoeb);
    }

    puts("\n**** Different values of beta,N at constant physics: ****\n");

    {
        const double betas[] = {0.8, 1.8, 3.2, 5.0, 7.2, 9.8};
        const int ns[] = {8, 12, 16, 20, 24, 28};

        FILE *fileTorus = fopen("data/cntnmTorus.dat", "w"); assert(fileTorus);
        FILE *fileMoeb = fopen("data/cntnmMoeb.dat", "w"); assert(fileTorus);

        for (int i=0; i<6; i++)
        {
            NewLattice(betas[i], ns[i], "torus", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileTorus, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));

            if (i==0)
            {
                FILE *fileSmallBeta = fopen("data/smallBeta.dat", "w"); assert(fileSmallBeta);
                for (int j=0; j<ITERS; j++) fprintf(fileSmallBeta, "%.0f\n", data[j]);
                fclose(fileSmallBeta);
            }
            if (i==4)
            {
                FILE *fileHighBeta = fopen("data/highBeta.dat", "w"); assert(fileHighBeta);
                for (int j=0; j<ITERS; j++) fprintf(fileHighBeta, "%.0f\n", data[j]);
                fclose(fileHighBeta);
            }

            NewLattice(betas[i], ns[i], "moebius", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileMoeb, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));

            if (i==4)
            {
                FILE *fileMoebEvo = fopen("data/moebEvo.dat", "w"); assert(fileMoebEvo);
                for (int j=0; j<ITERS; j++) fprintf(fileMoebEvo, "%.0f\n", data[j]);
                fclose(fileMoebEvo);
            }
        }

        fclose(fileTorus);
        fclose(fileMoeb);
    }

    free(data);
    RndFinalize();

    return 0;
}
