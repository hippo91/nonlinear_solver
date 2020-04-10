set( LIBRARY_NAME "test_utils" )
add_library( ${LIBRARY_NAME} INTERFACE)
target_sources( ${LIBRARY_NAME} INTERFACE
                "test_utils.h"
              )
target_include_directories( ${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR} )