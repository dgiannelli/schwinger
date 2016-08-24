#include <gsl/gsl_rng.h>

#include "random.h"

gsl_rng *r;

void RndInit()
{
    r = gsl_rng_alloc(gsl_rng_default);
}

void RndFinalize()
{
    gsl_rng_free(r);
}

double RndUniform()
{
    return gsl_rng_uniform();
}
