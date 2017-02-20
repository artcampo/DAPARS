include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
add_subdirectory(verification)

add_definitions( -std=c++11 -g)

set(lib_parser_comp
    PARSER
    GRAMMAR
    BARECOMPILER
    BASE_PARSER
)

add_executable(test_grammar test_grammar.cpp)
target_link_libraries(test_grammar ${lib_parser_comp})

add_executable(test_grammar2 test_grammar2.cpp)
target_link_libraries(test_grammar2 ${lib_parser_comp})

add_executable(test_grammar_dalang test_grammar_dalang.cpp)
target_link_libraries(test_grammar_dalang ${lib_parser_comp})

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser ${lib_parser_comp})

add_executable(test_errors_parser test_errors_parser.cpp)
target_link_libraries(test_errors_parser ${lib_parser_comp})

add_executable(test_parser_compiler test_parser_compiler.cpp)
target_link_libraries(test_parser_compiler ${lib_parser_comp})

add_executable(test_code_generator test_code_generator.cpp)
target_link_libraries(test_code_generator ${lib_parser_comp})


INSTALL(TARGETS test_grammar DESTINATION tests/ll1)
INSTALL(TARGETS test_grammar2 DESTINATION tests/ll1)
INSTALL(TARGETS test_grammar_dalang DESTINATION tests/ll1)
INSTALL(TARGETS test_parser DESTINATION tests/ll1)
INSTALL(TARGETS test_errors_parser DESTINATION tests/ll1)
INSTALL(TARGETS test_parser_compiler DESTINATION tests/ll1)
INSTALL(TARGETS test_code_generator DESTINATION tests/ll1)

