set( LIBRARY_NAME "array" )
add_library( ${LIBRARY_NAME} )
target_sources( ${LIBRARY_NAME} PRIVATE
                "array.h"
                "array.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

add_executable( test_array test_array.c )
target_link_libraries( test_array  
  PRIVATE
    array
    test_utils
)
add_test( NAME Test_build_array
          COMMAND test_array 0 )
add_test( NAME Test_build_array_label_too_long
          COMMAND test_array 1 )
add_test( NAME Test_build_array_size_too_high
          COMMAND test_array 2 )
add_test( NAME Test_fill_array
          COMMAND test_array 3 )
add_test( NAME Test_fill_array_null_ptr
          COMMAND test_array 4 )
add_test( NAME Test_fill_array_null_size
          COMMAND test_array 5 )
add_test( NAME Test_fill_array_data_null_ptr
          COMMAND test_array 6 )
add_test( NAME Test_clear_array
          COMMAND test_array 7 )
add_test( NAME Test_is_valid_array
          COMMAND test_array 8 )
add_test( NAME Test_copy_array
          COMMAND test_array 9 )
add_test( NAME Test_copy_array_size_mismatch
          COMMAND test_array 10 )
