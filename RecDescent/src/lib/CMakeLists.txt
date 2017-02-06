include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")

set(files
    ParserLL1RecDesc.cpp
)

add_definitions( -std=c++11 -g)
add_library(PARSER STATIC ${files})
