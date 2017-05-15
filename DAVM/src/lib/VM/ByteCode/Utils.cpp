#include "VM/ByteCode/Utils.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRPrint.hpp"
#include "VM/ByteCode/MemChunk.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>

namespace VM{
namespace VMUtils{

void print(ByteCode const &byte_code, const bool extra_dump){
  using namespace IRBuilder;
  byte_code.Dump(extra_dump);

}

void printRaw(ByteCode const &byte_code){
  using namespace IRBuilder;
  std::cout << "RAW bytecode:\n";
  for ( auto const inst : byte_code.stream){
    std::cout << inst << "\n";
  }
}

void writeByteCode(const ByteCode& byte_code, std::string const &file_name){
  std::unique_ptr<std::ofstream> outputFile( new std::ofstream() );
  outputFile->open (file_name);
  *outputFile << "davm" << "\n";
  *outputFile << byte_code.stream.size() << "\n";

  for ( auto const inst : byte_code.stream){
    *outputFile << inst << "\n";
  }

  *outputFile << "MemChunk" << "\n";
  *outputFile << byte_code.mem_stack_<< "\n";
  *outputFile << byte_code.mem_user_space_.size()<< "\n";
  for(const auto &m : byte_code.mem_user_space_) *outputFile << m << "\n";

  *outputFile << byte_code.perform_compiler_test_ <<"\n";

  outputFile->close();
}

ByteCode* readByteCode(std::string const &file_name){
  std::unique_ptr<std::ifstream> inputFile( new std::ifstream() );
  inputFile->open (file_name);

  std::string s; *inputFile >> s;
  if(s != "davm"){
    std::cout << "Loading of ByteCode failed: ";
    std::cout << s;
    exit(1);
  }

  int size; *inputFile >> size;
  ByteCode* byte_code = new ByteCode();
  byte_code->stream.resize(size);
  for(auto &inst : byte_code->stream) *inputFile >> inst;

  *inputFile >> s;  //"MemChunk" << "\n";

  *inputFile >> byte_code->mem_stack_;
  *inputFile >> size;
  byte_code->mem_user_space_.resize(size);
  for(auto &m : byte_code->mem_user_space_) *inputFile >> m;

  *inputFile >> byte_code->perform_compiler_test_;

  inputFile->close();
  return byte_code;
}


}//end namespace VMUtils

std::ostream& operator<<(std::ostream& os, const MemChunk& m){
  os << m.low_<<"\n";
  os << m.high_<<"\n";
  return os;
}

std::istream& operator>>(std::istream &is, MemChunk& m){
  is >> m.low_;
  is >> m.high_;
  return is;
}

}//end namespace VM
