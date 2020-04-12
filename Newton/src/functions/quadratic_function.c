/*
 * quadratic_function.c
 *
 *  Created on: 19 sept. 2015
 *      Author: guillaume2
 */
#include <omp.h>
#include <stdlib.h>

void quadratic_function(__attribute__((unused)) void *empty, double *x, size_t size_of_pb, double *func,
                        double *dfunc)
{
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        func[i] = x[i] * x[i] + 2. * x[i] - 9.0;
        dfunc[i] = 2 * x[i] + 2.;
    }
}
