cmake_minimum_required( VERSION 3.12...3.16 )

project( vNRInternalEnergy VERSION 1.0 LANGUAGES C )

# Disable in-source builds to prevent source tree corruption.
if( " ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "FATAL: In-source builds are not allowed.
                       You should create a separate directory for build files." )
endif()

option( BUILD_SHARED_LIBS "Build all libraries as shared objects" ON )

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_COMPILE_FEATURES c_std_99 )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -fopenmp" )
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -pg" )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE STRING "Directory where the libraries should be put" )

enable_testing()
add_executable( test_solver_perfs src/test_solver_perfs.c )
add_executable( test_solver src/test_solver.c )

add_subdirectory( src/array )
add_subdirectory( src/eos )
add_subdirectory( src/functions )
add_subdirectory( src/incrementation )
add_subdirectory( src/criterions )
add_subdirectory( src/newton )
add_subdirectory( src/launch_vnr_resolution )
add_subdirectory( src/test_utils )

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