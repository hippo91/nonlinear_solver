#include <stdbool.h>

#include "miegruneisen.h"
#include "test_utils.h"

#define PB_SIZE 2


/**
 * @brief Launch the test of the MieGruneisen equation of state 
 * 
 * @return int : success (0) or failure (1)
 * @todo : adds test for non zero values of S2, S3 and ezero
 */
int main() {
	double czero = 3940.0, S1 = 1.489, S2 = 0., S3 = 0., rhozero = 8930.0, grunzero = 2.02, b = 0.47, ezero = 0.;
	MieGruneisenParameters_t copper_mat = {czero, S1, S2, S3, rhozero, grunzero, b, ezero, solveVolumeEnergyVec};

    double density[PB_SIZE] = {8700., 9200.};
    double specific_volume[PB_SIZE] = {1./density[0], 1./density[1]};
    double internal_energy[PB_SIZE] = {1.e+4, 1.e+6};
    double pressure[PB_SIZE] = {0., 0.};
    double gamma_per_vol[PB_SIZE] = {0., 0.};
    double cson[PB_SIZE] = {0., 0.};
    solveVolumeEnergyVec(&copper_mat, PB_SIZE, specific_volume, internal_energy, pressure, gamma_per_vol, cson);

    bool success = true;
    double expected_pressure[] = {-3.391122999999982e+09,  2.248138143555919e+10};
    double expected_gamma[] = {17930.499999999996, 18165.5};
    double expected_cson[] = {3837.312029974254, 4663.450646599814};

    if (!assert_equal_arrays(pressure, expected_pressure, PB_SIZE, "pressure")) success = false;
    if (!assert_equal_arrays(gamma_per_vol, expected_gamma, PB_SIZE, "gamma_per_vol")) success = false;
    if (!assert_equal_arrays(cson, expected_cson, PB_SIZE, "cson")) success = false;

	if (!success) return (EXIT_FAILURE);
	return EXIT_SUCCESS;
}