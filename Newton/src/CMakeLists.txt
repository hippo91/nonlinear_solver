cmake_minimum_required( VERSION 3.1...3.16 )

project( vNRInternalEnergy VERSION 1.0 LANGUAGES C )

# Disable in-source builds to prevent source tree corruption.
if( " ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "FATAL: In-source builds are not allowed.
                       You should create a separate directory for build files." )
endif()

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_COMPILE_FEATURES c_std_99 )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -fopenmp" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_executable( nonlinearsolver main.c )
add_executable( test_solver test_solver.c )
enable_testing()

add_subdirectory( eos )
add_subdirectory( functions )
add_subdirectory( utils )
add_subdirectory( incrementation )
add_subdirectory( criterions )
add_subdirectory( newton )
add_subdirectory( launch_vnr_resolution )

target_link_libraries( nonlinearsolver PRIVATE 
  launch_vnr_resolution
  utils
  )

target_link_libraries( test_solver PRIVATE 
  launch_vnr_resolution
  utils
  m
  )

  add_test( NAME "Test_solver"
            COMMAND test_solver )