include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES test_grammar_lr1.ver
              g1.input
              g1_2.input
        DESTINATION tests/lr1)
