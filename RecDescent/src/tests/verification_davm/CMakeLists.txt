include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES #input lang files
              functions.lang
              lang_internal_test.lang
              lang_internal_test_false.lang
              error_stack_overflow.lang

              #output of davm of their execution
              functions.vm.ver
              lang_internal_test.vm.ver
              lang_internal_test_false.vm.ver
              error_stack_overflow.vm.ver
        DESTINATION tests/davm_tests)
