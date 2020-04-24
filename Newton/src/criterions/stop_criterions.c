
#include "stop_criterions.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"


/**
 * @brief Check the convergence of the function and update the value of x vector according to 
 * 		  the incrementation vector.
 * 		  The convergence is obtained if the value of the function is sufficiently close to zero
 * 
 * @param x_k : current Newton unknown vector
 * @param delta_x_k : Newton's incrementation vector
 * @param func : vector of the function's values
 * @param x_k_pun : new Newton unknown vector
 * @param has_converged : vector of boolean indicating the convergence 
 * @return int : EXIT_SUCCESS (0) in case of success
 *               EXIT_FAILURE (1) otherwise
 */
int relative_gap(p_array x_k, p_array delta_x_k, p_array func, p_array x_k_pun, bool *has_converged)
{
    if (!is_valid_array(x_k) || !is_valid_array(delta_x_k) || !is_valid_array(func) ||
        !is_valid_array(x_k_pun))
    {
        fprintf(stderr, "One of the array is invalid! Cannot proceed!\n");
        return EXIT_FAILURE;
    }

    if ((x_k->size != delta_x_k->size) || (x_k->size != func->size) || (x_k->size != x_k_pun->size)) {
        fprintf(stderr, "The arrays should have all the same size!\n");
        fprintf(stderr, "Array : %s -> size = %u\n", x_k->label, x_k->size);
        fprintf(stderr, "Array : %s -> size = %u\n", delta_x_k->label, delta_x_k->size);
        fprintf(stderr, "Array : %s -> size = %u\n", func->label, func->size);
        fprintf(stderr, "Array : %s -> size = %u\n", x_k_pun->label, x_k_pun->size);
        return EXIT_FAILURE;
    }

    const double epsilon = 1.0e-08;
    const double precision = 1.0e-09;
    for (size_t i = 0; i < x_k->size; ++i)
    {
        if (!has_converged[i])
        {
            x_k_pun->data[i] = x_k->data[i] + delta_x_k->data[i];
        }
        if (fabs(func->data[i]) < epsilon * fabs(delta_x_k->data[i]) + precision)
        {
            // CONVERGENCE
            has_converged[i] = true;
        }
    }

    return EXIT_SUCCESS;
}
