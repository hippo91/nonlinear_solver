#include "miegruneisen.h"
#include <math.h>
#include <omp.h>
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

/**
 * @brief Compute and returns the pressure in case of compression.
 *        Also computes the value of pressure and internal energy on hugoniot (phi & einth)
 * 
 * @param rho_zero : initial density
 * @param c_zero_2 : square of the initial sound speed
 * @param e_zero : initial internal energy
 * @param denom : square root of the denominator of the pressure on the hugoniot
 * @param epsv : compression of the material
 * @param gamma_per_vol : Gruneisen coefficient divided by the specific volume : dp/de
 * @param internal_energy : internal energy
 * @param phi : pressure on the hugoniot
 * @param einth : internal energy on the hugoniot
 * @return double : pressure
 */
static inline double compute_pressure_compression(const double rho_zero, const double c_zero_2, const double e_zero, const double denom,
                                           const double epsv, const double gamma_per_vol, const double internal_energy,
                                           double *phi, double *einth)
{
    *phi = rho_zero * c_zero_2 * epsv * denom * denom;
    *einth = e_zero + *phi * epsv / (2. * rho_zero);
    return *phi + gamma_per_vol * (internal_energy - *einth);
}

/**
 * @brief Compute and returns the pressure in case of release.
 *        Also computes the value of pressure and internal energy on hugoniot (phi & einth)
 * 
 * @param rho_zero : initial density
 * @param c_zero_2 : square of initial sound speed
 * @param e_zero : initial internal energy
 * @param epsv : compression of the material
 * @param gamma_per_vol : Gruneisen coefficient divided by the specific volume : dp/de
 * @param internal_energy : internal energy
 * @param phi : pressure on the hugoniot
 * @param einth : internal energy on the hugoniot
 * @return double : pressure
 */
static inline double compute_pressure_release(const double rho_zero, const double c_zero_2, const double e_zero,
                                       const double epsv, const double gamma_per_vol, const double internal_energy,
                                       double *phi, double *einth)
{
    *phi = rho_zero * c_zero_2 * epsv / (1. - epsv);
    *einth = e_zero;
    return *phi + gamma_per_vol * (internal_energy - *einth);
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
void compute_pressure_and_derivative(MieGruneisenParameters_t *params, const int nb_cells,
                                     const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol)
{
    const double c_zero_2 = params->c_zero * params->c_zero;
    const double rho_zero = params->rho_zero;
    const double gamma_zero = params->gamma_zero;
    const double coeff_b = params->coeff_b;
    const double s1 = params->s1;
    const double s2 = params->s2;
    const double s3 = params->s3;
    const double e_zero = params->e_zero;
    for (int i = 0; i < nb_cells; ++i)
    {
        // phi : pressure on hugoniot
        double phi = 0;
        // einth : specific internal energy on hugoniot
        double einth = 0;
        // epsv compression of the material
        const double epsv = compute_epsv(rho_zero, specific_volume[i]);
        // Gruneisen coefficient divided by the specific volume : dp/de
        gamma_per_vol[i] = compute_dp_de(gamma_zero, coeff_b, epsv, specific_volume[i]);
        if (epsv > 0)
        {
            const double denom = compute_denom(s1, s2, s3, epsv);
            pressure[i] = compute_pressure_compression(rho_zero, c_zero_2, e_zero, denom, epsv, gamma_per_vol[i], internal_energy[i], &phi, &einth);
        }
        else
        {
            pressure[i] = compute_pressure_release(rho_zero, c_zero_2, e_zero, epsv, gamma_per_vol[i], internal_energy[i], &phi, &einth);
        }
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
void compute_pressure_and_sound_speed(MieGruneisenParameters_t *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son)
{
    const double c_zero_2 = params->c_zero * params->c_zero;
    const double rho_zero = params->rho_zero;
    const double gamma_zero = params->gamma_zero;
    const double coeff_b = params->coeff_b;
    const double s1 = params->s1;
    const double s2 = params->s2;
    const double s3 = params->s3;
    const double e_zero = params->e_zero;
    const double dgam = params->rho_zero * (params->gamma_zero - params->coeff_b);
    for (int i = 0; i < nb_cells; ++i)
    {
        // phi : pressure on hugoniot
        double phi = 0;
        // einth : specific internal energy on hugoniot
        double einth = 0;
        // epsv compression of the material
        const double epsv = compute_epsv(rho_zero, specific_volume[i]);
        // Gruneisen coefficient divided by the specific volume : dp/de
        const double gamma_per_vol = compute_dp_de(gamma_zero, coeff_b, epsv, specific_volume[i]);
        const double redond_a = s1 + 2. * s2 * epsv + 3. * s3 * epsv * epsv;
        double dpdv = 0.;
        if (epsv > 0)
        {
            const double denom = compute_denom(s1, s2, s3, epsv);
            pressure[i] = compute_pressure_compression(rho_zero, c_zero_2, e_zero, denom, epsv, gamma_per_vol, internal_energy[i], &phi, &einth);
            const double dphi = phi * rho_zero * (-1. / epsv - 2. * redond_a * denom);
            const double deinth = phi * (-1. - epsv * redond_a * denom);
            dpdv = dphi + (dgam - gamma_per_vol) * (internal_energy[i] - einth) / specific_volume[i] - gamma_per_vol * deinth;
        }
        else
        {
            pressure[i] = compute_pressure_release(rho_zero, c_zero_2, e_zero, epsv, gamma_per_vol, internal_energy[i], &phi, &einth);
            const double dphi = -c_zero_2 / (specific_volume[i] * specific_volume[i]);
            dpdv = dphi + (dgam - gamma_per_vol) * (internal_energy[i] - einth) / specific_volume[i];
        }
        double vson_2 = specific_volume[i] * specific_volume[i] * (pressure[i] * gamma_per_vol - dpdv);
        if (vson_2 < 0)
        {
            printf("Carré de la vitesse du son < 0\n");
            printf("specific_volume[%d] = %15.9g\n", i, specific_volume[i]);
            printf("pressure[%d] = %15.9g\n", i, pressure[i]);
            printf("dpsurde[%d] = %15.9g\n", i, gamma_per_vol);
            printf("dpdv[%d] = %15.9g\n", i, dpdv);
            raise(SIGABRT);
        }
        c_son[i] = sqrt(vson_2);
    }
}