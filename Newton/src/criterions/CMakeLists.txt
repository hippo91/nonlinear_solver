set( LIBRARY_NAME "criterions" )
add_library( ${LIBRARY_NAME} SHARED "" )
target_sources( ${LIBRARY_NAME} PRIVATE
                "stop_criterions.h"
                "stop_criterions.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )