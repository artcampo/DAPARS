include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_definitions( -std=c++11 -g)

add_executable(test_regex test_regex.cpp)
target_link_libraries(test_regex PARSER)


INSTALL(TARGETS test_regex DESTINATION tests)
