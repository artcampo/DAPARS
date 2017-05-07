#include "VM/ByteCode/Utils.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRPrint.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <iostream>
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

void writeByteCode(ByteCode const &byte_code, std::string const &file_name){
  std::unique_ptr<std::ofstream> outputFile( new std::ofstream() );
  outputFile->open (file_name);
  *outputFile << "davm" << "\n";
  *outputFile << byte_code.stream.size() << "\n";

  for ( auto const inst : byte_code.stream){
    *outputFile << inst << "\n";
  }

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

  for ( auto &inst : byte_code->stream){
    *inputFile >> inst;
  }

  inputFile->close();
  return byte_code;
}

}//end namespace VMUtils
}//end namespace VM
