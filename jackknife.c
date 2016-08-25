#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct dataListElementStruct
{
    double data;
    struct dataListElementStruct *next;
} dataListElementType;
typedef dataListElementType *dataListType;

int main(int argc, char *argv[])
{
    if (argc != 2) {printf("**** ERROR, you should insert one filename\n"); return 1;}

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {printf("**** ERROR opening the file\n"); return 1;}

    dataListType dataList = NULL, aux;
    double data, dataSum = 0.;
    long int count = 0;

    while (fscanf(file,"%lf", &data) != -1)
    {
        dataSum += data;
        count++;

        aux = malloc(sizeof(dataListElementType));
        aux->data = data;
        aux->next = dataList;
        dataList = aux;
    }

    fclose(file);

    const double mean = dataSum / (double)count;

    printf("\n**** Computing mean and his error with Jackknife method ****\n\n");
    printf("**** Mean: %.16e\n", mean);

    double meanJack = 0.;
    double varJack = 0.;
    int i;
    for (i=0; i<count; i++)
    {
        meanJack = (dataSum - dataList->data) / (double)(count - 1);
        varJack += pow( (meanJack)-mean , 2 );
        meanJack = 0.;

        aux = dataList;
        dataList = dataList->next;
        free(aux);
    }

    varJack *= (double)(count-1) / count;

    printf("  samples |     error\n");
    printf("%9li | %.3e\n", count, sqrt(varJack) );

    return 0;
}

