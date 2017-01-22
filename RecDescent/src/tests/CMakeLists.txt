include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)

add_executable(test_regex test_regex.cpp)
target_link_libraries(test_regex PARSER)

add_executable(test_grammar test_grammar.cpp)
target_link_libraries(test_grammar PARSER GRAMMAR)

add_executable(test_grammar2 test_grammar2.cpp)
target_link_libraries(test_grammar2 PARSER GRAMMAR)

INSTALL(TARGETS test_regex DESTINATION tests)
INSTALL(TARGETS test_grammar DESTINATION tests)
INSTALL(TARGETS test_grammar2 DESTINATION tests)
