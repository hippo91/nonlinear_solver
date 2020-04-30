set( LIBRARY_NAME "functions" )
add_library( ${LIBRARY_NAME} )
target_sources( ${LIBRARY_NAME} PRIVATE
                "vnr_internalenergy_evolution.h"
                "vnr_internalenergy_evolution.c" 
                "cubic.h"
                "cubic.c"
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries( ${LIBRARY_NAME}
  PUBLIC
    array
  PRIVATE
    eos
)