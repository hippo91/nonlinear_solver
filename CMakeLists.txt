cmake_minimum_required( VERSION 3.12...3.16 )

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

enable_testing()

add_subdirectory( src )
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
