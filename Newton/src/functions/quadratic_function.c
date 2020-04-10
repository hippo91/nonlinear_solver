/*
 * quadratic_function.c
 *
 *  Created on: 19 sept. 2015
 *      Author: guillaume2
 */
#include <stdlib.h>
#include <omp.h>

void quadratic_function(void *empty, double *x, size_t size_of_pb, double *func, double *dfunc) {
	for (int i=0; i<size_of_pb; ++i) {
		func[i] = x[i] * x[i] + 2. * x[i] - 9.0;
		dfunc[i] = 2 * x[i] + 2.;
	}
}

