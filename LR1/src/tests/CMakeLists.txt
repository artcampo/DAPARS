include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)


add_executable(test_grammar_lr1 test_grammar_lr1.cpp)
target_link_libraries(test_grammar_lr1 PARSER_LR1 GRAMMAR)


INSTALL(TARGETS test_grammar_lr1 DESTINATION tests)
