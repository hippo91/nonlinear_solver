#ifndef ARRAY_H
#define ARRAY_H
#include <stdbool.h>

#define MAX_LABEL_SIZE 128
#define MAX_ARRAY_SIZE 1000000000
#define PRINT_ARRAY_CHUNK_SIZE 10

/**
 * @brief This MACRO creates an array which name is the same as the pointer pointing to it.
 * 
 */
#define BUILD_ARRAY(name, size) p_array name = build_array(size, #name);

/**
 * @brief This MACRO clears the array in argument and frees the memory allocated.
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
    unsigned int size;
    char label[128];
    double *data;
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

#endif