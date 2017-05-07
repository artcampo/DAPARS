#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/Utils.hpp"
#include <iostream>

int main(int argc, char **argv){

  using namespace VM;

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
