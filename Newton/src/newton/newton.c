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
// #define DEBUG

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

#define SAFE_BUILD_ARRAY(name, size) BUILD_ARRAY(name, size) \
                                     if (!is_valid_array(name)) { \
                                        fprintf(stderr, "Error when building the array " #name "!\n"); \
                                        return EXIT_FAILURE; \
                                     }

/**
 * @brief Use Newton-Raphson to solve a function
 * 
 * @param Newton : parameters of the Newton-Raphson algorithm
 * @param func_variables : parameters of the function to solve
 * @param x_ini : initial values of the unknown
 * @param x_sol : solution
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
    SAFE_BUILD_ARRAY(x_k, pb_size)
    // Array of unknwon at k+1 iteration
    SAFE_BUILD_ARRAY(x_k_pun, pb_size)
    // Array of the values of the function to vanish
    SAFE_BUILD_ARRAY(F_k, pb_size)
    // Array of the values of the derivative of the function to vanish
    SAFE_BUILD_ARRAY(dF_k, pb_size)
    // Array of the values of incrementation
    SAFE_BUILD_ARRAY(delta_x_k, pb_size)
    // Array of convergence markers
    bool *has_converged;
    allocVecBoolForOMP(pb_size, &has_converged);

    copy_array(x_ini, x_k);

    while ((!allConverged(has_converged, pb_size)) && (iter++ < NB_ITER_MAX))
    {
#ifdef DEBUG
        printf("<--- ITERATION : %d --->\n", iter);
#endif
        // Compute F and dF
        Newton->evaluate_the_function(func_variables, x_k->data, pb_size, F_k->data, dF_k->data);
        // Compute delta_x
        Newton->increment_the_vector(x_k->data, F_k->data, dF_k->data, pb_size, delta_x_k->data);
        // Check the convergence and apply increments
        Newton->check_convergence(x_k->data, delta_x_k->data, F_k->data, pb_size, x_k_pun->data, has_converged);

#ifdef DEBUG
        print_array(F_k);
        print_array(dF_k);
        print_array(x_k);
        print_array(delta_x_k);
        print_array(x_k_pun);
        for (size_t i = 0; i < pb_size; ++i)
            printf("has_converged[%zu] = %s\n", i, has_converged[i] ? "True" : "False");
#endif

        copy_array(x_k_pun, x_k);
    }
    /*
	 * FIN DU NEWTON
	 */
    if (iter < NB_ITER_MAX)
    {
        copy_array(x_k_pun, x_sol);
#ifdef DEBUG
        printf("Convergence acquired afer %d iterations!\n", iter);
        print_array(x_k);
#endif
    }
    else
    {
        fprintf(stderr, "Maximum iterations number reached (%d)!\n", NB_ITER_MAX);
        fprintf(stderr, "Newton-Raphson algorithm has not converged!\n");
        return EXIT_FAILURE;
    }
    /*
	 * Libération de la mémoire
	 */
    DELETE_ARRAY(x_k)
    DELETE_ARRAY(x_k_pun)
    DELETE_ARRAY(F_k)
    DELETE_ARRAY(dF_k)
    DELETE_ARRAY(delta_x_k)
    free(has_converged);
    
    return EXIT_SUCCESS;
}
