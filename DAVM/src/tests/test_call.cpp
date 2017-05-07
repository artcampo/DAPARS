#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/VMSpec.hpp"
#include <iostream>

int main(){
  using namespace VM;
  std::unique_ptr<ByteCode> bc(new ByteCode());


  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace Spec;

  bc->stream = std::vector<uint32_t> {
    LoadI (IR_REG0, 2),
    Call  (Target(3)),
    Stop  (),
    ///////////////////////
    LoadI (IR_REG1, 3), //f1, ip = 3
    Call  (Target(6)),
    Return(),
    ///////////////////////
    LoadI (IR_REG2, 4), //f2, ip = 6
    Return()
  };

  if(!checkIRCodification()){
    std::cout << "IR sizes invalid\n";
    return 1;
  }

  //Execute bytecode
  std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
  vm->ExecProcess();
  vm->DumpExecutionContext();

  return 0;
}
