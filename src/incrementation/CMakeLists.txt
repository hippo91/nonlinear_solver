set( LIBRARY_NAME "incrementation" )
add_library( ${LIBRARY_NAME} )
target_sources( ${LIBRARY_NAME} PRIVATE
                "incrementations_methods.h"
                "incrementations_methods.c" 
              )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries( ${LIBRARY_NAME}
  PUBLIC
    array
)


add_executable( test_incrementation_methods test_incrementation_methods )
target_link_libraries( test_incrementation_methods
  PRIVATE
    array
    incrementation
    test_utils
)
add_test( NAME Test_classical_incremention
          COMMAND test_incrementation_methods 0
        )   
add_test( NAME Test_damped_incremention
          COMMAND test_incrementation_methods 1
        )   
add_test( NAME Test_ensure_positivity_incremention
          COMMAND test_incrementation_methods 2
        )   