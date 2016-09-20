/*
 * Main program for generating field configurations and measuring the topological charge with both
 * torus and moebius boundary conditions in order to get the infinite volume and the continuum limit.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lattice.h"
#include "random.h"
#include "jackknife.h"

#define ITERS 10000
#define JSIZE 20

int main()
{
    RndInit();
    double *data, jMean, jError;
    data = calloc(ITERS, sizeof(double));
	system("if [ ! -d 'data' ]; then mkdir -p data; fi");

    puts("\n**** Fixed beta = 1 and different values of N: ****\n");

    FILE *fileTorus = fopen("data/inftyTorus.dat", "w"); assert(fileTorus);
    FILE *fileMoeb = fopen("data/inftyMoeb.dat", "w"); assert(fileMoeb);

    {
        const double betas[] = {1., 1., 1., 1., 1., 1.};
        const int ns[] = {5, 10, 15, 20, 25, 30};

        for (int i=0; i<6; i++)
        {
            NewLattice(betas[i], ns[i], "torus", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(f, ITERS, JSIZE, &jMean, &jError);
            fprintf(fileTorus, "%.1f\t%02i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, jError);

            NewLattice(betas[i], ns[i], "moebius", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(f, ITERS, JSIZE, &jMean, &jError);
            fprintf(fileMoeb, "%.1f\t%02i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, jError);
        }
    }

    fclose(fileTorus);
    fclose(fileMoeb);

    puts("\n**** Different values of beta,N at constant physics: ****\n");
    
    FILE *fileTorus = fopen("data/cntnmTorus.dat", "w"); assert(fileTorus);
    FILE *fileMoeb = fopen("data/cntnmMoeb.dat", "w"); assert(fileTorus);

    {
        const double betas[] = {0.8, 1.8, 3.2, 5.0, 7.2, 9.8};
        const int ns[] = {8, 12, 16, 20, 24, 28};

        for (int i=0; i<6; i++)
        {
            NewLattice(betas[i], ns[i], "torus", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(f, ITERS, JSIZE, &jMean, &jError);
            fprintf(fileTorus, "%.1f\t%02i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, jError);

            NewLattice(betas[i], ns[i], "moebius", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(f, ITERS, JSIZE, &jMean, &jError);
            fprintf(fileMoeb, "%.1f\t%02i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, jError);
        }
    }

    fclose(fileTorus);
    fclose(fileMoeb);

    RndFinalize();

    return 0;
}
