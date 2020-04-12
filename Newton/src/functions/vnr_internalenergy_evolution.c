#include "vnr_internalenergy_evolution.h"

#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #define DEBUG

void internal_energy_evolution_VNR(void *variables, double *newton_var,
                                   size_t size_of_pb, double *func,
                                   double *dfunc)
{
    VnrVariables_t *vars = (VnrVariables_t *)variables;
    double *pression = NULL;
    double *dpsurde = NULL;
    double *dummy = NULL;
    double *specific_volume = NULL;
    allocVecForOMP(size_of_pb, 0., &pression);
    allocVecForOMP(size_of_pb, 0., &dpsurde);
    allocVecForOMP(size_of_pb, 0., &dummy);
    allocVecForOMP(size_of_pb, 0., &specific_volume);
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        specific_volume[i] = 1. / vars->density_new[i];
#ifdef DEBUG
        printf("specific_volume[%zu] = %15.9g | density_new[%zu] = %15.9g\n", i,
               specific_volume[i], i, vars->density_new[i]);
#endif
    }
    // Appel de l'eos
    vars->miegruneisen->get_pressure_and_derivative(vars->miegruneisen, size_of_pb, specific_volume,
                                                    newton_var, pression, dpsurde);
    double delta_v = 0.;
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        delta_v = 1. / vars->density_new[i] - 1. / vars->density_old[i];
        // Fonction à annuler
        func[i] = newton_var[i] + pression[i] * delta_v / 2. +
                  vars->pressure[i] * delta_v / 2. - vars->internal_energy_old[i];
        // Dérivée de la fonction à annuler
        dfunc[i] = 1. + dpsurde[i] * delta_v / 2.;
    }
    free(pression);
    free(dpsurde);
    free(dummy);
    free(specific_volume);
}
