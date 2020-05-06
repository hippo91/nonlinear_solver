set( PYTHON_MODULE_NAME "vnr_internal_energy" CACHE STRING "Name of the python module to generate" )

# The library created/used by python should follow a specific pattern which will be set by setup.py
# If not generated via setup.py doesn't take care
set( PYTHON_LIBRARY_NAME "_${PYTHON_MODULE_NAME}.so" CACHE STRING "Name of the library that will be used by setup.py" )

# Need SWIG
find_package( SWIG REQUIRED )

# Try to find Python3 Interpreter and Development with the find_package command.
# In case of failure to find Development target, use the interpreter to locate python3
# include directory. This situation occure currently on Travis Xenial VM. It uses CMake 3.12.4.
function( search_python3_include_dir )
    message( STATUS "Python3 development artifacts cannot be found!" )
    message( STATUS "Asking the interpreter..." )
    # Taken from https://martinopilia.com/posts/2018/09/15/building-python-extension.html
    # This comes to hand if we also need to use the NumPy C API
    execute_process( COMMAND "${Python3_EXECUTABLE}" -c "from sysconfig import get_paths;print(get_paths()['include'])" 
                        RESULT_VARIABLE PYTHON3_INCLUDE_DIRS_FOUND
                        OUTPUT_VARIABLE Python3_INCLUDE_DIRS
                        ERROR_VARIABLE PYTHON3_INCLUDE_DIRS_ERROR
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_STRIP_TRAILING_WHITESPACE
                    )
    if ( NOT ${PYTHON3_INCLUDE_DIRS_FOUND} EQUAL 0 )
        message( FATAL_ERROR "Manual search of the python3 include directory has failed!" )
    endif()
    set( Python3_INCLUDE_DIRS ${Python3_INCLUDE_DIRS} SCOPE_PARENT )
endfunction() 

# Need Python
find_package( Python3 COMPONENTS Interpreter Development )
if( NOT Python3_Interpreter_FOUND )
    # Stop the process if the interpreter is not found
    message( FATAL_ERROR "Python3 interpreter cannot be found!" ) 
endif()
if( NOT Python3_Development_FOUND )
    search_python3_include_dir()
    message( STATUS "Found Python3 Include Directory : ${Python3_INCLUDE_DIRS}" )
endif()

# Taken from https://martinopilia.com/posts/2018/09/15/building-python-extension.html
# This comes to hand if we also need to use the NumPy C API
execute_process( COMMAND "${Python3_EXECUTABLE}" -c "import numpy; print(numpy.get_include())" 
                 RESULT_VARIABLE NUMPY_FOUND
                 OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
                 ERROR_VARIABLE NUMPY_ERROR
                 OUTPUT_STRIP_TRAILING_WHITESPACE
                 ERROR_STRIP_TRAILING_WHITESPACE
                )
if( NOT ${NUMPY_FOUND} EQUAL 0 )
    message( WARNING "The command that should find Numpy headers exited with error : ")
    message( WARNING "${NUMPY_ERROR}" )
    message( FATAL_ERROR "Numpy headers cannot be found. " )
else()
    message( STATUS "Found Numpy: ${NUMPY_INCLUDE_DIR}" )
    # In fact we don't care about NUMPY_INCLUDE_DIR variable.
    # The only dependency to numpy is the numpy.i file which is
    # bundlded in the project.
endif()

include( UseSWIG )
# Override the module name found in the swig source (.i) file
# https://gitlab.kitware.com/cmake/cmake/issues/18374
list( APPEND CMAKE_SWIG_FLAGS "-module;${PYTHON_MODULE_NAME}" )
set_property( SOURCE launch_vnr_resolution.i PROPERTY USE_TARGET_INCLUDE_DIRECTORIES TRUE )

if( CMAKE_VERSION VERSION_LESS 3.13 )
    set( UseSWIG_TARGET_NAME_PREFERENCE STANDARD )
    # request file.i file to use library include directories.
    # This way the swig_link_libraries will allow the .i file to include the headers of the linked lib
    # See https://gitlab.kitware.com/cmake/cmake/issues/18003
    message( STATUS "The property USE_LIBRARY_INCLUDE_DIRECTORIES is set to TRUE" )
    set_property( SOURCE launch_vnr_resolution.i PROPERTY USE_LIBRARY_INCLUDE_DIRECTORIES TRUE )
endif()

swig_add_library( ${PYTHON_MODULE_NAME}
    TYPE SHARED
    LANGUAGE Python
    SOURCES launch_vnr_resolution.i
    OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}"
    OUTFILE_DIR "${CMAKE_CURRENT_LIST_DIR}"
)

target_include_directories( ${PYTHON_MODULE_NAME}
    PRIVATE
        ${Python3_INCLUDE_DIRS}
)

target_link_libraries( ${PYTHON_MODULE_NAME}
    PRIVATE
        launch_vnr_resolution
        array
        newton
        functions
        incrementation
        criterions
        eos
)

if( CMAKE_VERSION VERSION_LESS 3.13 )
    message( STATUS "The property SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES is set to TRUE" )
    set_property( TARGET ${PYTHON_MODULE_NAME} PROPERTY SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES TRUE )
    get_property( LAUNCH_VNR_RESOLUTION_INCLUDE_DIR TARGET launch_vnr_resolution PROPERTY INCLUDE_DIRECTORIES )
    message( STATUS "The property SWIG_INCLUDE_DIRECTORIES is set to ${LAUNCH_VNR_RESOLUTION_INCLUDE_DIR}" )
    set_property( TARGET ${PYTHON_MODULE_NAME} PROPERTY SWIG_INCLUDE_DIRECTORIES "${LAUNCH_VNR_RESOLUTION_INCLUDE_DIR}" )
endif()

# This command is usefull only when cmake is called via setup.py.
# For example the library generated by swig is _vnr_internal_energy.so.
# This command will copy this library into the src/launch_vnr_resolution_c directory
# where the python wrapper vnr_internal_energy.py has been also generated.
# This package is then copied/installed by pip during the installation process.
# Last step of the command is to rename the generated library into a python/pip 
# compliant name where the suffix '.cpython-37m-x86_64-linux-gnu.so' will be added
add_custom_command( TARGET ${PYTHON_MODULE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "_${PYTHON_MODULE_NAME}.so" "${CMAKE_CURRENT_LIST_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E rename "_${PYTHON_MODULE_NAME}.so" "${PYTHON_LIBRARY_NAME}" 
    WORKING_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    COMMENT
"Working inside ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
copy _${PYTHON_MODULE_NAME}.so to ${CMAKE_CURRENT_LIST_DIR}/
rename _${PYTHON_MODULE_NAME}.so to ${PYTHON_LIBRARY_NAME}" 
)