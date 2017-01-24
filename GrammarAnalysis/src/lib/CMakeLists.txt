include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")


set(files
    Grammar.cpp
    Rule.cpp
    Symbol.cpp
    GrammarLR1.cpp
)

add_definitions( -std=c++11 -g)
add_library(GRAMMAR STATIC ${files})
