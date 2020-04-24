#include <stdbool.h>
#include <stdlib.h>
#include "array.h"

typedef struct NewtonParameters
{
    void (*evaluate_the_function)(void *, double *, size_t, double *, double *);
    void (*increment_the_vector)(double *, double *, double *, size_t, double *);
    int (*check_convergence)(p_array, p_array, p_array, p_array, bool *);
} NewtonParameters_s;

int solveNewton(NewtonParameters_s *, void *, p_array, p_array);
