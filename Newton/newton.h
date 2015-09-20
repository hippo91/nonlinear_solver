#include <stdlib.h>
#include <stdbool.h>

typedef void ComputeFandDerivative(void *, double *, size_t , double *, double *);

typedef void IncrementVector(double *, double *, double *, size_t, double *);

typedef void CheckCriterion(double *, double *, double *, size_t, double *, bool *);

typedef struct NewtonParameters NewtonParameters_t;

struct NewtonParameters {
    ComputeFandDerivative *evaluate_the_function;
    IncrementVector *increment_the_vector;
    CheckCriterion *check_convergence;
};

void solveNewton(NewtonParameters_t *, void *, double *, size_t, double *);
