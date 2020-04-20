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
add_test( NAME Test_array 
          COMMAND test_array )
