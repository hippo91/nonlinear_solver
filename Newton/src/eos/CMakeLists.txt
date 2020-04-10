set( LIBRARY_NAME "eos" )
add_library( ${LIBRARY_NAME} SHARED "" )
target_sources( ${LIBRARY_NAME} PRIVATE
                "miegruneisen.h"
                "miegruneisen.c" 
              )
target_link_libraries( ${LIBRARY_NAME} PRIVATE m )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )