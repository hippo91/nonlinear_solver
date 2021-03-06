#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#ifndef NEWTON
#include "newton.h"
#define NEWTON
#endif

int solveNewton(NewtonParameters_s *newton_parameters, void *func_parameters, p_array x_ini, p_array x_sol)
{
    if (x_ini->size != x_sol->size) {
        fprintf(stderr, "Size mismatch between array x_ini (%s with size %u) and x_sol (%s with size %u)\n",
                x_ini->label, x_ini->size, x_sol->label, x_sol->size);
        return EXIT_FAILURE;
    }

    int iter = 0;
    const int NB_ITER_MAX = 40;
    const unsigned int pb_size = x_ini->size;

    // Array of the values of the function to vanish
    BUILD_ARRAY(F_k, pb_size)
    // Array of the values of the derivative of the function to vanish
    BUILD_ARRAY(dF_k, pb_size)
    // Array of the values of incrementation
    BUILD_ARRAY(delta_x_k, pb_size)

    p_array built_arrays[] = {F_k, dF_k, delta_x_k};
    const size_t nb_arrays = sizeof(built_arrays) / sizeof(p_array);

    // Array of convergence markers
    bool *has_converged = (bool *)calloc(pb_size, sizeof(bool));

    // Check that every array building has been successfull
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE || has_converged == NULL)
    {
        fprintf(stderr, "Error during allocation/creation of arrays!\n");
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    // Initialization
    enum e_solver_status {SUCCESS, FAILURE} solver_status = SUCCESS;
    p_array x_k = x_sol;
    if (copy_array(x_ini, x_k) == EXIT_FAILURE) {
        fprintf(stderr, "Unable to initialize the Newton-Raphson solver!\n");
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    while (true)
    {
        // Compute F and dF
        newton_parameters->evaluate_the_function(func_parameters, x_k, F_k, dF_k);
        // Compute delta_x
        newton_parameters->compute_increment_vector(x_k, F_k, dF_k, delta_x_k);
        // Apply increments
        for (unsigned int i = 0; i < pb_size; ++i)
        {
            if (!has_converged[i])
            {
                x_k->data[i] += delta_x_k->data[i];
            }
        }
        // Check the convergence
        if (newton_parameters->check_convergence(delta_x_k, F_k, has_converged))
        {
            solver_status = SUCCESS;
            break;
        }

        if (iter == NB_ITER_MAX) {
            solver_status = FAILURE;
            break;
        }

        ++iter;
    }

    // Clear the memory
    cleanup_memory(built_arrays, nb_arrays);
    free(has_converged);

    if (solver_status == FAILURE)
    {
        fprintf(stderr, "Maximum iterations number reached (%d)!\n", NB_ITER_MAX);
        fprintf(stderr, "Newton-Raphson algorithm has not converged!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
