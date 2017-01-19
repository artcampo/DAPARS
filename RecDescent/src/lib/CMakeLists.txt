include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")


set(files
    Parser.cpp
    Tokenizer.cpp
    ParserGrammar.cpp
)

add_definitions( -std=c++11 -g)
add_library(PARSER STATIC ${files})
