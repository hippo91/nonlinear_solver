# ---------------------------------------------------------------------------------------
# FUNCTION : search_python3_include_dir
#
# This function uses the interpreter to locate python3 include directory.
#
# This is currently needed on Travis Xenial VM where the find_package( Python3 ... )
# is not able to retrieve the include directory
#
# REQUIRES : the variable Python3_EXECUTABLE should be define in the parent scope
# FURNISHES : the variable PYTHON3_INCLUDE_DIRS_FOUND will be available in the parent scope
# WARNING : better use the find_package( Python3 ... ) command 
function( search_python3_include_dir )
    # Begins with a negative message because it is not the preferred way
    # to use python development artifacts
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
    set( Python3_INCLUDE_DIRS ${Python3_INCLUDE_DIRS} PARENT_SCOPE )
endfunction() 
# ---------------------------------------------------------------------------------------


# ---------------------------------------------------------------------------------------
# FUNCTION : search_numpy_include_dir
#
# This function uses the python interpreter to locate numpy include directory.
#
# REQUIRES : the variable Python3_EXECUTABLE should be define in the parent scope
# FURNISHES : the variable NUMPY_INCLUDE_DIR will be available in the parent scope
# WARNING : better use the find_package( Python3 ... NumPy ) command available 
#           with CMake 3.14 and above
function( search_numpy_include_dir )
    if( ${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.14)
        message( DEPRECATION "Better use find_package( Python3 ...) with component NumPy!" )
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
    endif()
    set( NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} PARENT_SCOPE )
endfunction()
# ---------------------------------------------------------------------------------------
