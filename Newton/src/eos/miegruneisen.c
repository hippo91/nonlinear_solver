#include "miegruneisen.h"
#include <math.h>
#include <omp.h>
#include <signal.h>
#include <stdio.h>

void solveVolumeEnergyVec(MieGruneisenParameters_t *params, const int nb_cells,
                          const double *specific_volume,
                          const double *internal_energy, double *pressure,
                          double *gamma_per_vol, double *c_son)
{
    // phi : pression sur l hugoniot
    // einth : energie interne specifique sur l hugoniot
    // dpdv : dp/dv
    // Carré de la vitesse du son initiale
    double c_zero_2 = params->c_zero * params->c_zero;
    // Dérivee du coefficient de gruneisen
    double dgam = params->rho_zero * (params->gamma_zero - params->coeff_b);
    double rho_zero = params->rho_zero;
    double gamma_zero = params->gamma_zero;
    double coeff_b = params->coeff_b;
    double s1 = params->s1;
    double s2 = params->s2;
    double s3 = params->s3;
    double e_zero = params->e_zero;
    for (int i = 0; i < nb_cells; ++i)
    {
        double epsv = 0;
        double epsv2 = 0;
        double redond_a = 0;
        double denom = 0;
        double phi = 0;
        double dpdv = 0;
        double dphi = 0;
        double einth = 0;
        double deinth = 0;
        double vson_2 = 0;
        epsv = 1.0 - rho_zero * specific_volume[i];
        epsv2 = epsv * epsv;
        // Coefficient de gruneisen
        gamma_per_vol[i] =
            (gamma_zero * (1.0 - epsv) + coeff_b * epsv) / specific_volume[i];
        redond_a = s1 + 2. * s2 * epsv + 3. * s3 * epsv2;
        if (epsv > 0)
        {
            // ============================================================
            // si epsv > 0, la pression depend de einth et phi.
            // denom : racine du denominateur de phi
            // dphi : derivee de ph par rapport a
            // deinth : derivee de einth par rapport a v
            // ============================================================
            denom = 1. / (1. - (s1 + s2 * epsv + s3 * epsv2) * epsv);
            phi = rho_zero * c_zero_2 * epsv * denom * denom;
            einth = e_zero + phi * epsv / (2. * rho_zero);
            //
            dphi = phi * rho_zero * (-1. / epsv - 2. * redond_a * denom);
            //
            deinth = phi * (-1. - epsv * redond_a * denom);
            //
            dpdv = dphi +
                   (dgam - gamma_per_vol[i]) * (internal_energy[i] - einth) /
                       specific_volume[i] -
                   gamma_per_vol[i] * deinth;
        }
        else
        {
            // ============================================================
            // traitement en tension : epsv < 0
            // la pression depend d une fonction de v : phi
            // et
            // de e0 appelee einth
            // ============================================================
            phi = rho_zero * c_zero_2 * epsv / (1. - epsv);
            // einth ---> e0
            einth = e_zero;
            //
            dphi = -c_zero_2 / (specific_volume[i] * specific_volume[i]);
            //
            dpdv = dphi + (dgam - gamma_per_vol[i]) * (internal_energy[i] - einth) /
                              specific_volume[i];
        }
        // ****************************
        // Pression :
        // ****************************
        pressure[i] = phi + gamma_per_vol[i] * (internal_energy[i] - einth);
        // ======================================
        // Carre de la vitesse du son :
        // ======================================
        vson_2 = specific_volume[i] * specific_volume[i] *
                 (pressure[i] * gamma_per_vol[i] - dpdv);
        if (vson_2 < 0)
        {
            printf("Carré de la vitesse du son < 0\n");
            printf("specific_volume[%d] = %15.9g\n", i, specific_volume[i]);
            printf("pressure[%d] = %15.9g\n", i, pressure[i]);
            printf("dpsurde[%d] = %15.9g\n", i, gamma_per_vol[i]);
            printf("dpdv[%d] = %15.9g\n", i, dpdv);
            raise(SIGABRT);
        }
        c_son[i] = sqrt(vson_2);
        //
        if ((c_son[i] <= 0.) && (c_son[i] < 10000.))
        {
            c_son[i] = 0.;
        }
    }
}

/**
 * @brief Compute the compression ($\dfrac{\rho - \rho_0}{\rho}$)
 * 
 * @param rho_zero : initial density
 * @param specific_volume : specific volume
 * @return double : the compression
 */
inline double compute_epsv(const double rho_zero, const double specific_volume)
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
inline double compute_dp_de(const double gamma_zero, const double coeff_b,
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
inline double compute_denom(const double s1, const double s2, const double s3,
                            const double epsv)
{
    return 1. / (1. - (s1 + s2 * epsv + s3 * epsv * epsv) * epsv);
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
        double epsv = compute_epsv(rho_zero, specific_volume[i]);
        // Gruneisen coefficient divided by the specific volume : dp/de
        gamma_per_vol[i] = compute_dp_de(gamma_zero, coeff_b, epsv, specific_volume[i]);
        if (epsv > 0)
        {
            const double denom = compute_denom(s1, s2, s3, epsv);
            phi = rho_zero * c_zero_2 * epsv * denom * denom;
            einth = e_zero + phi * epsv / (2. * rho_zero);
        }
        else
        {
            phi = rho_zero * c_zero_2 * epsv / (1. - epsv);
            einth = e_zero;
        }
        pressure[i] = phi + gamma_per_vol[i] * (internal_energy[i] - einth);
    }
}