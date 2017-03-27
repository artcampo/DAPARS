#include "VM.hpp"
#include "ByteCode.hpp"
#include "Utils.hpp"
#include <iostream>

int main(int argc, char **argv){
  
  if(argc != 2){
    std::cout << "Missing .bc input file\n";
    exit(1);
  }
  
  std::unique_ptr<ByteCode> bc( VMUtils::readByteCode(std::string(argv[1])) );
  
  std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
  vm->ExecProcess();
  vm->DumpExecutionContext();
  
  return 0;
}