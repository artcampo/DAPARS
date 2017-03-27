#include "VM.hpp"
#include "ByteCode.hpp"
#include "IRCodification.hpp"
#include "IRBuilder.hpp"
#include "IRDefinition.hpp"
#include "Utils.hpp"
#include <iostream>

int main(){
  std::unique_ptr<ByteCode> bc(new ByteCode());
  
  using namespace IRCodification;
  using namespace IRBuilder;
  using namespace IRDefinition;
  using namespace IRBuilderAPI;
  using namespace SubtypesComparison;
  
  bc->stream = std::vector<uint32_t> {
    Load( IR_REG5, 42),
    Add ( IR_REG0, IR_REG1, IR_REG2),
    Sub ( IR_REG0, IR_REG1, IR_REG2),
    Mul ( IR_REG0, IR_REG1, IR_REG2),
    Div ( IR_REG0, IR_REG1, IR_REG2),
    Comp( IR_REG0, IR_REG1, IR_REG2, IR_NOT),
    Comp( IR_REG0, IR_REG1, IR_REG2, IR_EQL),
    Comp( IR_REG0, IR_REG1, IR_REG2, IR_LST),
    Comp( IR_REG0, IR_REG1, IR_REG2, IR_LTE),
    Stop()
  };
  
  
  if(!checkIRCodification()){
    std::cout << "IR sizes invalid\n";
    return 1;
  }
  
  VMUtils::printRaw(*bc);
  VMUtils::print(*bc);
//   std::cout << "test1 - started" << std::endl;
//   for(auto it : bc->stream)
//     std::cout << it <<", ";
  
//   std::unique_ptr<VirtualMachine> vm(new VirtualMachine(*bc) );
//   vm->ExecProcess();
//   vm->DumpExecutionContext();
  
  return 0;
}