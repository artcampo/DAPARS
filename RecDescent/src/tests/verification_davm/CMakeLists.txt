include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES #input lang files
              functions.lang
              lang_internal_test.lang
              lang_internal_test_false.lang
              error_stack_overflow.lang
              lang_while.lang
              lang_if.lang
              lang_args.lang
              lang_pointers.lang
              lang_member.lang

              #output of their compilation
              functions.comp.ver
              lang_internal_test.comp.ver
              lang_internal_test_false.comp.ver
              error_stack_overflow.comp.ver
              lang_while.comp.ver
              lang_if.comp.ver
              lang_args.comp.ver
              lang_pointers.comp.ver
              lang_member.comp.ver

              #output of davm of their execution
              functions.vm.ver
              lang_internal_test.vm.ver
              lang_internal_test_false.vm.ver
              error_stack_overflow.vm.ver
              lang_while.vm.ver
              lang_if.vm.ver
              lang_args.vm.ver
              lang_pointers.vm.ver
              lang_member.vm.ver
        DESTINATION tests/davm_tests)
