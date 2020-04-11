set( LIBRARY_NAME "criterions" )
add_library( ${LIBRARY_NAME} SHARED "" )
target_sources( ${LIBRARY_NAME} PRIVATE
                "stop_criterions.h"
                "stop_criterions.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )


add_executable( test_stop_criterions test_stop_criterions.c )
target_link_libraries( test_stop_criterions  
  PRIVATE
    criterions
    test_utils
)
add_test( NAME Test_criterions 
          COMMAND test_stop_criterions )