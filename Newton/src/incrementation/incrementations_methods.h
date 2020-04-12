#include <stdlib.h>

void classical_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments);

void damped_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments);

void ensure_same_sign_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments);
