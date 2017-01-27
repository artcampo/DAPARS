include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")

set(files
    BaseParser.cpp
    Tokenizer.cpp
)

add_definitions( -std=c++11 -g)
add_library(BASE_PARSER STATIC ${files})
