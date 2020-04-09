set( LIBRARY_NAME "incrementation" )
add_library( ${LIBRARY_NAME} SHARED "" )
target_sources( ${LIBRARY_NAME} PRIVATE
                "incrementations_methods.h"
                "incrementations_methods.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )