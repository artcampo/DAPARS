include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)


add_executable(test_grammar test_grammar.cpp)
target_link_libraries(test_grammar PARSER GRAMMAR)

add_executable(test_grammar2 test_grammar2.cpp)
target_link_libraries(test_grammar2 PARSER GRAMMAR)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PARSER GRAMMAR BARECOMPILER BASE_PARSER)

add_executable(test_parser_compiler test_parser_compiler.cpp)
target_link_libraries(test_parser_compiler PARSER GRAMMAR BARECOMPILER BASE_PARSER)


INSTALL(TARGETS test_grammar DESTINATION tests/ll1)
INSTALL(TARGETS test_grammar2 DESTINATION tests/ll1)
INSTALL(TARGETS test_parser DESTINATION tests/ll1)
INSTALL(TARGETS test_parser_compiler DESTINATION tests/ll1)
