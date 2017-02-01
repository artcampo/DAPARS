include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")

set(files
    Grammar.cpp
    Rule.cpp
    Symbol.cpp
    GrammarLR1.cpp
    GrammarLALR.cpp
    LR1_Item.cpp
    Output.cpp
)

add_definitions( -std=c++11 -g)
add_library(GRAMMAR STATIC ${files})
