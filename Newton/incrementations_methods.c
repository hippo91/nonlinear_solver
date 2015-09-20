#include <stdlib.h>
#include <omp.h>

void classical_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments) {
#pragma omp parallel for
	for (int i=0; i<size_of_pb; ++i) {
		vector_of_increments[i] = - func[i] / dfunc[i];
	}
}

void damped_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments) {
	double damping_coeff = 0.5;
#pragma omp parallel for
	for (int i=0; i<size_of_pb; ++i) {
		vector_of_increments[i] = - damping_coeff * func[i] / dfunc[i];
	}
}

void ensure_positivity_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments) {
	double damping_coeff = 0.5;
#pragma omp parallel for
	for (int i=0; i<size_of_pb; ++i) {
		double min_authorized = damping_coeff * x_k[i];
		double optimal_value = - func[i] / dfunc[i];
		if (optimal_value < min_authorized) {
			vector_of_increments[i] = min_authorized;
		} else {
			vector_of_increments[i] = optimal_value;
		}

	}
}
