#include "Utils.hpp"
#include "ByteCode.hpp"
#include "IRBuilder.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>

namespace VMUtils{

void print(ByteCode const &byte_code, const bool print_num_line){
  using namespace IRBuilder;
  int line = 0;
  for ( auto const inst : byte_code.stream){
    if(print_num_line) std::cout << line<< ": ";
    std::cout << PrintInstruction(inst) << "\n";
    ++line;
  }
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

}; //end namespace VMUtils
