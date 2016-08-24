#include <stdlib.h>

#include "random.h"

void RndInit()
{
    srand(997);
}

void RndFinalize()
{
}

double RndUniform()
{
    return (double)rand() / (RAND_MAX - 1);
}
