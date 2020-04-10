#include <math.h>
#include <stdio.h>

#define EPSILON 1e-15

/**
 * @brief Return true if the value if equal to the reference
 * 		  up to a relative precision
 * 
 * @param value : value to be checked
 * @param reference : the reference value
 * @return true : if value is equal to reference up to a relative precision
 * @return false : otherwise
 */
bool almost_equal(const double value, const double reference) {
	return (fabs(value - reference) / fabs(reference)) <= EPSILON;
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
	fprintf(stderr, "%s[%zu] = %22.16g instead of %22.16g\n", array_name, index, array[index], expected);
}

/**
 * @brief Compares if two arrays are equals and print an error message if not
 * 
 * @param arr_a : first array
 * @param arr_b : second array
 * @param size : size of both array
 * @param arr_a_name : name of first array
 * @return true : if arrays are equals
 * @return false : otherwise
 */
bool assert_equal_arrays(const double* const arr_a, const double* const arr_b, const size_t size,
						 const char* arr_a_name) {
	bool equality = true;
	for (size_t i = 0; i < size; ++i) {
		if (!almost_equal(arr_a[i], arr_b[i])) {
			print_array_index_error(arr_a_name, i, arr_a, arr_b[i]);
			equality = false;
		}
	}
	return equality;
}