include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../LR1/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)

add_executable(test_grammar_lalr test_grammar_lalr.cpp)
target_link_libraries(test_grammar_lalr PARSER_LALR GRAMMAR BASE_PARSER)

INSTALL(TARGETS test_grammar_lalr DESTINATION tests/lalr)



