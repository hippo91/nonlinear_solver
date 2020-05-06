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

int init(MieGruneisenEOS_s *eos, const unsigned int nb_cells, const double * const specific_volume)
{
    if (eos->phi == NULL)
    {
        eos->phi = (double *)calloc(nb_cells, sizeof(double));
        if (eos->phi == NULL)
        {
            fprintf(stderr, "Error during allocation of eos->phi array (size requested : %u)!\n", nb_cells);
            return EXIT_FAILURE;
        }
    }
    if (eos->dphi == NULL)
    {
        eos->dphi = (double *)calloc(nb_cells, sizeof(double));
        if (eos->dphi == NULL)
        {
            fprintf(stderr, "Error during allocation of eos->dphi array (size requested : %u)!\n", nb_cells);
            return EXIT_FAILURE;
        }
    }
    if (eos->einth == NULL)
    {
        eos->einth = (double *)calloc(nb_cells, sizeof(double));
        if (eos->einth == NULL)
        {
            fprintf(stderr, "Error during allocation of eos->einth array (size requested : %u)!\n", nb_cells);
            return EXIT_FAILURE;
        }
    }
    if (eos->deinth == NULL)
    {
        eos->deinth = (double *)calloc(nb_cells, sizeof(double));
        if (eos->deinth == NULL)
        {
            fprintf(stderr, "Error during allocation of eos->deinth array (size requested : %u)!\n", nb_cells);
            return EXIT_FAILURE;
        }
    }
    if (eos->gamma_per_vol == NULL)
    {
        eos->gamma_per_vol = (double *)calloc(nb_cells, sizeof(double));
        if (eos->gamma_per_vol == NULL)
        {
            fprintf(stderr, "Error during allocation of eos->gamma_per_vol array (size requested : %u)!\n", nb_cells);
            return EXIT_FAILURE;
        }
    }

    const double s1 = eos->params->s1;
    const double s2 = eos->params->s2;
    const double s3 = eos->params->s3;
    const double rho_zero = eos->params->rho_zero;
    const double gamma_zero = eos->params->gamma_zero;
    const double coeff_b = eos->params->coeff_b;
    const double e_zero = eos->params->e_zero;
    const double c_zero_2 = eos->params->c_zero * eos->params->c_zero;
    const double rho_czero2 = rho_zero * c_zero_2;
    const double inv_rhozero_x2 = 1. / (2.* rho_zero); 
    for (unsigned int i = 0; i < nb_cells; ++i)
    {
        const double epsv = compute_epsv(rho_zero, specific_volume[i]);
        eos->gamma_per_vol[i] = compute_dp_de(gamma_zero, coeff_b, epsv, specific_volume[i]);
        if (epsv > 0)
        {
            const double denom = compute_denom(s1, s2, s3, epsv);
            const double phi = rho_czero2 * epsv * denom * denom;
            eos->phi[i] = phi;
            eos->einth[i] = e_zero + phi * epsv * inv_rhozero_x2;
            const double redond_a = (s1 + 2. * s2 * epsv + 3. * s3 * epsv * epsv);
            eos->dphi[i] = phi * rho_zero * (-1. / epsv - 2. * redond_a * denom);
            eos->deinth[i] = phi * (-1. - epsv * redond_a * denom);
        }
        else
        {
            eos->phi[i] = rho_czero2 * epsv / (1. - epsv);
            eos->einth[i] = e_zero;
            eos->dphi[i] = -c_zero_2 / (specific_volume[i] * specific_volume[i]);
        }
    }
    return EXIT_SUCCESS;
}

void finalize(MieGruneisenEOS_s *eos)
{
    free(eos->gamma_per_vol);
    free(eos->phi);
    free(eos->dphi);
    free(eos->einth);
    free(eos->deinth);
}

void compute_pressure_and_derivative(MieGruneisenEOS_s *eos, const int nb_cells,
                                     __attribute__((unused)) const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol)
{
    for (int i = 0; i < nb_cells; ++i)
    {
        gamma_per_vol[i] = eos->gamma_per_vol[i];
        pressure[i] = eos->phi[i] + eos->gamma_per_vol[i] * (internal_energy[i] - eos->einth[i]);
    }
}

void compute_pressure_and_sound_speed(MieGruneisenEOS_s *eos, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son)
{
    const double dgam = eos->params->rho_zero * (eos->params->gamma_zero - eos->params->coeff_b);
    for (int i = 0; i < nb_cells; ++i)
    {
        pressure[i] = eos->phi[i] + eos->gamma_per_vol[i] * (internal_energy[i] - eos->einth[i]);
        double dpdv = eos->dphi[i] + (dgam - eos->gamma_per_vol[i]) * (internal_energy[i] - eos->einth[i]) / specific_volume[i] - eos->gamma_per_vol[i] * eos->deinth[i];
        double vson_2 = specific_volume[i] * specific_volume[i] * (pressure[i] * eos->gamma_per_vol[i] - dpdv);
        if (vson_2 < 0)
        {
            printf("Carré de la vitesse du son < 0\n");
            printf("specific_volume[%d] = %15.9g\n", i, specific_volume[i]);
            printf("pressure[%d] = %15.9g\n", i, pressure[i]);
            printf("dpsurde[%d] = %15.9g\n", i, eos->gamma_per_vol[i]);
            printf("dpdv[%d] = %15.9g\n", i, dpdv);
            raise(SIGABRT);
        }
        c_son[i] = sqrt(vson_2);
    }
}