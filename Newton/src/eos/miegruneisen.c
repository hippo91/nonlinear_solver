#include "miegruneisen.h"
#include <math.h>
#include <signal.h>
#include <stdio.h>

/**
 * @brief Compute the compression ($\dfrac{\rho - \rho_0}{\rho}$)
 * 
 * @param rho_zero : initial density
 * @param specific_volume : specific volume
 * @return double : the compression
 */
static inline double compute_epsv(const double rho_zero, const double specific_volume)
{
    return 1.0 - rho_zero * specific_volume;
}

/**
 * @brief Compute the Gruneisen coefficent divided by specific volume i.e the
 *        derivative of the pressure with respect to the specific internal energy
 * 
 * @param gamma_zero : initial Gruneisen coefficient
 * @param coeff_b : b coefficient
 * @param epsv : compression of the material
 * @param specific_volume : specific volume
 * @return double : dp/de
 */
static inline double compute_dp_de(const double gamma_zero, const double coeff_b,
                            const double epsv, const double specific_volume)
{
    return (gamma_zero * (1.0 - epsv) + coeff_b * epsv) / specific_volume;
}

/**
 * @brief Computes the square root of the denominator of the pressure on the hugoniot
 * 
 * @param s1 : s1 parameter
 * @param s2 : s2 parameter
 * @param s3 : s3 parameter
 * @param epsv : compression of the material
 * @return double : the denominator
 */
static inline double compute_denom(const double s1, const double s2, const double s3,
                            const double epsv)
{
    return 1. / (1. - (s1 + s2 * epsv + s3 * epsv * epsv) * epsv);
}

int init(MieGruneisenParameters_s *params, const unsigned int nb_cells, const double * const specific_volume)
{
    if (params->phi == NULL)
    {
        params->phi = (double *)calloc(nb_cells, sizeof(double));
        if (params->phi == NULL)
        {
            fprintf(stderr, "Error during allocation of params->phi array!\n");
            return EXIT_FAILURE;
        }
    }
    if (params->dphi == NULL)
    {
        params->dphi = (double *)calloc(nb_cells, sizeof(double));
        if (params->dphi == NULL)
        {
            fprintf(stderr, "Error during allocation of params->dphi array!\n");
            return EXIT_FAILURE;
        }
    }
    if (params->einth == NULL)
    {
        params->einth = (double *)calloc(nb_cells, sizeof(double));
        if (params->einth == NULL)
        {
            fprintf(stderr, "Error during allocation of params->einth array!\n");
            return EXIT_FAILURE;
        }
    }
    if (params->deinth == NULL)
    {
        params->deinth = (double *)calloc(nb_cells, sizeof(double));
        if (params->deinth == NULL)
        {
            fprintf(stderr, "Error during allocation of params->deinth array!\n");
            return EXIT_FAILURE;
        }
    }
    if (params->gamma_per_vol == NULL)
    {
        params->gamma_per_vol = (double *)calloc(nb_cells, sizeof(double));
        if (params->gamma_per_vol == NULL)
        {
            fprintf(stderr, "Error during allocation of params->gamma_per_vol array!\n");
            return EXIT_FAILURE;
        }
    }

    const double s1 = params->s1;
    const double s2 = params->s2;
    const double s3 = params->s3;
    const double rho_zero = params->rho_zero;
    const double gamma_zero = params->gamma_zero;
    const double coeff_b = params->coeff_b;
    const double e_zero = params->e_zero;
    const double c_zero_2 = params->c_zero * params->c_zero;
    for (unsigned int i = 0; i < nb_cells; ++i)
    {
        const double epsv = compute_epsv(rho_zero, specific_volume[i]);
        params->gamma_per_vol[i] = compute_dp_de(gamma_zero, coeff_b, epsv, specific_volume[i]);
        if (epsv > 0)
        {
            const double denom = compute_denom(s1, s2, s3, epsv);
            params->phi[i] = rho_zero * c_zero_2 * epsv * denom * denom;
            params->einth[i] = e_zero + params->phi[i] * epsv / (2. * rho_zero);
            const double redond_a = s1 + 2. * s2 * epsv + 3. * s3 * epsv * epsv;
            params->dphi[i] = params->phi[i] * rho_zero * (-1. / epsv - 2. * redond_a * denom);
            params->deinth[i] = params->phi[i] * (-1. - epsv * redond_a * denom);
        }
        else
        {
            params->phi[i] = rho_zero * c_zero_2 * epsv / (1. - epsv);
            params->einth[i] = e_zero;
            params->dphi[i] = -c_zero_2 / (specific_volume[i] * specific_volume[i]);
        }
    }
    return EXIT_SUCCESS;
}

void finalize(MieGruneisenParameters_s *params)
{
    free(params->gamma_per_vol);
    free(params->phi);
    free(params->dphi);
    free(params->einth);
    free(params->deinth);
}

/**
 * @brief Compute the pressure and the derivative of the pressure with respect to the specific
 *        internal energy
 * 
 * @param params : parameters of the equation of state
 * @param nb_cells : size of the arrays
 * @param specific_volume : specific volume array
 * @param internal_energy : internal energy array
 * @param pressure : pressure array
 * @param gamma_per_vol : dp/de array
 */
void compute_pressure_and_derivative(MieGruneisenParameters_s *params, const int nb_cells,
                                     __attribute__((unused)) const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol)
{
    for (int i = 0; i < nb_cells; ++i)
    {
        gamma_per_vol[i] = params->gamma_per_vol[i];
        pressure[i] = params->phi[i] + params->gamma_per_vol[i] * (internal_energy[i] - params->einth[i]);
    }
}

/**
 * @brief Compute the pressure and the sound speed
 * 
 * @param params : parameters of the equation of state
 * @param nb_cells : size of the arrays
 * @param specific_volume : specific volume array
 * @param internal_energy : internal energy array
 * @param pressure : pressure array
 * @param c_son : sound speed array
 * @todo : factorizes the comutation of einth and phi to share with compute_pressure_and_derivative
 */
void compute_pressure_and_sound_speed(MieGruneisenParameters_s *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son)
{
    const double dgam = params->rho_zero * (params->gamma_zero - params->coeff_b);
    for (int i = 0; i < nb_cells; ++i)
    {
        pressure[i] = params->phi[i] + params->gamma_per_vol[i] * (internal_energy[i] - params->einth[i]);
        double dpdv = params->dphi[i] + (dgam - params->gamma_per_vol[i]) * (internal_energy[i] - params->einth[i]) / specific_volume[i] - params->gamma_per_vol[i] * params->deinth[i];
        double vson_2 = specific_volume[i] * specific_volume[i] * (pressure[i] * params->gamma_per_vol[i] - dpdv);
        if (vson_2 < 0)
        {
            printf("Carré de la vitesse du son < 0\n");
            printf("specific_volume[%d] = %15.9g\n", i, specific_volume[i]);
            printf("pressure[%d] = %15.9g\n", i, pressure[i]);
            printf("dpsurde[%d] = %15.9g\n", i, params->gamma_per_vol[i]);
            printf("dpdv[%d] = %15.9g\n", i, dpdv);
            raise(SIGABRT);
        }
        c_son[i] = sqrt(vson_2);
    }
}