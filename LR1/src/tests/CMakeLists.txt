include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)

add_executable(unit_items unit_items.cpp)
target_link_libraries(unit_items PARSER_LR1 GRAMMAR)

add_executable(test_grammar_lr1 test_grammar_lr1.cpp)
target_link_libraries(test_grammar_lr1 PARSER_LR1 GRAMMAR)

add_executable(test_grammar_dragon_lr1 test_grammar_dragon_lr1.cpp)
target_link_libraries(test_grammar_dragon_lr1 PARSER_LR1 GRAMMAR)

add_executable(test_parse_g1 test_parse_g1.cpp)
target_link_libraries(test_parse_g1 PARSER_LR1 GRAMMAR BASE_PARSER)

add_executable(test_parse_strings test_parse_strings.cpp)
target_link_libraries(test_parse_strings PARSER_LR1 GRAMMAR BASE_PARSER)

INSTALL(TARGETS unit_items DESTINATION tests/lr1)
INSTALL(TARGETS test_grammar_lr1 DESTINATION tests/lr1)
INSTALL(TARGETS test_grammar_dragon_lr1 DESTINATION tests/lr1)
INSTALL(TARGETS test_parse_g1 DESTINATION tests/lr1)
INSTALL(TARGETS test_parse_strings DESTINATION tests/lr1)


