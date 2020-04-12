#include "launch_vnr_resolution.h"
#include "test_utils.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Check if an array is filled with a unique uniform value.
 * 		  Two values are said to be equal if their relative difference is below 1.e-15.
 * 
 * @param array : array to check
 * @param size : size of the array
 * @param value : value expected to be at every index of the array
 * @param unvalid_index : an array, which size is the same as the array to check, and will hold the indices
 * 						  where the value found in the array is different from the one expected
 * @return true : if all the indices of the array owns the same value
 * @return false : otherwise
 */
bool is_uniform_array_value(const double *const array, const size_t size, const double value, int *unvalid_index)
{
    size_t inv_ind = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (!almost_equal(array[i], value))
        {
            unvalid_index[inv_ind++] = i;
        }
    };
    return inv_ind == 0;
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
bool check_uniform_array_value(const double *const arr, const size_t size,
                               const char *arr_name, const double expected)
{
    int invalid_entries[size];
    for (size_t i = 0; i < size; ++i)
        invalid_entries[i] = -1;

    bool valid = is_uniform_array_value(arr, size, expected, invalid_entries);
    if (!valid)
    {
        fprintf(stderr, "Wrong values for %s!\n", arr_name);
        size_t _index = 0;
        while (_index < size && invalid_entries[_index] != -1)
        {
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
int main()
{
    double *solution;
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

    bool success = true;
    if (!check_uniform_array_value(old_density, pb_size, "old_density", 8230.))
        success = false;
    if (!check_uniform_array_value(new_density, pb_size, "new_density", 9500.))
        success = false;
    if (!check_uniform_array_value(pressure, pb_size, "pressure", 10.e+09))
        success = false;
    if (!check_uniform_array_value(internal_energy, pb_size, "internal_energy", 1.325e+04))
        success = false;
    if (!check_uniform_array_value(solution, pb_size, "solution", 200765.8953965593))
        success = false;
    if (!check_uniform_array_value(new_pressure, pb_size, "new_pressure", 13088079183.59054))
        success = false;
    if (!check_uniform_array_value(new_cson, pb_size, "new_cson", 4503.84710590959))
        success = false;

    free(solution);
    free(old_density);
    free(new_density);
    free(pressure);
    free(internal_energy);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
