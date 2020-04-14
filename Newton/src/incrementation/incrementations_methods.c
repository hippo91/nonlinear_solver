#include <stdlib.h>

void classical_incrementation(__attribute__((unused)) double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments)
{
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        vector_of_increments[i] = -func[i] / dfunc[i];
    }
}

void damped_incrementation(__attribute__((unused)) double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments)
{
    double damping_coeff = 0.5;
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        vector_of_increments[i] = -damping_coeff * func[i] / dfunc[i];
    }
}

void ensure_same_sign_incrementation(double *x_k, double *func, double *dfunc, size_t size_of_pb, double *vector_of_increments)
{
    for (size_t i = 0; i < size_of_pb; ++i)
    {
        double min_authorized = 0.;
        double optimal_value = -func[i] / dfunc[i];
        double target = x_k[i] + optimal_value;
        if (target * x_k[i] < 0)
        {
            vector_of_increments[i] = (min_authorized - x_k[i]) * 0.5;
        }
        else
        {
            vector_of_increments[i] = optimal_value;
        }
    }
}
