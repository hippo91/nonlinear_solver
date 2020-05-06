# nonlinear_solver

This code is a collection of libraries, written in `C`, that implement a Newton-Raphson solver.
A Newton-Raphson solver allows to find the solution of an equation of type :

![General equation to solve](/doc/images/fxzero.gif)

While the present solver may be used to solve any kind of one dimensional equation,
it has been developped in order to give a fast computation process of internal energy evolution in the Von Neumann - Richtmyer (VNR) scheme.
This computation process is available through a *c* library or through a *python* module. Both of them use *OpenMP* to accelerate the computation.
The python module is used for example in the XVOF code [https://github.com/hippo91/XVOF].


## Requirements

In order to build the code, the following tools are required : 

- *cmake*, version 3.12 or higher; 
- *gcc* or *clang* or any other compiler compatible with *cmake* and compliant with the standard 99 of the `C` language;
- optionnalty *openmp*
- and *python* (version doesn't matter but 3.7.7 has been tested)

## Download

Just use git :

    git clone https://github.com/hippo91/nonlinear_solver.git

## Installation

The library may be configured and compiled thanks to `cmake`.

The main options are : 

- `-DCMAKE_BUILD_TYPE=[Release|Debug]` : to select the type of build (release is the default); 
- `-DBUILD_SHARED_LIBS=[ON|OFF]` : to select if the libraries will be static or dynamic (default);
- `-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=Path` : path to the directory where the dynamic libraries will be built;
- `-DBUILD_PYTHON_VNR_MODULE=[ON|OFF]` : whether or not build the python module used to compute the evolution of internal energy in the VNR scheme.

For example to compile a release version with static libraries :

    cmake -DBUILD_SHARED_LIBS=OFF /path/to/the/nonlinear_solver
    make -j 4

Here the 4 stands for using 4 threads during compilation.

If the library has to be compiled under debug mode, with static libraries :

    cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF /path/to/the/nonlinear_solver
    make -j 4

The library comes with units tests that may run using `ctest` after the code has been compiled :

    ctest -j 4

To build the *python* module :

    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_PYTHON_VNR_MODULE=ON /path/to/the/nonlinear_solver

Or even simpler :

    pip install . 

## Code structure

All the source files are stored inside the [src](/src) directory.

The following packages are linked to the Newton-Raphson algorithm :

- [array](/src/array): establishes an array structure and defines some functions to work on;
- [criterions](/src/criterions): holds functions that check if the algorithm has converged;
- [functions](/src/functions): stores the functions that may be solved by the Newton-Raphson algorithm;
- [incrementation](/src/incrementation): stores the functions that compute the Newton-Raphson increment;
- [newton](/src/newton): the Newton-Raphson kernel;

The package [test_utils](/src/test_utils) groups functions that are usefull especially when unit testing the solver.

The remaining packages are dedicated to solve the equation governing the evolution of internal energy in the VNR scheme and to build the corresponding python module :

- [eos](/src/eos): holds the equations of state (i.e function that compute pressure and sound speed according to the density and internal energy)
- [launch_vnr_resolution](/src/eos): orchestrates the resolution of the `vnr_internal_energy` function;
- [launch_vnr_resolution_c](/src/launch_vnr_resolution_c): package that will produce the *python* module, analoguous of the preceeding package.


## Examples of use

### Solve a simple equation

In this example the solver is used to solve the following equation :

![Cubic function](/doc/images/cubic_function.gif)

#### Code the function!

First of all, the function should be coded.
Let's create a header and a source file named [`cubic.h`](src/functions/cubic.h) and [`cubic.c`](src/functions/cubic.c) and put them in the `functions` directory in the `src` directory.

The code in `cubic.h` declares the function prototype :

```C
void cubic_function(void *params, const p_array x, p_array fx, p_array dfx);
```

The first parameter is a pointer to a structure that contains parameters of the function. It is useless here.
The next three parameters are respectively the unknown x (input), the value of the function (output) and the value of the derivative of the function according to x. Those 3 parameters are `p_array`'s that is to say pointers
to a structure describing the array (see [`array.h`](src/array/array.h)) :

```C
struct array
{
    unsigned int size;
    char label[128];
    double *data;
};
```

This structure holds the size of the array, its label (i.e name for example) and the data.

Knowing this, the code inside `cubic.c` file may be written :

```C
void cubic_function(__attribute__((unused)) void *params, const p_array x, p_array fx, p_array dfx)
{
    for (unsigned int i = 0; i < x->size; ++i)
    {
        fx->data[i] = x->data[i] * x->data[i] * x->data[i] - 2. * x->data[i] * x->data[i] + 1;
        dfx->data[i] = 3. * x->data[i] * x->data[i] - 4. * x->data[i];
    }
}
```

The attribute `__attribute__((unused))` is here to declare to the compiler that this parameters is not used. Otherwise the compiler will complain.

In order to compile those files, they have to be added to the `target_sources` command in the [`CMakeLists.txt`](src/functions/CMakeLists.txt) file of the `src/functions` directory:

```CMake
target_sources( ${LIBRARY_NAME} PRIVATE
                "vnr_internalenergy_evolution.h"
                "vnr_internalenergy_evolution.c" 
                "cubic.h"
                "cubic.c"
            )
```

#### Combines the solver and the function

To solve this equation, an executable, named for exemple `solve_cubic` has to be produced.
In order to achieve this another source file, named [`solve_cubic.c`](src/solve_cubic.c) is created inside the `src` directory.

In this source file, a function `main` is defined. It takes no argument and return 0 (`EXIT_SUCCESS`) in case of success and 1 (`EXIT_FAILURE`) otherwise. 

The first line of code in this function sets up the newton-raphson algorithm that will be used :

```C
int main()
{
    // First parametrize the solver
    NewtonParameters_s newton = {cubic_function, classical_incrementation, relative_gap};
    ...
}
```

Here the first member is the function to solve. The second one is a function for incrementing the vector of unknowns during the solver run. There are 3 possibilities for this paramerter (see the [`incrementations_methods.h`](src/incrementation/incrementations_methods.h) header):

- *classical_incrementation* : the vector of increment is computed according to :

    ![Classical incrementation](/doc/images/classical_inc.gif)

- *damped_incrementation* : the vector of increment is computed according to :

    ![Damped incrementation](/doc/images/damped_inc.gif)

- *ensure_same_sign_incrementation* : the formula is a bit too complicated to be exposed here, but this method computes the classical incrementation and maximize it in order to not change the sign of the unknown.

The last member is a function that decides if the convergence is achieved or not. For the moment only one function is coded : *relative_gap*.

After the newton algorithm setup, the usefull arrays are created :

```C
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
```

Here the arrays have a size equal to 3, that is to say, the solver will solve three times the equation for three different initial values of x :

```C
    // Initialize the vector of unknowns
    x->data[0] = -1;
    x->data[1] = 0.25;
    x->data[2] = 2.;
```

Then the solver is called, its status checked and printings are made : 

```C
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
```

The compilation of this file is included in the build process by adding the following lines at the end of the [`CMakeLists.txt`](src/CMakeLists.txt) file :

```CMake
add_executable( solve_cubic solve_cubic.c )
target_link_libraries( solve_cubic 
  PRIVATE 
    array
    functions
    criterions
    incrementation
    newton
)
```

After compilation and execution the results are :

```
SUCCESS!
Initial vector :
x[0] =              -1
x[1] =            0.25
x[2] =               2
Solution vector :
sol[0] =    -0.618033989
sol[1] =      1.61803399
sol[2] =      1.61803399
```

While these results are correct, only two roots of the polynomial are found.
If the incrementation is changed :

```C
    // First parametrize the solver
    NewtonParameters_s newton = {cubic_function, damped_incrementation, relative_gap};
```

then the results are :

```
SUCCESS!
Initial vector :
x[0] =              -1
x[1] =            0.25
x[2] =               2
Solution vector :
sol[0] =    -0.618033989
sol[1] =               1
sol[2] =      1.61803399
```

All three roots are found. The explanation is simple but beyond the scope of this tutorial.

<!-- ## Run
******
Lancement avec openmp
******
export OMP_NUM_THREADS=4 #-> Nombre de threads
export GOMP_CPU_AFFINITY="0-3" #-> Pinning des threads -->

