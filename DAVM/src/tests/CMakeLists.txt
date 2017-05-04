include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
add_subdirectory(verification)

add_definitions( -std=c++14 -g)

add_executable(test1 test1.cpp)
target_link_libraries(test1 VM)

add_executable(test2 test2.cpp)
target_link_libraries(test2 VM)

add_executable(test3 test3.cpp)
target_link_libraries(test3 VM)

add_executable(test_mem test_mem.cpp)
target_link_libraries(test_mem VM)

add_executable(test_stack test_stack.cpp)
target_link_libraries(test_stack VM)

add_executable(test_call test_call.cpp)
target_link_libraries(test_call VM)

add_executable(davm main.cpp)
target_link_libraries(davm VM)

add_executable(unit_test unit_test.cpp)
target_link_libraries(unit_test VM)


INSTALL(TARGETS
        test1
        test2
        test3
        test_mem
        test_stack
        test_call
        davm
        unit_test
        DESTINATION tests)
