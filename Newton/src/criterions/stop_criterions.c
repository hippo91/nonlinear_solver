#include "stop_criterions.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"


/**
 * @brief Check the convergence of the function.
 * 		  The convergence is obtained if the value of the function is sufficiently close to zero.
 * 
 * @param delta_x_k : Newton's incrementation vector
 * @param func : vector of the function's values
 * @param has_converged : vector of boolean indicating the convergence 
 * @return true : if all cells have converged
 * @return false : otherwise
 */
bool relative_gap(p_array delta_x_k, p_array func, bool *has_converged)
{
    assert(is_valid_array(delta_x_k));
    assert(is_valid_array(func));
    assert(delta_x_k->size == func->size);

    const double epsilon = 1.0e-08;
    const double precision = 1.0e-09;
    bool all_converged = true;
    for (size_t i = 0; i < func->size; ++i)
    {
        if (fabs(func->data[i]) < epsilon * fabs(delta_x_k->data[i]) + precision)
        {
            // CONVERGENCE
            has_converged[i] = true;
        }
        else
        {
            all_converged = false;
        }
        
    }

    return all_converged;
}
