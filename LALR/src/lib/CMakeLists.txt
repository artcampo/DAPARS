include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../LR1/src/include")

set(files
    ParserLALR.cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LR1/src/lib/ParserLR1.cpp"
)

add_definitions( -std=c++14 -g)
add_library(PARSER_LALR STATIC ${files})
