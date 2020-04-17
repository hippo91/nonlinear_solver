set( PYTHON_MODULE_NAME "vnr_internal_energy" )
# Where to build the module
set( PYTHON_MODULE_DIR "${CMAKE_BINARY_DIR}/python_module")

# Need SWIG
find_package( SWIG )

# Need Python
if ( CMAKE_VERSION VERSION_LESS 3.12 )
    find_package( PythonLibs )
    set( "Python_INCLUDE_DIRS" ${PYTHON_INCLUDE_DIRS} )
else()
    find_package( Python COMPONENTS Development )
endif()

if ( NOT SWIG_FOUND )
    message( STATUS "SWIG is needed to build the python module but cannot be found!" )
    message( STATUS "Skipping the build of python module" )
    return()
endif()

if ( NOT PythonLibs_FOUND AND NOT Python_FOUND )
    message( STATUS "Python is needed to build the python module but cannot be found!" )
    message( STATUS "Skipping the build of python module" )
    return()
endif()

# Override the module name found in the swig source (.i) file
# https://gitlab.kitware.com/cmake/cmake/issues/18374
if( CMAKE_VESION VERSION_LESS "3.18" ) 
    set( CMAKE_SWIG_FLAGS -module ${PYTHON_MODULE_NAME} )
else()
    set_property( SOURCE launch_vnr_resolution.i PROPERTY SWIG_MODULE_NAME ${PYTHON_MODULE_NAME} )
endif()

include( UseSWIG )
swig_add_library( ${PYTHON_MODULE_NAME}
    TYPE SHARED
    LANGUAGE Python
    SOURCES "launch_vnr_resolution.i"
    OUTPUT_DIR "${PYTHON_MODULE_DIR}"
    OUTFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/swig_gen"
)
target_sources( ${PYTHON_MODULE_NAME}
    PRIVATE
        "launch_vnr_resolution.h"
        "launch_vnr_resolution.c"
)
set_target_properties( ${PYTHON_MODULE_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PYTHON_MODULE_DIR}"
)
target_include_directories( ${PYTHON_MODULE_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        ${Python_INCLUDE_DIRS}
)
target_link_libraries( ${PYTHON_MODULE_NAME}
    PRIVATE
        newton
        functions
        incrementation
        criterions
        eos
        utils
)