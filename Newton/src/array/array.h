#ifndef ARRAY_H
#define ARRAY_H
#include <stdbool.h>

#define MAX_LABEL_SIZE 128
#define PRINT_ARRAY_CHUNK_SIZE 10


/**
 * @brief Defines an array object
 * 
 */
typedef struct array {
    unsigned int size;
    char label[128];
    double* data;
} s_array, *p_array;

/**
 * @brief Build an array
 * 
 * @param[in] size : size of the array
 * @param[in] label : label of the array
 * @param[in] arr : the array
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int build_array(const unsigned int size, const char * label, p_array arr); 

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

#endif