include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../GrammarAnalysis/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DACOMPILER/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../DAVM/src/include")

set(files
    ParserLL1RecDesc.cpp
    LL1RecDescTopLevel.cpp
    LL1RecDescFunction.cpp
    LL1RecDescControlFlow.cpp
    LL1RecDescExpressions.cpp
    LL1RecDescDeclarations.cpp
    LL1RecDescName.cpp
    LL1RecDescClass.cpp
)

add_definitions( -std=c++14 -g)
add_library(PARSER STATIC ${files})
