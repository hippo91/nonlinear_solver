find_package( OpenMP REQUIRED )


set( LIBRARY_NAME "launch_vnr_resolution" )
add_library( ${LIBRARY_NAME} )
target_sources( ${LIBRARY_NAME} PRIVATE
                "launch_vnr_resolution.h"
                "launch_vnr_resolution.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries( ${LIBRARY_NAME}
PUBLIC
  array
  eos
PRIVATE
  OpenMP::OpenMP_C
  newton
  functions
  incrementation
  criterions
  )
