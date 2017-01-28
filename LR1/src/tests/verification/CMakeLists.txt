include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

INSTALL(FILES unit_items.ver
              test_grammar_lr1.ver
              test_grammar_dragon_lr1.ver
              g1.input
              g1.ver
              g1_2.input
              g1_2.ver
        DESTINATION tests/lr1)
