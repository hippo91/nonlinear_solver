#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"
#ifndef NEWTON
#include "newton.h"
#define NEWTON
#endif

/*
 * allConverged :
 * 	retourne true si tous les membres du tableau has_converged sont à true
 */
static bool allConverged(bool *has_converged, size_t pb_size)
{
    for (size_t i = 0; i < pb_size; ++i)
    {
        if (!has_converged[i])
        {
            return (false);
        }
    }
    return (true);
}

/**
 * @brief Cleanup the memory by deleting the arrays
 * 
 * @param built_arrays : pointers to the arrays that have been built
 * @param nb_arrays : number of arrays
 */
static void cleanup_memory(p_array* built_arrays, const size_t nb_arrays) 
{
    for (unsigned int j = 0; j < nb_arrays; ++j) {
        DELETE_ARRAY(built_arrays[j]);
    }
}


/**
 * @brief Use Newton-Raphson to solve a function
 * 
 * @param Newton : parameters of the Newton-Raphson algorithm
 * @param func_variables : parameters of the function to solve
 * @param x_ini : initial values of the unknown
 * @param x_sol : solution
 * @warning : the solution is modified in any cases, even in case of ERROR or FAILURE!
 * 
 * @return EXIT_SUCCESS (0) in case of success
 *         EXIT_FAILURE (1) otherwise
 */
int solveNewton(NewtonParameters_s *Newton, void *func_variables, p_array x_ini, p_array x_sol)
{
    if (x_ini->size != x_sol->size) {
        fprintf(stderr, "Size mismatch between array x_ini (%s with size %u) and x_sol (%s with size %u)\n",
                x_ini->label, x_ini->size, x_sol->label, x_sol->size);
        return EXIT_FAILURE;
    }

    int iter = 0;
    const int NB_ITER_MAX = 40;
    const unsigned int pb_size = x_ini->size;

    // Array of unknwon at k iteration
    BUILD_ARRAY(x_k, pb_size)
    // Array of the values of the function to vanish
    BUILD_ARRAY(F_k, pb_size)
    // Array of the values of the derivative of the function to vanish
    BUILD_ARRAY(dF_k, pb_size)
    // Array of the values of incrementation
    BUILD_ARRAY(delta_x_k, pb_size)

    // Check that every array building has been successfull
    p_array built_arrays[4] = {x_k, F_k, dF_k, delta_x_k};
    const size_t nb_arrays = sizeof(built_arrays) / sizeof(p_array);
    for (unsigned int i = 0; i < nb_arrays; ++i) {
        if (built_arrays[i] == NULL) {
            fprintf(stderr, "Error when building the %d th array!\n", i);
            cleanup_memory(built_arrays, nb_arrays);
            return EXIT_FAILURE;
        }
    }

    // Initialization
    enum e_solver_status {SUCCESS, FAILURE, ERROR} solver_status = SUCCESS;
    if (copy_array(x_ini, x_k) == EXIT_FAILURE) {
        fprintf(stderr, "Unable to initialize the Newton-Raphson solver!\n");
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }
    // Array of unknwon at k+1 iteration
    p_array x_k_pun = x_sol;
    // Array of convergence markers
    bool *has_converged;
    // TODO: check successfull creation of has_converged array
    allocVecBoolForOMP(pb_size, &has_converged);

    while (true)
    {
        // Compute F and dF
        Newton->evaluate_the_function(func_variables, x_k->data, pb_size, F_k->data, dF_k->data);
        // Compute delta_x
        Newton->increment_the_vector(x_k->data, F_k->data, dF_k->data, pb_size, delta_x_k->data);
        // Check the convergence and apply increments
        Newton->check_convergence(x_k->data, delta_x_k->data, F_k->data, pb_size, x_k_pun->data, has_converged);

        if (allConverged(has_converged, pb_size)) {
            solver_status = SUCCESS;
            break;
        }
        if (iter == NB_ITER_MAX) {
            solver_status = FAILURE;
            break;
        }

        // Update
        if (copy_array(x_k_pun, x_k) == EXIT_FAILURE) {
            solver_status = ERROR;
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
    if (solver_status == ERROR)
    {
        fprintf(stderr, "An error has been encountered during the Newton-Raphson algorithm!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
