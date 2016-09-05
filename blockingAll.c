#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_BLOCK 30

typedef struct DataListElementStruct
{
    double data;
    struct DataListElementStruct *next;
} DataListElementType;
typedef DataListElementType *DataListType;

double MeanBlock(DataListType dataList, int block); 

int main(int argc, char *argv[])
{
    if (argc != 2) {printf("**** ERROR, you should insert one filename\n"); return 1;}

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {printf("**** ERROR opening the file\n"); return 1;}

    DataListType dataList=NULL;
    double data, mean = 0., variance = 0.;
    int count = 0;

    while (fscanf(file,"%lf", &data) != -1)
    {
        mean += data;
        variance += pow(data,2);

        DataListType aux = malloc(sizeof(DataListElementType));
        aux->data = data;
        aux->next = dataList;
        dataList = aux;

        count++;
    }

    fclose(file);

    mean /= (double) count;
    variance -= count*pow(mean,2);
    variance /= count - 1;

    printf("\n**** Checking the correlation with blocking method at all block size up to %i ****\n\n", MAX_BLOCK);
    printf("**** Mean: %.16e\n", mean);
    printf("  samples | block |     error\n");
    printf("%9i |     1 | %.3e\n", count, sqrt(variance/count) );

    if (count < MAX_BLOCK) printf("**** ERROR, too few data for blocks of this size\n");

    else 
    {
        for(int block=2; block<=MAX_BLOCK; block++)
        {
            DataListType dataListHead = dataList;
            variance = 0.;
            double meanBlock = 0;
            for (int i=0; i<count; i++)
            {
                meanBlock += dataList->data;
                if (i%block == block-1)
                {
                    meanBlock /= block;
                    variance += pow( meanBlock , 2);
                    meanBlock = 0.;
                }
                dataList = dataList->next;
            }

            variance -= (count/block)*pow(mean,2);
            variance /= (count/block) - 1;
            printf("%9i | %5i | %.3e\n", count/block, block, sqrt(variance/(count/block)) );

            dataList = dataListHead;
        }
    }

    for (int i=0; i<count; i++)
    {
        DataListType aux = dataList;
        dataList = dataList->next;
        free(aux);
    }

    return 0;
}

