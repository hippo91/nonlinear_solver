#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

void relative_gap(double *x_k, double *delta_x_k, double *func, size_t size_of_pb, double *x_k_pun, bool *has_converged) {
	double epsilon = 1.0e-08;
	double precision = 1.0e-09;
	for (size_t i=0; i<size_of_pb; ++i) {
		if (! has_converged[i]) {
			x_k_pun[i] = x_k[i] + delta_x_k[i];
		}
		if ( fabs(func[i]) < epsilon * fabs(delta_x_k[i]) + precision) {
			// CONVERGENCE
			has_converged[i] = true;
		}
	}
}
