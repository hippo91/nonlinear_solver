#include <stdio.h>
#include "array.h"
#include "cubic.h"
#include "incrementations_methods.h"
#include "stop_criterions.h"
#include "newton.h"

int main(__attribute__((unused)) const int argc, __attribute__((unused)) const char* argv[])
{
    // First parametrize the solver
    NewtonParameters_s newton = {cubic_function, damped_incrementation, relative_gap};

    // Create the arrays of initial unknown and solution
    BUILD_ARRAY(x, 3)
    BUILD_ARRAY(sol, 3)
    // It is usefull to store the created arrays in an array
    p_array built_arrays[] = {x, sol};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);

    // Use array helpers to check everything is fine after building
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE)
    {
        // If something wrong then clean the memory and returns
        fprintf(stderr, "An error occured during the building of arrays!\n");
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    // Initialize the vector of unknowns
    x->data[0] = -1;
    x->data[1] = 0.25;
    x->data[2] = 2.;
    
    int status = solveNewton(&newton, NULL, x, sol);

    if (status == EXIT_FAILURE) {
        fprintf(stderr, "An error occured during the solver run!\n");
    } else {
        printf("SUCCESS!\n");
        printf("Initial vector :\n");
        print_array(x);
        printf("Solution vector :\n");
        print_array(sol);
    }

    // Dont forget to clean the memory
    cleanup_memory(built_arrays, nb_arrays);
    return status;
}