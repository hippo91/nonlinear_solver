/**
 * @file array.h
 * @author guillaume.peillex@gmail.com
 * @brief Simple implementation of an array structure
 * @version 1.0.0
 * @date 2020-04-30
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef ARRAY_H
#define ARRAY_H
#include <stdbool.h>

/**
 * @brief Maximum size of the string describing the array
 * 
 */
#define MAX_LABEL_SIZE 128
/**
 * @brief Maximum size of the array
 * 
 */
#define MAX_ARRAY_SIZE 1000000000
/**
 * @brief Any array with a size above 2 times this size will not be
 *        printed entirely. Only the beginning and the end will be printed.
 * 
 */
#define PRINT_ARRAY_CHUNK_SIZE 10

/**
 * @brief This MACRO creates an array which name is the same as the pointer pointing to it.
 * 
 */
#define BUILD_ARRAY(name, size) p_array name = build_array(size, #name);

/**
 * @brief Clears the array in argument and frees the memory allocated.
 *        This is the recommended way to ends the life of an array
 * 
 */
#define DELETE_ARRAY(arr_ptr) \
    clear_array(arr_ptr);     \
    free(arr_ptr);

/**
 * @brief Defines an array object
 * 
 */
typedef struct array
{
    unsigned int size;  /**< Number of cells in the array */
    char label[128];  /**< A small string naming of describing the array */
    double *data;  /**< The underlying array */
} s_array, *p_array;

/**
 * @brief Build an array and returns a pointer to it.
 *        Once used, the array should be cleared thanks to the function
 *        clear_array and the memory released by freeing the pointer.
 *        For an easy way of deleting an array please use the macro DELETE_ARRAY.
 * 
 * @param[in] size : size of the array
 * @param[in] label : label of the array
 * @return p_array : pointer on the newly created array in case of success, NULL otherwise
 */
p_array build_array(const unsigned int size, const char *label);

/**
 * @brief Clear the array by freeing the data memory, setting the size to zero 
 *        and the label to empty string. The array in itself should be freed by
 *        a call to free(arr). 
 *        For an easier way of deleting an array please use the DELETE_ARRAY macro.
 * 
 * @param arr : array to clear
 */
void clear_array(const p_array arr);

/**
 * @brief Print the array on standard output.
 *        If the array size is above twice PRINT_ARRAY_CHUNK_SIZE
 *        then only the first and last PRINT_ARRAY_CHUNK_SIZE indexes
 *        are printed
 * 
 * @param[in] arr : array to print
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int print_array(const p_array arr);

/**
 * @brief Fill the array with the value
 *
 * @param[in] p_array : array to fill 
 * @param[in] value : value to fill the array with
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int fill_array(const p_array, const double value);

/**
 * @brief Copies origin's data into destination's one
 * 
 * @warning : arrays should have been created and have the same size
 * @param origin : array to be copied
 * @param dest : modified array
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int copy_array(const p_array origin, p_array dest);

/**
 * @brief Check if the array is valid.
 *        An array is valid if:
 *          - its pointer is not NULL;
 *          - its data member is not NULL;
 *          - its size member is not equal to zero.
 * 
 * @param arr : array to check
 * @return true : if the array is valid
 * @return false : otherwise
 */
bool is_valid_array(const p_array arr);


/**
 * @brief Cleanup the memory by deleting the arrays
 * 
 * @param built_arrays : pointers to the arrays that have been built
 * @param nb_arrays : number of arrays
 */
void cleanup_memory(p_array* built_arrays, const unsigned int nb_arrays);


/**
 * @brief Check if all the arrays have been successfully built
 * 
 * @param built_arrays : pointers to the arrays that have been built
 * @param nb_arrays : number of arrays
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
int check_arrays_building(p_array* built_arrays, const unsigned int nb_arrays);


#endif