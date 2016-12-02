/*
 * Main program for generating field configurations and measuring observables
 * Reads the parameters in the file specified as argv
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "random.h"
#include "lattice.h"

int main(int argc, char *argv[])
{
    assert(argc == 2);
    RndInit();

    {
        FILE *fileTorus = fopen("data/inftyTorus.dat", "w"); assert(fileTorus);
        FILE *fileMoeb = fopen("data/inftyMoeb.dat", "w"); assert(fileMoeb);

        puts("\n**** Fixed beta = 1 and different values of N: ****\n");

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


    {
        FILE *fileTorus = fopen("data/contTorus.dat", "w"); assert(fileTorus);
        FILE *fileMoeb = fopen("data/contMoeb.dat", "w"); assert(fileTorus);

        puts("\n**** Different values of beta,N at constant physics: ****\n");

        for (int i=0; i<6; i++)
        {
            NewLattice(betas[i], ns[i], "torus", "charge");
            GetMeasures(data, i<3 && i>4 ? ITERS : ITERS*10);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileTorus, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));

            if (i==0)
            {
                FILE *fileSmallBeta = fopen("data/smallBeta.dat", "w"); assert(fileSmallBeta);
                for (int j=0; j<1000; j++) fprintf(fileSmallBeta, "%.0f\n", data[j]);
                fclose(fileSmallBeta);
            }
            if (i==3)
            {
                FILE *fileHighBeta = fopen("data/highBeta.dat", "w"); assert(fileHighBeta);
                for (int j=0; j<1000; j++) fprintf(fileHighBeta, "%.0f\n", data[j]);
                fclose(fileHighBeta);
            }

            NewLattice(betas[i], ns[i], "moebius", "charge");
            GetMeasures(data, ITERS);
            DeleteLattice();
            Jackknife(fJack, data, ITERS, JSETS, &jMean, &jVar);
            fprintf(fileMoeb, "%.1f\t%i\t%.16e\t%.16e\n", betas[i], ns[i], jMean, sqrt(jVar));

            if (i==3)
            {
                FILE *fileMoebEvo = fopen("data/moebEvo.dat", "w"); assert(fileMoebEvo);
                for (int j=0; j<1000; j++) fprintf(fileMoebEvo, "%.0f\n", data[j]);
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
