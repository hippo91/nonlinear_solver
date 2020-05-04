set( LIBRARY_NAME "test_utils" )
add_library( ${LIBRARY_NAME} INTERFACE )
target_include_directories( ${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries( ${LIBRARY_NAME} INTERFACE array )