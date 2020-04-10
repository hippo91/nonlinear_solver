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