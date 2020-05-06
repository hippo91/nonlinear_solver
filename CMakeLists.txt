cmake_minimum_required( VERSION 3.12...3.16 )

#----------------------------------------------------------------------------------------
# FUNCTION identify_cpu
#
# This function compute the md5 sum of the /proc/cpuinfo file
#
# FURNISHES : CPUINFO_SUM variable is made available in the parent scope
function( identify_cpu )
  execute_process( COMMAND bash "-c" "cat /proc/cpuinfo|grep -v 'cpu MHz'|md5sum|gawk '{print $1}'" 
                   RESULT_VARIABLE MD5SUM_OK
                   OUTPUT_VARIABLE CPUINFO_SUM
                   ERROR_VARIABLE MD5SUM_ERROR
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   ERROR_STRIP_TRAILING_WHITESPACE
                  )
  if( NOT ${MD5SUM_OK} EQUAL 0 )
    message( STATUS "Unable to compute the checksum of the /proc/cpuinfo file!" )
    message( STATUS "The error is ${MD5SUM_ERROR}" )
  else()
    message( STATUS "md5sum(/proc/cpuinfo) = ${CPUINFO_SUM}" )
  endif()
  set( CPUINFO_SUM ${CPUINFO_SUM} PARENT_SCOPE )
endfunction()
#----------------------------------------------------------------------------------------

project( vNRInternalEnergy VERSION 1.0 LANGUAGES C )

# Disable in-source builds to prevent source tree corruption.
if( " ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "FATAL: In-source builds are not allowed.
                       You should create a separate directory for build files." )
endif()

option( BUILD_SHARED_LIBS "Build all libraries as shared objects" ON )
option( BUILD_PYTHON_VNR_MODULE "Build the python module that solves the evolution of internal energy in VNR scheme" OFF )

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_COMPILE_FEATURES c_std_99 )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra" )
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -pg" )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE STRING "Directory where the libraries should be put" )

identify_cpu()
if( NOT DEFINED CPUINFO_SUM )
  message( FATAL_ERROR "Unable to identify the CPU!" )
endif()
if( ${CPUINFO_SUM} STREQUAL "f9e79d6af8d27678f0bebc2ff1ee4066" )
  message( STATUS "The processor seems to be the one of G560 laptop" )
  set( TIME_LIMIT "57.2" )
  message( STATUS "Setting time limit of the test_solver_perfs to ${TIME_LIMIT}")
else()
  message( STATUS "The processor is unknown" )
  set( TIME_LIMIT "57.2" )
  message( STATUS "Setting time limit of the test_solver_perfs to ${TIME_LIMIT}")
endif()
configure_file( ./src/test_solver_perfs.h.in ./src/test_solver_perfs.h @ONLY )

enable_testing()

add_executable( test_solver_perfs src/test_solver_perfs.c )
target_include_directories( test_solver_perfs PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src" )
add_executable( test_solver src/test_solver.c )

add_subdirectory( src/array )
add_subdirectory( src/eos )
add_subdirectory( src/functions )
add_subdirectory( src/incrementation )
add_subdirectory( src/criterions )
add_subdirectory( src/newton )
add_subdirectory( src/launch_vnr_resolution )
add_subdirectory( src/test_utils )
if( ${BUILD_PYTHON_VNR_MODULE} )
  add_subdirectory( src/launch_vnr_resolution_c )
endif()

target_link_libraries( test_solver_perfs PRIVATE 
  launch_vnr_resolution
  test_utils
  )

target_link_libraries( test_solver PRIVATE
  array
  launch_vnr_resolution
  test_utils
  )

add_test( NAME "Test_solver"
          COMMAND test_solver )
add_test( NAME "Test_solver_performances"
          COMMAND test_solver_perfs )
set_tests_properties( Test_solver_performances PROPERTIES
                      ENVIRONMENT OMP_NUM_THREADS=1 )

add_executable( solve_cubic src/solve_cubic.c )
target_link_libraries( solve_cubic 
  PRIVATE 
    array
    functions
    criterions
    incrementation
    newton
)