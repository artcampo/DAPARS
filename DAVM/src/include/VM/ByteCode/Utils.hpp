#pragma once
#include <string>
#include "VM/ByteCode/MemChunk.hpp"

namespace VM{

class ByteCode;

namespace VMUtils{

class ASTVisitorCodeGenerator;

void print(ByteCode const &byte_code, const bool extra_dump = false);

void      writeByteCode(ByteCode const &byte_code,
                          std::string const &file_name);
ByteCode* readByteCode(std::string const &file_name);

//only useful for debugging/TESTING
void printRaw(ByteCode const &byte_code);

std::ostream& operator<<(std::ostream& os, const MemChunk& m);
// std::istream& operator>>(std::istream &is, MemChunk& m);

}//end namespace VMUtils
}//end namespace VM
