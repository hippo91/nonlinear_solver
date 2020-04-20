#ifndef ARRAY_H
#define ARRAY_H
#include <stdbool.h>

#define MAX_LABEL_SIZE 128


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
 * @param size : size of the array
 * @param label : label of the array
 * @param arr : the array
 * @return EXIT_SUCCESS (0) : in case of success
 * @return EXIT_FAILURE (1) : otherwise
 */
int build_array(const unsigned int size, const char * label, p_array arr); 

#endif