include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES test_grammar.ver 
              test_grammar2.ver
              test_parse1.ver
              test_parse2.ver
              test_parse3.ver
              test_parse4.ver
              test_parse1.input
              test_parse2.input
              test_parse3.input
              test_parse4.input
        DESTINATION tests)
