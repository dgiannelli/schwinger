#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_BLOCK 6

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

    dataListType dataList=NULL, aux;
    double data, dataPrec=0., mean = 0., variance = 0.;
    long int count = 0;

    while (fscanf(file,"%lf", &data) != -1)
    {
        mean += data;
        variance += pow(data,2);

        if (count%2 == 1)
        {
            aux = malloc(sizeof(dataListElementType));
            aux->data = 0.5*(data + dataPrec);
            aux->next = dataList;
            dataList = aux;
        }

        count++;
        dataPrec = data;
    }

    fclose(file);

    mean /= (double) count;
    variance -= count*pow(mean,2);
    variance /= count - 1;

    printf("\n**** Checking the correlation with blocking method up to block of size %i ****\n\n", MAX_BLOCK);
    printf("**** Mean: %.16e\n", mean);
    printf("  samples | block |     error\n");
    printf("%9li |     1 | %.3e\n", count, sqrt(variance/count) );

    if (count < pow(2,MAX_BLOCK))
    {
        printf("**** ERROR, too few data for this number of blocks\n");
    }

    else 
    {
        int block;
        for(block=2; block<=MAX_BLOCK; block++)
        {
            dataListType firstElement = dataList;
            aux = dataList;
            variance = 0.;
            count /= 2;
            int i;
            for (i=0; i<count; i++)
            {
                variance += pow(dataList->data,2);

                if (i%2 == 1 && block<MAX_BLOCK)
                {
                    aux->data = 0.5 * (dataList->data + aux->data);
                    aux->next = dataList->next;
                    free(dataList);
                    dataList = aux->next;
                }
                else
                {
                    aux = dataList;
                    dataList = dataList->next;
                }
            }
            if (count%2 == 1) free(dataList);

            variance -= count*pow(mean,2);
            variance /= count - 1;
            printf("%9li | %5i | %.3e\n", count, block, sqrt(variance/count) );

            dataList = firstElement;
        }
    }

    int i;
    for (i=0; i<count; i++)
    {
        aux = dataList;
        dataList = dataList->next;
        free(aux);
    }

    return 0;
}

