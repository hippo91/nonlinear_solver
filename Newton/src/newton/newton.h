#include <stdbool.h>
#include <stdlib.h>

typedef struct NewtonParameters
{
    void (*evaluate_the_function)(void *, double *, size_t, double *, double *);
    void (*increment_the_vector)(double *, double *, double *, size_t, double *);
    void (*check_convergence)(double *, double *, double *, size_t, double *, bool *);
} NewtonParameters_s;

void solveNewton(NewtonParameters_s *, void *, double *, size_t, double *);
