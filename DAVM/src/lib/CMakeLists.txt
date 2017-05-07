include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")


set(files
    VM/Execution/Process.cpp
    VM/Execution/VM.cpp
    VM/Execution/VM_instructions_implementation.cpp
    VM/ByteCode/IRCodification.cpp
    VM/ByteCode/IRBuilder.cpp
    VM/ByteCode/IRPrint.cpp
    VM/ByteCode/Utils.cpp
)

add_definitions( -std=c++14 -g)
add_library(VM STATIC ${files})
