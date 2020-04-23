#include "array.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_valid_array(const p_array arr)
{
    if (arr == NULL) {
        fprintf(stderr, "The array has not been created! (NULL pointer)\n");
        return false;
    }
    if (arr->data == NULL) {
        fprintf(stderr, "The data member of the array %s is null! (NULL pointer)\n", arr->label);
        return false;
    }
    if (arr->size == 0) {
        fprintf(stderr, "The size of the array %s is nill!\n", arr->label);
        return false;
    }
    return true;
}

p_array build_array(const unsigned int size, const char *label)
{
    // Checks correctness of arguments
    if (strlen(label) >= MAX_LABEL_SIZE)
    {
        fprintf(stderr, "An error has occured when building the array %s\n", label);
        fprintf(stderr, "The label size is above the limit : %d!\n", MAX_LABEL_SIZE);
        return NULL;
    }
    if (size > MAX_ARRAY_SIZE)
    {
        fprintf(stderr, "An error has occured when building the array %s\n", label);
        fprintf(stderr, "The size of array (%u) is above the limit : %d!\n", size, MAX_ARRAY_SIZE);
        return NULL;
    }

    // Allocate memory for the array structure
    p_array arr_ptr = (p_array)malloc(sizeof(s_array));

    // Fill the array structure
    arr_ptr->data = (double *)calloc(size, sizeof(double));

    if (arr_ptr->data == NULL)
    {
        fprintf(stderr, "An error has occured when building the array %s\n", label);
        fprintf(stderr, "The allocation has failed!\n");
        free(arr_ptr);
        return NULL;
    }

    strcpy(arr_ptr->label, label);
    arr_ptr->size = size;
    return arr_ptr;
};

int print_array(const p_array arr)
{
    if (!arr)
    {
        fprintf(stderr, "Cannot print an unexisting array (NULL pointer)!");
        return EXIT_FAILURE;
    }

    if (arr->size == 0)
    {
        printf("%s[] = empty\n", arr->label);
    }

    unsigned int chunk_size = PRINT_ARRAY_CHUNK_SIZE;
    for (unsigned int i = 0; i < arr->size; ++i)
    {
        if ((i == chunk_size) && (arr->size > chunk_size * 2))
        {
            i = arr->size - chunk_size;
            printf("...\n");
        }
        int ret = printf("%s[%d] = %15.9g\n", arr->label, i, *(arr->data + i));
        if (ret < 0)
        {
            perror("An error occured during the print of the array!");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int fill_array(const p_array arr, const double value)
{
    if (!is_valid_array(arr)) {
        fprintf(stderr, "The array is not valid!\n");
        return EXIT_FAILURE;
    }
    for (unsigned int i = 0; i < arr->size; ++i)
        arr->data[i] = value;
    return EXIT_SUCCESS;
}

void clear_array(const p_array arr)
{
    if (arr)
    {
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        strcpy(arr->label, "");
    }
}

int copy_array(const p_array origin, p_array dest)
{
    if (!is_valid_array(origin))
    {
        fprintf(stderr, "The origin array is not valid!\n");
        return EXIT_FAILURE;
    }
    if (!is_valid_array(dest))
    {
        fprintf(stderr, "The destination array is not valid!\n");
        return EXIT_FAILURE;
    }
    if (origin->size != dest->size) {
        fprintf(stderr, "The sizes of the arrays mismatch!\n");
        fprintf(stderr, "Size of origin array (%s) is (%u)\n", origin->label, origin->size);
        fprintf(stderr, "Size of destination array (%s) is (%u)\n", dest->label, dest->size);
        return EXIT_FAILURE;
    }
    for (unsigned int i = 0; i < origin->size; ++i) {
        dest->data[i] = origin->data[i];
    }
    return EXIT_SUCCESS;
}