#include <stdbool.h>
#include <stdlib.h>
#include "array.h"
#include "incrementations_methods.h"
#include "stop_criterions.h"

/**
 * @brief This structure holds the parameters of the Newton solver
 * 
 */
typedef struct NewtonParameters
{
    void (*evaluate_the_function)(void *, const p_array, p_array, p_array); /**< Function to vanish */
    incrementation_fct_ptr compute_increment_vector;  /**< Function that compute the Newton increment */
    criterion_fct_ptr check_convergence;  /**< Function that determines the convergence */
} NewtonParameters_s;

/**
 * @brief Launch the Newton-Raphson algorithm
 * 
 * @param newton_parameters : parameters of the Newton-Raphson algorithm
 * @param func_parameters : parameters of the function to solve
 * @param x_ini : initial values of the unknown
 * @param x_sol : solution
 * @warning : the solution is modified in any cases, even in case of FAILURE!
 * 
 * @return EXIT_SUCCESS (0) in case of success
 *         EXIT_FAILURE (1) otherwise
 */
int solveNewton(NewtonParameters_s *newton_parameters, void *func_parameters, p_array x_ini, p_array x_sol);
