include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES test_loadi.ver
              test_arith.ver
              test_comparisons.ver
              test_mem.ver
              test_stack.ver
              test_call.ver
              test_logic.ver
              test_error_mem.ver

              test1.bc.ver
              test2.bc.ver
              test3.bc.ver
              test4.bc.ver
              test1.bc.out.ver
              test2.bc.out.ver
              test3.bc.out.ver
              test4.bc.out.ver
        DESTINATION tests/davm_tests)
