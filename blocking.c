#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_BLOCK 5

int main(int argc, char *argv[])
{
    if (argc != 2) {printf("**** ERROR, you should insert one filename\n"); return 1;}

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {printf("**** ERROR opening the file\n"); return 1;}

    double data1, data2, mean = 0.;
    long int count = 0;


    while (fscanf(file,"%lf", &data1) != -1)
    {
        mean += data1;
        count++;
    }
    mean /= (double) count;
    printf("\n**** Checking the correlation with blocking method up to block of size %i ****\n\n", MAX_BLOCK-1);
    printf("**** Mean: %.10e\n", mean);

    if (fseek(file, 0, SEEK_SET)) {printf("**** ERROR seeking to start of file\n"); return 1;}

    double *dataArray = malloc((count/2)*sizeof(double));
    double variance=0.;
    int i;
    for (i=0; i<count/2; i++)
    {
        fscanf(file,"%lf%lf", &data1, &data2);
        variance += pow(data1,2) + pow(data2,2);
        dataArray[i] = 0.5 * (data1 + data2);
    }

    if (count%2 == 1)
    {
        fscanf(file,"%lf", &data1);
        variance += pow(data1,2);
    }

    variance -= count*pow(mean,2);
    variance /= count - 1;

    printf("samples | block |       error\n");
    printf("%7i |     1 | %.5e\n", count, sqrt(variance/count) );

    int block, countNew;
    for(block=2; block<MAX_BLOCK; block++)
    {
        variance = 0.;
        count /= 2;
        countNew = 0;
        for (i=0; i<count; i++)
        {
            variance += pow(dataArray[i],2);
            if (i%2 == 1 && block<MAX_BLOCK-1)
            {
                dataArray[countNew] = 0.5 * (dataArray[i] + dataArray[i-1]);
                countNew++;
            }
        }
        variance -= count*pow(mean,2);
        variance /= count - 1;
        printf("%7i | %5i | %.5e\n", count, block, sqrt(variance/count) );
    }


    free(dataArray);

    return 0;
}


