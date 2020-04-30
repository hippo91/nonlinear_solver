# nonlinear_solver

This code is a collection of libraries, written in `C`, that implement a Newton-Raphson solver.
A Newton-Raphson solver allows to find the solution of an equation of type :

![General equation to solve](/doc/images/fxzero.gif)

While the present solver may be used to solve any kind of one dimensional equation,
it has been developped in order to give a fast computation process of internal energy evolution in the Von Neumann - Richtmyer (VNR) scheme. This scheme is used to solve shock propagation problems as in [https://github.com/hippo91/XVOF].

<!-- The code is parallelized thanks to **OpenMP**. -->

## Requirements

In order to build the code, the following tools are required : 

- *cmake*, version 3.12 or higher; 
- *gcc* or *clang* or any other compiler compatible with *cmake* and compliant with the standard 99 of the `C` language;
- *openmp*
- and eventually *python* (version doesn't matter but 3.7.7 has been tested)

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

## Examples of use

### Solve a simple equation

In this example the solver is used to solve the following equation :

![Cubic function](/doc/images/cubic_function.gif)

First of all, the function should be coded.
Let's create a header and a source file named `cubic.h` and `cubic.c` and put them in the `functions` directory in the `src` directory.

The code in `cubic.h` declares the function prototype :

```C
void cubic_function(void *params, const p_array x, p_array fx, p_array dfx);
```

The first parameter is a pointer to a structure that contains parameters of the function. It is useless here.
The next three parameters are respectively the unknown x (input), the value of the function (output) and the value of the derivative of the function according to x. Those 3 parameters are `p_array`'s that is to say pointers
to a structure describing the array :

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

In order to compile those files, they have to be added to the `target_sources` command in the `CMakeLists.txt` file of the `src/functions` directory:

```CMake
target_sources( ${LIBRARY_NAME} PRIVATE
                "vnr_internalenergy_evolution.h"
                "vnr_internalenergy_evolution.c" 
                "cubic.h"
                "cubic.c"
            )
```

## Run
******
Lancement avec openmp
******
export OMP_NUM_THREADS=4 #--> Nombre de threads
export GOMP_CPU_AFFINITY="0-3" #--> Pinning des threads

