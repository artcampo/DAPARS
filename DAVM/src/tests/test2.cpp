#include "VM/Execution/VM.hpp"
#include "VM/ByteCode/ByteCode.hpp"
#include "VM/ByteCode/IRCodification.hpp"
#include "VM/ByteCode/IRBuilder.hpp"
#include "VM/ByteCode/IRDefinition.hpp"
#include "VM/ByteCode/Utils.hpp"
#include <iostream>

int main(){
  using namespace VM;
  std::unique_ptr<ByteCode> bc(new ByteCode());

  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace SubtypesComparison;

  bc->stream = std::vector<uint32_t> {
    LoadI( IR_REG5, 42),
    Move ( IR_REG5, IR_REG1),
    Add  ( IR_REG0, IR_REG1, IR_REG2),
    Sub  ( IR_REG0, IR_REG1, IR_REG2),
    Mul  ( IR_REG0, IR_REG1, IR_REG2),
    Div  ( IR_REG0, IR_REG1, IR_REG2),
    Comp ( IR_REG0, IR_REG1, IR_REG2, IR_NOT),
    Comp ( IR_REG0, IR_REG1, IR_REG2, IR_EQT),
    Comp ( IR_REG0, IR_REG1, IR_REG2, IR_LST),
    Comp ( IR_REG0, IR_REG1, IR_REG2, IR_LTE),
    Stop ()
  };


  if(!checkIRCodification()){
    std::cout << "IR sizes invalid\n";
    return 1;
  }

  VMUtils::printRaw(*bc);
  VMUtils::print(*bc);

  //Execute bytecode
  std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
  vm->ExecProcess();
  vm->DumpExecutionContext();

  return 0;
}
