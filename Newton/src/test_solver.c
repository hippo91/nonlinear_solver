#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "launch_vnr_resolution.h"

/**
 * @brief Check if an array is filled with a unique uniform value 
 * 
 * @param array : array to check
 * @param size : size of the array
 * @param value : value expected to be at every index of the array
 * @param unvalid_index : an array, which size is the same as the array to check, and will hold the indices
 * 						  where the value found in the array is different from the one expected
 * @return true : if all the indices of the array owns the same value
 * @return false : otherwise
 */
bool is_uniform_array_value(const double* const array, const size_t size, const double value, int* unvalid_index) {
	size_t inv_ind = 0;
	for (size_t i = 0; i < size; ++i) {
		if (array[i] != value) {
			unvalid_index[inv_ind++] = i;
		}
	};
	return inv_ind == 0;
}

/**
 * @brief Print to the standard error a formatted error message
 * 
 * @param array_name : name of the array 
 * @param index : index of the array from which the value will be printed
 * @param array : array 
 * @param expected : expected value that should be found a the arr[index] place
 */
void print_array_index_error(const char* array_name, const size_t index, const double* const array, const double expected) {
	fprintf(stderr, "%s[%zu] = %15.9g instead of %15.9g\n", array_name, index, array[index], expected);
}

/**
 * @brief Check if an array is filled with a unique uniform value and prints an error message
 * 		  if not.
 * 
 * @param arr : array to check
 * @param size : size of the array
 * @param arr_name : name of the array
 * @param expected : value expected to fill the array
 * @return true : if the array is uniformely filled with the expected value
 * @return false : otherwise
 */
bool check_uniform_array_value(const double * const arr, const size_t size,
							   const char* arr_name, const double expected) {
	int invalid_entries[size];
	for (size_t i = 0; i < size; ++i) invalid_entries[i] = -1;

	bool valid = is_uniform_array_value(arr, size, expected, invalid_entries);
	if (!valid) {
		fprintf(stderr, "Wrong values for %s!\n", arr_name);
		size_t _index = 0;
		while(_index < size && invalid_entries[_index] != -1) {
			print_array_index_error(arr_name, invalid_entries[_index],
					 				arr, expected);
			_index++;
		}
	}

	return valid;
}

/**
 * @brief Launch the test of the nonlinear solver
 * 
 * @return int : success (0) or failure (1)
 */
int main() {
	double* solution;
	size_t pb_size = 10;
	double *old_density;
	double *new_density;
	double *pressure;
	double *internal_energy;
	double *new_pressure;
	double *new_cson;
	allocVecForOMP(pb_size, 8230., &old_density);
	allocVecForOMP(pb_size, 9500., &new_density);
	allocVecForOMP(pb_size, 10.e+09, &pressure);
	allocVecForOMP(pb_size, 1.325e+04, &internal_energy);
	allocVecForOMP(pb_size, 0., &solution);
	allocVecForOMP(pb_size, 0., &new_pressure);
	allocVecForOMP(pb_size, 0., &new_cson);

	launch_vnr_resolution(old_density, new_density, pressure, internal_energy, pb_size, solution, new_pressure, new_cson);

	old_density[3] = 8230.0001;
	old_density[5] = 8230.02;
	old_density[7] = 8230.0003;
	new_density[2] = 9204.;
	new_density[6] = 9500.011;

	bool success = true;
	if (!check_uniform_array_value(old_density, pb_size, "old_density", 8230.)) success = false;
	if (!check_uniform_array_value(new_density, pb_size, "new_density", 9500.)) success = false;

	free(solution);
	free(old_density);
	free(new_density);
	free(pressure);
	free(internal_energy);

	if (!success) return (EXIT_FAILURE);
	return EXIT_SUCCESS;
}
