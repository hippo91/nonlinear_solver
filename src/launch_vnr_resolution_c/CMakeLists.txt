set( PYTHON_MODULE_NAME "vnr_internal_energy" CACHE STRING "Name of the python module to generate" )
# The library created/used by python should follow a specific pattern which will be set by setup.py
# If not generated via setup.py doesn't take care
# set( PYTHON_LIBRARY_NAME "_${PYTHON_MODULE_NAME}.so" CACHE STRING "Name of the library that will be used by setup.py" )

# Need SWIG
find_package( SWIG )

# Need Python
find_package( Python COMPONENTS Development Interpreter )

if ( NOT SWIG_FOUND )
    message( "SWIG is needed to build the python module but cannot be found. " 
             "Skipping the build of python module!" )
    return()
endif()

if ( NOT PythonLibs_FOUND AND NOT Python_FOUND )
    message( "Python is needed to build the python module but cannot be found. "
             "Skipping the build of python module!" )
    return()
endif()

# Taken from https://martinopilia.com/posts/2018/09/15/building-python-extension.html
# This comes to hand if we also need to use the NumPy C API
execute_process( COMMAND "${Python_EXECUTABLE}" -c "import numpy; print(numpy.get_include())" 
                 RESULT_VARIABLE NUMPY_FOUND
                 OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
                 ERROR_VARIABLE NUMPY_ERROR
                 OUTPUT_STRIP_TRAILING_WHITESPACE
                 ERROR_STRIP_TRAILING_WHITESPACE
                )
if( NOT ${NUMPY_FOUND} EQUAL 0 )
    message( "Numpy headers cannot be found. " )
    message( "The command that should find Numpy headers exited with error : ")
    message( "${NUMPY_ERROR}" )
    message( "However the python module can (and will) be built " )
    message( "but you would need to install numpy in order to use it!" )
else()
    message( STATUS "Found Numpy: ${NUMPY_INCLUDE_DIR}" )
    # In fact we don't care about NUMPY_INCLUDE_DIR variable.
    # The only dependency to numpy is the numpy.i file which is
    # bundlded in the project.
endif()

include( UseSWIG )
# Override the module name found in the swig source (.i) file
# https://gitlab.kitware.com/cmake/cmake/issues/18374
if( CMAKE_VERSION VERSION_LESS "3.18" ) 
    set( CMAKE_SWIG_FLAGS -module ${PYTHON_MODULE_NAME} )
else()
    set_property( SOURCE launch_vnr_resolution.i PROPERTY SWIG_MODULE_NAME ${PYTHON_MODULE_NAME} )
endif()

set_property( SOURCE launch_vnr_resolution.i PROPERTY USE_TARGET_INCLUDE_DIRECTORIES TRUE )

swig_add_library( ${PYTHON_MODULE_NAME}
    TYPE SHARED
    LANGUAGE Python
    SOURCES launch_vnr_resolution.i
    OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}"
    OUTFILE_DIR "${CMAKE_CURRENT_LIST_DIR}"
)
set_target_properties( ${PYTHON_MODULE_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)
target_include_directories( ${PYTHON_MODULE_NAME}
    PRIVATE
        ${Python_INCLUDE_DIRS}
)

swig_link_libraries( ${PYTHON_MODULE_NAME} PRIVATE launch_vnr_resolution )
target_link_libraries( ${PYTHON_MODULE_NAME}
    PRIVATE
        array
        newton
        functions
        incrementation
        criterions
        eos
)

add_custom_command( TARGET ${PYTHON_MODULE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "_${PYTHON_MODULE_NAME}.so" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PYTHON_LIBRARY_NAME}" 
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Working inside ${CMAKE_CURRENT_LIST_DIR} : copying _${PYTHON_MODULE_NAME}.so to ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PYTHON_LIBRARY_NAME}"
)