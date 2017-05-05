include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES #input lang files
              functions.lang

              #output of davm of their execution
              functions.vm.ver
        DESTINATION tests/davm_tests)
