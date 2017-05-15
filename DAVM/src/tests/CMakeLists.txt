include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
add_subdirectory(verification)

add_definitions( -std=c++14 -g)

add_executable(test_loadi test_loadi.cpp)
target_link_libraries(test_loadi VM)

add_executable(test_arith test_arith.cpp)
target_link_libraries(test_arith VM)

add_executable(test_comparisons test_comparisons.cpp)
target_link_libraries(test_comparisons VM)

add_executable(test_mem test_mem.cpp)
target_link_libraries(test_mem VM)

add_executable(test_stack test_stack.cpp)
target_link_libraries(test_stack VM)

add_executable(test_call test_call.cpp)
target_link_libraries(test_call VM)

add_executable(test_logic test_logic.cpp)
target_link_libraries(test_logic VM)

add_executable(unit_test unit_test.cpp)
target_link_libraries(unit_test VM)

#VM interpreter
add_executable(davm main.cpp)
target_link_libraries(davm VM)


INSTALL(TARGETS
        test_loadi
        test_arith
        test_comparisons
        test_mem
        test_stack
        test_call
        test_logic
        davm
        unit_test
        DESTINATION tests/davm_tests)
