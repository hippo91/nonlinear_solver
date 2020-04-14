#include <math.h>
#include <stdlib.h>
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif

typedef struct VnrVariables VnrVariables_t;

struct VnrVariables
{
    double *specific_volume_old;
    double *specific_volume_new;
    double *internal_energy_old;
    double *pressure;
    MieGruneisenParameters_t *miegruneisen;
};

void internal_energy_evolution_VNR(void *variables, double *newton_var,
                                   size_t size_of_pb, double *func,
                                   double *dfunc);
