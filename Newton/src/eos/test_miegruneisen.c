#include <math.h>
#include <stdbool.h>

#include "miegruneisen.h"
#include "test_utils.h"


int main() {
	double czero = 3940.0, S1 = 1.489, S2 = 0., S3 = 0., rhozero = 8930.0, grunzero = 2.02, b = 0.47, ezero = 0.;
	MieGruneisenParameters_t copper_mat = {czero, S1, S2, S3, rhozero, grunzero, b, ezero, solveVolumeEnergyVec};

    double density[] = {8700., 9200.};
    double specific_volume[] = {1./density[0], 1./density[1]};
    double internal_energy[] = {1.e+4, 1.e+6};
    double pressure[] = {0., 0.};
    double gamma_per_vol[] = {0., 0.};
    double cson[] = {0., 0.};
    solveVolumeEnergyVec(&copper_mat, 2, specific_volume, internal_energy, pressure, gamma_per_vol, cson);

    bool success = true;
    double expected_pressure[] = {-3.391122999999982e+09,  2.248138143555919e+10};
    double expected_gamma[] = {17930.499999999996, 18165.5};
    double expected_cson[] = {3837.312029974254, 4663.450646599814};
    if (!almost_equal(pressure[0], expected_pressure[0])) {
        print_array_index_error("pressure", 0, pressure, expected_pressure[0]);
        success = false;
    }
    if (!almost_equal(pressure[1], expected_pressure[1])) {
        print_array_index_error("pressure", 1, pressure, expected_pressure[1]);
        success = false;
    }
    if (!almost_equal(gamma_per_vol[0], expected_gamma[0])) {
        print_array_index_error("gamma_per_vol", 0, gamma_per_vol, expected_gamma[0]);
        success = false;
    }
    if (!almost_equal(gamma_per_vol[1], expected_gamma[1])) {
        print_array_index_error("gamma_per_vol", 1, gamma_per_vol, expected_gamma[1]);
        success = false;
    }
    if (!almost_equal(cson[0], expected_cson[0])) {
        print_array_index_error("cson", 0, cson, expected_cson[0]);
        success = false;
    }
    if (!almost_equal(cson[1], expected_cson[1])) {
        print_array_index_error("cson", 1, cson, expected_cson[1]);
        success = false;
    }

	if (!success) return (EXIT_FAILURE);
	return EXIT_SUCCESS;
}