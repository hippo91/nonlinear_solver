#include <stdlib.h>
#include "array.h"

void classical_incrementation(__attribute__((unused)) const p_array x_k, const  p_array func, const p_array dfunc, p_array vector_of_increments)
{
    for (size_t i = 0; i < func->size; ++i)
    {
        vector_of_increments->data[i] = -func->data[i] / dfunc->data[i];
    }
}

void damped_incrementation(__attribute__((unused)) const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments)
{
    const double damping_coeff = 0.5;
    for (size_t i = 0; i < func->size; ++i)
    {
        vector_of_increments->data[i] = -damping_coeff * func->data[i] / dfunc->data[i];
    }
}

void ensure_same_sign_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments)
{
    for (size_t i = 0; i < func->size; ++i)
    {
        double min_authorized = 0.;
        double optimal_value = -func->data[i] / dfunc->data[i];
        double target = x_k->data[i] + optimal_value;
        if (target * x_k->data[i] < 0)
        {
            vector_of_increments->data[i] = (min_authorized - x_k->data[i]) * 0.5;
        }
        else
        {
            vector_of_increments->data[i] = optimal_value;
        }
    }
}
