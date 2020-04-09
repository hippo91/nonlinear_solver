#include <stdlib.h>

void classical_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb,double  *vector_of_increments);

void damped_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb,double  *vector_of_increments);

void ensure_positivity_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb,double  *vector_of_increments);
