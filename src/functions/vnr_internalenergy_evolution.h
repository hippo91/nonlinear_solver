#include <math.h>
#include <stdlib.h>
#include "array.h"
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif

typedef struct VnrVariables
{
    const p_array specific_volume_old;
    const p_array specific_volume_new;
    const p_array internal_energy_old;
    const p_array pressure;
    MieGruneisenParameters_s *miegruneisen;
} VnrVariables_s;

void internal_energy_evolution_VNR(void *variables, const p_array newton_var,
                                   p_array func, p_array dfunc);
