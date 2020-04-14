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
    allocVecForOMP(size_of_pb, 0., &pression);
    allocVecForOMP(size_of_pb, 0., &dpsurde);
    // Appel de l'eos
    vars->miegruneisen->get_pressure_and_derivative(vars->miegruneisen, size_of_pb, vars->specific_volume_new,
                                                    newton_var, pression, dpsurde);
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        const double delta_v = vars->specific_volume_new[i] - vars->specific_volume_old[i];
        // Fonction à annuler
        func[i] = newton_var[i] + (pression[i] + vars->pressure[i]) * delta_v * 0.5 - vars->internal_energy_old[i];
        // Dérivée de la fonction à annuler
        dfunc[i] = 1. + dpsurde[i] * delta_v * 0.5;
    }
    free(pression);
    free(dpsurde);
}
