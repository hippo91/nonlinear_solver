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
