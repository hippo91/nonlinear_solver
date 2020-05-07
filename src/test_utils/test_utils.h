/**
 * @file test_utils.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief Differents utility functions for unit testing
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "array.h"

/**
 * @brief Defines the precision under which two floating point values are considered equal
 * 
 */
#define EPSILON 1e-15

/**
 * @brief Return true if the value if equal to the reference
 * 		  up to a relative precision
 * 
 * @param[in] value : value to be checked
 * @param[in] reference : the reference value
 * @return true : if value is equal to reference up to a relative precision
 * @return false : otherwise
 */
static bool almost_equal(const double value, const double reference)
{
    return (fabs(value - reference) / fabs(reference)) <= EPSILON;
}

/**
 * @brief Print to the standard error a formatted error message
 * 
 * @param[in] array_name : name of the array 
 * @param[in] index : index of the array from which the value will be printed
 * @param[in] array : array 
 * @param[in] expected : expected value that should be found a the arr[index] place
 */
static void print_array_index_error(const char *array_name, const size_t index, const double *const array, const double expected)
{
    fprintf(stderr, "%s[%zu] = %22.16g instead of %22.16g\n", array_name, index, array[index], expected);
}


/**
 * @brief Check if an array is filled with a unique uniform value.
 * 		  Two values are said to be equal if their relative difference is below 1.e-15.
 * 
 * @param[in] array : array to check
 * @param[in] size : size of the array
 * @param[in] value : value expected to be at every index of the array
 * @param[out] unvalid_index : an array, which size is the same as the array to check, and will hold the indices
 * 						       where the value found in the array is different from the one expected
 * @return true : if all the indices of the array owns the same value
 * @return false : otherwise
 */
static bool is_uniform_array_value(const double *const array, const size_t size, const double value, int *unvalid_index)
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
 * @brief Compares if two arrays are equals and print an error message if not
 * 
 * @param[in] arr_a : first array
 * @param[in] arr_b : second array
 * @param[in] size : size of both array
 * @param[in] arr_a_name : name of first array
 * @return true : if arrays are equals
 * @return false : otherwise
 */
bool assert_equal_arrays(const double *const arr_a, const double *const arr_b, const size_t size,
                         const char *arr_a_name)
{
    bool equality = true;
    for (size_t i = 0; i < size; ++i)
    {
        if (!almost_equal(arr_a[i], arr_b[i]))
        {
            print_array_index_error(arr_a_name, i, arr_a, arr_b[i]);
            equality = false;
        }
    }
    return equality;
}

/**
 * @brief Return True if two arrays are equal up to precision
 * 
 * @param[in] arr_a : first array
 * @param[in] arr_b : second array
 * @return true : if arrays are equals
 * @return false : otherwise
 */
bool assert_equal(const p_array arr_a, const p_array arr_b)
{
    if (arr_a->size != arr_b->size) return false;

    return assert_equal_arrays(arr_a->data, arr_b->data, arr_a->size, arr_a->label);
}

/**
 * @brief Compares if two arrays of bool are equals and print an error message if not
 * 
 * @param[in] arr_a : first array
 * @param[in] arr_b : second array
 * @param[in] size : size of both array
 * @param[in] arr_a_name : name of first array
 * @return true : if arrays are equals
 * @return false : otherwise
 */
bool assert_equal_bool_arrays(const bool *const arr_a, const bool *const arr_b, const size_t size,
                              const char *arr_a_name)
{
    bool equality = true;
    for (size_t i = 0; i < size; ++i)
    {
        if (arr_a[i] != arr_b[i])
        {
            fprintf(stderr, "%s[%zu] = %s instead of %s\n", arr_a_name, i, arr_a[i] ? "true" : "false", arr_b[i] ? "true" : "false");
            equality = false;
        }
    }
    return equality;
}


/**
 * @brief Check if an array is filled with a unique uniform value and prints an error message
 * 		  if not.
 * 
 * @param[in] arr : array to check
 * @param[in] size : size of the array
 * @param[in] arr_name : name of the array
 * @param[in] expected : value expected to fill the array
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
 * @brief Check if an array is filled with a unique uniform value and prints an error message
 * 		  if not.
 * 
 * @param[in] arr : array to check
 * @param[in] expected : value expected to fill the array
 * @return true : if the array is uniformely filled with the expected value
 * @return false : otherwise
 */
bool check_uniform_value(const p_array arr, const double expected) {
    return check_uniform_array_value(arr->data, arr->size, arr->label, expected);
}

#endif