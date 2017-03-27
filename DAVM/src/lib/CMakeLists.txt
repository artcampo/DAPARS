include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")


set(files
    ByteCode.cpp
    ExecutionContext.cpp
    Process.cpp
    RegisterBank.cpp
    VM.cpp
    VM_instructions_implementation.cpp
    IRCodification.cpp
    IRBuilder.cpp
    Utils.cpp
)

add_definitions( -std=c++14 -g)
add_library(VM STATIC ${files})
