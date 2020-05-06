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
  set( TIME_LIMIT "1000." )
  message( STATUS "Setting time limit of the test_solver_perfs to ${TIME_LIMIT}")
endif()
configure_file( test_solver_perfs.h.in test_solver_perfs.h @ONLY )

add_executable( test_solver_perfs test_solver_perfs.c )
target_include_directories( test_solver_perfs PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" )
target_link_libraries( test_solver_perfs PRIVATE 
  launch_vnr_resolution
  test_utils
  )

add_executable( test_solver test_solver.c )
target_link_libraries( test_solver PRIVATE
  launch_vnr_resolution
  test_utils
  )

add_executable( solve_cubic solve_cubic.c )
target_link_libraries( solve_cubic 
  PRIVATE 
    array
    functions
    criterions
    incrementation
    newton
)

add_test( NAME "Test_solver"
          COMMAND test_solver )
add_test( NAME "Test_solver_performances"
          COMMAND test_solver_perfs )
set_tests_properties( Test_solver_performances PROPERTIES
                      ENVIRONMENT OMP_NUM_THREADS=1 )
