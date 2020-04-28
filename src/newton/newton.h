#include <stdbool.h>
#include <stdlib.h>
#include "array.h"
#include "incrementations_methods.h"
#include "stop_criterions.h"

typedef struct NewtonParameters
{
    void (*evaluate_the_function)(void *, const p_array, p_array, p_array);
    incrementation_fct_ptr compute_increment_vector;
    criterion_fct_ptr check_convergence;
} NewtonParameters_s;

int solveNewton(NewtonParameters_s *, void *, p_array, p_array);
