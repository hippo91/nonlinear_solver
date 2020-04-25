#include "vnr_internalenergy_evolution.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"

void internal_energy_evolution_VNR(void *variables, const p_array newton_var,
                                   p_array func, p_array dfunc)
{
    assert(is_valid_array(newton_var));
    assert(is_valid_array(func));
    assert(is_valid_array(dfunc));
    assert(newton_var->size == func->size);
    assert(newton_var->size == dfunc->size);

    VnrVariables_t *vars = (VnrVariables_t *)variables;

    const unsigned int pb_size = newton_var->size;
    double *pression = NULL;
    double *dpsurde = NULL;
    allocVecForOMP(pb_size, 0., &pression);
    allocVecForOMP(pb_size, 0., &dpsurde);
    // Appel de l'eos
    vars->miegruneisen->get_pressure_and_derivative(vars->miegruneisen, pb_size, vars->specific_volume_new->data,
                                                    newton_var->data, pression, dpsurde);
    for (size_t i = 0; i < pb_size; ++i)
    {
        const double delta_v = vars->specific_volume_new->data[i] - vars->specific_volume_old->data[i];
        // Fonction à annuler
        func->data[i] = newton_var->data[i] + (pression[i] + vars->pressure->data[i]) * delta_v * 0.5 - vars->internal_energy_old->data[i];
        // Dérivée de la fonction à annuler
        dfunc->data[i] = 1. + dpsurde[i] * delta_v * 0.5;
    }
    free(pression);
    free(dpsurde);
}
